#include "Game.h"

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

    window.display();
}
