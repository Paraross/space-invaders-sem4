#pragma once

#include "vec2.hpp"

/// @brief
/// Marks an event that has been processed and is to be deleted.
struct ProcessedEventComp {};

struct Event {
    virtual void send(entt::registry &registry) = 0;
};

struct EnemyHitEvent : public Event {
    glm::vec2 position;

    EnemyHitEvent(glm::vec2 position) {
        this->position = position;
    }

    void send(entt::registry &registry) {
        auto event = registry.create();
        registry.emplace<EnemyHitEvent>(event, position);
    }
};
