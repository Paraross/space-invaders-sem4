#include "raylib.h"

class Input {
    // prywatne structy? nie we wspaniałym języku zwanym c++
    enum class InputType {
        Keyboard,
        Mouse,
    };

    union KbOrM {
        KeyboardKey keyboard_key;
        MouseButton mouse_button;
    };

    InputType type;
    KbOrM input;

public:
    Input(KeyboardKey key)
        : type(InputType::Keyboard) {
        input.keyboard_key = key;
    }

    Input(MouseButton button)
        : type(InputType::Mouse) {
        input.mouse_button = button;
    }

    auto is_down() -> bool {
        return type == InputType::Keyboard ? IsKeyDown(input.keyboard_key) : IsMouseButtonDown(input.mouse_button);
    }
};

struct Keybinds {
    Input move_right;
    Input move_left;
    Input move_up;
    Input move_down;
    Input shoot;

    Keybinds()
        : move_right(KEY_RIGHT), move_left(KEY_LEFT), move_up(KEY_UP), move_down(KEY_DOWN), shoot(KEY_SPACE) {}
};
