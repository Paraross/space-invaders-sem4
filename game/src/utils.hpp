#pragma once

#include "raylib.h"
#include "glm.hpp"

void center_rect_pos(Rectangle &rect, glm::vec2 pos) {
    rect.x = pos.x - rect.width / 2.0f;
    rect.y = pos.y - rect.height / 2.0f;
}

auto centered_text_pos(const char *text, int pos_x, int pos_y, int font_size) -> glm::ivec2 {
    auto text_width = MeasureText(text, font_size);

    auto top_left_x = pos_x - text_width / 2;
    auto top_left_y = pos_y - font_size / 2;

    return glm::ivec2(top_left_x, top_left_y);
}

// nice random PascalCase
void DrawTextCentered(const char *text, int pos_x, int pos_y, int font_size, Color color) {
    auto centered_pos = centered_text_pos(text, pos_x, pos_y, font_size);

    DrawText(text, centered_pos.x, centered_pos.y, font_size, color);
}

auto half_screen_width() -> float {
    return GetScreenWidth() / 2.0f;
}

auto half_screen_height() -> float {
    return GetScreenHeight() / 2.0f;
}
