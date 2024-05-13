#pragma once

namespace game_screen {
    enum class GameScreenType {
        MainMenu,
        // Settings,
        Gameplay,
        // GameplayPause,
    };

    struct Screen {
        virtual void load() = 0;
        virtual auto update() -> GameScreenType= 0;
        virtual void draw() = 0;
        virtual auto id() -> GameScreenType= 0;
    };
}
