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

    sf::RenderWindow window;
    sf::CircleShape player;
    float playerSpeed = 200.f;

    std::vector<Bullet> bullets;
    sf::Clock shootCooldown;
};