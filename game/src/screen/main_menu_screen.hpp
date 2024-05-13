#pragma once

#include "entt.hpp"

#include "screen/game_screen.hpp"

namespace main_menu_screen {
    using game_screen::GameScreen;
    using game_screen::Screen;

    class MainMenuScreen : public Screen {
        entt::registry registry;

    public:
        auto update() -> GameScreen {
            auto next_screen = GameScreen::MainMenu;

            draw_main_menu_screen(registry);
            next_screen = process_inputs();

            return next_screen;
        }

        void draw() {
            draw_main_menu_screen(registry);
        }

        auto id() -> GameScreen {
            return GameScreen::MainMenu;
        }

    private:
        auto process_inputs() -> GameScreen {
            if (IsKeyPressed(KEY_ENTER)) {
                return GameScreen::Gameplay;
                // load_gameplay_screen();
            }
            return GameScreen::MainMenu;
        }
    };
}
