#include <WiFi.h>
#include "Arduino.h"

#define WIFI_NETWORK "DOJA&NINA 2.4G"
#define WIFI_PASSWORD "Gatinhas123"

class WIFI
{
public:
    WIFI()
    {
    }

    void setup()
    {
        this->configureStaticIP();
        this->connect();
    }

private:
    void configureStaticIP()
    {
        IPAddress localIP(192, 168, 0, 182);
        IPAddress gateway(192, 168, 0, 1);
        IPAddress subnet(255, 255, 0, 0);
        IPAddress primaryDNS(8, 8, 8, 8);
        IPAddress secondaryDNS(8, 8, 4, 4);

        WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS);
    }

    void connect()
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
        }

        Serial.println("WiFi connected!");
    }
};

WIFI wifi;