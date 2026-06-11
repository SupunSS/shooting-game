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

                // ---- Settings screen clicks ----
                if (gameState == GameState::Settings) {
                    if (mainMenu.isRebinding()) continue;

                    sf::FloatRect soundBox({ gameWidth - 130.f, 86.f }, { 80.f, 30.f });
                    if (soundBox.contains(mousePos)) {
                        mainMenu.toggleSound();
                        std::cout << "[Info] Sound: "
                                  << (mainMenu.getSoundEnabled() ? "ON" : "OFF") << "\n";
                    }

                    sf::FloatRect musicBox({ gameWidth - 130.f, 132.f }, { 80.f, 30.f });
                    if (musicBox.contains(mousePos)) {
                        mainMenu.toggleMusic();
                        std::cout << "[Info] Music: "
                                  << (mainMenu.getMusicEnabled() ? "ON" : "OFF") << "\n";
                    }

                    for (int i = 0; i < 5; i++) {
                        float y = 244.f + i * 46.f - 4.f;
                        sf::FloatRect keyBox({ gameWidth - 160.f, y }, { 120.f, 30.f });
                        if (keyBox.contains(mousePos)) {
                            mainMenu.startRebinding(i);
                            std::cout << "[Info] Rebinding control " << i << "\n";
                        }
                    }

                    sf::FloatRect backBox(
                        { gameWidth / 2.f - 80.f, gameHeight - 76.f }, { 160.f, 40.f });
                    if (backBox.contains(mousePos)) {
                        gameState = GameState::MainMenu;
                        std::cout << "[Info] Back to main menu\n";
                    }
                    continue;
                }

                // ---- High scores screen clicks ----
                if (gameState == GameState::HighScores) {
                    sf::FloatRect backBox(
                        { gameWidth / 2.f - 80.f, gameHeight - 76.f }, { 160.f, 40.f });
                    if (backBox.contains(mousePos)) {
                        gameState = GameState::MainMenu;
                        std::cout << "[Info] Back from high scores\n";
                    }
                    continue;
                }

                mainMenu.handleMouseClick(mousePos, gameState, window);

                // Reset game when transitioning to Playing
                if (gameState == GameState::Playing &&
                    (prevState == GameState::GameOver ||
                     prevState == GameState::MainMenu)) {
                    resetGame();
                }
            }
        }

        // ---- Keyboard ----
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

            // ADDED: F11 toggles fullscreen / windowed (works in any game state)
            if (keyPressed->scancode == sf::Keyboard::Scancode::F11) {
                toggleFullscreen();
                continue;
            }

            // If rebinding — capture the key
            if (mainMenu.isRebinding()) {
                mainMenu.applyRebind(keyPressed->scancode);
                continue;
            }

            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                if (gameState == GameState::Playing) {
                    gameState = GameState::Paused;
                    movingLeft = movingRight = movingUp = movingDown = false;
                    std::cout << "[Info] Game paused via Escape\n";
                } else if (gameState == GameState::Paused) {
                    gameState = GameState::Playing;
                    std::cout << "[Info] Game resumed via Escape\n";
                } else if (gameState == GameState::Settings) {
                    mainMenu.startRebinding(-1);
                    gameState = GameState::MainMenu;
                    std::cout << "[Info] Back to main menu via Escape\n";
                } else if (gameState == GameState::HighScores) {
                    gameState = GameState::MainMenu;
                    std::cout << "[Info] Back from high scores via Escape\n";
                } else {
                    window.close();
                }
            }

            if (gameState != GameState::Playing) continue;

            // Custom key bindings
            if (keyPressed->scancode == mainMenu.getMoveLeftKey()  ||
                keyPressed->scancode == sf::Keyboard::Scancode::Left)  movingLeft  = true;
            if (keyPressed->scancode == mainMenu.getMoveRightKey() ||
                keyPressed->scancode == sf::Keyboard::Scancode::Right) movingRight = true;
            if (keyPressed->scancode == mainMenu.getMoveUpKey()    ||
                keyPressed->scancode == sf::Keyboard::Scancode::Up)    movingUp    = true;
            if (keyPressed->scancode == mainMenu.getMoveDownKey()  ||
                keyPressed->scancode == sf::Keyboard::Scancode::Down)  movingDown  = true;

            if (keyPressed->scancode == mainMenu.getShootKey()) {
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

        // ---- Key released ----
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (keyReleased->scancode == mainMenu.getMoveLeftKey()  ||
                keyReleased->scancode == sf::Keyboard::Scancode::Left)  movingLeft  = false;
            if (keyReleased->scancode == mainMenu.getMoveRightKey() ||
                keyReleased->scancode == sf::Keyboard::Scancode::Right) movingRight = false;
            if (keyReleased->scancode == mainMenu.getMoveUpKey()    ||
                keyReleased->scancode == sf::Keyboard::Scancode::Up)    movingUp    = false;
            if (keyReleased->scancode == mainMenu.getMoveDownKey()  ||
                keyReleased->scancode == sf::Keyboard::Scancode::Down)  movingDown  = false;
        }
    }
}