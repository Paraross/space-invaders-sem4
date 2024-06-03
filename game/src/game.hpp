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

    class Game {
        MainMenuScreen main_menu_screen;
        GameplayScreen gameplay_screen;
        PauseScreen pause_screen;
        GameScreenType current_screen;

        std::array<ScreenData, 3> screen_ptrs;

        auto current_screen_ptr() -> ScreenData & {
            return screen_ptrs[(size_t)current_screen];
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
    public:
        Game() {
            current_screen = GameScreenType::MainMenu;

            main_menu() = ScreenData(&main_menu_screen);
            gameplay() = ScreenData(&gameplay_screen);
            pause_menu() = ScreenData(&pause_screen);

            current_screen_ptr().load();
        }

        void update() {
            // TODO: change this whole shit, loop unnecessary
            for (auto &screen : screen_ptrs) {
                auto transition = screen.update();
                if (transition.is_transition()) {
                    transition_screen(transition);
                    break;
                }
            }
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

    private:
        void transition_screen(Transition transition) {
            if (!transition.is_transition()) {
                return;
            }

            auto next_screen = transition.next_screen();

            if (next_screen == GameScreenType::MainMenu) {
                gameplay().unload();
                pause_menu().unload();
            } else if (next_screen == GameScreenType::Gameplay) {
                main_menu().unload();
                pause_menu().unload();
            } else if (next_screen == GameScreenType::Pause) {
                main_menu().unload();
                gameplay().deactivate();
            } else {
                throw std::exception("Tried to change to invalid screen.");
            }

            std::cout << "--- screen changed from " << (int)current_screen << " to " << (int)next_screen << " ---\n";

            current_screen = next_screen;
            current_screen_ptr().load();
        }
    };
}
