#pragma once

#include "raylib.h"
#include "entt.hpp"
#include "vec2.hpp"

#include "components.hpp"
#include "screen/game_screen.hpp"
#include "draw.hpp"
#include "utils.hpp"

namespace pause_screen {
    using game_screen::GameScreenType;
    using game_screen::Screen;
    using game_screen::Transition;

    class PauseScreen : public Screen {
        entt::registry registry;

    public:
        PauseScreen() {

        }

        void load() {
            auto inner_box = registry.create();

            auto box = Rectangle();
            box.height = (float)half_screen_height();
            box.width = half_screen_width();
            center_rect_pos(box, glm::vec2(half_screen_width(), half_screen_height()));

            registry.emplace<RectangleComp>(inner_box, box.x, box.y, box.width, box.height);
            registry.emplace<ColorComp>(inner_box, WHITE);

            auto outer_box = registry.create();

            auto box2 = Rectangle();
            box2.height = half_screen_height() + 20.0f;
            box2.width = half_screen_width() + 20.0f;
            center_rect_pos(box2, glm::vec2(half_screen_width(), half_screen_height()));

            registry.emplace<RectangleComp>(outer_box, box2.x, box2.y, box2.width, box2.height);
            registry.emplace<ColorComp>(outer_box, BLACK);
        }

        void unload() {
            registry.clear();
        }

        auto update() -> Transition {
            auto transition = process_inputs();

            return transition;
        }

        void draw() {
            draw_color_rectangles(registry);

            auto pause_text = "Pause Menu";
            auto font_size = 40;

            DrawTextCentered(pause_text, (int)half_screen_width(), (int)half_screen_height(), font_size, BLACK);
        }

        auto id() -> GameScreenType {
            return GameScreenType::Pause;
        }

    private:
        auto process_inputs() -> Transition {
            if (IsKeyPressed(KEY_ESCAPE)) {
                return Transition::to(id(), GameScreenType::Gameplay);
            }
            if (IsKeyPressed(KEY_ENTER)) {
                return Transition::to(id(), GameScreenType::MainMenu);
            }
            return Transition::no_transition();
        }
    };
}
