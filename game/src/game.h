#pragma once

#include <string>

#include "raylib.h"

#include "player.h"

struct Game {
    Player player;

    Game()
        : player(Player()) {}

    void update() {
        this->update_player_movement();
    }

    void draw() {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
        // DrawText(text.c_str(), 10, 10, 40, BLACK);

        DrawFPS(10, 10);

        DrawRectangleRec(this->player.rect, this->player.color);

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
};
