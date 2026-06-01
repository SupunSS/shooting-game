#include "Game.h"
#include <iostream>
#include <algorithm>

void Game::updateBackground(float dt) {
    backgroundScrollOffset += backgroundScrollSpeed * dt;
}

void Game::update(float dt) {
    if (gameState == GameState::MainMenu || gameState == GameState::GameOver) {
        return;
    }

    if (gameState != GameState::Playing) {
        return;
    }

    // Update background
    updateBackground(dt);

    // Update player position based on input
    sf::Vector2f playerPos = playerSprite.getPosition();
    if (movingLeft)  playerPos.x -= playerSpeed * dt;
    if (movingRight) playerPos.x += playerSpeed * dt;
    if (movingUp)    playerPos.y -= playerSpeed * dt;
    if (movingDown)  playerPos.y += playerSpeed * dt;

    // Clamp player to game boundaries
    if (playerPos.x - playerHalfSize.x < 0.f) playerPos.x = playerHalfSize.x;
    if (playerPos.x + playerHalfSize.x > gameWidth) playerPos.x = gameWidth - playerHalfSize.x;
    if (playerPos.y - playerHalfSize.y < 0.f) playerPos.y = playerHalfSize.y;
    if (playerPos.y + playerHalfSize.y > gameHeight) playerPos.y = gameHeight - playerHalfSize.y;

    playerSprite.setPosition(playerPos);

    // Update player invincibility
    if (playerInvincible) {
        invincibleTimer -= dt;
        if (invincibleTimer <= 0.f) {
            playerInvincible = false;
        }
    }

    // Spawn enemies periodically
    if (!waveActive) {
        waveActive = true;
    }

    if (waveActive) {
        float spawnElapsed = enemySpawnTimer.getElapsedTime().asSeconds();
        if (spawnElapsed > enemySpawnInterval) {
            spawnEnemy();
            enemySpawnTimer.restart();
        }
    }

    // Update enemies
    for (auto& enemy : enemies) {
        if (enemy.dead) continue;

        enemy.zigzagTime += dt;
        float sinVal = std::sin(enemy.zigzagTime * enemy.zigzagSpeedX + enemy.phaseOffsetX);
        float cosVal = std::cos(enemy.zigzagTime * enemy.zigzagSpeedY + enemy.phaseOffsetY);

        enemy.sprite.move({ sinVal * 50.f * dt, enemy.speedY * dt });

        if (enemy.sprite.getPosition().y > waveBoundaryY && !enemy.reachedBase) {
            enemy.reachedBase = true;
        }

        if (enemy.sprite.getPosition().y > gameHeight + 50.f) {
            enemy.dead = true;
        }

        // Enemy shooting
        enemy.shootTimer += dt;
        if (enemy.shootTimer >= enemy.shootInterval) {
            enemy.shootTimer = 0.f;
            if (enemyBulletTextureValid || true) {
                EnemyBullet eb(enemyBulletTexture);
                eb.sprite.setPosition(enemy.sprite.getPosition());
                enemyBullets.push_back(eb);
            }
        }
    }

    // Update enemy bullets
    for (auto& eb : enemyBullets) {
        if (eb.dead) continue;
        eb.sprite.move(eb.velocity * dt);
        if (eb.sprite.getPosition().y > gameHeight) {
            eb.dead = true;
        }
    }

    // Update player bullets
    for (auto& b : bullets) {
        if (b.dead) continue;
        b.shape.move({ 0.f, -b.speed * dt });
        if (b.shape.getPosition().y < 0.f) {
            b.dead = true;
        }
    }

    // Check collisions
    checkCollisions();

    // Remove dead entities
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return e.dead; }), enemies.end());
    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [](const EnemyBullet& eb) { return eb.dead; }), enemyBullets.end());
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.dead; }), bullets.end());

    // Check game over condition
    if (playerHealth <= 0) {
        gameState = GameState::GameOver;
        finalScore = score;
    }
}