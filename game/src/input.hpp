#include "raylib.h"

enum class InputType {
    Keyboard,
    Mouse,
};

union KbOrM {
    KeyboardKey keyboard_key;
    MouseButton mouse_button;
};

class Input {
    friend class Keybind;

    InputType type;
    KbOrM input;

public:
    Input() {
        type = InputType::Keyboard;
        input.keyboard_key = KEY_NULL;
    }

    Input(KeyboardKey key) {
        type = InputType::Keyboard;
        input.keyboard_key = key;
    }

    Input(MouseButton button) {
        type = InputType::Mouse;
        input.mouse_button = button;
    }
};

class Keybind {
    Input inputs[2];

    auto is_in_state(bool (*fn)(int)) -> bool {
        auto &first = inputs[0];
        auto &second = inputs[1];

        auto first_in_state = first.type == InputType::Keyboard ? fn(first.input.keyboard_key) : fn(first.input.mouse_button);
        auto second_in_state = second.type == InputType::Keyboard ? fn(second.input.keyboard_key) : fn(second.input.mouse_button);

        return first_in_state || second_in_state;
    }

public:
    Keybind() {
        inputs[0] = Input();
        inputs[1] = Input();
    }

    Keybind(Input input) {
        inputs[0] = input;
        inputs[1] = Input();
    }

    Keybind(Input input1, Input input2) {
        inputs[0] = input1;
        inputs[1] = input2;
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
