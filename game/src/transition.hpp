#pragma once

#include "game_screen.hpp"

namespace transition {
    using game_screen::GameScreenType;

    struct Transition {
        union TransData {
            int score;
            bool empty;
        };

        GameScreenType prev;
        GameScreenType next;
        TransData data;

        auto no_transition() -> bool {
            return prev == next;
        }
    };
}
