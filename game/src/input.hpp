#include "raylib.h"

class Keybind {
    // prywatne structy? nie we wspaniałym języku zwanym c++
    enum class InputType {
        Keyboard,
        Mouse,
    };

    union KbOrM {
        KeyboardKey keyboard_key;
        MouseButton mouse_button;
    };

    InputType type[2];
    KbOrM input[2];

    auto is_in_state(bool (*fn)(int)) -> bool {
        auto first_in_state = type[0] == InputType::Keyboard ? fn(input[0].keyboard_key) : fn(input[0].mouse_button);
        auto second_in_state = type[1] == InputType::Keyboard ? fn(input[1].keyboard_key) : fn(input[1].mouse_button);

        return first_in_state || second_in_state;
    }

public:
    Keybind(KeyboardKey key) {
        type[0] = InputType::Keyboard;
        type[1] = InputType::Keyboard;
        input[0].keyboard_key = key;
        input[1].keyboard_key = KeyboardKey::KEY_NULL;
    }

    Keybind(MouseButton button) {
        type[0] = InputType::Mouse;
        type[1] = InputType::Keyboard;
        input[0].mouse_button = button;
        input[1].keyboard_key = KeyboardKey::KEY_NULL;
    }

    auto is_down() -> bool {
        return is_in_state(IsKeyDown);
    }

    auto is_up() -> bool {
        return is_in_state(IsKeyUp);
    }

    auto is_pressed() -> bool {
        return is_in_state(IsKeyPressed);
    }

    auto is_released() -> bool {
        return is_in_state(IsKeyReleased);
    }
};

struct Keybinds {
    Keybind move_right;
    Keybind move_left;
    Keybind move_up;
    Keybind move_down;
    Keybind shoot;

    Keybinds()
        : move_right(KEY_RIGHT), move_left(KEY_LEFT), move_up(KEY_UP), move_down(KEY_DOWN), shoot(KEY_SPACE) {}
};
