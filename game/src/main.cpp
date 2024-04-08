#include "raylib.h"

#include "game.h"
// #include "player.h"

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

int main() {
    // auto player = Player::init(20.0f, 50.0f, 20.0f, 20.0f, BLACK);

    auto game = Game();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Invaders");

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while (!WindowShouldClose()) {
        game.update();
        game.draw();
    }

    CloseWindow();
}
