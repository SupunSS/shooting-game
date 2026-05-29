#include "Game.h"
#include <algorithm>
#include <string>

float Game::getBackgroundScale(const sf::Texture& texture) const {
    sf::Vector2u textureSize = texture.getSize();
    if (textureSize.x == 0) {
        return 1.f;
    }

    return gameWidth / static_cast<float>(textureSize.x);
}

float Game::getBackgroundHeight(const sf::Texture& texture) const {
    sf::Vector2u textureSize = texture.getSize();
    return static_cast<float>(textureSize.y) * getBackgroundScale(texture);
}

void Game::drawBackground() {
    float totalBackgroundHeight = 0.f;
    for (int i = 0; i < backgroundCount; i++) {
        if (backgroundTexturesValid[i]) {
            totalBackgroundHeight += getBackgroundHeight(backgroundTextures[i]);
        }
    }

    if (totalBackgroundHeight <= 0.f) {
        return;
    }

    float y = backgroundScrollOffset - totalBackgroundHeight;
    while (y < gameHeight) {
        for (int i = 0; i < backgroundCount && y < gameHeight; i++) {
            if (!backgroundTexturesValid[i]) {
                continue;
            }

            sf::Texture& texture = backgroundTextures[i];
            float scale = getBackgroundScale(texture);
            float height = getBackgroundHeight(texture);

            if (y + height > 0.f) {
                sf::Sprite background(texture);
                background.setScale({ scale, scale });
                background.setPosition({ 0.f, y });
                window.draw(background);
            }

            y += height;
        }
    }
}

void Game::drawGlowBullet(const Bullet& b) {
    sf::CircleShape outer = b.shape;
    outer.setRadius(b.shape.getRadius() * 3.0f);
    outer.setOrigin({ b.shape.getRadius() * 3.0f, b.shape.getRadius() * 3.0f });
    outer.setPosition(b.shape.getPosition());
    outer.setTexture(nullptr);
    outer.setFillColor(sf::Color(100, 180, 255, 40));
    window.draw(outer, sf::BlendAdd);

    sf::CircleShape mid = b.shape;
    mid.setRadius(b.shape.getRadius() * 1.8f);
    mid.setOrigin({ b.shape.getRadius() * 1.8f, b.shape.getRadius() * 1.8f });
    mid.setPosition(b.shape.getPosition());
    mid.setTexture(nullptr);
    mid.setFillColor(sf::Color(150, 210, 255, 110));
    window.draw(mid, sf::BlendAdd);

    sf::CircleShape core = b.shape;
    core.setFillColor(sf::Color(255, 255, 255, 255));
    window.draw(core);
}

void Game::drawGlowEnemyBullet(const EnemyBullet& b) {
    sf::Vector2f pos = b.sprite.getPosition();

    sf::CircleShape outer;
    outer.setRadius(b.radius * 3.0f);
    outer.setOrigin({ b.radius * 3.0f, b.radius * 3.0f });
    outer.setPosition(pos);
    outer.setFillColor(sf::Color(255, 60, 60, 40));
    window.draw(outer, sf::BlendAdd);

    sf::CircleShape mid;
    mid.setRadius(b.radius * 1.8f);
    mid.setOrigin({ b.radius * 1.8f, b.radius * 1.8f });
    mid.setPosition(pos);
    mid.setFillColor(sf::Color(255, 100, 100, 110));
    window.draw(mid, sf::BlendAdd);

    if (enemyBulletTextureValid) {
        window.draw(b.sprite);
    } else {
        sf::CircleShape core;
        core.setRadius(b.radius);
        core.setOrigin({ b.radius, b.radius });
        core.setPosition(pos);
        core.setFillColor(sf::Color::Red);
        window.draw(core);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.setView(gameView);

    drawBackground();

    for (auto& e : enemies)
        window.draw(e.sprite);

    if (playerTextureValid)
        window.draw(playerSprite);
    else {
        debugRect.setPosition(playerSprite.getPosition());
        window.draw(debugRect);
    }

    for (auto& b : bullets)
        drawGlowBullet(b);

    for (auto& eb : enemyBullets)
        drawGlowEnemyBullet(eb);

    drawHUD();

    window.display();
}

void Game::drawHUD() {
    // ---- Health icons (bottom left) ----
    for (int i = 0; i < 3; i++) {
        float x = healthHudX + i * (healthIconWidth + healthIconGap);

        bool isFull = (i < playerHealth);

        sf::Texture& tex = isFull ? healthFullTexture : healthDepletedTexture;
        bool         valid = isFull ? healthFullValid : healthDepletedValid;

        if (valid) {
            sf::Sprite icon(tex);

            // Scale to the configured health icon size.
            auto texSize = tex.getSize();
            float scaleX = healthIconWidth / static_cast<float>(texSize.x);
            float scaleY = healthIconHeight / static_cast<float>(texSize.y);
            icon.setScale({ scaleX, scaleY });
            icon.setPosition({ x, healthHudY });
            window.draw(icon);
        } else {
            // Fallback circles if texture missing
            float fallbackRadius = std::min(healthIconWidth, healthIconHeight) / 2.f;
            sf::CircleShape fallback(fallbackRadius);
            fallback.setOrigin({ fallbackRadius, fallbackRadius });
            fallback.setPosition({ x + healthIconWidth / 2.f, healthHudY + healthIconHeight / 2.f });
            fallback.setFillColor(isFull
                ? sf::Color(220, 50, 80)
                : sf::Color(60, 60, 60));
            window.draw(fallback);
        }
    }

    // ---- Score icon + number (bottom right) ----
    if (scoreIconValid) {
        sf::Sprite scoreIcon(scoreTexture);
        auto texSize = scoreTexture.getSize();
        float scaleX = scoreIconWidth / static_cast<float>(texSize.x);
        float scaleY = scoreIconHeight / static_cast<float>(texSize.y);
        scoreIcon.setScale({ scaleX, scaleY });
        scoreIcon.setPosition({ scoreHudX, scoreHudY });
        window.draw(scoreIcon);
    }

    if (hudFontValid) {
        sf::Text scoreText(hudFont);
        scoreText.setString(std::to_string(score));
        scoreText.setCharacterSize(scoreTextSize);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition({ scoreHudX + scoreIconWidth + 8.f, scoreHudY });
        window.draw(scoreText);
    } else {
        // Fallback score background when no font is available.
        sf::RectangleShape scoreBg({ 70.f, scoreIconHeight });
        scoreBg.setFillColor(sf::Color(0, 0, 0, 160));
        scoreBg.setPosition({ scoreHudX + scoreIconWidth + 4.f, scoreHudY });
        window.draw(scoreBg);
    }
}
