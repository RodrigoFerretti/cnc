#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <wifi.cpp>
#include "gcode.cpp"

class WebSocket : public AsyncWebSocket
{
public:
    WebSocket() : AsyncWebSocket("/ws")
    {
    }

    void setup()
    {
        onEvent(handler);
    }

private:
    AwsEventHandler handler = [=](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
    {
        if (type != WS_EVT_DATA)
        {
            return;
        }

        AwsFrameInfo *info = (AwsFrameInfo *)arg;

        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            data[len] = 0;
            String message = (char *)data;
            GCode gcode(message);
            gcode.run();

            return;
        }
    };
};

WebSocket webSocket;