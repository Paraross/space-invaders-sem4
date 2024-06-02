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
    using main_menu_screen::MainMenuScreen;
    using gameplay_screen::GameplayScreen;
    using pause_screen::PauseScreen;

    class Game {
        MainMenuScreen main_menu_screen;
        GameplayScreen gameplay_screen;
        PauseScreen pause_screen;
        GameScreenType current_screen;

        std::array<Screen *, 3> screen_ptrs;

    public:
        Game() {
            current_screen = GameScreenType::MainMenu;

            screen_ptrs[(size_t)GameScreenType::MainMenu] = &main_menu_screen;
            screen_ptrs[(size_t)GameScreenType::Gameplay] = &gameplay_screen;
            screen_ptrs[(size_t)GameScreenType::Pause] = &pause_screen;

            current_screen_ptr()->load();
        }

        void update() {
            auto next_screen = current_screen_ptr()->update();

            if (next_screen == current_screen) {
                return;
            }

            std::cout << "--- screen changed from " << (int)current_screen << " to " << (int)next_screen << " ---\n";

            // auto transition = Transition(current_screen, next_screen);
            // transition_to_other_screen(transition);

            transition_to_other_screen(next_screen);
        }

        void draw() {
            BeginDrawing();

            ClearBackground(LIGHTGRAY);

            // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
            // auto text = std::to_string(player.fire_cooldown);
            // DrawText(text.c_str(), 10, 10, 40, BLACK);

            // DrawFPS(10, 10);

            if (current_screen == GameScreenType::Pause) {
                gameplay_screen.draw();
            }
            current_screen_ptr()->draw();

            EndDrawing();
        }

    private:
        auto current_screen_ptr() -> Screen * {
            // either this or ifs
            return screen_ptrs[(size_t)current_screen];
        }

        void transition_to_other_screen(GameScreenType next_screen) {
            if (next_screen == GameScreenType::MainMenu) {
                main_menu_screen.load();
                gameplay_screen.unload();
                pause_screen.unload();
            } else if (next_screen == GameScreenType::Gameplay) {
                gameplay_screen.load();
                main_menu_screen.unload();
                pause_screen.unload();
            } else if (next_screen == GameScreenType::Pause) {
                pause_screen.load();
                main_menu_screen.unload();
                // don't unload gameplay
            } else {
                throw std::exception("Tried to change to invalid screen.");
            }

            current_screen = next_screen;
        }
    };
}
