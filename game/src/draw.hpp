#pragma once

#include "raylib.h"
#include "entt.hpp"

#include "components.hpp"

void draw_color_rectangles(entt::registry &registry) {
    auto drawables = registry.view<const RectangleComp, const ColorComp>();

    for (const auto [_, rect, color] : drawables.each()) {
        DrawRectangleRec(rect.rect, color.color);
    }
}

void draw_textures_with_rectangles(entt::registry &registry) {
    auto textures = registry.view<const TextureComp, const RectangleComp>();

    for (const auto [_, texture, rect] : textures.each()) {
        auto target_width = rect.rect.width;
        auto texture_width = texture.texture.width;

        auto scale = target_width / texture_width;

        auto pos_x = rect.rect.x;
        auto pos_y = rect.rect.y;

        DrawTextureEx(texture.texture, Vector2(pos_x, pos_y), 0.0f, scale, WHITE);
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
