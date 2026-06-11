#include "EnemyAI.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numeric>

// ============================================================
//  Internal constants
// ============================================================
static constexpr float PI            = 3.14159265f;
static constexpr int   MAX_HISTORY   = 30;   // ~3 seconds at 0.1s sample rate
static constexpr float GAME_WIDTH    = 480.f;
static constexpr float GAME_HEIGHT   = 640.f;

// ============================================================
//  Utility helpers
// ============================================================
static float randF(float lo, float hi) {
    return lo + static_cast<float>(std::rand()) / RAND_MAX * (hi - lo);
}

static sf::Vector2f normalize(sf::Vector2f v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len < 0.0001f) return { 0.f, 1.f };
    return { v.x / len, v.y / len };
}

// ============================================================
//  levelForWave
//  Every 3 waves share the same difficulty level.
//  Waves 1-3 → Level 1, 4-6 → Level 2, ... 16+ → Level 6 (stays forever)
//
//  Schedule:
//   Wave  1- 3  →  Level 1  (Dumb)
//   Wave  4- 6  →  Level 2  (Aware)
//   Wave  7- 9  →  Level 3  (Aggressive)
//   Wave 10-12  →  Level 4  (Tactical)
//   Wave 13-15  →  Level 5  (Evasive)
//   Wave 16+    →  Level 6  (Adaptive — stays here until player dies)
// ============================================================
AILevel EnemyAI::levelForWave(int waveNumber) {
    int lvl = std::clamp((waveNumber - 1) / 3 + 1, 1, 6);
    return static_cast<AILevel>(lvl);
}

// ============================================================
//  initialize
//  Sets up AI state when an enemy is first spawned.
// ============================================================
void EnemyAI::initialize(EnemyAIState& ai, int waveNumber, int /*enemyTextureIndex*/) {
    ai = EnemyAIState{};  // reset everything

    ai.level = levelForWave(waveNumber);

    // Base shoot interval decreases (faster) with level
    float baseInterval = 2.5f - static_cast<float>(static_cast<int>(ai.level) - 1) * 0.3f;
    ai.shootCooldown = baseInterval + randF(0.f, 1.f);

    switch (ai.level) {
        case AILevel::Dumb:
            ai.trackStrength = 0.f;
            ai.leadFactor    = 0.f;
            break;

        case AILevel::Aware:
            ai.trackStrength = 0.12f;
            ai.leadFactor    = 0.f;
            break;

        case AILevel::Aggressive:
            ai.trackStrength = 0.25f;
            ai.leadFactor    = 0.f;
            break;

        case AILevel::Tactical:
            ai.trackStrength = 0.30f;
            ai.leadFactor    = 0.4f;  // aim 0.4 s ahead of player
            break;

        case AILevel::Evasive:
            ai.trackStrength   = 0.30f;
            ai.leadFactor      = 0.5f;
            ai.evasionCooldown = randF(0.5f, 1.5f);
            break;

        case AILevel::Adaptive:
            ai.trackStrength    = 0.35f;
            ai.leadFactor       = 0.6f;
            ai.evasionCooldown  = randF(0.3f, 0.8f);
            ai.adaptiveDodgeDir = (std::rand() % 2 == 0) ? 1.f : -1.f;
            break;
    }

    std::cout << "[AI] Enemy initialised at level "
              << static_cast<int>(ai.level)
              << " (wave " << waveNumber << ")\n";
}

