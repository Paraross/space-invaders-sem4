#pragma once

#include <string>
#include <sstream>
#include <array>

#include "raylib.h"
#include "entt.hpp"

#include "other.hpp"
#include "draw.hpp"
#include "event.hpp"
#include "input.hpp"
#include "screen/game_screen.hpp"
#include "screen/main_menu_screen.hpp"
#include "screen/gameplay_screen.hpp"
#include "screen/pause_screen.hpp"
#include "screen/game_over_screen.hpp"

namespace game {
    using input::Keybinds;
    using input::InputAction;
    using game_screen::GameScreenType;
    using game_screen::Screen;
    using game_screen::ScreenState;
    using game_screen::ScreenData;
    using game_screen::Transition;
    using main_menu_screen::MainMenuScreen;
    using gameplay_screen::GameplayScreen;
    using pause_screen::PauseScreen;
    using game_over_screen::GameOverScreen;

    class Game {
        MainMenuScreen main_menu_screen;
        GameplayScreen gameplay_screen;
        PauseScreen pause_screen;
        GameOverScreen game_over_screen;
        GameScreenType current_screen_type;

        std::array<ScreenData, 4> screen_ptrs;

        auto current_screen() -> ScreenData & {
            return screen_ptrs[(size_t)current_screen_type];
        }

        auto main_menu() -> ScreenData & {
            return screen_ptrs[(size_t)GameScreenType::MainMenu];
        }

        auto gameplay() -> ScreenData & {
            return screen_ptrs[(size_t)GameScreenType::Gameplay];
        }

        auto pause_menu() -> ScreenData & {
            return screen_ptrs[(size_t)GameScreenType::Pause];
        }

        auto game_over() -> ScreenData & {
            return screen_ptrs[(size_t)GameScreenType::GameOver];
        }

    public:
        Game() {
            current_screen_type = GameScreenType::MainMenu;

            main_menu() = ScreenData(&main_menu_screen);
            gameplay() = ScreenData(&gameplay_screen);
            pause_menu() = ScreenData(&pause_screen);
            game_over() = ScreenData(&game_over_screen);

            current_screen().load();
        }

        void run() {
            update();
            draw();
        }

    private:
        void update() {
            auto transition = current_screen().update();

            transition_screen(transition);
        }

        void draw() {
            BeginDrawing();

            ClearBackground(LIGHTGRAY);

            // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
            // auto text = std::to_string(player.fire_cooldown);
            // DrawText(text.c_str(), 10, 10, 40, BLACK);

            // DrawFPS(10, 10);

            for (auto screen : screen_ptrs) {
                screen.draw();
            }

            EndDrawing();
        }
        void transition_screen(Transition transition) {
            if (!transition.is_transition()) {
                return;
            }

            auto next_screen = transition.next_screen();

            if (next_screen == GameScreenType::MainMenu) {
                gameplay().unload();
                pause_menu().unload();
                game_over().unload();
            } else if (next_screen == GameScreenType::Gameplay) {
                main_menu().unload();
                pause_menu().unload();
            } else if (next_screen == GameScreenType::Pause) {
                gameplay().deactivate();
            } else if (next_screen == GameScreenType::GameOver) {
                game_over_screen.set_score(transition.get_score());
                gameplay().deactivate();
            }

            std::cout << "--- screen changed from " << (int)current_screen_type << " to " << (int)next_screen << " ---\n";

            current_screen_type = next_screen;
            current_screen().load();
        }
    };
}
