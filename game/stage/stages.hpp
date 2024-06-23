#pragma once

#include "raylib.h"
#include "entt.hpp"
#include "vec2.hpp"

#include "other.hpp"

namespace stage {
    void spawn_enemy_at_pos(entt::registry &registry, glm::vec2 pos) {
        auto texture = Texture2D();
        texture = LoadTexture("resources/enemy.png");

        auto enemy = registry.create();
        registry.emplace<EnemyComp>(enemy);
        registry.emplace<RectangleComp>(enemy, pos.x, pos.y, 75.0f, 50.0f);
        registry.emplace<TextureComp>(enemy, texture);
        registry.emplace<HealthComp>(enemy, 3.0f);
        registry.emplace<ScoreComp>(enemy, 1);
    }

    void spawn_shooting_enemy_at_pos(entt::registry &registry, glm::vec2 pos) {
        auto texture = Texture2D();
        texture = LoadTexture("resources/enemy.png");

        auto enemy = registry.create();
        registry.emplace<EnemyComp>(enemy);
        registry.emplace<RectangleComp>(enemy, pos.x, pos.y, 75.0f, 50.0f);
        registry.emplace<TextureComp>(enemy, texture);
        registry.emplace<HealthComp>(enemy, 3.0f);
        registry.emplace<FireCooldownComp>(enemy, 2.0f);
        registry.emplace<ScoreComp>(enemy, 1);
    }

    void stage0_init(entt::registry &registry) {
        spawn_enemy_at_pos(registry, glm::vec2(GetScreenWidth() / 2.0f, 30.0f + 50.0f + 30.0f));
    }

    void stage1_init(entt::registry &registry) {
        auto enemy_count = 5;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            spawn_enemy_at_pos(registry, glm::vec2((space * (float)(i + 1)) + 75.0f * (float)i, 30.0f));
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            spawn_enemy_at_pos(registry, glm::vec2(space * (float)(i + 1) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f));
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            spawn_shooting_enemy_at_pos(registry, glm::vec2(space * (float)(i + 1) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f));
        }
    }

    void stage2_init(entt::registry &registry) {
        auto enemy_count = 8;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            spawn_enemy_at_pos(registry, glm::vec2((space * (float)(i + 1)) + 75.0f * (float)i, 30.0f));
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            spawn_enemy_at_pos(registry, glm::vec2(space * (float)(i + 1) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f));
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            spawn_shooting_enemy_at_pos(registry, glm::vec2(space * (float)(i + 1) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f));
        }
    }
}