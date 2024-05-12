#pragma once

#include <vector>

#include "vec2.hpp"

/// @brief
/// Marks an event that has been processed and is to be deleted.
struct ProcessedEventComp {};

struct EnemyHitEvent {
    glm::vec2 position;
};

void send_enemy_hit_event(entt::registry &registry, glm::vec2 position) {
    auto event = registry.create();
    registry.emplace<EnemyHitEvent>(event, position);
}
