#include "modules/wifimodule.h"
#include <ESP8266WiFi.h>

void WifiModule::connectBlocking(unsigned int timeout)
{
    WiFi.begin(credentials.ssid, credentials.password);
    connectionTimer.start();

    while(WiFi.status() != WL_CONNECTED && connectionTimer.time() < timeout*1000)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.print("Connected to wifi AP with IP: ");
    Serial.println(WiFi.localIP());
}

void WifiModule::connect(unsigned int timeout)
{
    WiFi.begin(credentials.ssid, credentials.password);
    connectionTimer.start();
    this->timeout = timeout*1000;
    isConnecting = true;
}

void WifiModule::disconnect()
{
    isConnecting = false;
    WiFi.disconnect(true);
}

bool WifiModule::canConnect()
{
    return hasCredentials && !isConnecting;
}

bool WifiModule::connectionTimeout()
{
    return connectionTimer.started() && connectionTimer.time() > timeout;
}

void WifiModule::handleConnectionTimeout()
{
    connectionTimer.stop();
}

void WifiModule::setupAp()
{
    IPAddress ip = IPAddress(10, 0, 0, 13);
    WiFi.disconnect(true); //Might be okay to change the channel instead of disconnecting
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Lamp Config");
}

void WifiModule::scanWifi(WifiAp* ap)
{
    int n = WiFi.scanNetworks();

    Serial.print("Scanned networks: ");
    Serial.print(WiFi.SSID(n - 1));
    Serial.print(", ");
    Serial.println(WiFi.RSSI(n - 1));

    ap->ssid = WiFi.SSID(0);
    ap->signalStrength = WiFi.RSSI(0);
    send<WifiAp>(ap);
}

void WifiModule::setup()
{
    setupAp();
    scanTimer.start();

    subscribe<WifiInfo>(&WifiModule::setCredentials);
    respond<WifiAp>(&WifiModule::scanWifi);
}

void WifiModule::loop()
{
    if (!WiFi.isConnected() && canConnect())
    {
        Serial.println("Attempting to connect");
        connect(10);
    }

    if (connectionTimeout())
    {
        Serial.println("Connection timed out");
        handleConnectionTimeout();
    }

    if(WiFi.isConnected())
    {
        Serial.print("Connected");
    }
}

void WifiModule::setCredentials(WifiInfo *msg)
{
    Serial.print("ssid: "); Serial.println(msg->ssid);
    Serial.print("password: "); Serial.println(msg->password);

    credentials.ssid = msg->ssid;
    credentials.password = msg->password;
    WiFi.softAPdisconnect();
    hasCredentials = true;
}