#pragma once

#include <string>
#include <vector>

#include "raylib.h"

#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "other.h"

struct Game {
    entt::registry registry;

    Game() {
        //+ player
        auto player = registry.create();
        registry.emplace<PlayerComp>(player);
        registry.emplace<RectangleComp>(player, GetScreenWidth() / 2.0f, GetScreenHeight() - 90.0f, 40.0f, 40.0f);
        registry.emplace<MaxSpeedComp>(player, 1000.0f);
        registry.emplace<FireCooldownComp>(player, 0.2f);

        //+ enemies
        auto enemy_count = 10;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f, 75.0f, 50.0f);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
        }
    }

    void update() {
        this->update_player_movement();
        this->update_player_shooting();
        this->update_player_fire_cd();
        this->update_bullet_movement();
        this->update_bullet_despawning();
    }

    void draw() {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
        // auto text = std::to_string(player.fire_cooldown);
        // DrawText(text.c_str(), 10, 10, 40, BLACK);

        // DrawFPS(10, 10);

        auto player_view = registry.view<RectangleComp, PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto player_rect = player_view.get<RectangleComp>(player).rect;

        DrawRectangleRec(player_rect, BLACK); //todo make the color not hard-coded here

        auto enemy_view = registry.view<RectangleComp, EnemyComp>();

        for (auto &enemy : enemy_view) {
            auto enemy_rect = registry.get<RectangleComp>(enemy).rect;

            DrawRectangleRec(enemy_rect, RED); //todo same here
        }
        
        auto bullet_view = registry.view<RectangleComp, BulletComp>();

        for (auto &bullet : bullet_view) {
            auto bullet_rect = registry.get<RectangleComp>(bullet).rect;

            DrawRectangleRec(bullet_rect, BLUE); //todo same here
        }

        EndDrawing();
    }

    void update_player_movement() {
        auto player_view = registry.view<RectangleComp, const MaxSpeedComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &rect = player_view.get<RectangleComp>(player).rect;
        auto &max_speed = player_view.get<MaxSpeedComp>(player).max_speed;

        auto speed = max_speed * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) {
            rect.x += speed;
        }
        if (IsKeyDown(KEY_LEFT)) {
            rect.x -= speed;
        }
        if (IsKeyDown(KEY_UP)) {
            rect.y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            rect.y += speed;
        }
    }

    void update_player_shooting() {
        auto player_view = registry.view<RectangleComp, FireCooldownComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &fire_cd_comp = player_view.get<FireCooldownComp>(player);
        auto &fire_cd = fire_cd_comp.fire_cooldown;
        const auto &max_fire_cd = fire_cd_comp.max_fire_cooldown;

        if (!IsKeyDown((KEY_SPACE)) || fire_cd != 0.0f) {
            return;
        }

        auto player_rect = player_view.get<RectangleComp>(player).rect;

        auto bullet = registry.create();
        registry.emplace<BulletComp>(bullet);
        registry.emplace<RectangleComp>(bullet, player_rect.x, player_rect.y, 15.0f, 30.0f);

        fire_cd = max_fire_cd;
    }

    void update_player_fire_cd() {
        auto player_view = registry.view<RectangleComp, FireCooldownComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &fire_cd_comp = player_view.get<FireCooldownComp>(player);
        auto &fire_cd = fire_cd_comp.fire_cooldown;

        fire_cd -= GetFrameTime();
        
        if (fire_cd < 0.0f) {
            fire_cd = 0.0f;
        }
    }

    void update_bullet_movement() {
        auto bullet_speed = 1000.0f;

        auto bullet_view = registry.view<RectangleComp, const BulletComp>();

        for (auto &bullet : bullet_view) {
            auto &bullet_rect = bullet_view.get<RectangleComp>(bullet).rect;

            bullet_rect.y -= bullet_speed * GetFrameTime();
        }
    }

    void update_bullet_despawning() {
        //todo do this shit
    }
};
