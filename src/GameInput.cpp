#include "Game.h"
#include <iostream>
#include <optional>

void Game::processEvents() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gameView);

    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            std::cout << "[Info] Window close event received\n";
            window.close();
        }

        if (event->is<sf::Event::Resized>()) {
            updateView();
        }

        // Handle mouse clicks
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                GameState previousState = gameState;
                mainMenu.handleMouseClick(mousePos, gameState, window);
                
                // Handle resetGame when transitioning from GameOver
                if (previousState == GameState::GameOver && gameState == GameState::Playing) {
                    resetGame();
                } else if (previousState == GameState::GameOver && gameState == GameState::MainMenu) {
                    resetGame();
                }
            }
        }

        // Handle keyboard input only during gameplay
        if (gameState == GameState::Playing) {
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    std::cout << "[Info] Escape pressed - closing\n";
                    window.close();
                }

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
                        std::cout << "[Info] Shot fired, total bullets: " << bullets.size() << "\n";
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

    // Update button hover states for menu
    mainMenu.updateButtonHover(mousePos, gameState);
}
