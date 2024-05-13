#pragma once

namespace game_screen {
    enum class GameScreen {
        MainMenu,
        // Settings,
        Gameplay,
        // GameplayPause,
    };

    struct Screen {
        virtual auto update() -> GameScreen = 0;
        virtual void draw() = 0;
        virtual auto id() -> GameScreen = 0;
    };
}
