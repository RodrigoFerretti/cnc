#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "web-socket.cpp"

#define SERVER_PORT 80

class WebServer : public AsyncWebServer
{
public:
    WebServer() : AsyncWebServer(SERVER_PORT)
    {
    }

    void setup()
    {
        this->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/plain", "Hello, world"); });

        this->onNotFound([](AsyncWebServerRequest *request)
                         { request->send(404, "text/plain", "Not found"); });

        this->addHandler(&webSocket);

        this->begin();
    }
};

WebServer webServer;