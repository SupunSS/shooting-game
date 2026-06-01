#include "MainMenu.h"
#include <iostream>
#include <filesystem>
#include <string>

MainMenu::MainMenu()
    : gameWidth(480.f), gameHeight(640.f), font(nullptr), fontValid(false) {
}

MainMenu::MainMenu(float width, float height, sf::Font* gameFont)
    : gameWidth(width), gameHeight(height), font(gameFont), fontValid(false) {
    if (font) {
        fontValid = true;
    }
}

MainMenu::~MainMenu() {
    // Delete button sprites
    if (playButton.sprite) delete playButton.sprite;
    if (quitButton.sprite) delete quitButton.sprite;
    if (retryButton.sprite) delete retryButton.sprite;
    if (mainMenuButton.sprite) delete mainMenuButton.sprite;
}

void MainMenu::initialize(float width, float height, sf::Font* gameFont, bool fontLoaded) {
    gameWidth = width;
    gameHeight = height;
    font = gameFont;
    fontValid = (font != nullptr && fontLoaded);
}

void MainMenu::loadAssets() {
    // Menu background - try all background images to use the first one as menu background
    const std::string menuBackgroundPath = "assets/background/menu_background.png";
    if (!std::filesystem::exists(menuBackgroundPath)) {
        std::cerr << "[Error] Menu background not found: " << menuBackgroundPath << '\n';
    } else if (!menuBackgroundTexture.loadFromFile(menuBackgroundPath)) {
        std::cerr << "[Error] Failed to load menu background\n";
    } else {
        menuBackgroundTexture.setSmooth(true);
        menuBackgroundValid = true;
        std::cout << "[Info] Loaded menu background\n";
    }

    // Play button
    const std::string playButtonPath = "assets/Menu/play_button.png";
    if (!std::filesystem::exists(playButtonPath)) {
        std::cerr << "[Error] Play button not found: " << playButtonPath << '\n';
    } else if (!playButtonTexture.loadFromFile(playButtonPath)) {
        std::cerr << "[Error] Failed to load play button\n";
    } else {
        playButtonTexture.setSmooth(true);
        playButtonValid = true;
        playButton.sprite = new sf::Sprite(playButtonTexture);
        playButton.position = { gameWidth / 2.f, gameHeight / 2.f - 50.f };
        playButton.size = { 200.f, 100.f };
        playButton.sprite->setPosition(playButton.position);
        playButton.sprite->setOrigin({ playButtonTexture.getSize().x / 2.f, playButtonTexture.getSize().y / 2.f });
        playButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded play button\n";
    }

    // Quit button
    const std::string quitButtonPath = "assets/Menu/quit_button.png";
    if (!std::filesystem::exists(quitButtonPath)) {
        std::cerr << "[Error] Quit button not found: " << quitButtonPath << '\n';
    } else if (!quitButtonTexture.loadFromFile(quitButtonPath)) {
        std::cerr << "[Error] Failed to load quit button\n";
    } else {
        quitButtonTexture.setSmooth(true);
        quitButtonValid = true;
        quitButton.sprite = new sf::Sprite(quitButtonTexture);
        quitButton.position = { gameWidth / 2.f, gameHeight / 2.f + 50.f };
        quitButton.size = { 160.f, 80.f };
        quitButton.sprite->setPosition(quitButton.position);
        quitButton.sprite->setOrigin({ quitButtonTexture.getSize().x / 2.f, quitButtonTexture.getSize().y / 2.f });
        quitButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded quit button\n";
    }

    // Retry button
    const std::string retryButtonPath = "assets/Menu/retry_button.png";
    if (!std::filesystem::exists(retryButtonPath)) {
        std::cerr << "[Error] Retry button not found: " << retryButtonPath << '\n';
    } else if (!retryButtonTexture.loadFromFile(retryButtonPath)) {
        std::cerr << "[Error] Failed to load retry button\n";
    } else {
        retryButtonTexture.setSmooth(true);
        retryButtonValid = true;
        retryButton.sprite = new sf::Sprite(retryButtonTexture);
        retryButton.position = { gameWidth / 2.f - 70.f, gameHeight / 2.f + 100.f };
        retryButton.size = { 160.f, 80.f };
        retryButton.sprite->setPosition(retryButton.position);
        retryButton.sprite->setOrigin({ retryButtonTexture.getSize().x / 2.f, retryButtonTexture.getSize().y / 2.f });
        retryButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded retry button\n";
    }

    // Main menu button
    const std::string mainMenuButtonPath = "assets/Menu/main_menu_button.png";
    if (!std::filesystem::exists(mainMenuButtonPath)) {
        std::cerr << "[Error] Main menu button not found: " << mainMenuButtonPath << '\n';
    } else if (!mainMenuButtonTexture.loadFromFile(mainMenuButtonPath)) {
        std::cerr << "[Error] Failed to load main menu button\n";
    } else {
        mainMenuButtonTexture.setSmooth(true);
        mainMenuButtonValid = true;
        mainMenuButton.sprite = new sf::Sprite(mainMenuButtonTexture);
        mainMenuButton.position = { gameWidth / 2.f + 70.f, gameHeight / 2.f + 100.f };
        mainMenuButton.size = { 160.f, 80.f };
        mainMenuButton.sprite->setPosition(mainMenuButton.position);
        mainMenuButton.sprite->setOrigin({ mainMenuButtonTexture.getSize().x / 2.f, mainMenuButtonTexture.getSize().y / 2.f });
        mainMenuButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded main menu button\n";
    }
}

