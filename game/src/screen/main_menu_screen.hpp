#pragma once

#include "raylib.h"
#include "entt.hpp"

#include "screen/game_screen.hpp"
#include "utils.hpp"

namespace main_menu_screen {
    using game_screen::GameScreenType;
    using game_screen::Screen;
    using game_screen::Transition;

    class MainMenuScreen : public Screen {
    public:
        MainMenuScreen() {
            
        }

        void load() {

        }

        void unload() {
            
        }

        auto update() -> Transition {
            auto transition = process_inputs();

            return transition;
        }

        void draw() {
            DrawTextCentered("Main Menu", (int)half_screen_width(), (int)half_screen_height(), 40, BLACK);
        }

        auto id() -> GameScreenType {
            return GameScreenType::MainMenu;
        }

    private:
        auto process_inputs() -> Transition {
            if (IsKeyPressed(KEY_ENTER)) {
                return Transition::to(id(), GameScreenType::Gameplay);
            }
            return Transition::no_transition();
        }
    };
}
