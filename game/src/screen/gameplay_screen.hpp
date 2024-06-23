#pragma once

#include <future>

#include "entt.hpp"

#include "other.hpp"
#include "input.hpp"
#include "screen/game_screen.hpp"
#include "stage/stage.hpp"

namespace gameplay_screen {
    using input::Keybinds;
    using input::InputAction;
    using game_screen::GameScreenType;
    using game_screen::Screen;
    using game_screen::Transition;
    using stage::StageManager;

    class GameplayScreen : public Screen {
        entt::registry registry;
        Keybinds keybinds;
        StageManager stage_manager;

    public:
        GameplayScreen() {
            keybinds.read_from_file("keybinds.txt");
        }

        void load() {
            auto score = registry.create();
            registry.emplace<TheScoreComp>(score);
            registry.emplace<ScoreComp>(score, 0);
            registry.emplace<TextComp>(score, "Score: ", glm::vec2(10.0f, 10.0f), 40, BLACK);

            //+ player
            auto player = registry.create();
            registry.emplace<PlayerComp>(player);
            registry.emplace<RectangleComp>(player, half_screen_width(), GetScreenHeight() - 90.0f, 65.0f, 65.0f);
            registry.emplace<VelocityComp>(player, glm::vec2(0.0f, 0.0f));
            registry.emplace<MaxSpeedComp>(player, 1000.0f);
            registry.emplace<FireCooldownComp>(player, 0.2f);
            auto texture = Texture2D();
            texture = LoadTexture("resources/player_ship.png");
            registry.emplace<TextureComp>(player, texture);
            registry.emplace<HealthComp>(player, 1.0f);

            stage_manager.reset_current_stage();
            stage_manager.init_next_stage(registry);
        }

        void unload() {
            registry.clear();
        }

        auto update() -> Transition {
            auto future = std::async(std::launch::async, [this]() {
                check_bullet_enemy_collisions();
                check_bullet_player_collisions();
            });

            update_player_movement();

            update_player_shooting();
            update_enemy_shooting();
            update_fire_cd();

            update_rectangle_position();
            update_on_screen_left_despawning();
            update_score_text();

            future.get();

            receive_enemy_hit_events();

            kill_with_no_health();
            destroy_processed_events();
            destroy_entities();
            manage_stage();

            auto transition = check_transition();

            return transition;
        }

        void draw() {
            draw_color_rectangles(registry);
            draw_textures_with_rectangles(registry);
            draw_text(registry);
        }

        auto id() -> GameScreenType {
            return GameScreenType::Gameplay;
        }

    private:
        auto player_died() -> bool {
            auto player_view = registry.view<VelocityComp, const MaxSpeedComp, const PlayerComp>();
            auto player = player_view.front();

            return !registry.valid(player);
        }

        auto get_score_value() -> int {
            auto score_view = registry.view<const ScoreComp, const TheScoreComp>();
            auto score_entity = score_view.front();
            auto &score = score_view.get<ScoreComp>(score_entity).score;

            return score;
        }

        // for a lack of a better name
        auto check_transition() -> Transition {
            if (player_died()) {
                return Transition::to_with_score(id(), GameScreenType::GameOver, get_score_value());
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                return Transition::to(id(), GameScreenType::Pause);
            }
            return Transition::no_transition();
        }

        void receive_enemy_hit_events() {
            const auto text_color = BLACK;
            const auto font_size = 20;
            auto text_pos = glm::vec2(10.0f, 100.0f);

            auto texts = registry.group<const TextComp, const EnemyHitEventTextComp>();
            auto text_count = texts.size();

            auto enemy_hit_events = registry.view<EnemyHitEvent>();

            for (auto [event_entity, event] : enemy_hit_events.each()) {
                auto hit_pos = event.position;

                text_pos.y += (font_size + 5.0f) * text_count;

                auto text_stream = std::stringstream();
                // text_stream << text_count << " Hit pos: (" << hit_pos.x << ", " << hit_pos.y << ")";
                auto text = text_stream.str();

                auto text_entity = registry.create();
                registry.emplace<TextComp>(text_entity, text, text_pos, font_size, text_color);
                registry.emplace<EnemyHitEventTextComp>(text_entity);

                registry.emplace<ProcessedEventComp>(event_entity);
            }
        }

        void manage_stage() {
            auto enemies = registry.view<const RectangleComp, const HealthComp, const EnemyComp>();

            if (enemies.size_hint() == 0) {
                stage_manager.init_next_stage(registry);
            }
        }

