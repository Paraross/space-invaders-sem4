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
    using main_menu_screen::MainMenuScreen;
    using gameplay_screen::GameplayScreen;

    struct Game {
        // entt::registry registry;
        Keybinds keybinds;
        MainMenuScreen main_menu_screen;
        GameplayScreen gameplay_screen;
        GameScreen current_screen;

        Game() {
            init_inital_boot();
            // keybinds.write_to_file("keybinds.txt");
        }

        void init_inital_boot() {
            keybinds.read_from_file("keybinds.txt");

            current_screen = GameScreen::MainMenu;

            init_main_menu_screen();
        }

        void init_main_menu_screen() {
            main_menu_screen = MainMenuScreen();
        }
        
        void init_gameplay_screen() {
            gameplay_screen = GameplayScreen();
        }

        void update() {
            auto next_screen = current_screen;

            if (current_screen == GameScreen::MainMenu) {
                next_screen = main_menu_screen.update();
            } else if (current_screen == GameScreen::Gameplay) {
                next_screen = gameplay_screen.update();
            }

            if (next_screen != current_screen) {
                std::cout << "--- screen changed from " << (int)current_screen << " to " << (int)next_screen << " ---\n";
                if (next_screen == GameScreen::MainMenu) {
                    init_main_menu_screen();
                } else if (next_screen == GameScreen::Gameplay) {
                    init_gameplay_screen();
                }
            }

            current_screen = next_screen;
        }

        void draw() {
            BeginDrawing();

            ClearBackground(LIGHTGRAY);

            // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
            // auto text = std::to_string(player.fire_cooldown);
            // DrawText(text.c_str(), 10, 10, 40, BLACK);

            // DrawFPS(10, 10);

            if (current_screen == GameScreen::MainMenu) {
                main_menu_screen.draw();
            } else if (current_screen == GameScreen::Gameplay) {
                gameplay_screen.draw();
            }

            EndDrawing();
        }

    };
}
