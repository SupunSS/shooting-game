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
