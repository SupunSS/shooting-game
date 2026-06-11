#include "Game.h"
#include <algorithm>
#include <cmath>
#include <iostream>

bool Game::circlesOverlap(sf::Vector2f posA, float rA,
                           sf::Vector2f posB, float rB) const {
    float dx     = posA.x - posB.x;
    float dy     = posA.y - posB.y;
    float radius = rA + rB;
    return (dx * dx + dy * dy) < (radius * radius);
}

void Game::checkCollisions() {
    sf::Vector2f playerPos    = playerSprite.getPosition();
    float        playerRadius = playerHalfSize.x * 0.8f;

    // ---- Player bullets vs enemies ----
    for (auto& b : bullets) {
        if (b.dead) continue;

        sf::Vector2f bPos = b.shape.getPosition();
        float        bR   = b.shape.getRadius();

        for (auto& e : enemies) {
            if (e.dead) continue;

            if (circlesOverlap(bPos, bR, e.sprite.getPosition(), e.radius)) {
                b.dead = true;
                e.health--;

                if (e.health <= 0) {
                    // Enemy dead — score depends on type
                    // type 0 (red) = 50, type 1 (large green/purple) = 30, type 2/3/4 = 20
                    e.dead = true;
                    int points = (e.textureIndex == 0) ? 50 :
                                 (e.textureIndex == 1) ? 30 : 20;
                    score += points;
                    e.sprite.setColor(sf::Color::White);
                    std::cout << "[Info] Enemy killed (type " << e.textureIndex
                              << ") +" << points << " Score: " << score << "\n";
                } else {
                    // Still alive — tint red proportional to damage taken
                    float ratio = static_cast<float>(e.health) /
                                  static_cast<float>(e.maxHealth);
                    uint8_t g  = static_cast<uint8_t>(255.f * ratio);
                    uint8_t b2 = g;
                    e.sprite.setColor(sf::Color(255, g, b2));
                    std::cout << "[Info] Enemy hit (type " << e.textureIndex
                              << ") hp=" << e.health << "/" << e.maxHealth << "\n";
                }
                break;
            }
        }
    }

    // ---- Enemy bullets vs player ----
    if (!playerInvincible) {
        for (auto& eb : enemyBullets) {
            if (eb.dead) continue;

            if (circlesOverlap(eb.sprite.getPosition(), eb.radius,
                               playerPos, playerRadius)) {
                eb.dead = true;
                playerHealth--;
                playerInvincible = true;
                invincibleTimer  = 0.f;
                playerFlashTimer = 0.f;
                std::cout << "[Info] Player hit by bullet! Health: "
                          << playerHealth << "\n";
                break;
            }
        }
    }

    // ---- Enemy vs player (ramming) ----
    if (!playerInvincible) {
        for (auto& e : enemies) {
            if (e.dead) continue;

            if (circlesOverlap(e.sprite.getPosition(), e.radius,
                               playerPos, playerRadius)) {
                e.dead = true;
                playerHealth--;
                playerInvincible = true;
                invincibleTimer  = 0.f;
                playerFlashTimer = 0.f;
                std::cout << "[Info] Player hit by enemy! Health: "
                          << playerHealth << "\n";
                break;
            }
        }
    }

    // ---- Remove dead objects ----
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b)      { return b.dead;  }), bullets.end());

    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [](const EnemyBullet& eb){ return eb.dead; }), enemyBullets.end());

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e)       { return e.dead;  }), enemies.end());
}