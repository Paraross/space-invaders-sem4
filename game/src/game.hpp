#pragma once

#include <string>

#include "raylib.h"
#include "entt.hpp"

#include "player.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "other.hpp"
#include "draw.hpp"

struct Game {
    entt::registry registry;

    Game() {
        //+ player
        auto player = registry.create();
        registry.emplace<PlayerComp>(player);
        registry.emplace<RectangleComp>(player, GetScreenWidth() / 2.0f, GetScreenHeight() - 90.0f, 40.0f, 40.0f);
        registry.emplace<VelocityComp>(player, glm::vec2(0.0f, 0.0f));
        registry.emplace<MaxSpeedComp>(player, 1000.0f);
        registry.emplace<FireCooldownComp>(player, 0.2f);
        registry.emplace<ColorComp>(player, BLACK);

        //+ enemies
        auto enemy_count = 10;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
        }
    }

    void update() {
        update_player_movement();
        update_player_shooting();
        update_fire_cd();
        update_rectangle_position();
        update_on_screen_left_despawning();
    }

    void draw() {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
        // auto text = std::to_string(player.fire_cooldown);
        // DrawText(text.c_str(), 10, 10, 40, BLACK);

        // DrawFPS(10, 10);

        draw_color_rectangles(registry);

        EndDrawing();
    }

    void update_player_movement() {
        auto player_view = registry.view<VelocityComp, const MaxSpeedComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &max_speed = player_view.get<MaxSpeedComp>(player).max_speed;
        auto speed = max_speed;

        auto &velocity = player_view.get<VelocityComp>(player).velocity;
        velocity = glm::vec2(0.0f, 0.0f);

        if (IsKeyDown(KEY_RIGHT)) {
            velocity.x = speed;
        }
        if (IsKeyDown(KEY_LEFT)) {
            velocity.x = -speed;
        }
        if (IsKeyDown(KEY_UP)) {
            velocity.y = -speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            velocity.y = speed;
        }
    }

    void update_player_shooting() {
        auto player_view = registry.view<FireCooldownComp, const RectangleComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &fire_cd_comp = player_view.get<FireCooldownComp>(player);
        auto &fire_cd = fire_cd_comp.fire_cooldown;
        const auto &max_fire_cd = fire_cd_comp.max_fire_cooldown;

        if (!IsKeyDown((KEY_SPACE)) || fire_cd != 0.0f) {
            return;
        }

        auto &player_rect = player_view.get<RectangleComp>(player).rect;

        auto bullet = registry.create();
        registry.emplace<BulletComp>(bullet);
        registry.emplace<RectangleComp>(bullet, player_rect.x, player_rect.y, 15.0f, 30.0f);
        registry.emplace<VelocityComp>(bullet, glm::vec2(0.0f, -1000.0f));
        registry.emplace<ColorComp>(bullet, BLUE);
        registry.emplace<DespawnOnScreenLeftComp>(bullet);

        fire_cd = max_fire_cd;
    }

    void update_fire_cd() {
        auto fire_cds = registry.view<FireCooldownComp>();

        for (auto [_, fire_cd_comp] : fire_cds.each()) {
            auto &fire_cd = fire_cd_comp.fire_cooldown;

            fire_cd -= GetFrameTime();
            
            if (fire_cd < 0.0f) {
                fire_cd = 0.0f;
            }
        }
    }

    void update_rectangle_position() {
        auto rectangles = registry.view<RectangleComp, const VelocityComp>();

        for (auto [_, rectangle, velocity] : rectangles.each()) {
            auto &rect = rectangle.rect;
            auto &vel = velocity.velocity;

            rect.x += vel.x * GetFrameTime();
            rect.y += vel.y * GetFrameTime();
        }
    }

    void update_on_screen_left_despawning() {
        auto entities = registry.view<RectangleComp, const DespawnOnScreenLeftComp>();

        for (auto [entity, rectangle] : entities.each()) {
            auto &rect = rectangle.rect;

            auto pos_x = rect.x;
            auto pos_y = rect.y;
            auto width = rect.width;
            auto height = rect.height;

            auto left_on_left = pos_x < -width;
            auto left_on_right = pos_x > GetScreenWidth() + width;
            auto left_on_top = pos_y < -height;
            auto left_on_bottom = pos_y > GetScreenHeight() + height;

            if (left_on_left || left_on_right || left_on_top || left_on_bottom) {
                registry.destroy(entity);
            }
        }
    }
};
