#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "MainMenu.h"

struct Bullet {
    sf::CircleShape shape;
    float speed = 500.f;
    bool dead = false;
};

struct EnemyBullet {
    explicit EnemyBullet(const sf::Texture& texture)
        : sprite(texture) {}

    sf::Sprite   sprite;
    sf::Vector2f velocity{ 0.f, 300.f };
    float        radius = 5.f;
    bool         dead   = false;
};

struct Enemy {
    Enemy(const sf::Texture& texture, int index)
        : sprite(texture)
        , textureIndex(index) {}

    sf::Sprite  sprite;
    int         textureIndex = 0;
    float       speedX        = 120.f;
    float       speedY        = 40.f;
    float       zigzagTime    = 0.f;
    float       shootTimer    = 0.f;
    float       shootInterval = 2.f;

    float       phaseOffsetX  = 0.f;
    float       phaseOffsetY  = 0.f;
    float       zigzagSpeedX  = 0.f;
    float       zigzagSpeedY  = 0.f;
    float       baseY         = 0.f;
    bool        reachedBase   = false;
    float       radius        = 18.f;
    bool        dead          = false;
};

class Game {
public:
    Game();
    void run();

private:
    void loadBackgroundAssets();
    void loadPlayerAssets();
    void loadBulletAssets();
    void loadEnemyAssets();
    void loadEnemyBulletAssets();
    void loadUIAssets();

    void processEvents();
    void update(float dt);
    void updateBackground(float dt);
    void render();
    void drawBackground();
    void drawGlowBullet(const Bullet& b);
    void drawGlowEnemyBullet(const EnemyBullet& b);
    void spawnEnemy();
    void updateView();
    void drawHUD();

    void resetGame();

    float getBackgroundScale(const sf::Texture& texture) const;
    float getBackgroundHeight(const sf::Texture& texture) const;
    // Collision helpers
    bool circlesOverlap(sf::Vector2f posA, float rA, sf::Vector2f posB, float rB) const;
    void checkCollisions();

    static constexpr float gameWidth  = 480.f;
    static constexpr float gameHeight = 640.f;
    static constexpr int backgroundCount = 3;

    static constexpr int scorePerKill = 10;

    sf::RenderWindow   window;
    sf::View           gameView;

    // Background
    sf::Texture backgroundTextures[backgroundCount];
    bool        backgroundTexturesValid[backgroundCount] = { false, false, false };
    float       backgroundScrollOffset = 0.f;
    float       backgroundScrollSpeed = 180.f;

    // Player
    sf::Texture        playerTexture;
    sf::Sprite         playerSprite;
    bool               playerTextureValid = false;
    sf::RectangleShape debugRect;
    unsigned           playerSampledAlpha = 255;
    sf::Vector2f       playerHalfSize{ 15.f, 15.f };
    float              playerSpeed = 200.f;
    bool movingLeft  = false;
    bool movingRight = false;
    bool movingUp    = false;
    bool movingDown  = false;

    // Player bullets
    sf::Texture         bulletTexture;
    bool                bulletTextureValid = false;
    float               bulletRadius = 3.f;
    std::vector<Bullet> bullets;
    sf::Clock           shootCooldown;

    // Player health
    int   playerHealth       = 3;
    bool  playerInvincible   = false;
    float invincibleTimer    = 0.f;
    float invincibleDuration = 2.f;
    float playerFlashTimer   = 0.f;

    // Enemies
    std::vector<Enemy>       enemies;
    std::vector<EnemyBullet> enemyBullets;
    sf::Clock                enemySpawnTimer;
    float                    enemySpawnInterval     = 3.f;
    bool                     waveActive             = false;
    int                      enemiesPerWave         = 5;
    float                    waveBoundaryY          = 0.f;

    // Enemy textures - one per ship type
    sf::Texture enemyTextures[5];
    bool        enemyTexturesValid[5] = { false, false, false, false, false };

    // Enemy bullet texture
    sf::Texture enemyBulletTexture;
    bool        enemyBulletTextureValid = false;

    // HUD / UI
    sf::Texture healthFullTexture;
    sf::Texture healthDepletedTexture;
    sf::Texture scoreTexture;
    bool        healthFullValid     = false;
    bool        healthDepletedValid = false;
    bool        scoreIconValid      = false;
    int         score               = 0;
    sf::Font    hudFont;
    bool        hudFontValid        = false;

    // HUD layout - To adjust values 
    float healthHudX      = 10.f;
    float healthHudY      = gameHeight - 34.f;
    float healthIconWidth = 24.f;
    float healthIconHeight = 24.f;
    float healthIconGap   = 8.f;

    float scoreHudX       = gameWidth - 120.f;
    float scoreHudY       = gameHeight - 54.f;
    float scoreIconWidth  = 48.f;
    float scoreIconHeight = 48.f;
    unsigned int scoreTextSize = 20;

    // Game State
    GameState gameState = GameState::MainMenu;
    GameState prevState = GameState::MainMenu;

    // Menu
    MainMenu mainMenu;

    // Game Over / Score display
    float gameOverTimer = 0.f;
    int finalScore = 0;

};
