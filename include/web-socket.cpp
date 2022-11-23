#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "stepper.cpp"
#include "generator.cpp"

AsyncWebSocket wss("/ws");

String get_value_from_letter(String message, String letter)
{
    message = message + " ";
    int letter_index = message.indexOf(letter);
    return ((letter_index != -1) ? message.substring(letter_index + 1, message.substring(letter_index, message.length()).indexOf(" ") + letter_index) : "");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
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

        String g_value = get_value_from_letter(message, "G");
        String m_value = get_value_from_letter(message, "M");

        float x_value = get_value_from_letter(message, "X").toFloat();
        float y_value = get_value_from_letter(message, "Y").toFloat();

        float i_value = get_value_from_letter(message, "I").toFloat();
        float j_value = get_value_from_letter(message, "J").toFloat();

        float r_value = get_value_from_letter(message, "R").toFloat();

        float f_value = fabs(get_value_from_letter(message, "F").toFloat());

        if (m_value == "00")
        {
            stepper_x0.rapidStop();
            stepper_x1.rapidStop();
            return;
        }

        if (g_value == "01")
        {
            stepper_x0.moveWithSpeed(x_value, f_value);
            stepper_x1.moveWithSpeed(x_value, f_value);
            return;
        }

        if (g_value == "02")
        {
            float initial_position[2] = {0, 0};
            float target_position[2] = {x_value, y_value};
            float initial_distance_to_center[2] = {i_value, j_value};
            float radius = r_value;
            bool is_clockwise = true;
            float feed_rate = f_value;

            circular_motion.setup(initial_position, target_position, initial_distance_to_center, radius, is_clockwise, feed_rate);
            return;
        }
    }
}

struct web_socket_t
{
    void setup()
    {
        wss.onEvent(onEvent);
    }
};

web_socket_t web_socket = {};