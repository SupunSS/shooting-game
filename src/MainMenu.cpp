#include "MainMenu.h"
#include <iostream>
#include <filesystem>
#include <string>

MainMenu::MainMenu()
    : gameWidth(480.f), gameHeight(640.f), font(nullptr), fontValid(false) {}

MainMenu::MainMenu(float width, float height, sf::Font* gameFont)
    : gameWidth(width), gameHeight(height), font(gameFont), fontValid(false) {
    if (font) fontValid = true;
}

MainMenu::~MainMenu() {
    if (playButton.sprite)     delete playButton.sprite;
    if (quitButton.sprite)     delete quitButton.sprite;
    if (retryButton.sprite)    delete retryButton.sprite;
    if (mainMenuButton.sprite) delete mainMenuButton.sprite;
    if (pauseButton.sprite)    delete pauseButton.sprite;
    if (resumeButton.sprite)   delete resumeButton.sprite;
}

void MainMenu::initialize(float width, float height, sf::Font* gameFont, bool fontLoaded) {
    gameWidth  = width;
    gameHeight = height;
    font       = gameFont;
    fontValid  = (font != nullptr && fontLoaded);
}

void MainMenu::loadAssets() {

    // ---- Menu background ----
    const std::string bgPath = "assets/background/menu_background.png";
    if (std::filesystem::exists(bgPath) && menuBackgroundTexture.loadFromFile(bgPath)) {
        menuBackgroundTexture.setSmooth(true);
        menuBackgroundValid = true;
        std::cout << "[Info] Loaded menu background\n";
    }

    // ---- Name / title image ----
    const std::string namePath = "assets/Menu/Name.png";
    if (std::filesystem::exists(namePath) && nameTitleTexture.loadFromFile(namePath)) {
        nameTitleTexture.setSmooth(true);
        nameTitleValid = true;
        std::cout << "[Info] Loaded Name.png\n";
    }

    // ---- Play button ----
    if (std::filesystem::exists("assets/Menu/play_button.png") &&
        playButtonTexture.loadFromFile("assets/Menu/play_button.png")) {
        playButtonTexture.setSmooth(true);
        playButtonValid = true;
        playButton.sprite   = new sf::Sprite(playButtonTexture);
        playButton.position = { gameWidth / 2.f, gameHeight / 2.f - 30.f };
        playButton.size     = { 200.f, 80.f };
        playButton.sprite->setOrigin({
            playButtonTexture.getSize().x / 2.f,
            playButtonTexture.getSize().y / 2.f });
        playButton.sprite->setPosition(playButton.position);
        playButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded play_button.png\n";
    }

    // ---- Quit button ----
    if (std::filesystem::exists("assets/Menu/quit_button.png") &&
        quitButtonTexture.loadFromFile("assets/Menu/quit_button.png")) {
        quitButtonTexture.setSmooth(true);
        quitButtonValid = true;
        quitButton.sprite   = new sf::Sprite(quitButtonTexture);
        quitButton.position = { gameWidth / 2.f, gameHeight / 2.f + 70.f };
        quitButton.size     = { 200.f, 80.f };
        quitButton.sprite->setOrigin({
            quitButtonTexture.getSize().x / 2.f,
            quitButtonTexture.getSize().y / 2.f });
        quitButton.sprite->setPosition(quitButton.position);
        quitButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded quit_button.png\n";
    }

    // ---- Retry button ----
    if (std::filesystem::exists("assets/Menu/retry_button.png") &&
        retryButtonTexture.loadFromFile("assets/Menu/retry_button.png")) {
        retryButtonTexture.setSmooth(true);
        retryButtonValid = true;
        retryButton.sprite   = new sf::Sprite(retryButtonTexture);
        retryButton.position = { gameWidth / 2.f - 80.f, gameHeight / 2.f + 120.f };
        retryButton.size     = { 160.f, 70.f };
        retryButton.sprite->setOrigin({
            retryButtonTexture.getSize().x / 2.f,
            retryButtonTexture.getSize().y / 2.f });
        retryButton.sprite->setPosition(retryButton.position);
        retryButton.sprite->setScale({ 0.75f, 0.75f });
        std::cout << "[Info] Loaded retry_button.png\n";
    }

    // ---- Main menu button ----
    if (std::filesystem::exists("assets/Menu/main_menu_button.png") &&
        mainMenuButtonTexture.loadFromFile("assets/Menu/main_menu_button.png")) {
        mainMenuButtonTexture.setSmooth(true);
        mainMenuButtonValid = true;
        mainMenuButton.sprite   = new sf::Sprite(mainMenuButtonTexture);
        mainMenuButton.position = { gameWidth / 2.f + 80.f, gameHeight / 2.f + 120.f };
        mainMenuButton.size     = { 160.f, 70.f };
        mainMenuButton.sprite->setOrigin({
            mainMenuButtonTexture.getSize().x / 2.f,
            mainMenuButtonTexture.getSize().y / 2.f });
        mainMenuButton.sprite->setPosition(mainMenuButton.position);
        mainMenuButton.sprite->setScale({ 0.75f, 0.75f });
        std::cout << "[Info] Loaded main_menu_button.png\n";
    }

    // ---- Pause button (in-game HUD button) ----
if (std::filesystem::exists("assets/Menu/Paush_button.png") &&
    pauseButtonTexture.loadFromFile("assets/Menu/Paush_button.png")) {
    pauseButtonTexture.setSmooth(true);
    pauseButtonValid = true;
    pauseButton.sprite = new sf::Sprite(pauseButtonTexture);

    float desiredSize    = 40.f;                          // visual size on screen
    float clickboxSize   = 50.f;                          // slightly larger for easy clicking
    pauseButton.size     = { clickboxSize, clickboxSize }; // <-- clickbox
    pauseButton.position = { gameWidth - 40.f, 40.f };

    pauseButton.sprite->setOrigin({
        pauseButtonTexture.getSize().x / 2.f,
        pauseButtonTexture.getSize().y / 2.f });
    pauseButton.sprite->setPosition(pauseButton.position);

    float pScale = desiredSize / static_cast<float>(pauseButtonTexture.getSize().x);
    pauseButton.sprite->setScale({ pScale, pScale });
    std::cout << "[Info] Loaded Paush_button.png\n";
}

    // ---- Paused title overlay image ----
    if (std::filesystem::exists("assets/Menu/paushed.png") &&
        pausedTitleTexture.loadFromFile("assets/Menu/paushed.png")) {
        pausedTitleTexture.setSmooth(true);
        pausedTitleValid = true;
        std::cout << "[Info] Loaded paushed.png\n";
    }

    // ---- Resume button ----
    if (std::filesystem::exists("assets/Menu/resume_button.png") &&
        resumeButtonTexture.loadFromFile("assets/Menu/resume_button.png")) {
        resumeButtonTexture.setSmooth(true);
        resumeButtonValid = true;
        resumeButton.sprite   = new sf::Sprite(resumeButtonTexture);
        resumeButton.position = { gameWidth / 2.f, gameHeight / 2.f };
        resumeButton.size     = { 200.f, 80.f };
        resumeButton.sprite->setOrigin({
            resumeButtonTexture.getSize().x / 2.f,
            resumeButtonTexture.getSize().y / 2.f });
        resumeButton.sprite->setPosition(resumeButton.position);
        resumeButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded resume_button.png\n";
    }

    // ---- Game over image ----
    if (std::filesystem::exists("assets/Menu/game_over.png") &&
        gameOverTexture.loadFromFile("assets/Menu/game_over.png")) {
        gameOverTexture.setSmooth(true);
        gameOverValid = true;
        std::cout << "[Info] Loaded game_over.png\n";
    }
}