// ============================================================
//  update  —  main dispatch
// ============================================================
sf::Vector2f EnemyAI::update(EnemyAIState& ai,
                              const sf::Vector2f& enemyPos,
                              const PlayerState&  player,
                              float dt) {
    // Tick shoot cooldown
    if (ai.shootCooldown > 0.f) ai.shootCooldown -= dt;
    if (ai.burstCooldown > 0.f) ai.burstCooldown -= dt;
    if (ai.evasionCooldown > 0.f) ai.evasionCooldown -= dt;

    switch (ai.level) {
        case AILevel::Dumb:        return updateDumb      (ai, enemyPos, player, dt);
        case AILevel::Aware:       return updateAware     (ai, enemyPos, player, dt);
        case AILevel::Aggressive:  return updateAggressive(ai, enemyPos, player, dt);
        case AILevel::Tactical:    return updateTactical  (ai, enemyPos, player, dt);
        case AILevel::Evasive:     return updateEvasive   (ai, enemyPos, player, dt, {});
        case AILevel::Adaptive:    return updateAdaptive  (ai, enemyPos, player, dt);
        default:                   return { 0.f, 0.f };
    }
}

// ============================================================
//  wantsToShoot
// ============================================================
bool EnemyAI::wantsToShoot(EnemyAIState& ai,
                            const sf::Vector2f& enemyPos,
                            const PlayerState&  player,
                            sf::Vector2f&       shotDirection) {
    bool shouldFire = false;

    // Burst logic (levels 5 & 6)
    if (ai.inBurst && ai.burstCount > 0 && ai.burstCooldown <= 0.f) {
        shouldFire = true;
        ai.burstCount--;
        ai.burstCooldown = 0.12f;
        if (ai.burstCount == 0) {
            ai.inBurst = false;
            // Rearm for next burst
            float baseInterval = 2.5f - static_cast<float>(static_cast<int>(ai.level) - 1) * 0.3f;
            ai.shootCooldown   = baseInterval + randF(0.f, 0.8f);
        }
    }
    // Normal single shot
    else if (!ai.inBurst && ai.shootCooldown <= 0.f) {
        shouldFire = true;

        if (ai.level >= AILevel::Evasive) {
            // Start a burst of 3 shots
            ai.inBurst       = true;
            ai.burstCount    = 2;   // 1 fired now + 2 more
            ai.burstCooldown = 0.12f;
        } else {
            float baseInterval = 2.5f - static_cast<float>(static_cast<int>(ai.level) - 1) * 0.3f;
            ai.shootCooldown   = baseInterval + randF(0.f, 1.f);
        }
    }

    if (!shouldFire) return false;

    // ----- Compute shot direction -----
    sf::Vector2f target;

    if (ai.level >= AILevel::Adaptive && !ai.playerXHistory.empty()) {
        // Use adaptive prediction
        target = predictPlayerPosition(ai, 0.5f);
    } else if (ai.leadFactor > 0.f) {
        // Lead the shot
        target = player.position + player.velocity * ai.leadFactor;
    } else {
        target = player.position;
    }

    // Level 1 & 2: shoot straight down with a small random spread
    if (ai.level <= AILevel::Aware) {
        float spread = (ai.level == AILevel::Dumb) ? randF(-0.3f, 0.3f) : randF(-0.15f, 0.15f);
        shotDirection = normalize({ spread, 1.f });
    } else {
        shotDirection = normalize(target - enemyPos);
        // Add slight inaccuracy for levels 3-4
        if (ai.level <= AILevel::Tactical) {
            float inaccuracy = 0.12f - static_cast<float>(static_cast<int>(ai.level) - 3) * 0.04f;
            shotDirection.x += randF(-inaccuracy, inaccuracy);
            shotDirection = normalize(shotDirection);
        }
    }

    return true;
}

// ============================================================
//  Level 1 — Dumb
//  Pure zigzag, no player awareness whatsoever.
// ============================================================
sf::Vector2f EnemyAI::updateDumb(EnemyAIState& /*ai*/,
                                  const sf::Vector2f& /*enemyPos*/,
                                  const PlayerState&  /*player*/,
                                  float               /*dt*/) {
    // All movement handled by the existing zigzag in GameUpdate.cpp
    return { 0.f, 0.f };
}

