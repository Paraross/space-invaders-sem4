#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <ranges>
#include <exception>

#include "raylib.h"

namespace input {
    // auto string_to_input_type(std::string_view str) -> InputType;

    enum class InputType {
        Keyboard,
        Mouse,
    };

    auto string_to_input_type(std::string_view str) -> InputType {
        if (str == "K") {
            return InputType::Keyboard;
        } else if(str == "M") {
            return InputType::Mouse;
        } else {
            throw std::exception("Invalid str to convert to InputType.");
        }
    }

    class Input {
        friend class Keybind;

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
        }

        Keybinds(std::vector<Keybind> keybinds)
            : keybinds(keybinds) {}

        static auto default_keybinds() -> Keybinds {
            auto keybinds = std::vector<Keybind>();
            keybinds.resize((size_t)InputAction::EnumSize);

            keybinds[(size_t)InputAction::MoveRight] = Keybind(KEY_D, KEY_RIGHT);
            keybinds[(size_t)InputAction::MoveLeft] = Keybind(KEY_A, KEY_LEFT);
            keybinds[(size_t)InputAction::MoveUp] = Keybind(KEY_W, KEY_UP);
            keybinds[(size_t)InputAction::MoveDown] = Keybind(KEY_S, KEY_DOWN);
            keybinds[(size_t)InputAction::Shoot] = Keybind(MOUSE_BUTTON_LEFT, KEY_SPACE);

            return Keybinds(std::move(keybinds));
        }

        auto is_down(InputAction action) -> bool {
            return keybinds[(size_t)action].is_down();
        }

        auto is_up(InputAction action) -> bool {
            return keybinds[(size_t)action].is_up();
        }

        auto is_pressed(InputAction action) -> bool {
            return keybinds[(size_t)action].is_pressed();
        }

        auto is_released(InputAction action) -> bool {
            return keybinds[(size_t)action].is_released();
        }

        void read_from_file(const char *file_name) {
            auto file = std::ifstream(file_name);

            if (file.fail()) {
                auto new_keybinds_file_name = "keybinds.txt";

                std::cout << "--- keybinds file not found ---\n";
                std::cout << "--- assigning default keybinds ---\n";
                *this = default_keybinds();

                std::cout << "--- generating keybinds file ---\n";
                write_to_file(new_keybinds_file_name);
                std::cout << "--- generated keybinds file: \"" << new_keybinds_file_name << "\" ---\n";

                return;
            }

            std::cout << "--- keybinds file found ---\n";

            auto file_content = std::string(std::istreambuf_iterator<char>(file), {});

            auto lines = file_content
                | std::ranges::views::split('\n')
                | std::ranges::views::transform([](auto &&str) { return std::string_view(str); });

            for (auto [i, line] : lines | std::ranges::views::enumerate) {
                // skip last line
                if (line.empty()) {
                    break;
                }

                auto tokens = line
                    | std::ranges::views::split(' ')
                    | std::ranges::views::transform([](auto &&str) { return std::string_view(str); });

                auto token_count = std::ranges::distance(tokens);
                
                // ill-formed line
                if (token_count != 4) {
                    std::cout << "--- line " << i + 1 << ". is ill-formed ---\n";
                    std::cout << "--- \"" << line << "\" ---\n";

                    std::cout << "--- assigning default keybinds ---\n";
                    //todo default only the keybind corresponding to the improper line, leave rest
                    //todo make default keybinds into a vector or something
                    *this = default_keybinds();

                    return;
                }

                auto tokens_iter = tokens.begin();

                auto first_type = string_to_input_type(*tokens_iter);
                std::ranges::advance(tokens_iter, 1);
                auto first_value = std::stoi(std::string(*tokens_iter));
                std::ranges::advance(tokens_iter, 1);
                auto second_type = string_to_input_type(*tokens_iter);
                std::ranges::advance(tokens_iter, 1);
                auto second_value = std::stoi(std::string(*tokens_iter));

                auto input1 = Input(first_type == InputType::Keyboard ? Input((KeyboardKey)first_value) : Input((MouseButton)first_value));
                auto input2 = Input(second_type == InputType::Keyboard ? Input((KeyboardKey)second_value) : Input((MouseButton)second_value));

                auto keybind = Keybind(input1, input2);

                keybinds[i] = keybind;
            }

            std::cout << "--- keybinds successfully loaded from file: \"" << file_name << "\" ---\n";
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
}
