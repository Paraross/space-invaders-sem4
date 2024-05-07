#pragma once

#include "entt.hpp"

#include "other.hpp"

void draw_color_rectangles(entt::registry &registry) {
    auto drawables = registry.view<const RectangleComp, const ColorComp>();

    for (const auto [_, rect, color] : drawables.each()) {
        DrawRectangleRec(rect.rect, color.color);
    }
}
