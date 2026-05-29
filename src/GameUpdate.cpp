#include "Game.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void Game::update(float dt) {
    updateBackground(dt);

    // ---- Player movement ----
    sf::Vector2f move(0.f, 0.f);
    if (movingLeft)  move.x -= playerSpeed * dt;
    if (movingRight) move.x += playerSpeed * dt;
    if (movingUp)    move.y -= playerSpeed * dt;
    if (movingDown)  move.y += playerSpeed * dt;

    sf::Vector2f pos = playerSprite.getPosition() + move;
    pos.x = std::clamp(pos.x, playerHalfSize.x, gameWidth - playerHalfSize.x);
    pos.y = std::clamp(pos.y, playerHalfSize.y, gameHeight - playerHalfSize.y);
    playerSprite.setPosition(pos);

    // ---- Player bullets ----
    for (auto& b : bullets)
        b.shape.move({ 0.f, -b.speed * dt });

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.shape.getPosition().y < 0.f; }),
        bullets.end());

    // ---- Wave spawning ----
    if (!waveActive) {
        if (enemySpawnTimer.getElapsedTime().asSeconds() > enemySpawnInterval) {
            for (int i = 0; i < enemiesPerWave; i++)
                spawnEnemy();
            waveActive = true;
            enemySpawnTimer.restart();
            std::cout << "[Info] New wave spawned with " << enemiesPerWave << " enemies\n";
        }
    }

    // ---- Update enemies ----
    sf::Vector2f playerPos = playerSprite.getPosition();

    for (auto& e : enemies) {
        e.zigzagTime += dt;

        sf::Vector2f ePos = e.sprite.getPosition();
        float r = e.radius;

        if (!e.reachedBase) {
            ePos.y += e.speedY * dt;
            ePos.x += std::sin(e.zigzagTime * 1.5f + e.phaseOffsetX) * 60.f * dt;

            if (ePos.y >= e.baseY) {
                ePos.y = e.baseY;
                e.reachedBase = true;
            }
        } else {
            float zigzagX = std::sin(e.zigzagTime * 2.0f + e.phaseOffsetX) * e.zigzagSpeedX * dt;
            float oscillateY = std::sin(e.zigzagTime * 1.2f + e.phaseOffsetY) * e.zigzagSpeedY * dt;

            ePos.x += zigzagX;
            ePos.y += oscillateY;

            float minY = e.baseY - 45.f;
            float maxY = e.baseY + 45.f;
            if (maxY + r > waveBoundaryY) maxY = waveBoundaryY - r;
            if (minY < r)                 minY = r;
            ePos.y = std::clamp(ePos.y, minY, maxY);
        }

        ePos.x = std::clamp(ePos.x, r, gameWidth - r);
        e.sprite.setPosition(ePos);

        // Enemy shooting
        e.shootTimer += dt;
        if (e.shootTimer >= e.shootInterval) {
            e.shootTimer = 0.f;

            EnemyBullet eb(enemyBulletTexture);
            eb.radius = 5.f;

            if (enemyBulletTextureValid) {
                eb.sprite.setTexture(enemyBulletTexture, true);
                eb.sprite.setScale({ 0.4f, 0.4f });
                auto texSize = enemyBulletTexture.getSize();
                eb.sprite.setOrigin({
                    static_cast<float>(texSize.x) / 2.f,
                    static_cast<float>(texSize.y) / 2.f
                });
            }

            eb.sprite.setPosition(e.sprite.getPosition());

            sf::Vector2f dir = playerPos - e.sprite.getPosition();
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length > 0.f) {
                dir /= length;
            } else {
                dir = { 0.f, 1.f };
            }
            eb.velocity = dir * 300.f;

            enemyBullets.push_back(eb);
            std::cout << "[Info] Enemy fired!\n";
        }
    }

    // ---- Move enemy bullets ----
    for (auto& eb : enemyBullets)
        eb.sprite.move(eb.velocity * dt);

    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [&](const EnemyBullet& eb) {
            sf::Vector2f p = eb.sprite.getPosition();
            return p.y > gameHeight + 10.f ||
                   p.y < -10.f ||
                   p.x < -10.f ||
                   p.x > gameWidth + 10.f;
        }), enemyBullets.end());

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [&](const Enemy& e) {
            return e.sprite.getPosition().y > gameHeight + 30.f;
        }), enemies.end());

    // ---- Collision detection ----
    checkCollisions();

    // ---- Player invincibility timer after being hit ----
    if (playerInvincible) {
        invincibleTimer  += dt;
        playerFlashTimer += dt;

        bool flashOn = static_cast<int>(playerFlashTimer * 12.f) % 2 == 0;
        playerSprite.setColor(flashOn ? sf::Color::White : sf::Color(255, 255, 255, 90));

        if (invincibleTimer >= invincibleDuration) {
            playerInvincible = false;
            invincibleTimer = 0.f;
            playerFlashTimer = 0.f;
            playerSprite.setColor(sf::Color::White);
        }
    }

    // ---- Check if wave is cleared ----
    if (waveActive && enemies.empty()) {
        waveActive = false;
        enemySpawnTimer.restart();
        std::cout << "[Info] Wave cleared! Next wave in "
                  << enemySpawnInterval << "s\n";
    }
}

void Game::updateBackground(float dt) {
    float totalBackgroundHeight = 0.f;
    for (int i = 0; i < backgroundCount; i++) {
        if (backgroundTexturesValid[i]) {
            totalBackgroundHeight += getBackgroundHeight(backgroundTextures[i]);
        }
    }

    if (totalBackgroundHeight <= 0.f) {
        return;
    }

    backgroundScrollOffset += backgroundScrollSpeed * dt;
    backgroundScrollOffset = std::fmod(backgroundScrollOffset, totalBackgroundHeight);
}
