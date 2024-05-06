#pragma once

#include "raylib.h"

struct EnemyComp;

struct Enemy {
    Rectangle rect;
    Color color;

    Enemy(float pos_x, float pos_y)
     : Enemy(pos_x, pos_y, 75.0f, 50.0f, RED) {}

    Enemy(float pos_x, float pos_y, float width, float height, Color color) {
        this->rect.x = pos_x;
        this->rect.y = pos_y;
        this->rect.width = width;
        this->rect.height = height;
        this->color = color;
    }

    static auto at_position(float pos_x, float pos_y) -> Enemy {
        return Enemy(pos_x, pos_y);
    }
};
