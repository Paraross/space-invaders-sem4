#pragma once

#include <vector>

#include "entt.hpp"

#include "components.hpp"
#include "stage/stages.hpp"

namespace stage {
    class StageManager {
        using Stage = void (*)(entt::registry &);

        std::vector<Stage> stages;
        size_t current_stage;

    public:
        StageManager() {
            this->current_stage = 0;
            this->stages.push_back(stage0_init);
            this->stages.push_back(stage1_init);
            this->stages.push_back(stage2_init);
        }

        void init_next_stage(entt::registry &registry) {
            stages[current_stage](registry);
            if (current_stage != stages.size() - 1) {
                current_stage++;
            }
        }

        void reset_current_stage() {
            current_stage = 0;
        }
    };
}