// ============================================================
//  Level 2 — Aware
//  Gently drifts toward player's X position.
// ============================================================
sf::Vector2f EnemyAI::updateAware(EnemyAIState& ai,
                                   const sf::Vector2f& enemyPos,
                                   const PlayerState&  player,
                                   float               dt) {
    float dx     = player.position.x - enemyPos.x;
    float drift  = dx * ai.trackStrength * dt * 60.f;  // scale to ~60 fps
    drift        = std::clamp(drift, -30.f * dt, 30.f * dt);
    return { drift, 0.f };
}

// ============================================================
//  Level 3 — Aggressive
//  Faster tracking + closes vertical gap slightly.
// ============================================================
sf::Vector2f EnemyAI::updateAggressive(EnemyAIState& ai,
                                        const sf::Vector2f& enemyPos,
                                        const PlayerState&  player,
                                        float               dt) {
    float dx    = player.position.x - enemyPos.x;
    float drift = dx * ai.trackStrength * dt * 60.f;
    drift       = std::clamp(drift, -60.f * dt, 60.f * dt);
    return { drift, 0.f };
}

// ============================================================
//  Level 4 — Tactical
//  Leads shot + tracks predicted X position.
// ============================================================
sf::Vector2f EnemyAI::updateTactical(EnemyAIState& ai,
                                      const sf::Vector2f& enemyPos,
                                      const PlayerState&  player,
                                      float               dt) {
    // Predict where the player will be in leadFactor seconds
    sf::Vector2f predicted = player.position + player.velocity * ai.leadFactor;
    float dx    = predicted.x - enemyPos.x;
    float drift = dx * ai.trackStrength * dt * 60.f;
    drift       = std::clamp(drift, -80.f * dt, 80.f * dt);
    return { drift, 0.f };
}

// ============================================================
//  Level 5 — Evasive
//  Leads shots + dodges when an incoming bullet is close.
//  playerBulletPositions can be passed in from GameUpdate
//  (currently passed as empty; hook it up when ready).
// ============================================================
sf::Vector2f EnemyAI::updateEvasive(EnemyAIState& ai,
                                     const sf::Vector2f& enemyPos,
                                     const PlayerState&  player,
                                     float               dt,
                                     const std::vector<sf::Vector2f>& playerBullets) {
    // Base tracking toward predicted player position
    sf::Vector2f predicted = player.position + player.velocity * ai.leadFactor;
    float dx    = predicted.x - enemyPos.x;
    float drift = dx * ai.trackStrength * dt * 60.f;
    drift       = std::clamp(drift, -80.f * dt, 80.f * dt);

    // Check for nearby upward-traveling bullets (player bullets)
    constexpr float DANGER_RADIUS = 80.f;
    float closestDist = DANGER_RADIUS + 1.f;
    float closestBX   = enemyPos.x;

    for (const auto& bp : playerBullets) {
        float dist = std::sqrt((bp.x - enemyPos.x) * (bp.x - enemyPos.x) +
                               (bp.y - enemyPos.y) * (bp.y - enemyPos.y));
        if (dist < closestDist) {
            closestDist = dist;
            closestBX   = bp.x;
        }
    }

    // If a bullet is within danger radius, dodge away from it
    if (closestDist <= DANGER_RADIUS && ai.evasionCooldown <= 0.f) {
        float dodgeDir = (closestBX < enemyPos.x) ? 1.f : -1.f;
        ai.evasionVelX    = dodgeDir * 120.f;
        ai.evasionCooldown = 0.8f;
    }

    // Apply evasion velocity with decay
    if (std::abs(ai.evasionVelX) > 0.1f) {
        drift           += ai.evasionVelX * dt;
        ai.evasionVelX  *= std::pow(0.05f, dt);  // fast exponential decay
    }

    return { drift, 0.f };
}

// ============================================================
//  Level 6 — Adaptive
//  Records player movement history, predicts future position,
//  adjusts dodge direction based on observed patterns.
// ============================================================

