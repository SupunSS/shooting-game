#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Button structure
struct Button {
    sf::Sprite* sprite = nullptr;
    sf::Vector2f position;
    sf::Vector2f size;
    bool hovered = false;
    bool wasPressed = false;

    bool contains(sf::Vector2f point) const {
        return point.x >= position.x - size.x / 2.f &&
               point.x <= position.x + size.x / 2.f &&
               point.y >= position.y - size.y / 2.f &&
               point.y <= position.y + size.y / 2.f;
    }

    void updateHover(sf::Vector2f mousePos) {
        hovered = contains(mousePos);
    }

    void updateScale(float scale) {
        if (sprite) {
            sprite->setScale({ scale * (hovered ? 1.1f : 1.0f), 
                              scale * (hovered ? 1.1f : 1.0f) });
        }
    }
};

// Game state enum
enum class GameState {
    MainMenu,
    Playing,
    GameOver
};

class MainMenu {
public:
    MainMenu();  // Default constructor
    MainMenu(float gameWidth, float gameHeight, sf::Font* font);
    ~MainMenu();

    // Initialize menu with game dimensions and font
    void initialize(float width, float height, sf::Font* gameFont, bool fontLoaded);

    // Load all menu assets
    void loadAssets();
    void handleMouseClick(const sf::Vector2f& mousePos, GameState& gameState, sf::RenderWindow& window);
    void updateButtonHover(const sf::Vector2f& mousePos, GameState gameState);

    // Rendering
    void drawMainMenu(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int finalScore);

    // Getters for button states
    bool isPlayButtonPressed(const sf::Vector2f& mousePos) const;
    bool isQuitButtonPressed(const sf::Vector2f& mousePos) const;
    bool isRetryButtonPressed(const sf::Vector2f& mousePos) const;
    bool isMainMenuButtonPressed(const sf::Vector2f& mousePos) const;

private:
    void drawScore(sf::RenderWindow& window, int finalScore);

    // Game dimensions
    float gameWidth;
    float gameHeight;

    // Font pointer (owned by Game class)
    sf::Font* font;
    bool fontValid;

    // Menu background
    sf::Texture menuBackgroundTexture;
    bool menuBackgroundValid = false;

    // Button textures
    sf::Texture playButtonTexture;
    sf::Texture quitButtonTexture;
    sf::Texture retryButtonTexture;
    sf::Texture mainMenuButtonTexture;
    bool playButtonValid = false;
    bool quitButtonValid = false;
    bool retryButtonValid = false;
    bool mainMenuButtonValid = false;

    // Button instances
    Button playButton;
    Button quitButton;
    Button retryButton;
    Button mainMenuButton;
};
