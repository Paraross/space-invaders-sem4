#pragma once

#include "raylib.h"
#include "entt.hpp"

#include "screen/game_screen.hpp"
#include "utils.hpp"

namespace main_menu_screen {
    using game_screen::GameScreenType;
    using game_screen::Screen;

    class MainMenuScreen : public Screen {
    public:
        MainMenuScreen() {
            
        }

        void load() {

        }

        void unload() {
            
        }

        auto update() -> GameScreenType {
            auto next_screen = GameScreenType::MainMenu;

            next_screen = process_inputs();

            return next_screen;
        }

        void draw() {
            DrawTextCentered("Main Menu", (int)half_screen_width(), (int)half_screen_height(), 40, BLACK);
        }

        auto id() -> GameScreenType {
            return GameScreenType::MainMenu;
        }

    private:
        auto process_inputs() -> GameScreenType {
            if (IsKeyPressed(KEY_ENTER)) {
                return GameScreenType::Gameplay;
            }
            return GameScreenType::MainMenu;
        }
    };
}
