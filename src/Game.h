#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

struct Bullet {
    sf::CircleShape shape;
    float speed = 500.f;
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void drawGlowBullet(const Bullet& b);

    sf::RenderWindow window;
    sf::Texture playerTexture;
    sf::Sprite  playerSprite;
    bool playerTextureValid = false;
    sf::RectangleShape debugRect;
    sf::Texture bulletTexture;
    bool bulletTextureValid = false;
    unsigned playerSampledAlpha = 255;
    sf::Vector2f playerHalfSize{15.f,15.f};
    float playerSpeed = 200.f;
    float bulletRadius = 3.f;
    // movement flags (set via events for reliable input)
    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;

    std::vector<Bullet> bullets;
    sf::Clock shootCooldown;
};
