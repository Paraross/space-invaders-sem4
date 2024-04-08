#pragma once

#include <string>
#include <vector>

#include "raylib.h"

#include "player.h"
#include "enemy.h"
#include "bullet.h"

struct Game {
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    Game() {
        //+ player
        auto player = Player::at_position(GetScreenWidth() / 2.0f, GetScreenHeight() - 90.0f);

        this->player = player;

        //+ enemies
        auto enemy_count = 10;
        auto enemies = std::vector<Enemy>();
        // enemies.reserve(enemy_count);

        // row 1
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = Enemy::at_position((space * (float)(i + 1)) + 75.0f * (float)i, 30.0f);

            enemies.push_back(enemy);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = Enemy::at_position((space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f);

            enemies.push_back(enemy);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = Enemy::at_position((space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f);

            enemies.push_back(enemy);
        }

        this->enemies = enemies;
    }

    void update() {
        this->update_player_movement();
        this->update_player_shooting();
        this->update_bullet_movement();
        this->update_bullet_despawning();
    }

    void draw() {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
        // auto text = std::to_string(GetScreenWidth());
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
        if (!IsKeyDown((KEY_SPACE))) {
            return;
        }

        auto player_pos_x = this->player.rect.x;
        auto player_pos_y = this->player.rect.y;

        auto bullet = Bullet::at_position(player_pos_x, player_pos_y);

        this->bullets.push_back(bullet);
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