void EnemyAI::recordPlayerSample(EnemyAIState& ai, const PlayerState& player) {
    ai.playerXHistory.push_back(player.position.x);
    ai.playerYHistory.push_back(player.position.y);
    if (static_cast<int>(ai.playerXHistory.size()) > MAX_HISTORY)
        ai.playerXHistory.pop_front();
    if (static_cast<int>(ai.playerYHistory.size()) > MAX_HISTORY)
        ai.playerYHistory.pop_front();
}

sf::Vector2f EnemyAI::predictPlayerPosition(const EnemyAIState& ai, float secondsAhead) {
    if (ai.playerXHistory.size() < 3) {
        return { ai.predictedPlayerX, ai.predictedPlayerY };
    }

    // Simple linear regression on the last N samples
    int   n  = static_cast<int>(ai.playerXHistory.size());
    float dt = ai.sampleInterval;

    // Mean X velocity over history
    float sumDX = 0.f, sumDY = 0.f;
    for (int i = 1; i < n; i++) {
        sumDX += ai.playerXHistory[i] - ai.playerXHistory[i - 1];
        sumDY += ai.playerYHistory[i] - ai.playerYHistory[i - 1];
    }
    float vx = (sumDX / (n - 1)) / dt;
    float vy = (sumDY / (n - 1)) / dt;

    float predX = ai.playerXHistory.back()  + vx * secondsAhead;
    float predY = ai.playerYHistory.back()  + vy * secondsAhead;

    // Clamp to game bounds
    predX = std::clamp(predX, 20.f, GAME_WIDTH  - 20.f);
    predY = std::clamp(predY, 20.f, GAME_HEIGHT - 20.f);

    return { predX, predY };
}

sf::Vector2f EnemyAI::updateAdaptive(EnemyAIState& ai,
                                      const sf::Vector2f& enemyPos,
                                      const PlayerState&  player,
                                      float               dt) {
    // --- Sample player position periodically ---
    ai.sampleTimer += dt;
    if (ai.sampleTimer >= ai.sampleInterval) {
        ai.sampleTimer = 0.f;
        recordPlayerSample(ai, player);

        // Update stored prediction
        sf::Vector2f pred = predictPlayerPosition(ai, ai.leadFactor);
        ai.predictedPlayerX = pred.x;
        ai.predictedPlayerY = pred.y;
    }

    // --- Track toward predicted position ---
    float targetX = (ai.playerXHistory.size() >= 3) ? ai.predictedPlayerX : player.position.x;
    float dx      = targetX - enemyPos.x;
    float drift   = dx * ai.trackStrength * dt * 60.f;
    drift         = std::clamp(drift, -100.f * dt, 100.f * dt);

    // --- Adaptive dodge ---
    // Periodically switch dodge direction based on player tendency
    ai.adaptiveDodgeTimer -= dt;
    if (ai.adaptiveDodgeTimer <= 0.f) {
        ai.adaptiveDodgeTimer = randF(1.0f, 2.5f);

        // If player is consistently moving right, dodge left and vice versa
        if (ai.playerXHistory.size() >= 5) {
            float recentDX = ai.playerXHistory.back()
                           - ai.playerXHistory[ai.playerXHistory.size() - 5];
            // Dodge against player's dominant direction
            ai.adaptiveDodgeDir = (recentDX > 10.f) ? -1.f : 1.f;
        }
    }

    if (ai.evasionCooldown <= 0.f) {
        float dodgeAmount = ai.adaptiveDodgeDir * 60.f * dt;
        // Flip direction if drifting out of bounds
        if (enemyPos.x + dodgeAmount < 30.f || enemyPos.x + dodgeAmount > GAME_WIDTH - 30.f) {
            ai.adaptiveDodgeDir *= -1.f;
            dodgeAmount          = ai.adaptiveDodgeDir * 60.f * dt;
        }
        drift += dodgeAmount;
        ai.evasionCooldown = randF(0.4f, 1.0f);
    }

    return { drift, 0.f };
}