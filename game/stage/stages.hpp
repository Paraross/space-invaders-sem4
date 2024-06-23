#pragma once

#include "entt.hpp"

#include "other.hpp"

namespace stage {
    void stage0_init(entt::registry &registry) {
        auto enemy = registry.create();
        registry.emplace<EnemyComp>(enemy);
        registry.emplace<RectangleComp>(enemy, GetScreenWidth() / 2.0f, 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
        registry.emplace<ColorComp>(enemy, RED);
        registry.emplace<HealthComp>(enemy, 3.0f);
        registry.emplace<ScoreComp>(enemy, 2);
    }

    void stage1_init(entt::registry &registry) {
        auto enemy_count = 5;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 3.0f);
            registry.emplace<ScoreComp>(enemy, 2);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 1.0f);
            registry.emplace<ScoreComp>(enemy, 1);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 2.0f);
            registry.emplace<FireCooldownComp>(enemy, 2.0f);
            // registry.emplace<ScoreComp>(enemy, 1);
        }
    }

    void stage2_init(entt::registry &registry) {
        auto enemy_count = 8;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 3.0f);
            registry.emplace<ScoreComp>(enemy, 2);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 1.0f);
            registry.emplace<ScoreComp>(enemy, 1);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 2.0f);
            registry.emplace<FireCooldownComp>(enemy, 2.0f);
            // registry.emplace<ScoreComp>(enemy, 1);
        }
    }
}