#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ static_cast<unsigned>(gameWidth), static_cast<unsigned>(gameHeight) }), "Space Shooter")
    , gameView(sf::FloatRect({ 0.f, 0.f }, { gameWidth, gameHeight }))
    , playerSprite(playerTexture) {

    window.setFramerateLimit(60);

    try {
        std::cout << "[Info] CWD: " << std::filesystem::current_path() << '\n';
    } catch (...) {
        std::cout << "[Info] CWD: (unable to query)" << '\n';
    }
    std::cout << "[Info] Window size: " << window.getSize().x << "x" << window.getSize().y << '\n';

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    loadBackgroundAssets();
    loadPlayerAssets();
    loadBulletAssets();
    loadEnemyAssets();
    loadEnemyBulletAssets();
    loadUIAssets();

    // Initialize and load menu
    mainMenu.initialize(gameWidth, gameHeight, &hudFont, hudFontValid);
    mainMenu.loadAssets();

    updateView();

    waveBoundaryY = gameHeight * 0.45f;
    std::cout << "[Info] Wave boundary Y: " << waveBoundaryY << "\n";
}

void Game::run() {
    std::cout << "[Info] Game starting\n";
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
    std::cout << "[Info] Game stopped\n";
}

void Game::updateView() {
    const sf::Vector2u windowSize = window.getSize();
    if (windowSize.x == 0 || windowSize.y == 0) {
        return;
    }

    const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float gameRatio = gameWidth / gameHeight;

    float viewportLeft = 0.f;
    float viewportTop = 0.f;
    float viewportWidth = 1.f;
    float viewportHeight = 1.f;

    if (windowRatio > gameRatio) {
        viewportWidth = gameRatio / windowRatio;
        viewportLeft = (1.f - viewportWidth) / 2.f;
    } else if (windowRatio < gameRatio) {
        viewportHeight = windowRatio / gameRatio;
        viewportTop = (1.f - viewportHeight) / 2.f;
    }

    gameView.setCenter({ gameWidth / 2.f, gameHeight / 2.f });
    gameView.setSize({ gameWidth, gameHeight });
    gameView.setViewport(sf::FloatRect({ viewportLeft, viewportTop }, { viewportWidth, viewportHeight }));
    window.setView(gameView);
}

void Game::resetGame() {
    bullets.clear();
    enemyBullets.clear();
    enemies.clear();
    playerHealth     = 3;
    score            = 0;
    waveActive       = false;
    playerInvincible = false;
    invincibleTimer  = 0.f;
    playerFlashTimer = 0.f;
    movingLeft = movingRight = movingUp = movingDown = false;
    enemySpawnTimer.restart();
    shootCooldown.restart();
    playerSprite.setPosition({ gameWidth / 2.f, gameHeight - 60.f });
    playerSprite.setColor(sf::Color::White);
    std::cout << "[Info] Game reset\n";
}

