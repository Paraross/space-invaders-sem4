#pragma once

#include "raylib.h"
#include "entt.hpp"
#include "vec2.hpp"

#include "other.hpp"
#include "screen/game_screen.hpp"
#include "draw.hpp"
#include "utils.hpp"

namespace pause_screen {
    using game_screen::GameScreenType;
    using game_screen::Screen;

    class PauseScreen : public Screen {
        entt::registry registry;

    public:
        PauseScreen() {
            
        }

        void load() {
            auto inner_box = registry.create();

            auto box = Rectangle();
            box.height = (float)GetScreenHeight() / 2.0f;
            box.width = (float)GetScreenWidth() / 2.0f;
            center_rect_pos(box, glm::vec2(GetScreenWidth() / 2, GetScreenHeight() / 2));

            registry.emplace<RectangleComp>(inner_box, box.x, box.y, box.width, box.height);
            registry.emplace<ColorComp>(inner_box, WHITE);

            auto outer_box = registry.create();

            auto box2 = Rectangle();
            box2.height = GetScreenHeight() / 2 + 20.0f;
            box2.width = GetScreenWidth() / 2 + 20.0f;
            center_rect_pos(box2, glm::vec2(GetScreenWidth() / 2, GetScreenHeight() / 2));

            registry.emplace<RectangleComp>(outer_box, box2.x, box2.y, box2.width, box2.height);
            registry.emplace<ColorComp>(outer_box, BLACK);
        }

        auto update() -> GameScreenType {
            auto next_screen = GameScreenType::Pause;

            next_screen = process_inputs();

            return next_screen;
        }

        void draw() {
            draw_color_rectangles(registry);

            auto pause_text = "Pause Menu";
            auto font_size = 40;

            DrawTextCentered(pause_text, GetScreenWidth() / 2, GetScreenHeight() / 2, font_size, BLACK);
        }

        auto id() -> GameScreenType {
            return GameScreenType::Pause;
        }

    private:
        auto process_inputs() -> GameScreenType {
            if (IsKeyPressed(KEY_ESCAPE)) {
                registry.clear();
                return GameScreenType::Gameplay;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                registry.clear();
                return GameScreenType::MainMenu;
            }
            return GameScreenType::Pause;
        }
    };
}
