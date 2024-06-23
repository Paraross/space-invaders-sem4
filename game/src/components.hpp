#pragma once

#include <string>

#include "raylib.h"
#include "vec2.hpp"

struct PlayerComp {};

struct EnemyComp {};

struct BulletComp {};

struct DestroyEntityComp {};

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

struct ColorComp {
    Color color;
};

struct TextureComp {
    Texture2D texture;
};

struct VelocityComp {
    glm::vec2 velocity;
};

struct DespawnOnScreenLeftComp {};

struct TheScoreComp {};

struct ScoreComp {
    int score;
};

struct TextComp {
    std::string text;
    glm::vec2 position;
    int font_size;
    Color color;
};

struct HealthComp {
    float health;
};

struct DamageComp {
    float damage;
};

struct EnemyHitEventTextComp {};
