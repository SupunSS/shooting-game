#pragma once
#include <SFML/Graphics.hpp>

struct Button {
    sf::Sprite*  sprite = nullptr;
    sf::Vector2f position;
    sf::Vector2f size;
    bool         hovered    = false;
    bool         wasPressed = false;

    bool contains(sf::Vector2f point) const {
        return point.x >= position.x - size.x / 2.f &&
               point.x <= position.x + size.x / 2.f &&
               point.y >= position.y - size.y / 2.f &&
               point.y <= position.y + size.y / 2.f;
    }
    void updateHover(sf::Vector2f mousePos) { hovered = contains(mousePos); }
    void updateScale(float scale) {
        if (sprite)
            sprite->setScale({ scale * (hovered ? 1.1f : 1.0f),
                               scale * (hovered ? 1.1f : 1.0f) });
    }
};

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

class MainMenu {
public:
    MainMenu();
    MainMenu(float gameWidth, float gameHeight, sf::Font* font);
    ~MainMenu();

    void initialize(float width, float height, sf::Font* gameFont, bool fontLoaded);
    void loadAssets();

    void handleMouseClick(const sf::Vector2f& mousePos,
                          GameState& gameState,
                          sf::RenderWindow& window);
    void updateButtonHover(const sf::Vector2f& mousePos, GameState gameState);

    void drawMainMenu(sf::RenderWindow& window);
    void drawPauseButton(sf::RenderWindow& window);
    void drawPauseOverlay(sf::RenderWindow& window, int currentScore);
    void drawGameOver(sf::RenderWindow& window, int finalScore);

    bool isPauseButtonPressed(const sf::Vector2f& mousePos) const;
    bool isResumeButtonPressed(const sf::Vector2f& mousePos) const;
    bool isPlayButtonPressed(const sf::Vector2f& mousePos) const;
    bool isQuitButtonPressed(const sf::Vector2f& mousePos) const;
    bool isRetryButtonPressed(const sf::Vector2f& mousePos) const;
    bool isMainMenuButtonPressed(const sf::Vector2f& mousePos) const;

private:
    void drawScore(sf::RenderWindow& window, int finalScore);

    float     gameWidth;
    float     gameHeight;
    sf::Font* font;
    bool      fontValid;

    // Background + title
    sf::Texture menuBackgroundTexture;
    bool        menuBackgroundValid = false;
    sf::Texture nameTitleTexture;
    bool        nameTitleValid = false;
    sf::Texture gameOverTexture;
    bool        gameOverValid = false;

    // Button textures
    sf::Texture playButtonTexture;
    sf::Texture quitButtonTexture;
    sf::Texture retryButtonTexture;
    sf::Texture mainMenuButtonTexture;
    sf::Texture pauseButtonTexture;
    sf::Texture pausedTitleTexture;
    sf::Texture resumeButtonTexture;

    bool playButtonValid     = false;
    bool quitButtonValid     = false;
    bool retryButtonValid    = false;
    bool mainMenuButtonValid = false;
    bool pauseButtonValid    = false;
    bool pausedTitleValid    = false;
    bool resumeButtonValid   = false;

    Button playButton;
    Button quitButton;
    Button retryButton;
    Button mainMenuButton;
    Button pauseButton;
    Button resumeButton;
};