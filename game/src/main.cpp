#include "raylib.h"

#include "game.h"

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

int main() {
    auto game = Game();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Invaders");

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while (!WindowShouldClose()) {
        game.update();
        game.draw();
    }

    CloseWindow();
}
