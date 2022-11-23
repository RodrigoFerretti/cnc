#include "Arduino.h"

struct move_t
{
    float distance;
    float velocity;
};

struct circular_motion_t
{
    float precision;

    int current_segment[2];
    int number_of_segments;
    float theta_per_segment;

    float target_position[2];
    float center_position[2];

    float current_position[2];
    float previous_position[2];

    float center_to_current_position[2];
    float initial_distance_to_center[2];

    float feed_rate;

    float current_velocity[2];

    bool can_calculate_next_move(int i)
    {
        return this->current_segment[i] <= this->number_of_segments && number_of_segments != 0;
    }

    float calculate_full_arc_lenght(float center_to_current_position[2], float target_position[2], float center_position[2], bool is_clockwise)
    {
        float distance_from_center_to_target_position[2] = {
            target_position[0] - center_position[0],
            target_position[1] - center_position[1]};

        float full_arc_lenght = atan2(
            center_to_current_position[0] * distance_from_center_to_target_position[1] - center_to_current_position[1] * distance_from_center_to_target_position[0],
            center_to_current_position[0] * distance_from_center_to_target_position[0] + center_to_current_position[1] * distance_from_center_to_target_position[1]);

        if (is_clockwise && full_arc_lenght >= 0)
        {
            return full_arc_lenght - 2 * PI;
        }
        if (!is_clockwise && full_arc_lenght <= 0)
        {
            return full_arc_lenght + 2 * PI;
        }

        return full_arc_lenght;
    }

    int calculate_number_of_segments(float full_arc_lenght, float radius)
    {
        return floor(fabs(0.5 * full_arc_lenght * radius) / sqrt(this->precision * (2 * radius - this->precision)));
    }

    void setup(float initial_position[2], float target_position[2], float initial_distance_to_center[2], float radius, bool is_clockwise, float feed_rate)
    {
        this->feed_rate = feed_rate;

        this->target_position[0] = target_position[0];
        this->target_position[1] = target_position[1];

        this->previous_position[0] = initial_position[0];
        this->previous_position[1] = initial_position[1];

        this->initial_distance_to_center[0] = initial_distance_to_center[0];
        this->initial_distance_to_center[1] = initial_distance_to_center[1];

        this->center_position[0] = initial_position[0] + initial_distance_to_center[0];
        this->center_position[1] = initial_position[1] + initial_distance_to_center[1];

        this->center_to_current_position[0] = -initial_distance_to_center[0];
        this->center_to_current_position[1] = -initial_distance_to_center[1];

        float full_arc_lenght = calculate_full_arc_lenght(center_to_current_position, target_position, center_position, is_clockwise);

        this->current_segment[0] = 0;
        this->current_segment[1] = 0;
        this->number_of_segments = calculate_number_of_segments(full_arc_lenght, radius);

        if (number_of_segments)
        {
            this->theta_per_segment = full_arc_lenght / number_of_segments;

            this->current_velocity[0] = fabs(sin(full_arc_lenght)) * this->feed_rate;
            this->current_velocity[0] = fabs(cos(full_arc_lenght)) * this->feed_rate;
        }
    }

    move_t calculate_next_move(int i)
    {
        float cos_theta = cos(this->current_segment[i] * this->theta_per_segment);
        float sin_theta = sin(this->current_segment[i] * this->theta_per_segment);

        this->center_to_current_position[i] = i == 0
                                                  ? -this->initial_distance_to_center[0] * cos_theta + this->initial_distance_to_center[1] * sin_theta
                                                  : -this->initial_distance_to_center[0] * sin_theta - this->initial_distance_to_center[1] * cos_theta;

        this->current_position[i] = this->center_position[i] + this->center_to_current_position[i];

        move_t move = {
            .distance = (this->current_segment[i] != this->number_of_segments ? this->current_position : this->target_position)[i] - this->previous_position[i],
            .velocity = this->current_velocity[i],
        };

        this->current_velocity[i] = i == 0
                                        ? fabs(sin_theta) * this->feed_rate
                                        : fabs(cos_theta) * this->feed_rate;

        this->previous_position[i] = current_position[i];

        this->current_segment[i] = this->current_segment[i] + 1;

        return move;
    }
};

circular_motion_t circular_motion = {
    .precision = 0.001,
};