void MainMenu::handleMouseClick(const sf::Vector2f& mousePos, GameState& gameState, sf::RenderWindow& window) {
    if (gameState == GameState::MainMenu) {
        if (playButton.contains(mousePos)) {
            gameState = GameState::Playing;
            std::cout << "[Info] Play button clicked\n";
        }
        if (quitButton.contains(mousePos)) {
            window.close();
            std::cout << "[Info] Quit button clicked\n";
        }
    } else if (gameState == GameState::GameOver) {
        if (retryButton.contains(mousePos)) {
            gameState = GameState::Playing;
            std::cout << "[Info] Retry button clicked\n";
        }
        if (mainMenuButton.contains(mousePos)) {
            gameState = GameState::MainMenu;
            std::cout << "[Info] Main menu button clicked\n";
        }
    }
}

void MainMenu::updateButtonHover(const sf::Vector2f& mousePos, GameState gameState) {
    if (gameState == GameState::MainMenu) {
        playButton.updateHover(mousePos);
        quitButton.updateHover(mousePos);
    } else if (gameState == GameState::GameOver) {
        retryButton.updateHover(mousePos);
        mainMenuButton.updateHover(mousePos);
    }
}

bool MainMenu::isPlayButtonPressed(const sf::Vector2f& mousePos) const {
    return playButton.contains(mousePos);
}

bool MainMenu::isQuitButtonPressed(const sf::Vector2f& mousePos) const {
    return quitButton.contains(mousePos);
}

bool MainMenu::isRetryButtonPressed(const sf::Vector2f& mousePos) const {
    return retryButton.contains(mousePos);
}

bool MainMenu::isMainMenuButtonPressed(const sf::Vector2f& mousePos) const {
    return mainMenuButton.contains(mousePos);
}

void MainMenu::drawMainMenu(sf::RenderWindow& window) {
    // Draw menu background
    if (menuBackgroundValid) {
        sf::Sprite bgSprite(menuBackgroundTexture);
        float bgScale = gameWidth / static_cast<float>(menuBackgroundTexture.getSize().x);
        bgSprite.setScale({ bgScale, bgScale });
        bgSprite.setPosition({ 0.f, 0.f });
        window.draw(bgSprite);
    } else {
        window.clear(sf::Color::Black);
    }

    // Draw title with retro font
    if (fontValid) {
        sf::Text titleText(*font);
        titleText.setString("SPACE SHOOTER");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setOutlineThickness(2.f);
        titleText.setOutlineColor(sf::Color::Black);

        titleText.setPosition({ gameWidth / 2.f - 150.f, gameHeight / 4.f });
        window.draw(titleText);
    }

    // Draw play button
    if (playButtonValid && playButton.sprite) {
        playButton.updateScale(0.8f);
        window.draw(*playButton.sprite);

        // Draw button label
        if (fontValid) {
            sf::Text playText(*font);
            playText.setString("PLAY");
            playText.setCharacterSize(20);
            playText.setFillColor(playButton.hovered ? sf::Color::Yellow : sf::Color::White);
            playText.setOutlineThickness(1.f);
            playText.setOutlineColor(sf::Color::Black);

            playText.setPosition({
                playButton.position.x - 20.f,
                playButton.position.y - 10.f
            });
            window.draw(playText);
        }
    }

    // Draw quit button
    if (quitButtonValid && quitButton.sprite) {
        quitButton.updateScale(0.8f);
        window.draw(*quitButton.sprite);

        // Draw button label
        if (fontValid) {
            sf::Text quitText(*font);
            quitText.setString("QUIT");
            quitText.setCharacterSize(20);
            quitText.setFillColor(quitButton.hovered ? sf::Color::Yellow : sf::Color::White);
            quitText.setOutlineThickness(1.f);
            quitText.setOutlineColor(sf::Color::Black);

            quitText.setPosition({
                quitButton.position.x - 20.f,
                quitButton.position.y - 10.f
            });
            window.draw(quitText);
        }
    }
}

