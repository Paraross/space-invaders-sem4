#pragma once

#include "raylib.h"

struct PlayerComp {};

struct Player {
    Rectangle rect;
    Color color;
    // Vector2 velocity;
    float max_speed;
    float max_fire_cooldown;
    float fire_cooldown;

    Player() = default;

    Player(float pos_x, float pos_y)
     : Player(pos_x, pos_y, 40.0f, 40.0f, 1000.0f, 0.2f, BLACK) {}

    Player(float pos_x, float pos_y, float width, float height, float max_speed, float max_fire_cooldown, Color color) {
        this->rect.x = pos_x;
        this->rect.y = pos_y;
        this->rect.width = width;
        this->rect.height = height;
        this->color = color;
        this->max_speed = max_speed;
        this->max_fire_cooldown = max_fire_cooldown;
        this->fire_cooldown = max_fire_cooldown;
    }

    static auto at_position(float pos_x, float pos_y) -> Player {
        return Player(pos_x, pos_y);
    }
};
