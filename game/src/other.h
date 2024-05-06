#pragma once

#include "raylib.h"

// struct PositionComp {
//     float x;
//     float y;
// };

struct RectangleComp {
    Rectangle rect;

    RectangleComp(float pos_x, float pos_y, float width, float height) {
        this->rect.x = pos_x;
        this->rect.y = pos_y;
        this->rect.width = width;
        this->rect.height = height;
    }
};

struct MaxSpeedComp {
    float max_speed;
};

struct FireCooldownComp {
    float max_fire_cooldown;
    float fire_cooldown;

    FireCooldownComp(float cooldown)
        : max_fire_cooldown(cooldown), fire_cooldown(cooldown) {}
};
