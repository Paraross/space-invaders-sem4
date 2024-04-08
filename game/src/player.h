#pragma once

#include "raylib.h"

struct Player {
    Rectangle rect;
    Color color;
    Vector2 velocity;
    float max_speed;

    Player()
     : Player(120.0f, 50.0f, 40.0f, 40.0f, 1000.0f, BLACK) {}

    Player(float pos_x, float pos_y, float width, float height, float max_speed, Color color) {
        this->rect.x = pos_x;
        this->rect.y = pos_y;
        this->rect.width = width;
        this->rect.height = height;
        this->color = color;
        this->max_speed = max_speed;
    }

    static auto init(float pos_x, float pos_y, float width, float height, float max_speed, Color color) -> Player {
        return Player(pos_x, pos_y, width, height, max_speed, color);
    }
};
