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

        //+ enemies
        auto enemy_count = 10;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<PositionComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<PositionComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<PositionComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f);
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

        DrawRectangleRec(this->player.rect, this->player.color);

        for (auto &enemy: this->enemies) {
            DrawRectangleRec(enemy.rect, enemy.color);
        }

        for (auto &bullet: this->bullets) {
            DrawRectangleRec(bullet.rect, bullet.color);
        }

        EndDrawing();
    }

    void update_player_movement() {
        auto speed = player.max_speed * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) {
            player.rect.x += speed;
        }
        if (IsKeyDown(KEY_LEFT)) {
            player.rect.x -= speed;
        }
        if (IsKeyDown(KEY_UP)) {
            player.rect.y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            player.rect.y += speed;
        }
    }

    void update_player_shooting() {
        if (!IsKeyDown((KEY_SPACE)) || this->player.fire_cooldown != 0.0f) {
            return;
        }

        auto player_pos_x = this->player.rect.x;
        auto player_pos_y = this->player.rect.y;

        auto bullet = Bullet::at_position(player_pos_x, player_pos_y);

        this->bullets.push_back(bullet);
        this->player.fire_cooldown = this->player.max_fire_cooldown;
    }

    void update_player_fire_cd() {
        auto &fire_cd = this->player.fire_cooldown;

        fire_cd -= GetFrameTime();
        
        if (fire_cd < 0.0f) {
            fire_cd = 0.0f;
        }
    }

    void update_bullet_movement() {
        auto bullet_speed = 1000.0f;

        for (auto &bullet : this->bullets) {
            bullet.rect.y -= bullet_speed * GetFrameTime();
        }
    }

    void update_bullet_despawning() {
        //todo do this shit
    }
};
