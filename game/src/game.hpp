#pragma once

#include <string>
#include <sstream>

#include "raylib.h"
#include "entt.hpp"

#include "player.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "other.hpp"
#include "draw.hpp"
#include "event.hpp"

struct Game {
    entt::registry registry;
    Events events;

    Game() {
        //+ score
        auto score = registry.create();
        registry.emplace<TheScoreComp>(score);
        registry.emplace<ScoreComp>(score, 0);
        registry.emplace<TextComp>(score, "Score: ", glm::vec2(10.0f, 10.0f), 40, BLACK);

        //+ player
        auto player = registry.create();
        registry.emplace<PlayerComp>(player);
        registry.emplace<RectangleComp>(player, GetScreenWidth() / 2.0f, GetScreenHeight() - 90.0f, 40.0f, 40.0f);
        registry.emplace<VelocityComp>(player, glm::vec2(0.0f, 0.0f));
        registry.emplace<MaxSpeedComp>(player, 1000.0f);
        registry.emplace<FireCooldownComp>(player, 0.2f);
        registry.emplace<ColorComp>(player, BLACK);

        //+ enemies
        auto enemy_count = 10;

        // row 1
        for (size_t i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 3.0f);
            registry.emplace<ScoreComp>(enemy, 2);
        }

        enemy_count -= 1;