// ---------------------------------------------------------------- handleMouseClick
void MainMenu::handleMouseClick(const sf::Vector2f& mousePos,
                                 GameState& gameState,
                                 sf::RenderWindow& window) {
    if (gameState == GameState::MainMenu) {
        if (playButton.contains(mousePos)) {
            gameState = GameState::Playing;
            std::cout << "[Info] Play button clicked\n";
        }
        if (quitButton.contains(mousePos)) {
            window.close();
        }
    } else if (gameState == GameState::Paused) {
        if (resumeButton.contains(mousePos)) {
            gameState = GameState::Playing;
            std::cout << "[Info] Resume clicked\n";
        }
        if (mainMenuButton.contains(mousePos)) {
            gameState = GameState::MainMenu;
            std::cout << "[Info] Main menu from pause clicked\n";
        }
    } else if (gameState == GameState::GameOver) {
        if (retryButton.contains(mousePos)) {
            gameState = GameState::Playing;
            std::cout << "[Info] Retry clicked\n";
        }
        if (mainMenuButton.contains(mousePos)) {
            gameState = GameState::MainMenu;
            std::cout << "[Info] Main menu from game over clicked\n";
        }
    }
}

// ---------------------------------------------------------------- updateButtonHover
void MainMenu::updateButtonHover(const sf::Vector2f& mousePos, GameState gameState) {
    if (gameState == GameState::MainMenu) {
        playButton.updateHover(mousePos);
        quitButton.updateHover(mousePos);
    } else if (gameState == GameState::Paused) {
        resumeButton.updateHover(mousePos);
        mainMenuButton.updateHover(mousePos);
    } else if (gameState == GameState::GameOver) {
        retryButton.updateHover(mousePos);
        mainMenuButton.updateHover(mousePos);
    }
    // pause button always active during Playing
    pauseButton.updateHover(mousePos);
}

