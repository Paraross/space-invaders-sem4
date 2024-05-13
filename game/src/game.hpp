#pragma once

#include <string>
#include <sstream>

#include "raylib.h"
#include "entt.hpp"

#include "other.hpp"
#include "draw.hpp"
#include "event.hpp"
#include "input.hpp"
#include "screen/game_screen.hpp"
#include "screen/main_menu_screen.hpp"
#include "screen/gameplay_screen.hpp"

namespace game {
    using input::Keybinds;
    using input::InputAction;
    using game_screen::GameScreen;
    using game_screen::Screen;
    using main_menu_screen::MainMenuScreen;
    using gameplay_screen::GameplayScreen;

    class Game {
        MainMenuScreen main_menu_screen;
        GameplayScreen gameplay_screen;
        Screen *current_screen;

    public:
        Game() {
            current_screen = &main_menu_screen;
            current_screen->load();
        }

        void update() {
            auto next_screen_id = current_screen->update();

            // load next screen
            // unload prev screen
            if (next_screen_id != current_screen->id()) {
                std::cout << "--- screen changed from " << (int)current_screen->id() << " to " << (int)next_screen_id << " ---\n";
                if (next_screen_id == GameScreen::MainMenu) {
                    main_menu_screen.load();
                } else if (next_screen_id == GameScreen::Gameplay) {
                    gameplay_screen.load();
                }
            }

            change_current_screen(next_screen_id);
        }

        void draw() {
            BeginDrawing();

            ClearBackground(LIGHTGRAY);

            // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
            // auto text = std::to_string(player.fire_cooldown);
            // DrawText(text.c_str(), 10, 10, 40, BLACK);

            // DrawFPS(10, 10);

            current_screen->draw();

            EndDrawing();
        }

    private:
        void change_current_screen(GameScreen next_screen_id) {
            if (next_screen_id == GameScreen::MainMenu) {
                current_screen = &main_menu_screen;
            } else if (next_screen_id == GameScreen::Gameplay) {
                current_screen = &gameplay_screen;
            } else {
                throw std::exception("Tried to change to invalid screen.");
            }
        }
    };
}
