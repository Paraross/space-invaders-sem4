#pragma once

namespace game_screen {
    enum class GameScreenType {
        MainMenu,
        // Settings,
        Gameplay,
        Pause,
    };

    struct Screen {
        virtual void load() = 0;
        virtual void unload() = 0;
        virtual auto update() -> GameScreenType = 0;
        virtual void draw() = 0;
        virtual auto id() -> GameScreenType = 0;
    };

    enum class ScreenState {
        Active,
        Inactive,
        Unloaded,
    };
    
    class ScreenData {
        Screen *screen;
        ScreenState state;

    public:
        ScreenData() = default;
        
        ScreenData(Screen *screen) {
            this->screen = screen;
            this->state = ScreenState::Unloaded;
        }

        void activate() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            state = ScreenState::Active;
        }

        void load() {
            if (state == ScreenState::Unloaded) {
                screen->load();
                state = ScreenState::Inactive;
            }
        }

        void unload() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            screen->unload();
            state = ScreenState::Unloaded;
        }

        auto update() -> GameScreenType {
            if (state == ScreenState::Active) {
                return screen->update();
            }
            return screen->id();
        }

        void draw() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            screen->draw();
        }
    };
}