        void update_player_movement() {
            auto player_view = registry.view<VelocityComp, const MaxSpeedComp, const PlayerComp>();
            auto player = player_view.front();

            assert(registry.valid(player));

            auto &max_speed = player_view.get<MaxSpeedComp>(player).max_speed;
            auto speed = max_speed;

            auto &velocity = player_view.get<VelocityComp>(player).velocity;
            velocity = glm::vec2(0.0f, 0.0f);

            if (keybinds.is_down(InputAction::MoveRight)) {
                velocity.x += speed;
            }
            if (keybinds.is_down(InputAction::MoveLeft)) {
                velocity.x -= speed;
            }
            if (keybinds.is_down(InputAction::MoveUp)) {
                velocity.y -= speed;
            }
            if (keybinds.is_down(InputAction::MoveDown)) {
                velocity.y += speed;
            }
        }

        void update_player_shooting() {
            auto player_view = registry.view<FireCooldownComp, const RectangleComp, const PlayerComp>();
            auto player = player_view.front();

            assert(registry.valid(player));

            auto &fire_cd_comp = player_view.get<FireCooldownComp>(player);
            auto &fire_cd = fire_cd_comp.fire_cooldown;
            const auto &max_fire_cd = fire_cd_comp.max_fire_cooldown;

            if (!keybinds.is_down(InputAction::Shoot) || fire_cd != 0.0f) {
                return;
            }

            auto &player_rect = player_view.get<const RectangleComp>(player).rect; // TODO: const ?

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
                registry.emplace<RectangleComp>(bullet, rect.x, rect.y, 15.0f, 30.0f); // TODO: as above
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
                    registry.emplace<DestroyEntityComp>(entity);
                }
            }
        }

        void check_bullet_enemy_collisions() {
            auto bullets = registry.view<const RectangleComp, const DamageComp, const BulletComp, const PlayerComp>();

            for (auto [bullet, bullet_rect, bullet_damage] : bullets.each()) {
                auto enemies = registry.view<const RectangleComp, HealthComp, const EnemyComp>();

                for (auto [enemy, enemy_rect, enemy_health] : enemies.each()) {
                    auto collided = CheckCollisionRecs(bullet_rect.rect, enemy_rect.rect);

                    if (collided) {
                        auto hit_pos = glm::vec2(bullet_rect.rect.x, bullet_rect.rect.y);

                        enemy_health.health -= bullet_damage.damage;

                        EnemyHitEvent(hit_pos).send(registry);

                        registry.emplace<DestroyEntityComp>(bullet);
                        
                        break;
                    }
                }
            }
        }

        void check_bullet_player_collisions() {
            auto bullets = registry.view<const RectangleComp, const DamageComp, const BulletComp, const EnemyComp>();

            for (auto [bullet, bullet_rect, bullet_damage] : bullets.each()) {
                auto players = registry.view<const RectangleComp, HealthComp, const PlayerComp>();

                for (auto [playere, player_rect, player_health] : players.each()) {
                    auto collided = CheckCollisionRecs(bullet_rect.rect, player_rect.rect);

                    if (collided) {
                        player_health.health -= bullet_damage.damage;

                        registry.emplace<DestroyEntityComp>(bullet);
                        
                        break;
                    }
                }
            }
        }

        void kill_with_no_health() {
            auto the_score_view = registry.view<ScoreComp, const TheScoreComp>();
            auto the_score_entity = the_score_view.front();
            auto &the_score = the_score_view.get<ScoreComp>(the_score_entity).score;

            auto entities = registry.view<const HealthComp>();

            for (auto [entity, health] : entities.each()) {
                if (health.health > 0.0f) {
                    continue;
                }

                if (registry.all_of<ScoreComp>(entity)) {
                    auto score = registry.get<ScoreComp>(entity).score;
                    the_score += score;
                }

                registry.emplace<DestroyEntityComp>(entity);
            }
        }

        void destroy_entities() {
            auto entities = registry.view<const DestroyEntityComp>();

            for (auto [entity] : entities.each()) {
                registry.destroy(entity);
            }
        }

        void update_score_text() {
            auto score_view = registry.view<TextComp, const ScoreComp, const TheScoreComp>();
            auto score_entity = score_view.front();

            auto &score = score_view.get<ScoreComp>(score_entity).score;
            auto &text = score_view.get<TextComp>(score_entity).text;

            text = "Score: " + std::to_string(score);
        }

        void destroy_processed_events() {
            auto events = registry.view<const ProcessedEventComp>();

            for (auto [event_entity] : events.each()) {
                registry.emplace<DestroyEntityComp>(event_entity);
            }
        }
    };
}
