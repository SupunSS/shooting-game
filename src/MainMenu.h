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
    Settings,
    HighScores,   
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
    void drawSettings(sf::RenderWindow& window);

    bool isPauseButtonPressed(const sf::Vector2f& mousePos) const;
    bool isResumeButtonPressed(const sf::Vector2f& mousePos) const;
    bool isPlayButtonPressed(const sf::Vector2f& mousePos) const;
    bool isQuitButtonPressed(const sf::Vector2f& mousePos) const;
    bool isRetryButtonPressed(const sf::Vector2f& mousePos) const;
    bool isMainMenuButtonPressed(const sf::Vector2f& mousePos) const;
    bool isSettingsButtonPressed(const sf::Vector2f& mousePos) const;

    // Toggle functions for settings screen
    void toggleSound() { soundEnabled = !soundEnabled; }
    void toggleMusic() { musicEnabled = !musicEnabled; }
    bool getSoundEnabled() const { return soundEnabled; }
    bool getMusicEnabled() const { return musicEnabled; }

    void drawHighScores(sf::RenderWindow& window,
                    const std::vector<int>& scores);
bool isHighScoreButtonPressed(const sf::Vector2f& mousePos) const;

    // Key binding getters
sf::Keyboard::Scancode getMoveLeftKey()  const { return keyMoveLeft;  }
sf::Keyboard::Scancode getMoveRightKey() const { return keyMoveRight; }
sf::Keyboard::Scancode getMoveUpKey()    const { return keyMoveUp;    }
sf::Keyboard::Scancode getMoveDownKey()  const { return keyMoveDown;  }
sf::Keyboard::Scancode getShootKey()     const { return keyShoot;     }

// Called from GameInput when rebinding
void startRebinding(int index) { rebindingIndex = index; }
void applyRebind(sf::Keyboard::Scancode code);
bool isRebinding() const { return rebindingIndex >= 0; }

private:
    void drawScore(sf::RenderWindow& window, int finalScore);

    float     gameWidth;
    float     gameHeight;
    sf::Font* font;
    bool      fontValid;

    // Sound / music state
    bool soundEnabled = true;
    bool musicEnabled = true;

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
    sf::Texture settingsButtonTexture;

    bool playButtonValid     = false;
    bool quitButtonValid     = false;
    bool retryButtonValid    = false;
    bool mainMenuButtonValid = false;
    bool pauseButtonValid    = false;
    bool pausedTitleValid    = false;
    bool resumeButtonValid   = false;
    bool settingsButtonValid = false;

    Button playButton;
    Button quitButton;
    Button retryButton;
    Button mainMenuButton;
    Button pauseButton;
    Button resumeButton;
    Button settingsButton;

    // Key bindings
sf::Keyboard::Scancode keyMoveLeft  = sf::Keyboard::Scancode::A;
sf::Keyboard::Scancode keyMoveRight = sf::Keyboard::Scancode::D;
sf::Keyboard::Scancode keyMoveUp    = sf::Keyboard::Scancode::W;
sf::Keyboard::Scancode keyMoveDown  = sf::Keyboard::Scancode::S;
sf::Keyboard::Scancode keyShoot     = sf::Keyboard::Scancode::Space;

int rebindingIndex = -1; // -1 = not rebinding, 0-4 = which binding

std::string scancodeToString(sf::Keyboard::Scancode code) const;

int   conflictIndex = -1;  // which row is flashing red due to conflict
float conflictTimer = 0.f; // countdown until flash clears

sf::Texture highScoreButtonTexture;
bool        highScoreButtonValid = false;
Button      highScoreButton;
};