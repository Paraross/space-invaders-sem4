#pragma once

#include "raylib.h"

struct Bullet {
    Rectangle rect;
    Color color;

    Bullet(float pos_x, float pos_y)
     : Bullet(pos_x, pos_y, 10.0f, 30.0f, BLUE) {}

    Bullet(float pos_x, float pos_y, float width, float height, Color color) {
        this->rect.x = pos_x;
        this->rect.y = pos_y;
        this->rect.width = width;
        this->rect.height = height;
        this->color = color;
    }

    static auto at_position(float pos_x, float pos_y) -> Bullet {
        return Bullet(pos_x, pos_y);
    }
};
