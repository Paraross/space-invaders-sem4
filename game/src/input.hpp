#pragma once

#include <vector>
#include <string>
#include <fstream>

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

    auto is_in_state(bool (*keyboard_fn)(int), bool (*mouse_fn)(int)) const -> bool {
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

    auto as_string() const -> std::string {
        auto type_str = type == InputType::Keyboard ? 'K' : 'M';
        auto input_str = type == InputType::Keyboard ? input.keyboard_key : input.mouse_button;

        auto text_stream = std::stringstream();
        text_stream << type_str << ' ' << input_str;
        auto text = text_stream.str();

        return text;
    }
};

class Keybind {
    Input inputs[2];

    auto is_in_state(bool (*keyboard_fn)(int), bool (*mouse_fn)(int)) const -> bool {
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

    auto is_down() const -> bool {
        return is_in_state(IsKeyDown, IsMouseButtonDown);
    }

    auto is_up() const -> bool {
        return is_in_state(IsKeyUp, IsMouseButtonUp);
    }

    auto is_pressed() const -> bool {
        return is_in_state(IsKeyPressed, IsMouseButtonPressed);
    }

    auto is_released() const -> bool {
        return is_in_state(IsKeyReleased, IsMouseButtonReleased);
    }

    auto as_string() const -> std::string {
        auto first_str = inputs[0].as_string();
        auto second_str = inputs[1].as_string();

        return first_str + " " + second_str;
    }
};

enum class InputAction {
    MoveRight = 0,
    MoveLeft,
    MoveUp,
    MoveDown,
    Shoot,
    EnumSize, // this is probalby the most illegal thing I have ever done
};

class Keybinds {
    std::vector<Keybind> keybinds; // hash map? nah, i'll pass

public:
    Keybinds() {
        keybinds.resize((size_t)InputAction::EnumSize);

        keybinds[(size_t)InputAction::MoveRight] = Keybind(KEY_D, KEY_RIGHT);
        keybinds[(size_t)InputAction::MoveLeft] = Keybind(KEY_A, KEY_LEFT);
        keybinds[(size_t)InputAction::MoveUp] = Keybind(KEY_W, KEY_UP);
        keybinds[(size_t)InputAction::MoveDown] = Keybind(KEY_S, KEY_DOWN);
        keybinds[(size_t)InputAction::Shoot] = Keybind(MOUSE_BUTTON_LEFT, KEY_SPACE);
    }

    auto action(InputAction action) const -> const Keybind & {
        return keybinds[(size_t)action];
    }

    void write_to_file(const char *file_name) const {
        auto file = std::ofstream(file_name);

        for (const auto &keybind : keybinds) {
            auto keybind_str = keybind.as_string();

            file << keybind_str << '\n';
        }

        file.close();
    }
};
