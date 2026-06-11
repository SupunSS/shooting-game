#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>

// ---------------------------------------------------------------- Constructor
// CHANGED: Start windowed instead of fullscreen to avoid startup glitch.
//          Windowed size matches the game's logical resolution (480x640).
//          Press F11 at any time to toggle fullscreen / windowed.
Game::Game()
    : window(sf::VideoMode({ static_cast<unsigned>(gameWidth),
                              static_cast<unsigned>(gameHeight) }),
             "Space Shooter",
             sf::Style::Titlebar | sf::Style::Close)   // windowed on startup
    , gameView(sf::FloatRect({ 0.f, 0.f }, { gameWidth, gameHeight }))
    , playerSprite(playerTexture)
    , isFullscreen(false) {                             // track mode

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

    mainMenu.initialize(gameWidth, gameHeight, &hudFont, hudFontValid);
    mainMenu.loadAssets();

    scoreManager.load();

    updateView();

    waveBoundaryY = gameHeight * 0.45f;
    std::cout << "[Info] Wave boundary Y: " << waveBoundaryY << "\n";
}

// ---------------------------------------------------------------- toggleFullscreen
// Recreates the window in the desired mode, then restores the view.
// Using sf::State::Fullscreen (borderless desktop fullscreen) avoids the
// resolution-switch flicker that sf::Style::Fullscreen causes.
void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        // Borderless fullscreen — uses desktop resolution, no mode switch = no glitch
        window.create(sf::VideoMode::getDesktopMode(),
                      "Space Shooter",
                      sf::State::Fullscreen);
    } else {
        // Back to a normal resizable window at the game's logical size
        window.create(sf::VideoMode({ static_cast<unsigned>(gameWidth),
                                      static_cast<unsigned>(gameHeight) }),
                      "Space Shooter",
                      sf::Style::Titlebar | sf::Style::Close);
    }

    window.setFramerateLimit(60);

    // Recompute letterbox/pillarbox viewport for the new window dimensions
    updateView();

    std::cout << "[Info] Fullscreen: " << (isFullscreen ? "ON" : "OFF")
              << "  Window: " << window.getSize().x << "x" << window.getSize().y << '\n';
}

// ---------------------------------------------------------------- run
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

// ---------------------------------------------------------------- updateView
void Game::updateView() {
    const sf::Vector2u windowSize = window.getSize();
    if (windowSize.x == 0 || windowSize.y == 0) {
        return;
    }

    const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float gameRatio   = gameWidth / gameHeight;

    float viewportLeft   = 0.f;
    float viewportTop    = 0.f;
    float viewportWidth  = 1.f;
    float viewportHeight = 1.f;

    if (windowRatio > gameRatio) {
        // Window is wider than game → pillarbox (black bars left/right)
        viewportWidth = gameRatio / windowRatio;
        viewportLeft  = (1.f - viewportWidth) / 2.f;
    } else if (windowRatio < gameRatio) {
        // Window is taller than game → letterbox (black bars top/bottom)
        viewportHeight = windowRatio / gameRatio;
        viewportTop    = (1.f - viewportHeight) / 2.f;
    }

    gameView.setCenter({ gameWidth / 2.f, gameHeight / 2.f });
    gameView.setSize({ gameWidth, gameHeight });
    gameView.setViewport(sf::FloatRect({ viewportLeft, viewportTop },
                                       { viewportWidth, viewportHeight }));
    window.setView(gameView);
}

// ---------------------------------------------------------------- resetGame
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