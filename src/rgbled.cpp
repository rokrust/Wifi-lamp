#include "rgbled.h"

RgbLed::RgbLed(unsigned char rPin, unsigned char gPin, unsigned char bPin) : rPin(rPin), gPin(gPin), bPin(bPin) 
{
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(bPin, OUTPUT);
}

void RgbLed::brighten(unsigned char val)
{
    if(255 - val < rgb.r) val = 255 - rgb.r;
    if(255 - val < rgb.g) val = 255 - rgb.g;
    if(255 - val < rgb.b) val = 255 - rgb.b;

    setR(rgb.r + val);
    setG(rgb.g + val);
    setB(rgb.b + val);
}

void RgbLed::dim(unsigned char val)
{    
    if(val > rgb.r) val = rgb.r;
    if(val > rgb.g) val = rgb.g;
    if(val > rgb.b) val = rgb.b;
    
    setR(rgb.r - val);
    setG(rgb.g - val);
    setB(rgb.b - val);

/*
    Serial.print("rgb: ");
    Serial.print(rgb.r);
    Serial.print(", ");
    Serial.print(rgb.g);
    Serial.print(", ");
    Serial.print(rgb.b);
    Serial.print("\n");
    Serial.print("Val: ");
    Serial.print(val);
    Serial.print("\n");
*/  
}

void RgbLed::blink(uint16_t onTime, uint16_t offTime)
{   
    on();

    //Periodically turn on
    scheduleOn.attach_ms(onTime+offTime, [this](){
        Serial.println("Blink on");
        on();
    });

    //Periodically turn off
    scheduleOff.once_ms(onTime, [this, onTime, offTime](){
        this->scheduleOff.attach_ms_scheduled(onTime+offTime,[this](){
            Serial.println("Blink off");
            off();
        });
    });
}

void RgbLed::blink(uint16_t onTime, uint16_t offTime, uint16_t blinkTime) 
{ 
    blink(onTime, offTime); 
    deactivate.once_ms(blinkTime, [this]() { 
        this->blinkStop(); 
    }); 
}

void RgbLed::blinkWithFade(uint16_t period)
{
    uint16_t halfPeriod = period / 2;
    uint16_t fadeInterval = 30;
    fadeIn(halfPeriod, fadeInterval);

    uint16_t nCycles = halfPeriod / fadeInterval;
    unsigned char val = 255 / nCycles;
    if(val == 0) val = 10;

    scheduleOff.attach_ms(fadeInterval, [this, val](){ 
        bool dimLight = true;
        if(dimLight)
        {
            dim(val); 
        }
        else 
        {
            brighten(val);
        }
    });

    scheduleOn.attach_ms(fadeInterval, [this, val](){ 
        brighten(val);
        //if(rgb.r == 255 && rgb.g == 255 && rgb.b == 255)
            scheduleOn.detach();
    });
}

void RgbLed::fadeIn(uint16_t fadeTime, uint16_t fadeInterval)
{
    unsigned char lowestVal = rgb.r;
    if(rgb.g < lowestVal) lowestVal = rgb.g;
    if(rgb.b < lowestVal) lowestVal = rgb.b;
    
    uint16_t nCycles = fadeTime / fadeInterval;
    unsigned char brightVal = (255 - lowestVal) / nCycles;
    if(brightVal == 0) brightVal = 10;

    scheduleOn.attach_ms(fadeInterval, [this, brightVal](){ 
        brighten(brightVal);
        //if(rgb.r == 255 && rgb.g == 255 && rgb.b == 255)
            scheduleOn.detach();
    });

}

void RgbLed::fadeOut(uint16_t fadeTime, uint16_t fadeInterval)
{
    unsigned char highestVal = rgb.r;
    if(rgb.g > highestVal) highestVal = rgb.g;
    if(rgb.b > highestVal) highestVal = rgb.b;

    uint16_t nCycles = fadeTime / fadeInterval;
    unsigned char dimVal = highestVal / nCycles;
    if(dimVal == 0) dimVal = 10;

    scheduleOff.attach_ms(fadeInterval, [this, dimVal](){ 
        dim(dimVal); 
        //if(rgb.r == 0 && rgb.g == 0 && rgb.b == 0)
            scheduleOff.detach();
    });
    //deactivate.once_ms(fadeTime, [this](){ scheduleOff.detach(); Serial.println("Dim detached"); Serial.print("r: "); Serial.print(rgb.r); Serial.print("\n"); });
}

void RgbLed::sequence(const std::vector<Rgb>& seq, uint16_t intervalTime)
{
    scheduleOn.attach_ms(intervalTime, [this, seq](){
        static unsigned int i = 0;

        on(seq[i]);
        if(++i == seq.size()) 
            scheduleOn.detach();
    });
}

void RgbLed::on()
{
    broken.detach(); scheduleOn.detach(); scheduleOff.detach(); deactivate.detach();

    analogWrite(rPin, rgb.r);
    analogWrite(gPin, rgb.g);
    analogWrite(bPin, rgb.b);
}

void RgbLed::on(unsigned char r, unsigned char g, unsigned char b)
{ 
    setR(r);
    setG(g);
    setB(b);
}

void RgbLed::off()
{
    broken.detach(); scheduleOn.detach(); scheduleOff.detach(); deactivate.detach();

    analogWrite(rPin, 0);
    analogWrite(gPin, 0);
    analogWrite(bPin, 0);
}