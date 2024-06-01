#pragma once

#include "raylib.h"
#include "entt.hpp"

#include "other.hpp"

void draw_color_rectangles(entt::registry &registry) {
    auto drawables = registry.view<const RectangleComp, const ColorComp>();

    for (const auto [_, rect, color] : drawables.each()) {
        DrawRectangleRec(rect.rect, color.color);
    }
}

void draw_text(entt::registry &registry) {
    auto texts = registry.view<const TextComp>();

    for (const auto [_, text_comp] : texts.each()) {
        auto &text = text_comp.text;
        auto &position = text_comp.position;
        auto &font_size = text_comp.font_size;
        auto &color = text_comp.color;

        DrawText(text.c_str(), (int)position.x, (int)position.y, font_size, color);
    }
}
