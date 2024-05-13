#pragma once

#include "entt.hpp"

#include "screen/game_screen.hpp"

namespace main_menu_screen {
    using game_screen::GameScreenType;
    using game_screen::Screen;

    class MainMenuScreen : public Screen {
        entt::registry registry;

    public:
        MainMenuScreen() {
            
        }

        void load() {

        }

        auto update() -> GameScreenType {
            auto next_screen = GameScreenType::MainMenu;

            draw_main_menu_screen(registry);
            next_screen = process_inputs();

            return next_screen;
        }

        void draw() {
            draw_main_menu_screen(registry);
        }

        auto id() -> GameScreenType {
            return GameScreenType::MainMenu;
        }

    private:
        auto process_inputs() -> GameScreenType {
            if (IsKeyPressed(KEY_ENTER)) {
                return GameScreenType::Gameplay;
                // load_gameplay_screen();
            }
            return GameScreenType::MainMenu;
        }
    };
}
