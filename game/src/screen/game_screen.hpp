#pragma once

// I hate this god-forsaken language so much I cannot even begin to describe it.

namespace game_screen {
    class Transition;

    enum class GameScreenType {
        MainMenu,
        // Settings,
        Gameplay,
        Pause,
        GameOver,
    };

    struct Screen {
        virtual void load() = 0;
        virtual void unload() = 0;
        virtual auto update() -> Transition = 0;
        virtual void draw() = 0;
        virtual auto id() -> GameScreenType = 0;
    };

    enum class ScreenState {
        Active,
        Inactive,
        Unloaded,
    };

    // if this class is not in this exact place, C++ has a stroke
    class Transition {
        union TransData {
            int score;
            bool empty;
        };

        GameScreenType prev; // unused; for theoretical extendability
        GameScreenType next;
        TransData data;

        Transition() = delete;

        Transition(GameScreenType current) {
            this->prev = current;
            this->next = current;
            this->data.empty = true;
        }

        Transition(GameScreenType prev, GameScreenType next) {
            this->prev = prev;
            this->next = next;
            this->data.empty = true;
        }

        Transition(GameScreenType prev, GameScreenType next, int score) {
            this->prev = prev;
            this->next = next;
            this->data.score = score;
        }

    public:
        static auto no_transition() -> Transition {
            return Transition(GameScreenType::MainMenu);
        }

        static auto to(GameScreenType from, GameScreenType to) -> Transition {
            return Transition(from, to);
        }

        static auto to_with_score(GameScreenType from, GameScreenType to, int score) -> Transition {
            return Transition(from, to, score);
        }

        auto is_transition() -> bool {
            return prev != next;
        }

        auto next_screen() -> GameScreenType {
            return next;
        }

        auto get_score() -> int {
            return data.score;
        }
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

        void deactivate() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            state = ScreenState::Inactive;
        }

        void load() {
            if (state == ScreenState::Unloaded) {
                screen->load();
            }
            state = ScreenState::Active;
        }

        void unload() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            screen->unload();
            state = ScreenState::Unloaded;
        }

        auto update() -> Transition {
            if (state == ScreenState::Active) {
                return screen->update();
            }
            return Transition::no_transition();
        }

        void draw() {
            if (state == ScreenState::Unloaded) {
                return;
            }
            screen->draw();
        }

        auto id() -> GameScreenType {
            return screen->id();
        }
    };
}
