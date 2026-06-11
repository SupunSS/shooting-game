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
    if (playButton.sprite)      delete playButton.sprite;
    if (quitButton.sprite)      delete quitButton.sprite;
    if (retryButton.sprite)     delete retryButton.sprite;
    if (mainMenuButton.sprite)  delete mainMenuButton.sprite;
    if (pauseButton.sprite)     delete pauseButton.sprite;
    if (resumeButton.sprite)    delete resumeButton.sprite;
    if (settingsButton.sprite)  delete settingsButton.sprite;
    if (highScoreButton.sprite) delete highScoreButton.sprite;
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
        playButton.position = { gameWidth / 2.f, gameHeight / 2.f - 100.f };
        playButton.size     = { 200.f, 80.f };
        playButton.sprite->setOrigin({
            playButtonTexture.getSize().x / 2.f,
            playButtonTexture.getSize().y / 2.f });
        playButton.sprite->setPosition(playButton.position);
        playButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded play_button.png\n";
    }

    // ---- Settings button ----
    if (std::filesystem::exists("assets/Menu/settings_button.png") &&
        settingsButtonTexture.loadFromFile("assets/Menu/settings_button.png")) {
        settingsButtonTexture.setSmooth(true);
        settingsButtonValid = true;
        settingsButton.sprite   = new sf::Sprite(settingsButtonTexture);
        settingsButton.position = { gameWidth / 2.f, gameHeight / 2.f };
        settingsButton.size     = { 200.f, 80.f };
        settingsButton.sprite->setOrigin({
            settingsButtonTexture.getSize().x / 2.f,
            settingsButtonTexture.getSize().y / 2.f });
        settingsButton.sprite->setPosition(settingsButton.position);
        settingsButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded settings_button.png\n";
    }

    // ---- High score button ----
    if (std::filesystem::exists("assets/Menu/High score.png") &&
        highScoreButtonTexture.loadFromFile("assets/Menu/High score.png")) {
        highScoreButtonTexture.setSmooth(true);
        highScoreButtonValid = true;
        highScoreButton.sprite   = new sf::Sprite(highScoreButtonTexture);
        highScoreButton.position = { gameWidth / 2.f, gameHeight / 2.f + 100.f };
        highScoreButton.size     = { 200.f, 80.f };
        highScoreButton.sprite->setOrigin({
            highScoreButtonTexture.getSize().x / 2.f,
            highScoreButtonTexture.getSize().y / 2.f });
        highScoreButton.sprite->setPosition(highScoreButton.position);
        highScoreButton.sprite->setScale({ 0.8f, 0.8f });
        std::cout << "[Info] Loaded High score.png\n";
    }

    // ---- Quit button ----
    if (std::filesystem::exists("assets/Menu/quit_button.png") &&
        quitButtonTexture.loadFromFile("assets/Menu/quit_button.png")) {
        quitButtonTexture.setSmooth(true);
        quitButtonValid = true;
        quitButton.sprite   = new sf::Sprite(quitButtonTexture);
        quitButton.position = { gameWidth / 2.f, gameHeight / 2.f + 200.f };
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

    // ---- Pause button ----
    if (std::filesystem::exists("assets/Menu/Paush_button.png") &&
        pauseButtonTexture.loadFromFile("assets/Menu/Paush_button.png")) {
        pauseButtonTexture.setSmooth(true);
        pauseButtonValid = true;
        pauseButton.sprite   = new sf::Sprite(pauseButtonTexture);
        float desiredSize  = 40.f;
        float clickboxSize = 50.f;
        pauseButton.size     = { clickboxSize, clickboxSize };
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
        if (settingsButton.contains(mousePos)) {
            gameState = GameState::Settings;
            std::cout << "[Info] Settings clicked\n";
        }
        if (highScoreButton.contains(mousePos)) {
            gameState = GameState::HighScores;
            std::cout << "[Info] High scores clicked\n";
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
        settingsButton.updateHover(mousePos);
        highScoreButton.updateHover(mousePos);
        quitButton.updateHover(mousePos);
    } else if (gameState == GameState::Paused) {
        resumeButton.updateHover(mousePos);
        mainMenuButton.updateHover(mousePos);
    } else if (gameState == GameState::GameOver) {
        retryButton.updateHover(mousePos);
        mainMenuButton.updateHover(mousePos);
    }
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

    if (nameTitleValid) {
        sf::Sprite nameSprite(nameTitleTexture);
        float s = (gameWidth * 0.7f) / static_cast<float>(nameTitleTexture.getSize().x);
        nameSprite.setScale({ s, s });
        float scaledW = nameTitleTexture.getSize().x * s;
        nameSprite.setPosition({ (gameWidth - scaledW) / 2.f, gameHeight * 0.08f });
        window.draw(nameSprite);
    }

    if (playButtonValid && playButton.sprite) {
        playButton.updateScale(0.8f);
        window.draw(*playButton.sprite);
    }
    if (settingsButtonValid && settingsButton.sprite) {
        settingsButton.updateScale(0.8f);
        window.draw(*settingsButton.sprite);
    }
    if (highScoreButtonValid && highScoreButton.sprite) {
        highScoreButton.updateScale(0.8f);
        window.draw(*highScoreButton.sprite);
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
    sf::RectangleShape overlay({ gameWidth, gameHeight });
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

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
        t.setCharacterSize(28);
        t.setFillColor(sf::Color::Cyan);
        auto tb = t.getLocalBounds();
        t.setOrigin({ tb.size.x / 2.f, 0.f });
        t.setPosition({ gameWidth / 2.f, gameHeight * 0.2f });
        window.draw(t);
    }

    if (fontValid) {
        sf::Text sc(*font);
        sc.setString("SCORE: " + std::to_string(currentScore));
        sc.setCharacterSize(16);
        sc.setFillColor(sf::Color::White);
        auto b = sc.getLocalBounds();
        sc.setOrigin({ b.size.x / 2.f, 0.f });
        sc.setPosition({ gameWidth / 2.f, gameHeight * 0.42f });
        window.draw(sc);
    }

    if (resumeButtonValid && resumeButton.sprite) {
        resumeButton.updateScale(0.8f);
        window.draw(*resumeButton.sprite);
    }

    if (mainMenuButtonValid && mainMenuButton.sprite) {
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
        t.setCharacterSize(32);
        t.setFillColor(sf::Color::Red);
        auto tb = t.getLocalBounds();
        t.setOrigin({ tb.size.x / 2.f, 0.f });
        t.setPosition({ gameWidth / 2.f, gameHeight * 0.2f });
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
    label.setCharacterSize(20);
    label.setFillColor(sf::Color(180, 180, 180));
    auto lb = label.getLocalBounds();
    label.setOrigin({ lb.size.x / 2.f, 0.f });
    label.setPosition({ gameWidth / 2.f, gameHeight / 2.f - 20.f });
    window.draw(label);

    sf::Text val(*font);
    val.setString(std::to_string(finalScore));
    val.setCharacterSize(64);
    val.setFillColor(sf::Color::Cyan);
    val.setOutlineThickness(2.f);
    val.setOutlineColor(sf::Color(0, 80, 120));
    auto vb = val.getLocalBounds();
    val.setOrigin({ vb.size.x / 2.f, 0.f });
    val.setPosition({ gameWidth / 2.f, gameHeight / 2.f + 10.f });
    window.draw(val);
}

// ---------------------------------------------------------------- drawHighScores
void MainMenu::drawHighScores(sf::RenderWindow& window,
                               const std::vector<int>& scores) {
    if (menuBackgroundValid) {
        sf::Sprite bg(menuBackgroundTexture);
        float s = gameWidth / static_cast<float>(menuBackgroundTexture.getSize().x);
        bg.setScale({ s, s });
        bg.setPosition({ 0.f, 0.f });
        window.draw(bg);
    } else {
        sf::RectangleShape bg({ gameWidth, gameHeight });
        bg.setFillColor(sf::Color::Black);
        window.draw(bg);
    }

    if (!fontValid) return;

    // ---- Title ----
    sf::Text title(*font);
    title.setString("HIGH SCORES");
    title.setCharacterSize(22);
    title.setFillColor(sf::Color::White);
    auto tb = title.getLocalBounds();
    title.setOrigin({ tb.size.x / 2.f, 0.f });
    title.setPosition({ gameWidth / 2.f, 60.f });
    window.draw(title);

    sf::RectangleShape div({ gameWidth - 60.f, 2.f });
    div.setFillColor(sf::Color(80, 80, 120));
    div.setPosition({ 30.f, 104.f });
    window.draw(div);

    // ---- Score rows ----
    if (scores.empty()) {
        sf::Text empty(*font);
        empty.setString("NO SCORES YET");
        empty.setCharacterSize(14);
        empty.setFillColor(sf::Color(120, 120, 120));
        auto eb = empty.getLocalBounds();
        empty.setOrigin({ eb.size.x / 2.f, 0.f });
        empty.setPosition({ gameWidth / 2.f, 280.f });
        window.draw(empty);
    } else {
        for (int i = 0; i < static_cast<int>(scores.size()); i++) {
            float y = 130.f + i * 72.f;

            // Row background
            sf::RectangleShape row({ gameWidth - 80.f, 56.f });
            sf::Color rowColor;
            switch (i) {
                case 0:  rowColor = sf::Color(120, 100, 20, 180); break;
                case 1:  rowColor = sf::Color(80,  80,  90, 180); break;
                case 2:  rowColor = sf::Color(100, 60,  30, 180); break;
                default: rowColor = sf::Color(30,  30,  50, 180); break;
            }
            row.setFillColor(rowColor);
            row.setOutlineThickness(1.f);
            row.setOutlineColor(sf::Color(80, 80, 120));
            row.setPosition({ 40.f, y });
            window.draw(row);

            // Rank
            sf::Text rank(*font);
            rank.setString("#" + std::to_string(i + 1));
            rank.setCharacterSize(16);
            sf::Color rankColor;
            switch (i) {
                case 0:  rankColor = sf::Color(255, 215, 0);   break;
                case 1:  rankColor = sf::Color(192, 192, 192); break;
                case 2:  rankColor = sf::Color(205, 127, 50);  break;
                default: rankColor = sf::Color(160, 160, 160); break;
            }
            rank.setFillColor(rankColor);
            rank.setPosition({ 60.f, y + 16.f });
            window.draw(rank);

            // Score value
            sf::Text scoreText(*font);
            scoreText.setString(std::to_string(scores[i]));
            scoreText.setCharacterSize(20);
            scoreText.setFillColor(sf::Color::Cyan);
            auto sb = scoreText.getLocalBounds();
            scoreText.setOrigin({ sb.size.x, 0.f });
            scoreText.setPosition({ gameWidth - 60.f, y + 14.f });
            window.draw(scoreText);
        }
    }

    // ---- Back button ----
    sf::RectangleShape backBox({ 160.f, 40.f });
    backBox.setFillColor(sf::Color(30, 30, 60));
    backBox.setOutlineThickness(2.f);
    backBox.setOutlineColor(sf::Color(80, 80, 180));
    backBox.setPosition({ gameWidth / 2.f - 80.f, gameHeight - 76.f });
    window.draw(backBox);

    sf::Text backText(*font);
    backText.setString("BACK");
    backText.setCharacterSize(13);
    backText.setFillColor(sf::Color::White);
    auto bb = backText.getLocalBounds();
    backText.setOrigin({ bb.size.x / 2.f, 0.f });
    backText.setPosition({ gameWidth / 2.f, gameHeight - 66.f });
    window.draw(backText);
}

// ---------------------------------------------------------------- scancodeToString
std::string MainMenu::scancodeToString(sf::Keyboard::Scancode code) const {
    switch (code) {
        case sf::Keyboard::Scancode::A:        return "A";
        case sf::Keyboard::Scancode::B:        return "B";
        case sf::Keyboard::Scancode::C:        return "C";
        case sf::Keyboard::Scancode::D:        return "D";
        case sf::Keyboard::Scancode::E:        return "E";
        case sf::Keyboard::Scancode::F:        return "F";
        case sf::Keyboard::Scancode::G:        return "G";
        case sf::Keyboard::Scancode::H:        return "H";
        case sf::Keyboard::Scancode::I:        return "I";
        case sf::Keyboard::Scancode::J:        return "J";
        case sf::Keyboard::Scancode::K:        return "K";
        case sf::Keyboard::Scancode::L:        return "L";
        case sf::Keyboard::Scancode::M:        return "M";
        case sf::Keyboard::Scancode::N:        return "N";
        case sf::Keyboard::Scancode::O:        return "O";
        case sf::Keyboard::Scancode::P:        return "P";
        case sf::Keyboard::Scancode::Q:        return "Q";
        case sf::Keyboard::Scancode::R:        return "R";
        case sf::Keyboard::Scancode::S:        return "S";
        case sf::Keyboard::Scancode::T:        return "T";
        case sf::Keyboard::Scancode::U:        return "U";
        case sf::Keyboard::Scancode::V:        return "V";
        case sf::Keyboard::Scancode::W:        return "W";
        case sf::Keyboard::Scancode::X:        return "X";
        case sf::Keyboard::Scancode::Y:        return "Y";
        case sf::Keyboard::Scancode::Z:        return "Z";
        case sf::Keyboard::Scancode::Num0:     return "0";
        case sf::Keyboard::Scancode::Num1:     return "1";
        case sf::Keyboard::Scancode::Num2:     return "2";
        case sf::Keyboard::Scancode::Num3:     return "3";
        case sf::Keyboard::Scancode::Num4:     return "4";
        case sf::Keyboard::Scancode::Num5:     return "5";
        case sf::Keyboard::Scancode::Num6:     return "6";
        case sf::Keyboard::Scancode::Num7:     return "7";
        case sf::Keyboard::Scancode::Num8:     return "8";
        case sf::Keyboard::Scancode::Num9:     return "9";
        case sf::Keyboard::Scancode::Space:    return "SPACE";
        case sf::Keyboard::Scancode::Up:       return "UP";
        case sf::Keyboard::Scancode::Down:     return "DOWN";
        case sf::Keyboard::Scancode::Left:     return "LEFT";
        case sf::Keyboard::Scancode::Right:    return "RIGHT";
        case sf::Keyboard::Scancode::Enter:    return "ENTER";
        case sf::Keyboard::Scancode::Tab:      return "TAB";
        case sf::Keyboard::Scancode::LShift:   return "LSHIFT";
        case sf::Keyboard::Scancode::RShift:   return "RSHIFT";
        case sf::Keyboard::Scancode::LControl: return "LCTRL";
        case sf::Keyboard::Scancode::LAlt:     return "LALT";
        default:                               return "???";
    }
}

// ---------------------------------------------------------------- applyRebind
void MainMenu::applyRebind(sf::Keyboard::Scancode code) {
    if (code == sf::Keyboard::Scancode::Escape) {
        std::cout << "[Info] Escape is reserved\n";
        rebindingIndex = -1;
        return;
    }

    sf::Keyboard::Scancode allKeys[] = {
        keyMoveLeft, keyMoveRight, keyMoveUp, keyMoveDown, keyShoot
    };

    for (int i = 0; i < 5; i++) {
        if (i == rebindingIndex) continue;
        if (allKeys[i] == code) {
            std::cout << "[Info] Key " << scancodeToString(code)
                      << " already used — rejected\n";
            conflictIndex = i;
            conflictTimer = 1.2f;
            return;
        }
    }

    switch (rebindingIndex) {
        case 0: keyMoveLeft  = code; break;
        case 1: keyMoveRight = code; break;
        case 2: keyMoveUp    = code; break;
        case 3: keyMoveDown  = code; break;
        case 4: keyShoot     = code; break;
    }

    std::cout << "[Info] Rebound control " << rebindingIndex
              << " to " << scancodeToString(code) << "\n";
    rebindingIndex = -1;
}

// ---------------------------------------------------------------- drawSettings
void MainMenu::drawSettings(sf::RenderWindow& window) {
    // Tick conflict flash timer
    if (conflictTimer > 0.f) conflictTimer -= 0.016f;
    else                      conflictIndex  = -1;

    if (menuBackgroundValid) {
        sf::Sprite bg(menuBackgroundTexture);
        float s = gameWidth / static_cast<float>(menuBackgroundTexture.getSize().x);
        bg.setScale({ s, s });
        bg.setPosition({ 0.f, 0.f });
        window.draw(bg);
    } else {
        sf::RectangleShape bg({ gameWidth, gameHeight });
        bg.setFillColor(sf::Color::Black);
        window.draw(bg);
    }

    if (!fontValid) return;

    // Title
    sf::Text title(*font);
    title.setString("SETTINGS");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    auto tb = title.getLocalBounds();
    title.setOrigin({ tb.size.x / 2.f, 0.f });
    title.setPosition({ gameWidth / 2.f, 30.f });
    window.draw(title);

    sf::RectangleShape div1({ gameWidth - 60.f, 2.f });
    div1.setFillColor(sf::Color(80, 80, 120));
    div1.setPosition({ 30.f, 74.f });
    window.draw(div1);

    // Sound toggle
    sf::Text soundLabel(*font);
    soundLabel.setString("SOUND");
    soundLabel.setCharacterSize(13);
    soundLabel.setFillColor(sf::Color(200, 200, 200));
    soundLabel.setPosition({ 40.f, 90.f });
    window.draw(soundLabel);

    sf::RectangleShape soundBox({ 80.f, 30.f });
    soundBox.setFillColor(soundEnabled ? sf::Color(40,160,80) : sf::Color(160,40,40));
    soundBox.setPosition({ gameWidth - 130.f, 86.f });
    window.draw(soundBox);

    sf::Text soundVal(*font);
    soundVal.setString(soundEnabled ? "ON" : "OFF");
    soundVal.setCharacterSize(12);
    soundVal.setFillColor(sf::Color::White);
    auto sv = soundVal.getLocalBounds();
    soundVal.setOrigin({ sv.size.x / 2.f, 0.f });
    soundVal.setPosition({ gameWidth - 90.f, 93.f });
    window.draw(soundVal);

    // Music toggle
    sf::Text musicLabel(*font);
    musicLabel.setString("MUSIC");
    musicLabel.setCharacterSize(13);
    musicLabel.setFillColor(sf::Color(200, 200, 200));
    musicLabel.setPosition({ 40.f, 136.f });
    window.draw(musicLabel);

    sf::RectangleShape musicBox({ 80.f, 30.f });
    musicBox.setFillColor(musicEnabled ? sf::Color(40,160,80) : sf::Color(160,40,40));
    musicBox.setPosition({ gameWidth - 130.f, 132.f });
    window.draw(musicBox);

    sf::Text musicVal(*font);
    musicVal.setString(musicEnabled ? "ON" : "OFF");
    musicVal.setCharacterSize(12);
    musicVal.setFillColor(sf::Color::White);
    auto mv = musicVal.getLocalBounds();
    musicVal.setOrigin({ mv.size.x / 2.f, 0.f });
    musicVal.setPosition({ gameWidth - 90.f, 139.f });
    window.draw(musicVal);

    sf::RectangleShape div2({ gameWidth - 60.f, 2.f });
    div2.setFillColor(sf::Color(80, 80, 120));
    div2.setPosition({ 30.f, 178.f });
    window.draw(div2);

    // Controls title
    sf::Text ctrlTitle(*font);
    ctrlTitle.setString("CONTROLS");
    ctrlTitle.setCharacterSize(14);
    ctrlTitle.setFillColor(sf::Color(180, 180, 255));
    ctrlTitle.setPosition({ 40.f, 192.f });
    window.draw(ctrlTitle);

    sf::Text hint(*font);
    hint.setString("CLICK KEY TO REBIND");
    hint.setCharacterSize(9);
    hint.setFillColor(sf::Color(120, 120, 120));
    hint.setPosition({ 40.f, 218.f });
    window.draw(hint);

    const char* actionNames[] = {
        "MOVE LEFT", "MOVE RIGHT", "MOVE UP", "MOVE DOWN", "SHOOT"
    };
    sf::Keyboard::Scancode currentKeys[] = {
        keyMoveLeft, keyMoveRight, keyMoveUp, keyMoveDown, keyShoot
    };

    for (int i = 0; i < 5; i++) {
        float y = 244.f + i * 46.f;
        bool isBeingRebound = (rebindingIndex == i);
        bool isConflicting  = (conflictIndex  == i);

        sf::Text action(*font);
        action.setString(actionNames[i]);
        action.setCharacterSize(11);
        action.setFillColor(sf::Color(255, 220, 80));
        action.setPosition({ 40.f, y });
        window.draw(action);

        sf::RectangleShape keyBox({ 120.f, 30.f });
        if (isBeingRebound)     keyBox.setFillColor(sf::Color(80,  80,  180));
        else if (isConflicting) keyBox.setFillColor(sf::Color(180, 40,  40));
        else                    keyBox.setFillColor(sf::Color(40,  40,  70));
        keyBox.setOutlineThickness(1.f);
        keyBox.setOutlineColor(isBeingRebound
            ? sf::Color(150, 150, 255)
            : isConflicting
                ? sf::Color(255, 80, 80)
                : sf::Color(80, 80, 120));
        keyBox.setPosition({ gameWidth - 160.f, y - 4.f });
        window.draw(keyBox);

        sf::Text keyLabel(*font);
        keyLabel.setString(isBeingRebound
            ? "PRESS KEY..."
            : isConflicting
                ? "TAKEN!"
                : scancodeToString(currentKeys[i]));
        keyLabel.setCharacterSize(10);
        keyLabel.setFillColor(isBeingRebound || isConflicting
            ? sf::Color(200, 200, 255)
            : sf::Color::White);
        auto kl = keyLabel.getLocalBounds();
        keyLabel.setOrigin({ kl.size.x / 2.f, 0.f });
        keyLabel.setPosition({ gameWidth - 100.f, y + 2.f });
        window.draw(keyLabel);
    }

    sf::RectangleShape div3({ gameWidth - 60.f, 2.f });
    div3.setFillColor(sf::Color(80, 80, 120));
    div3.setPosition({ 30.f, gameHeight - 90.f });
    window.draw(div3);

    sf::RectangleShape backBox({ 160.f, 40.f });
    backBox.setFillColor(sf::Color(30, 30, 60));
    backBox.setOutlineThickness(2.f);
    backBox.setOutlineColor(sf::Color(80, 80, 180));
    backBox.setPosition({ gameWidth / 2.f - 80.f, gameHeight - 76.f });
    window.draw(backBox);

    sf::Text backText(*font);
    backText.setString("BACK");
    backText.setCharacterSize(13);
    backText.setFillColor(sf::Color::White);
    auto bb = backText.getLocalBounds();
    backText.setOrigin({ bb.size.x / 2.f, 0.f });
    backText.setPosition({ gameWidth / 2.f, gameHeight - 66.f });
    window.draw(backText);
}

// ---------------------------------------------------------------- simple getters
bool MainMenu::isPauseButtonPressed(const sf::Vector2f& p) const    { return pauseButton.contains(p); }
bool MainMenu::isResumeButtonPressed(const sf::Vector2f& p) const   { return resumeButton.contains(p); }
bool MainMenu::isPlayButtonPressed(const sf::Vector2f& p) const     { return playButton.contains(p); }
bool MainMenu::isQuitButtonPressed(const sf::Vector2f& p) const     { return quitButton.contains(p); }
bool MainMenu::isRetryButtonPressed(const sf::Vector2f& p) const    { return retryButton.contains(p); }
bool MainMenu::isMainMenuButtonPressed(const sf::Vector2f& p) const { return mainMenuButton.contains(p); }
bool MainMenu::isSettingsButtonPressed(const sf::Vector2f& p) const { return settingsButton.contains(p); }
bool MainMenu::isHighScoreButtonPressed(const sf::Vector2f& p) const { return highScoreButton.contains(p); }