        // row 2
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 1.0f);
            registry.emplace<ScoreComp>(enemy, 1);
        }

        enemy_count += 1;

        // row 3
        for (auto i = 0; i < enemy_count; i++) {
            const auto space = (GetScreenWidth() - enemy_count * 75.0f) / (enemy_count + 1.0f);

            auto enemy = registry.create();
            registry.emplace<EnemyComp>(enemy);
            registry.emplace<RectangleComp>(enemy, (space * (float)(i + 1)) + 75.0f * (float)i, 30.0f + 50.0f + 30.0f + 50.0f + 30.0f, 75.0f, 50.0f);
            registry.emplace<ColorComp>(enemy, RED);
            registry.emplace<HealthComp>(enemy, 2.0f);
            registry.emplace<FireCooldownComp>(enemy, 2.0f);
            // registry.emplace<ScoreComp>(enemy, 1);
        }
    }

    void update() {
        update_score_text();
        update_player_movement();
        update_player_shooting();
        update_enemy_shooting();
        update_fire_cd();
        update_rectangle_position();
        update_on_screen_left_despawning();
        check_bullet_enemy_collisions();
        check_bullet_player_collisions();
        kill_with_no_health_score();
        kill_with_no_health();
        receive_enemy_hit_events();

        events.clear();
    }

    void receive_enemy_hit_events() {
        const auto text_color = BLACK;
        const auto font_size = 20;
        auto text_pos = glm::vec2(10.0f, 100.0f);

        auto texts = registry.view<TextComp, const EnemyHitEventTextComp>();

        auto &enemy_hit_events = events.get_enemy_hit_events();

        auto i = 0ull;
        for (auto [text_entity, text_comp] : texts.each()) {
            if (enemy_hit_events.size() <= i) {
                break;
            }

            auto event = enemy_hit_events[i];
            auto hit_pos = event.position;

            auto text_stream = std::stringstream();
            text_stream << "Hit pos: (" << hit_pos.x << ", " << hit_pos.y << ")";
            auto text = text_stream.str();

            registry.replace<TextComp>(text_entity, text, text_pos, font_size, text_color);

            text_pos.y += font_size + 5.0f;
            i++;
        }

        // for (auto &event : events.get_enemy_hit_events()) {
        while (i < enemy_hit_events.size()) {
            auto event = enemy_hit_events[i];
            auto hit_pos = event.position;

            auto text_stream = std::stringstream();
            text_stream << "Hit pos: (" << hit_pos.x << ", " << hit_pos.y << ")";
            auto text = text_stream.str();

            auto text_entity = registry.create();
            registry.emplace<TextComp>(text_entity, text, text_pos, font_size, text_color);
            registry.emplace<EnemyHitEventTextComp>(text_entity);

            text_pos.y += font_size + 5.0f;
            i++;
        }
    }

    void draw() {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        // auto text = std::to_string(GetMonitorRefreshRate(GetCurrentMonitor()));
        // auto text = std::to_string(player.fire_cooldown);
        // DrawText(text.c_str(), 10, 10, 40, BLACK);

        // DrawFPS(10, 10);

        draw_color_rectangles(registry);
        draw_text(registry);

        EndDrawing();
    }

    void update_player_movement() {
        auto player_view = registry.view<VelocityComp, const MaxSpeedComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &max_speed = player_view.get<MaxSpeedComp>(player).max_speed;
        auto speed = max_speed;

        auto &velocity = player_view.get<VelocityComp>(player).velocity;
        velocity = glm::vec2(0.0f, 0.0f);

        if (IsKeyDown(KEY_RIGHT)) {
            velocity.x = speed;
        }
        if (IsKeyDown(KEY_LEFT)) {
            velocity.x = -speed;
        }
        if (IsKeyDown(KEY_UP)) {
            velocity.y = -speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            velocity.y = speed;
        }
    }

    void update_player_shooting() {
        auto player_view = registry.view<FireCooldownComp, const RectangleComp, const PlayerComp>();
        auto player = player_view.front();

        assert(registry.valid(player));

        auto &fire_cd_comp = player_view.get<FireCooldownComp>(player);
        auto &fire_cd = fire_cd_comp.fire_cooldown;
        const auto &max_fire_cd = fire_cd_comp.max_fire_cooldown;

        if (!IsKeyDown((KEY_SPACE)) || fire_cd != 0.0f) {
            return;
        }

        auto &player_rect = player_view.get<RectangleComp>(player).rect;

        auto bullet_dims = glm::vec2(15.0f, 30.0f);
        auto bullet_pos = glm::vec2(player_rect.x + player_rect.width / 2.0f - bullet_dims.x / 2.0f, player_rect.y - bullet_dims.y);

        auto bullet = registry.create();
        registry.emplace<BulletComp>(bullet);
        registry.emplace<PlayerComp>(bullet);
        registry.emplace<RectangleComp>(bullet, bullet_pos.x, bullet_pos.y, bullet_dims.x, bullet_dims.y);
        registry.emplace<VelocityComp>(bullet, glm::vec2(0.0f, -1000.0f));
        registry.emplace<ColorComp>(bullet, BLUE);
        registry.emplace<DespawnOnScreenLeftComp>(bullet);
        registry.emplace<DamageComp>(bullet, 1.0f);

        fire_cd = max_fire_cd;
    }

    void update_enemy_shooting() {
        auto enemies = registry.view<FireCooldownComp, const RectangleComp, const EnemyComp>();

        for (auto [enemy, fire_cd_comp, rectangle] : enemies.each()) {
            auto &fire_cd = fire_cd_comp.fire_cooldown;
            auto &max_fire_cd = fire_cd_comp.max_fire_cooldown;
            auto &rect = rectangle.rect;

            if (fire_cd != 0.0f) {
                continue;
            }

            auto bullet = registry.create();
            registry.emplace<BulletComp>(bullet);
            registry.emplace<EnemyComp>(bullet);
            registry.emplace<RectangleComp>(bullet, rect.x, rect.y, 15.0f, 30.0f);
            registry.emplace<VelocityComp>(bullet, glm::vec2(0.0f, 500.0f));
            registry.emplace<ColorComp>(bullet, RED);
            registry.emplace<DespawnOnScreenLeftComp>(bullet);
            registry.emplace<DamageComp>(bullet, 1.0f);

            fire_cd = max_fire_cd;
        }
    }

    void update_fire_cd() {
        auto fire_cds = registry.view<FireCooldownComp>();

        for (auto [_, fire_cd_comp] : fire_cds.each()) {
            auto &fire_cd = fire_cd_comp.fire_cooldown;

            fire_cd -= GetFrameTime();
            
            if (fire_cd < 0.0f) {
                fire_cd = 0.0f;
            }
        }
    }

    void update_rectangle_position() {
        auto rectangles = registry.view<RectangleComp, const VelocityComp>();

        for (auto [_, rectangle, velocity] : rectangles.each()) {
            auto &rect = rectangle.rect;
            auto &vel = velocity.velocity;

            rect.x += vel.x * GetFrameTime();
            rect.y += vel.y * GetFrameTime();
        }
    }

    void update_on_screen_left_despawning() {
        auto entities = registry.view<RectangleComp, const DespawnOnScreenLeftComp>();

        for (auto [entity, rectangle] : entities.each()) {
            auto &rect = rectangle.rect;

            auto pos_x = rect.x;
            auto pos_y = rect.y;
            auto width = rect.width;
            auto height = rect.height;

            auto left_on_left = pos_x < -width;
            auto left_on_right = pos_x > GetScreenWidth() + width;
            auto left_on_top = pos_y < -height;
            auto left_on_bottom = pos_y > GetScreenHeight() + height;

            if (left_on_left || left_on_right || left_on_top || left_on_bottom) {
                registry.destroy(entity);
            }
        }
    }

    void check_bullet_enemy_collisions() {
        auto bullets = registry.view<RectangleComp, const DamageComp, const BulletComp, const PlayerComp>();

        for (auto [bullet, bullet_rect, bullet_damage] : bullets.each()) {
            auto enemies = registry.view<RectangleComp, HealthComp, const EnemyComp>();

            for (auto [enemy, enemy_rect, enemy_health] : enemies.each()) {
                auto collided = CheckCollisionRecs(bullet_rect.rect, enemy_rect.rect);

                if (collided) {
                    auto hit_pos = glm::vec2(bullet_rect.rect.x, bullet_rect.rect.y);

                    enemy_health.health -= bullet_damage.damage;

                    events.send_enemy_hit_event(EnemyHitEvent(hit_pos));

                    registry.destroy(bullet);
                    
                    break;
                }
            }
        }
    }

    void check_bullet_player_collisions() {
        auto bullets = registry.view<RectangleComp, const DamageComp, const BulletComp, const EnemyComp>();

        for (auto [bullet, bullet_rect, bullet_damage] : bullets.each()) {
            auto players = registry.view<RectangleComp, HealthComp, const PlayerComp>();

            for (auto [playere, player_rect, player_health] : players.each()) {
                auto collided = CheckCollisionRecs(bullet_rect.rect, player_rect.rect);

                if (collided) {
                    player_health.health -= bullet_damage.damage;

                    registry.destroy(bullet);
                    
                    break;
                }
            }
        }
    }

    void kill_with_no_health() {
        auto entities = registry.view<const HealthComp>();

        for (auto [entity, health] : entities.each()) {
            if (health.health <= 0.0f) {
                registry.destroy(entity);
            }
        }
    }

    void kill_with_no_health_score() {
        auto the_score_view = registry.view<ScoreComp, const TheScoreComp>();
        auto the_score_entity = the_score_view.front();
        auto &the_score = the_score_view.get<ScoreComp>(the_score_entity).score;

        auto entities = registry.view<const HealthComp, const ScoreComp>();

        for (auto [entity, health, score] : entities.each()) {
            if (health.health <= 0.0f) {
                the_score += score.score;
                registry.destroy(entity);
            }
        }
    }

    void update_score_text() {
        auto score_view = registry.view<TextComp, const ScoreComp, const TheScoreComp>();
        auto score_entity = score_view.front();

        auto &score = score_view.get<ScoreComp>(score_entity).score;
        auto &text = score_view.get<TextComp>(score_entity).text;

        text = "Score: " + std::to_string(score);
    }
};
