#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "web-socket.cpp"

AsyncWebServer server(80);

struct web_server_t
{
    void setup()
    {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(200, "text/plain", "Hello, world"); });

        server.onNotFound([](AsyncWebServerRequest *request)
                          { request->send(404, "text/plain", "Not found"); });

        server.addHandler(&wss);
        server.begin();
    };
};

web_server_t web_server = {};