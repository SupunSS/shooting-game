#include "Game.h"
#include <filesystem>
#include <iostream>
#include <string>

void Game::loadPlayerAssets() {
    const std::string shipPath = "assets/Ships/player_ship.png";
    if (!std::filesystem::exists(shipPath)) {
        std::cerr << "[Error] Asset not found: " << shipPath << '\n';
    } else {
        std::cout << "[Info] Found asset: " << shipPath << '\n';
    }

    sf::Image image;
    if (!image.loadFromFile(shipPath)) {
        std::cerr << "[Error] failed to load player ship image: " << shipPath << "\n";
    } else if (!playerTexture.loadFromImage(image)) {
        std::cerr << "[Error] failed to create texture from image\n";
    } else {
        std::cout << "[Info] Loaded player texture successfully\n";
        playerTexture.setSmooth(false);
        playerTextureValid = true;

        auto texSize = playerTexture.getSize();
        std::cout << "[Info] Texture size: " << texSize.x << "x" << texSize.y << "\n";

        unsigned iw = image.getSize().x;
        unsigned ih = image.getSize().y;
        if (iw > 0 && ih > 0) {
            auto px = image.getPixel(sf::Vector2u(iw / 2, ih / 2));
            playerSampledAlpha = px.a;
            std::cout << "[Info] Sampled pixel (r,g,b,a): "
                << (int)px.r << "," << (int)px.g << ","
                << (int)px.b << "," << (int)px.a << "\n";
        }

        playerSprite.setTexture(playerTexture, true);
        playerSprite.setScale({ 0.5f, 0.5f });

        float w = static_cast<float>(texSize.x);
        float h = static_cast<float>(texSize.y);
        float scale = 0.5f;

        playerSprite.setOrigin({ w / 2.f, h / 2.f });
        playerSprite.setPosition({ 240.f, 580.f });
        playerHalfSize = { (w * scale) / 2.f, (h * scale) / 2.f };

        debugRect.setSize({ w * scale, h * scale });
        debugRect.setOrigin({ (w * scale) / 2.f, (h * scale) / 2.f });
        debugRect.setPosition(playerSprite.getPosition());
        if (playerSampledAlpha == 0) debugRect.setFillColor(sf::Color::Yellow);
        else                         debugRect.setFillColor(sf::Color::Magenta);
    }
}

void Game::loadBulletAssets() {
    const std::string bulletPath = "assets/Blullet/bullet_orb_blue.png";
    if (!std::filesystem::exists(bulletPath)) {
        std::cerr << "[Error] Asset not found: " << bulletPath << '\n';
    } else if (!bulletTexture.loadFromFile(bulletPath)) {
        std::cerr << "[Error] failed to load bullet orb image: " << bulletPath << "\n";
    } else {
        bulletTexture.setSmooth(false);
        bulletTextureValid = true;
        std::cout << "[Info] Loaded bullet orb texture successfully\n";
    }
}

void Game::loadEnemyAssets() {
    const std::string enemyPaths[5] = {
        "assets/Ships/enemy_green_small.png",
        "assets/Ships/enemy_green_large.png",
        "assets/Ships/enemy_purple_small.png",
        "assets/Ships/enemy_purple_large.png",
        "assets/Ships/enemy_red_large.png"
    };

    for (int i = 0; i < 5; i++) {
        if (!std::filesystem::exists(enemyPaths[i])) {
            std::cerr << "[Error] Enemy asset not found: " << enemyPaths[i] << '\n';
        } else if (!enemyTextures[i].loadFromFile(enemyPaths[i])) {
            std::cerr << "[Error] Failed to load enemy texture: " << enemyPaths[i] << '\n';
        } else {
            enemyTextures[i].setSmooth(false);
            enemyTexturesValid[i] = true;
            std::cout << "[Info] Loaded enemy texture: " << enemyPaths[i] << '\n';
        }
    }
}

void Game::loadEnemyBulletAssets() {
    const std::string enemyBulletPath = "assets/Blullet/bullet_orb_red.png";
    if (!std::filesystem::exists(enemyBulletPath)) {
        std::cerr << "[Error] Enemy bullet asset not found: " << enemyBulletPath << '\n';
    } else if (!enemyBulletTexture.loadFromFile(enemyBulletPath)) {
        std::cerr << "[Error] Failed to load enemy bullet texture\n";
    } else {
        enemyBulletTexture.setSmooth(false);
        enemyBulletTextureValid = true;
        std::cout << "[Info] Loaded enemy bullet texture\n";
    }
}