// ---------------------------------------------------------------- drawMainMenu
void MainMenu::drawMainMenu(sf::RenderWindow& window) {
    if (menuBackgroundValid) {
        sf::Sprite bg(menuBackgroundTexture);
        float s = gameWidth / static_cast<float>(menuBackgroundTexture.getSize().x);
        bg.setScale({ s, s });
        bg.setPosition({ 0.f, 0.f });
        window.draw(bg);
    } else {
        window.clear(sf::Color::Black);
    }

    // Name / game title image
    if (nameTitleValid) {
        sf::Sprite nameSprite(nameTitleTexture);
        float s = (gameWidth * 0.7f) / static_cast<float>(nameTitleTexture.getSize().x);
        nameSprite.setScale({ s, s });
        float scaledW = nameTitleTexture.getSize().x * s;
        nameSprite.setPosition({ (gameWidth - scaledW) / 2.f, gameHeight * 0.15f });
        window.draw(nameSprite);
    }

    if (playButtonValid && playButton.sprite) {
        playButton.updateScale(0.8f);
        window.draw(*playButton.sprite);
    }
    if (quitButtonValid && quitButton.sprite) {
        quitButton.updateScale(0.8f);
        window.draw(*quitButton.sprite);
    }
}

// ---------------------------------------------------------------- drawPauseButton
void MainMenu::drawPauseButton(sf::RenderWindow& window) {
    if (pauseButtonValid && pauseButton.sprite) {
        pauseButton.updateScale(1.0f);
        window.draw(*pauseButton.sprite);
    }
}

// ---------------------------------------------------------------- drawPauseOverlay
void MainMenu::drawPauseOverlay(sf::RenderWindow& window, int currentScore) {
    // Dark overlay over the game
    sf::RectangleShape overlay({ gameWidth, gameHeight });
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

    // Paused title image
    if (pausedTitleValid) {
        sf::Sprite pausedSprite(pausedTitleTexture);
        float s = (gameWidth * 0.6f) / static_cast<float>(pausedTitleTexture.getSize().x);
        pausedSprite.setScale({ s, s });
        float scaledW = pausedTitleTexture.getSize().x * s;
        pausedSprite.setPosition({ (gameWidth - scaledW) / 2.f, gameHeight * 0.2f });
        window.draw(pausedSprite);
    } else if (fontValid) {
        sf::Text t(*font);
        t.setString("PAUSED");
        t.setCharacterSize(42);
        t.setFillColor(sf::Color::Cyan);
        t.setPosition({ gameWidth / 2.f - 90.f, gameHeight * 0.2f });
        window.draw(t);
    }

    // Current score during pause
    if (fontValid) {
        sf::Text sc(*font);
        sc.setString("SCORE: " + std::to_string(currentScore));
        sc.setCharacterSize(20);
        sc.setFillColor(sf::Color::White);
        auto b = sc.getLocalBounds();
        sc.setOrigin({ b.size.x / 2.f, 0.f });
        sc.setPosition({ gameWidth / 2.f, gameHeight * 0.42f });
        window.draw(sc);
    }

    // Resume button
    if (resumeButtonValid && resumeButton.sprite) {
        resumeButton.updateScale(0.8f);
        window.draw(*resumeButton.sprite);
    }

    // Main menu button (reused)
    if (mainMenuButtonValid && mainMenuButton.sprite) {
        // reposition for pause screen
        mainMenuButton.position = { gameWidth / 2.f, gameHeight / 2.f + 100.f };
        mainMenuButton.sprite->setPosition(mainMenuButton.position);
        mainMenuButton.updateScale(0.75f);
        window.draw(*mainMenuButton.sprite);
    }
}

