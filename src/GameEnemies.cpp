#include "Game.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

void Game::spawnEnemy() {
    int validTextureIndices[5];
    int validTextureCount = 0;

    for (int i = 0; i < 5; i++) {
        if (enemyTexturesValid[i]) {
            validTextureIndices[validTextureCount] = i;
            validTextureCount++;
        }
    }

    if (validTextureCount == 0) {
        std::cerr << "[Error] Cannot spawn enemy: no enemy textures loaded\n";
        return;
    }

    int textureIndex = validTextureIndices[std::rand() % validTextureCount];
    Enemy e(enemyTextures[textureIndex], textureIndex);

    auto texSize = enemyTextures[e.textureIndex].getSize();
    float w = static_cast<float>(texSize.x);
    float h = static_cast<float>(texSize.y);
    e.sprite.setOrigin({ w / 2.f, h / 2.f });
    e.sprite.setScale({ 0.5f, 0.5f });

    if (e.textureIndex == 2 || e.textureIndex == 3 || e.textureIndex == 4) {
        e.sprite.setScale({ 0.5f, -0.5f });
    }

    // Use half the scaled size as radius for clamping.
    e.radius = (std::max(w, h) * 0.5f) / 2.f;

    float x = static_cast<float>(std::rand() % static_cast<int>(gameWidth - 60.f) + 30);
    e.sprite.setPosition({ x, -e.radius });

    e.phaseOffsetX  = static_cast<float>(std::rand() % 628) / 100.f;
    e.phaseOffsetY  = static_cast<float>(std::rand() % 628) / 100.f;
    e.zigzagSpeedX  = 80.f  + static_cast<float>(std::rand() % 100);
    e.zigzagSpeedY  = 25.f  + static_cast<float>(std::rand() % 40);
    e.speedY        = 30.f  + static_cast<float>(std::rand() % 30);
    e.baseY         = 80.f  + static_cast<float>(std::rand() % static_cast<int>(waveBoundaryY - 100.f));
    e.shootInterval = 1.5f  + static_cast<float>(std::rand() % 20) / 10.f;

    enemies.push_back(e);
    std::cout << "[Info] Enemy spawned (type " << e.textureIndex << ") at x=" << x
              << " baseY=" << e.baseY << "\n";
}