void MainMenu::drawGameOver(sf::RenderWindow& window, int finalScore) {
    // Draw semi-transparent overlay
    sf::RectangleShape overlay({ gameWidth, gameHeight });
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    // Draw game over text with retro font
    if (fontValid) {
        sf::Text gameOverText(*font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(56);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineThickness(3.f);
        gameOverText.setOutlineColor(sf::Color::White);

        gameOverText.setPosition({
            gameWidth / 2.f - 140.f,
            gameHeight / 4.f
        });
        window.draw(gameOverText);
    }

    // Draw score
    drawScore(window, finalScore);

    // Draw retry button
    if (retryButtonValid && retryButton.sprite) {
        retryButton.updateScale(0.8f);
        window.draw(*retryButton.sprite);

        // Draw button label
        if (fontValid) {
            sf::Text retryText(*font);
            retryText.setString("RETRY");
            retryText.setCharacterSize(16);
            retryText.setFillColor(retryButton.hovered ? sf::Color::Yellow : sf::Color::White);
            retryText.setOutlineThickness(1.f);
            retryText.setOutlineColor(sf::Color::Black);

            retryText.setPosition({
                retryButton.position.x - 25.f,
                retryButton.position.y - 8.f
            });
            window.draw(retryText);
        }
    }

    // Draw main menu button
    if (mainMenuButtonValid && mainMenuButton.sprite) {
        mainMenuButton.updateScale(0.8f);
        window.draw(*mainMenuButton.sprite);

        // Draw button label
        if (fontValid) {
            sf::Text menuText(*font);
            menuText.setString("MENU");
            menuText.setCharacterSize(16);
            menuText.setFillColor(mainMenuButton.hovered ? sf::Color::Yellow : sf::Color::White);
            menuText.setOutlineThickness(1.f);
            menuText.setOutlineColor(sf::Color::Black);

            menuText.setPosition({
                mainMenuButton.position.x - 15.f,
                mainMenuButton.position.y - 8.f
            });
            window.draw(menuText);
        }
    }
}

void MainMenu::drawScore(sf::RenderWindow& window, int finalScore) {
    if (!fontValid) return;

    // Draw "FINAL SCORE" label
    sf::Text scoreLabel(*font);
    scoreLabel.setString("FINAL SCORE");
    scoreLabel.setCharacterSize(32);
    scoreLabel.setFillColor(sf::Color::White);
    scoreLabel.setOutlineThickness(2.f);
    scoreLabel.setOutlineColor(sf::Color::Black);

    scoreLabel.setPosition({
        gameWidth / 2.f - 80.f,
        gameHeight / 2.f - 80.f
    });
    window.draw(scoreLabel);

    // Draw actual score value
    sf::Text scoreValue(*font);
    scoreValue.setString(std::to_string(finalScore));
    scoreValue.setCharacterSize(64);
    scoreValue.setFillColor(sf::Color::Cyan);
    scoreValue.setOutlineThickness(3.f);
    scoreValue.setOutlineColor(sf::Color::Black);

    scoreValue.setPosition({
        gameWidth / 2.f - 80.f,
        gameHeight / 2.f
    });
    window.draw(scoreValue);
}
