#include "Game.h"
#include <iostream>
#include <optional>

void Game::processEvents() {
    prevState = gameState;

    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event->is<sf::Event::Resized>()) {
            updateView();
        }

        // ---- Mouse moved — update hover ----
        if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(moved->position.x, moved->position.y), gameView);
            mainMenu.updateButtonHover(mousePos, gameState);
        }

        // ---- Mouse click ----
        if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    sf::Vector2i(mouse->position.x, mouse->position.y), gameView);

                // Pause button — only during gameplay
                if (gameState == GameState::Playing &&
                    mainMenu.isPauseButtonPressed(mousePos)) {
                    gameState = GameState::Paused;
                    movingLeft = movingRight = movingUp = movingDown = false;
                    std::cout << "[Info] Game paused\n";
                    continue;
                }

                mainMenu.handleMouseClick(mousePos, gameState, window);

                // Reset game when transitioning to Playing from GameOver or MainMenu
                if (gameState == GameState::Playing &&
                    (prevState == GameState::GameOver ||
                     prevState == GameState::MainMenu)) {
                    resetGame();
                }
            }
        }

        // ---- Keyboard ----
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                if (gameState == GameState::Playing) {
                    gameState = GameState::Paused;
                    movingLeft = movingRight = movingUp = movingDown = false;
                    std::cout << "[Info] Game paused via Escape\n";
                } else if (gameState == GameState::Paused) {
                    gameState = GameState::Playing;
                    std::cout << "[Info] Game resumed via Escape\n";
                } else {
                    window.close();
                }
            }

            // Block all gameplay keys when not playing
            if (gameState != GameState::Playing) continue;

            if (keyPressed->scancode == sf::Keyboard::Scancode::Left  ||
                keyPressed->scancode == sf::Keyboard::Scancode::A) movingLeft  = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Right ||
                keyPressed->scancode == sf::Keyboard::Scancode::D) movingRight = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Up    ||
                keyPressed->scancode == sf::Keyboard::Scancode::W) movingUp    = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Down  ||
                keyPressed->scancode == sf::Keyboard::Scancode::S) movingDown  = true;

            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                float elapsed = shootCooldown.getElapsedTime().asSeconds();
                if (elapsed > 0.25f) {
                    Bullet b;
                    b.shape.setRadius(bulletRadius);
                    b.shape.setOrigin({ bulletRadius, bulletRadius });
                    if (bulletTextureValid) {
                        b.shape.setTexture(&bulletTexture, true);
                        b.shape.setFillColor(sf::Color::White);
                    } else {
                        b.shape.setFillColor(sf::Color::Cyan);
                    }
                    sf::Vector2f tip = playerSprite.getPosition();
                    tip.y -= playerHalfSize.y;
                    tip.y -= bulletRadius;
                    b.shape.setPosition(tip);
                    bullets.push_back(b);
                    shootCooldown.restart();
                }
            }
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::Left  ||
                keyReleased->scancode == sf::Keyboard::Scancode::A) movingLeft  = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Right ||
                keyReleased->scancode == sf::Keyboard::Scancode::D) movingRight = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Up    ||
                keyReleased->scancode == sf::Keyboard::Scancode::W) movingUp    = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Down  ||
                keyReleased->scancode == sf::Keyboard::Scancode::S) movingDown  = false;
        }
    }
}