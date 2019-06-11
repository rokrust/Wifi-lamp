#include "webInterface.h"
#include "credentials.h"

#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <FS.h>

WebInterface::WebInterface() : client(credentials.mqtt.name.c_str(), credentials.mqtt.port, espClient)
{
    client.setCallback([](char* inTopic, uint8_t* payload, unsigned int length){
        for(int i = 0; i < length; i++)
        {
            Serial.print((char)payload[i]);
        }
        Serial.println();
    });
}

WebInterface::~WebInterface()
{
    server.stop();
    WiFi.disconnect();
    espClient.stop();
    client.disconnect();
}


String WebInterface::getContentType(String path)
{
    if(path.endsWith(".htm")) return "text/html";
    else if(path.endsWith(".html")) return "text/html";
    else if(path.endsWith(".css")) return "text/css";
    else if(path.endsWith(".js")) return "application/javascript";
    else if(path.endsWith(".png")) return "image/png";
    else if(path.endsWith(".gif")) return "image/gif";
    else if(path.endsWith(".jpg")) return "image/jpeg";
    else if(path.endsWith(".ico")) return "image/x-icon";
    else if(path.endsWith(".xml")) return "text/xml";
    else if(path.endsWith(".pdf")) return "application/x-pdf";
    else if(path.endsWith(".zip")) return "application/x-zip";
    else if(path.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

void WebInterface::uploadFile(String path)
{
    HTTPUpload& upload = server.upload();

    if(upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/" + filename;

        fileHandle = SPIFFS.open(filename, "w");
    }
    else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(fileHandle)
        {
            fileHandle.write(upload.buf, upload.currentSize);
        }
    }
    else if(upload.status == UPLOAD_FILE_END)
    {
        if(fileHandle)
        {
            fileHandle.close();
            server.sendHeader("Location", "/success.html"); //Redirect to success page
            server.send(303);
        }
        else
        {
            server.send(500, "text/plain", "500: Could not create file");
        }
    }
}

void WebInterface::setupServer()
{
    ip = IPAddress(10, 0, 0, 10);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");

    server.begin();

    server.on("/upload", HTTP_POST, [this](){
        String path = server.uri();

        if(SPIFFS.exists(path))
        {
            uploadFile(path);
        }

        else
        {
            Serial.println("File " + path + " does not exist");
        }
    });

    server.onNotFound([this](){
        String path = server.uri();
        Serial.println(String("Requested: ") + path);
        path = "/build" + path;
        if(path.endsWith("/")) path += "index.html";

        String contentType = getContentType(path);
        File file = SPIFFS.open(path, "r");
        size_t size = server.streamFile(file, contentType);
        file.close();
        Serial.println(String("Sent file: ") + path);
    });
}

void WebInterface::connectToWifi(WifiCredentials wifiAp)
{
    //WiFi.mode(WIFI_STA);
    if(!WiFi.isConnected())
    {
        WiFi.begin(credentials.wifi.ssid, credentials.wifi.password);
    }

}

void WebInterface::connectToMqtt(MqttCredentials mqtt, String topic)
{
    if(!client.connected())
    {
        if(client.connect("Wifi-lamp", credentials.mqtt.user.c_str(), credentials.mqtt.password.c_str()))
        {
            client.subscribe(topic.c_str());
        }
    }

}

void WebInterface::reconnectServices(Credentials credentials, String topic)
{
    connectToWifi(credentials.wifi);
    connectToMqtt(credentials.mqtt, topic);
}