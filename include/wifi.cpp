#include <WiFi.h>
#include "Arduino.h"

#define WIFI_NETWORK "DOJA&NINA 2.4G"
#define WIFI_PASSWORD "Gatinhas123"

class WIFI : public WiFiClass
{
public:
    WIFI() : WiFiClass()
    {
        this->configureStaticIP();
        this->mode(WIFI_STA);
        this->begin(WIFI_NETWORK, WIFI_PASSWORD);
    }

private:
    void configureStaticIP()
    {
        IPAddress localIP(192, 168, 0, 182);
        IPAddress gateway(192, 168, 0, 1);
        IPAddress subnet(255, 255, 0, 0);
        IPAddress primaryDNS(8, 8, 8, 8);
        IPAddress secondaryDNS(8, 8, 4, 4);

        this->config(localIP, gateway, subnet, primaryDNS, secondaryDNS);
    }
};

WIFI wifi;