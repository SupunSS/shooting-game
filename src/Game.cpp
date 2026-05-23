#include "Game.h"
#include <algorithm>

Game::Game()
    : window(sf::VideoMode({ 480u, 640u }), "Nokia Shooter") {

    window.setFramerateLimit(60);

    player.setRadius(15.f);
    player.setFillColor(sf::Color::Green);
    player.setOrigin({ 15.f, 15.f });
    player.setPosition({ 240.f, 580.f });
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                window.close();

            // Shoot on Space
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space &&
                shootCooldown.getElapsedTime().asSeconds() > 0.25f) {

                Bullet b;
                b.shape.setRadius(5.f);
                b.shape.setFillColor(sf::Color::Yellow);
                b.shape.setOrigin({ 5.f, 5.f });
                b.shape.setPosition(player.getPosition());
                bullets.push_back(b);
                shootCooldown.restart();
            }
        }
    }
}

void Game::update(float dt) {
    // Player movement
    sf::Vector2f move(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))  move.x -= playerSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) move.x += playerSpeed * dt;

    sf::Vector2f pos = player.getPosition() + move;
    pos.x = std::clamp(pos.x, 15.f, 465.f);
    player.setPosition(pos);

    // Move bullets up
    for (auto& b : bullets)
        b.shape.move({ 0.f, -b.speed * dt });

    // Remove off-screen bullets
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.shape.getPosition().y < 0.f; }),
        bullets.end());
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(player);
    for (auto& b : bullets)
        window.draw(b.shape);
    window.display();
}