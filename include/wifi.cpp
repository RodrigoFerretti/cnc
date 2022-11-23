#include <WiFi.h>
#include "Arduino.h"

struct wifi_t
{
    void setup()
    {
        IPAddress local_ip(192, 168, 0, 182);
        IPAddress gateway(192, 168, 0, 1);
        IPAddress subnet(255, 255, 0, 0);
        IPAddress primary_dns(8, 8, 8, 8);
        IPAddress secondary_dns(8, 8, 4, 4);

        WiFi.config(local_ip, gateway, subnet, primary_dns, secondary_dns);
        WiFi.mode(WIFI_STA);
        WiFi.begin("DOJA&NINA 2.4G", "Gatinhas123");

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
        }

        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
};

wifi_t wifi = {};