// ---------------------------------------------------------------- drawGameOver
void MainMenu::drawGameOver(sf::RenderWindow& window, int finalScore) {
    sf::RectangleShape overlay({ gameWidth, gameHeight });
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    // Game over image
    if (gameOverValid) {
        sf::Sprite goSprite(gameOverTexture);
        float s = (gameWidth * 0.7f) / static_cast<float>(gameOverTexture.getSize().x);
        goSprite.setScale({ s, s });
        float scaledW = gameOverTexture.getSize().x * s;
        goSprite.setPosition({ (gameWidth - scaledW) / 2.f, gameHeight * 0.15f });
        window.draw(goSprite);
    } else if (fontValid) {
        sf::Text t(*font);
        t.setString("GAME OVER");
        t.setCharacterSize(42);
        t.setFillColor(sf::Color::Red);
        t.setPosition({ gameWidth / 2.f - 130.f, gameHeight * 0.2f });
        window.draw(t);
    }

    drawScore(window, finalScore);

    if (retryButtonValid && retryButton.sprite) {
        retryButton.updateScale(0.75f);
        window.draw(*retryButton.sprite);
    }
    if (mainMenuButtonValid && mainMenuButton.sprite) {
        mainMenuButton.position = { gameWidth / 2.f + 80.f, gameHeight / 2.f + 120.f };
        mainMenuButton.sprite->setPosition(mainMenuButton.position);
        mainMenuButton.updateScale(0.75f);
        window.draw(*mainMenuButton.sprite);
    }
}

// ---------------------------------------------------------------- drawScore
void MainMenu::drawScore(sf::RenderWindow& window, int finalScore) {
    if (!fontValid) return;

    sf::Text label(*font);
    label.setString("FINAL SCORE");
    label.setCharacterSize(24);
    label.setFillColor(sf::Color::White);
    auto lb = label.getLocalBounds();
    label.setOrigin({ lb.size.x / 2.f, 0.f });
    label.setPosition({ gameWidth / 2.f, gameHeight / 2.f - 40.f });
    window.draw(label);

    sf::Text val(*font);
    val.setString(std::to_string(finalScore));
    val.setCharacterSize(52);
    val.setFillColor(sf::Color::Cyan);
    auto vb = val.getLocalBounds();
    val.setOrigin({ vb.size.x / 2.f, 0.f });
    val.setPosition({ gameWidth / 2.f, gameHeight / 2.f + 10.f });
    window.draw(val);
}

// ---------------------------------------------------------------- simple getters
bool MainMenu::isPauseButtonPressed(const sf::Vector2f& p) const  { return pauseButton.contains(p); }
bool MainMenu::isResumeButtonPressed(const sf::Vector2f& p) const { return resumeButton.contains(p); }
bool MainMenu::isPlayButtonPressed(const sf::Vector2f& p) const   { return playButton.contains(p); }
bool MainMenu::isQuitButtonPressed(const sf::Vector2f& p) const   { return quitButton.contains(p); }
bool MainMenu::isRetryButtonPressed(const sf::Vector2f& p) const  { return retryButton.contains(p); }
bool MainMenu::isMainMenuButtonPressed(const sf::Vector2f& p) const { return mainMenuButton.contains(p); }