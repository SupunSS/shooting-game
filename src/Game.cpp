#include "Game.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <cmath>

Game::Game()
    : window(sf::VideoMode({ 480u, 640u }), "Nokia Shooter")
    , playerSprite(playerTexture) {

    window.setFramerateLimit(60);

    // Diagnostics: print current working directory and window info
    try {
        std::cout << "[Info] CWD: " << std::filesystem::current_path() << '\n';
    } catch (...) {
        std::cout << "[Info] CWD: (unable to query)" << '\n';
    }
    std::cout << "[Info] Window size: " << window.getSize().x << "x" << window.getSize().y << '\n';

    // Load player ship image (relative path, assets are copied to the executable dir by CMake)
    const std::string shipPath = "assets/Ships/player_ship.png";
    if (!std::filesystem::exists(shipPath)) {
        std::cerr << "[Error] Asset not found: " << shipPath << '\n';
    } else {
        std::cout << "[Info] Found asset: " << shipPath << '\n';
    }

    sf::Image image;
    if (!image.loadFromFile(shipPath)) {
        std::cerr << "[Error] failed to load player ship image: " << shipPath << "\n";
    } else {
        if (!playerTexture.loadFromImage(image)) {
            std::cerr << "[Error] failed to create texture from image\n";
        } else {
            std::cout << "[Info] Loaded player texture successfully\n";
            playerTexture.setSmooth(false); // keep pixel art crisp
            playerTextureValid = true;

            // Print texture size
            auto texSize = playerTexture.getSize();
            std::cout << "[Info] Texture size: " << texSize.x << "x" << texSize.y << "\n";

            // sample a pixel from the image center
            unsigned iw = image.getSize().x;
            unsigned ih = image.getSize().y;
            if (iw > 0 && ih > 0) {
                auto px = image.getPixel(sf::Vector2u(iw/2, ih/2));
                playerSampledAlpha = px.a;
                std::cout << "[Info] Sampled pixel (r,g,b,a): " << (int)px.r << "," << (int)px.g << "," << (int)px.b << "," << (int)px.a << "\n";
            }

            // Assign texture to sprite now that it's valid
            playerSprite.setTexture(playerTexture, true);

            // Center the origin based on texture size
            float w = static_cast<float>(texSize.x);
            float h = static_cast<float>(texSize.y);
            playerSprite.setOrigin({ w / 2.f, h / 2.f });
            playerSprite.setPosition({ 240.f, 580.f });
            playerHalfSize = { w/2.f, h/2.f };

            // prepare debug rectangle in case texture is invisible
            debugRect.setSize({ w, h });
            debugRect.setOrigin({ w / 2.f, h / 2.f });
            debugRect.setPosition(playerSprite.getPosition());
            if (playerSampledAlpha == 0) debugRect.setFillColor(sf::Color::Yellow);
            else debugRect.setFillColor(sf::Color::Magenta);
        }
    }
}

void Game::run() {
    std::cout << "[Info] Game starting\n";
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
    std::cout << "[Info] Game stopped\n";
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            std::cout << "[Info] Window close event received\n";
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                std::cout << "[Info] Escape pressed - closing\n";
                window.close();
            }

            // movement flags
            if (keyPressed->scancode == sf::Keyboard::Scancode::Left || keyPressed->scancode == sf::Keyboard::Scancode::A) movingLeft = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Right || keyPressed->scancode == sf::Keyboard::Scancode::D) movingRight = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Up || keyPressed->scancode == sf::Keyboard::Scancode::W) movingUp = true;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Down || keyPressed->scancode == sf::Keyboard::Scancode::S) movingDown = true;

            // Shoot on Space
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                float elapsed = shootCooldown.getElapsedTime().asSeconds();
                if (elapsed > 0.25f) {
                    Bullet b;
                    b.shape.setRadius(5.f);
                    b.shape.setFillColor(sf::Color::Yellow);
                    b.shape.setOrigin({ 5.f, 5.f });
                    b.shape.setPosition(playerSprite.getPosition());
                    bullets.push_back(b);
                    shootCooldown.restart();
                    std::cout << "[Info] Shot fired, total bullets: " << bullets.size() << "\n";
                } else {
                    std::cout << "[Warning] Shoot on cooldown (" << elapsed << "s)\n";
                }
            }
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::Left || keyReleased->scancode == sf::Keyboard::Scancode::A) movingLeft = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Right || keyReleased->scancode == sf::Keyboard::Scancode::D) movingRight = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Up || keyReleased->scancode == sf::Keyboard::Scancode::W) movingUp = false;
            if (keyReleased->scancode == sf::Keyboard::Scancode::Down || keyReleased->scancode == sf::Keyboard::Scancode::S) movingDown = false;
        }
    }
}

void Game::update(float dt) {
    // Player movement (allow up/down/front/back)
    sf::Vector2f move(0.f, 0.f);
    if (movingLeft)  move.x -= playerSpeed * dt;
    if (movingRight) move.x += playerSpeed * dt;
    if (movingUp)    move.y -= playerSpeed * dt;
    if (movingDown)  move.y += playerSpeed * dt;

    const sf::Vector2f unclamped = playerSprite.getPosition() + move;
    sf::Vector2f pos = unclamped;
    // clamp with respect to playerHalfSize and window size
    pos.x = std::clamp(pos.x, playerHalfSize.x, (float)window.getSize().x - playerHalfSize.x);
    pos.y = std::clamp(pos.y, playerHalfSize.y, (float)window.getSize().y - playerHalfSize.y);
    if (std::fabs(pos.x - unclamped.x) > 0.001f || std::fabs(pos.y - unclamped.y) > 0.001f) {
        std::cout << "[Info] Player clamped to (" << pos.x << "," << pos.y << ") (attempted (" << unclamped.x << "," << unclamped.y << "))\n";
    }
    playerSprite.setPosition(pos);

    // Move bullets up
    for (auto& b : bullets)
        b.shape.move({ 0.f, -b.speed * dt });

    // Remove off-screen bullets
    size_t before = bullets.size();
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.shape.getPosition().y < 0.f; }),
        bullets.end());
    size_t after = bullets.size();
    if (before != after) {
        std::cout << "[Info] Removed " << (before - after) << " off-screen bullets, now " << after << " remain\n";
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    if (playerTextureValid) {
        window.draw(playerSprite);
    } else {
        // draw magenta fallback so missing texture is visible
        debugRect.setPosition(playerSprite.getPosition());
        window.draw(debugRect);
    }
    for (auto& b : bullets)
        window.draw(b.shape);
    window.display();
}