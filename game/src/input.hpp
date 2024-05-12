#include "raylib.h"

class Input {
    friend class Keybind;

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

    auto is_in_state(bool (*keyboard_fn)(int), bool (*mouse_fn)(int)) -> bool {
        return type == InputType::Keyboard ? keyboard_fn(input.keyboard_key) : mouse_fn(input.mouse_button);
    }

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

    auto is_in_state(bool (*keyboard_fn)(int), bool (*mouse_fn)(int)) -> bool {
        auto first_in_state = inputs[0].is_in_state(keyboard_fn, mouse_fn);
        auto second_in_state = inputs[1].is_in_state(keyboard_fn, mouse_fn);

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
        return is_in_state(IsKeyDown, IsMouseButtonDown);
    }

    auto is_up() -> bool {
        return is_in_state(IsKeyUp, IsMouseButtonUp);
    }

    auto is_pressed() -> bool {
        return is_in_state(IsKeyPressed, IsMouseButtonPressed);
    }

    auto is_released() -> bool {
        return is_in_state(IsKeyReleased, IsMouseButtonReleased);
    }
};

struct Keybinds {
    Keybind move_right;
    Keybind move_left;
    Keybind move_up;
    Keybind move_down;
    Keybind shoot;

    Keybinds() {
        move_right = Keybind(KEY_D, KEY_RIGHT);
        move_left = Keybind(KEY_A, KEY_LEFT);
        move_up = Keybind(KEY_W, KEY_UP);
        move_down = Keybind(KEY_S, KEY_DOWN);
        shoot = Keybind(MOUSE_BUTTON_LEFT, KEY_SPACE);
    }
};
