#include "raylib.h"

#include "game.hpp"

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Invaders");

    auto game = Game();

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while (!WindowShouldClose()) {
        game.update();
        game.draw();
    }

    CloseWindow();
}
