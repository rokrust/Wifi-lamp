#include <Arduino.h>
#include <ticker.h>
#include <functional>
#include <vector>

struct Rgb 
{
    unsigned char r, g, b;

    Rgb() : r(255), g(255), b(255) { }
    Rgb(unsigned char val) : r(val), g(val), b(val) { }
    Rgb(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) { }

    bool isMax() { return r == 255 || g == 255 || b == 255; }
    bool isMin() { return r == 0 || g == 0 || b == 0; }
};

class RgbLed
{
    private:
        unsigned char rPin, gPin, bPin;
        Rgb rgb;
        Ticker broken, scheduleOn, scheduleOff, deactivate; //Schedule does not work for some reason

        void setR(unsigned char r) { rgb.r = r; analogWrite(rPin, rgb.r); }
        void setG(unsigned char g) { rgb.g = g; analogWrite(gPin, rgb.g); }
        void setB(unsigned char b) { rgb.b = b; analogWrite(bPin, rgb.b); }

    public:
        RgbLed() { }
        RgbLed(unsigned char rPin, unsigned char gPin, unsigned char bPin);

        void brighten(unsigned char val);
        void dim(unsigned char val);
        void blink(uint16_t onTime, uint16_t offTime);
        void blink(uint16_t frequency) { blink(frequency, frequency); }
        void blink(uint16_t onTime, uint16_t offTime, uint16_t blinkTime);
        void fadeOut(uint16_t fadeTime, uint16_t fadeInterval=30);
        void fadeIn(uint16_t fadeTime, uint16_t fadeInterval=30);
        void blinkWithFade(uint16_t period);
        void blinkStop() { scheduleOn.detach(); scheduleOff.detach(); }
        void sequence(const std::vector<Rgb>& seq, uint16_t intervalTime);

        void on();
        void on(unsigned char r, unsigned char g, unsigned char b);
        void on(Rgb rgb) { setR(rgb.r); setR(rgb.g); setB(rgb.b); }
        void off();

};