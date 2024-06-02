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

    enum class ScreenState {
        Active,
        Inactive,
        Unloaded,
    };
    
    struct ScreenData {
        Screen *screen;
        ScreenState state;

        ScreenData() = default;
        
        ScreenData(Screen *screen) {
            this->screen = screen;
            this->state = ScreenState::Unloaded;
        }

        void activate() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            state = ScreenState::Active;
        }

        void load() {
            if (state == ScreenState::Unloaded) {
                screen->load();
                state = ScreenState::Inactive;
            }
        }

        void unload() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            screen->unload();
            state = ScreenState::Unloaded;
        }

        auto update() -> GameScreenType {
            if (state == ScreenState::Active) {
                return screen->update();
            }
            return screen->id();
        }

        void draw() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            screen->draw();
        }
    };

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
            current_screen_ptr().activate();
        }

        void update() {
            auto next_screen = current_screen_ptr().update();

            transition_screen(next_screen);
        }

        void draw() {
            BeginDrawing();

            ClearBackground(LIGHTGRAY);

            // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
            // auto text = std::to_string(player.fire_cooldown);
            // DrawText(text.c_str(), 10, 10, 40, BLACK);

            // DrawFPS(10, 10);

            // if (current_screen == GameScreenType::Pause) {
            //     gameplay_screen.draw();
            // }
            // current_screen_ptr().draw();

            for (auto screen : screen_ptrs) {
                screen.draw();
            }

            EndDrawing();
        }

    private:
        void transition_screen(GameScreenType next_screen) {
            if (next_screen == current_screen) {
                return;
            }

            if (next_screen == GameScreenType::MainMenu) {
                main_menu().load();
                gameplay().unload();
                pause_menu().unload();
            } else if (next_screen == GameScreenType::Gameplay) {
                gameplay().load();
                main_menu().unload();
                pause_menu().unload();
            } else if (next_screen == GameScreenType::Pause) {
                pause_menu().load();
                main_menu().unload();
                // don't unload gameplay
            } else {
                throw std::exception("Tried to change to invalid screen.");
            }

            current_screen = next_screen;
            current_screen_ptr().activate();

            std::cout << "--- screen changed from " << (int)current_screen << " to " << (int)next_screen << " ---\n";
        }
    };
}
