#pragma once

#include <vector>

#include "vec2.hpp"

struct EnemyHitEvent {
    glm::vec2 position;
};

class Events {
    std::vector<EnemyHitEvent> enemy_hit_events;

public:
    Events() {
        enemy_hit_events.reserve(10);

    }

    auto get_enemy_hit_events() -> const std::vector<EnemyHitEvent> & {
        return enemy_hit_events;
    }

    void send_enemy_hit_event(EnemyHitEvent event) {
        enemy_hit_events.push_back(event);
    }

    void clear() {
        enemy_hit_events.clear();
    }
};
