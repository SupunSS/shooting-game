#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Bullet {
    sf::CircleShape shape;
    float speed = 500.f;
};

struct EnemyBullet {
    explicit EnemyBullet(const sf::Texture& texture)
        : sprite(texture) {}

    sf::Sprite   sprite;
    sf::Vector2f velocity{ 0.f, 300.f };
    float        radius = 5.f;
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
};

class Game {
public:
    Game();
    void run();

private:
    void loadPlayerAssets();
    void loadBulletAssets();
    void loadEnemyAssets();
    void loadEnemyBulletAssets();

    void processEvents();
    void update(float dt);
    void render();
    void drawGlowBullet(const Bullet& b);
    void drawGlowEnemyBullet(const EnemyBullet& b);
    void spawnEnemy();
    void updateView();

    static constexpr float gameWidth  = 480.f;
    static constexpr float gameHeight = 640.f;

    sf::RenderWindow   window;
    sf::View           gameView;

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
};
