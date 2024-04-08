#pragma once

#include "raylib.h"

struct Enemy {
    Rectangle rect;
    Color color;

    Enemy()
     : Enemy(120.0f, 50.0f, 40.0f, 40.0f, BLACK) {}

    Enemy(float pos_x, float pos_y, float width, float height, Color color) {
        this->rect.x = pos_x;
        this->rect.y = pos_y;
        this->rect.width = width;
        this->rect.height = height;
        this->color = color;
    }
};
