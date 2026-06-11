#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

// ============================================================
//  AI Level Definitions
// ============================================================
//  Level 1 — Dumb:        Pure zigzag, random shooting, ignores player
//  Level 2 — Aware:       Drifts horizontally toward player slowly
//  Level 3 — Aggressive:  Faster tracking + shoots more often
//  Level 4 — Tactical:    Leads the shot (predicts player movement)
//  Level 5 — Evasive:     Dodges incoming player bullets + shoots bursts
//  Level 6 — Adaptive:    Learns player movement patterns over time,
//                         predicts position, evades bullets, coordinates
//                         spread fire based on observed behavior
// ============================================================

enum class AILevel {
    Dumb       = 1,
    Aware      = 2,
    Aggressive = 3,
    Tactical   = 4,
    Evasive    = 5,
    Adaptive   = 6
};

// --------------------------------------------------------
//  Snapshot of the player that the AI system reads each frame.
// --------------------------------------------------------
struct PlayerState {
    sf::Vector2f position;       // current world position
    sf::Vector2f velocity;       // estimated velocity (pos delta / dt)
    float        speed    = 0.f; // scalar speed this frame
    bool         isFiring = false;
};

// --------------------------------------------------------
//  Per-enemy AI state block.
//  Add one of these inside your Enemy struct (or store
//  separately in a parallel vector — your choice).
// --------------------------------------------------------
struct EnemyAIState {
    AILevel level         = AILevel::Dumb;

    // Tracking / drift
    float   trackStrength = 0.f;  // 0 = ignore player X,  1 = snap to player X
    float   leadFactor    = 0.f;  // how far ahead of the player to aim

    // Shooting
    float   shootCooldown    = 0.f;  // time until next shot is allowed
    float   burstCount       = 0;    // shots remaining in current burst
    float   burstCooldown    = 0.f;  // gap between burst shots
    bool    inBurst          = false;

    // Evasion
    float   evasionCooldown  = 0.f;  // time until evasion move re-arms
    float   evasionVelX      = 0.f;  // lateral evasion impulse

    // ---------- Level-6 adaptive data ----------
    // Rolling history of recent player X positions (sampled every 0.1 s)
    std::deque<float> playerXHistory;   // max 30 entries (~3 s of data)
    std::deque<float> playerYHistory;
    float   sampleTimer     = 0.f;
    float   sampleInterval  = 0.1f;

    // Derived prediction
    float   predictedPlayerX = 0.f;
    float   predictedPlayerY = 0.f;

    // Dodge direction memory (avoids oscillating)
    float   adaptiveDodgeDir = 1.f;
    float   adaptiveDodgeTimer = 0.f;
};

// ============================================================
//  EnemyAI  —  stateless helper class
//  All methods are static so you do not need an instance.
// ============================================================
class EnemyAI {
public:
    // ----------------------------------------------------------
    //  Call once when an enemy is first spawned.
    //  waveNumber starts at 1 and increases each wave.
    // ----------------------------------------------------------
    static void initialize(EnemyAIState& ai, int waveNumber, int enemyTextureIndex);

    // ----------------------------------------------------------
    //  Call every frame from GameUpdate.cpp.
    //  Returns a velocity delta that should be ADDED to the
    //  enemy's existing zigzag velocity this frame.
    //  Also manages shoot timers — call wantsToShoot() afterwards.
    // ----------------------------------------------------------
    static sf::Vector2f update(EnemyAIState& ai,
                               const sf::Vector2f& enemyPos,
                               const PlayerState&  player,
                               float dt);

    // ----------------------------------------------------------
    //  Returns true when the enemy should fire a bullet.
    //  Resets the internal timer automatically.
    //  shotDirection is set to the direction the bullet should travel.
    // ----------------------------------------------------------
    static bool wantsToShoot(EnemyAIState& ai,
                              const sf::Vector2f& enemyPos,
                              const PlayerState&  player,
                              sf::Vector2f&       shotDirection);

    // ----------------------------------------------------------
    //  Helper: map wave number (1-∞) to an AI level (1-6).
    //  Levels scale up every 2 waves, capping at 6.
    // ----------------------------------------------------------
    static AILevel levelForWave(int waveNumber);

private:
    // Internal per-level update helpers
    static sf::Vector2f updateDumb      (EnemyAIState&, const sf::Vector2f&, const PlayerState&, float dt);
    static sf::Vector2f updateAware     (EnemyAIState&, const sf::Vector2f&, const PlayerState&, float dt);
    static sf::Vector2f updateAggressive(EnemyAIState&, const sf::Vector2f&, const PlayerState&, float dt);
    static sf::Vector2f updateTactical  (EnemyAIState&, const sf::Vector2f&, const PlayerState&, float dt);
    static sf::Vector2f updateEvasive   (EnemyAIState&, const sf::Vector2f&, const PlayerState&, float dt,
                                         const std::vector<sf::Vector2f>& /*playerBulletPositions*/);
    static sf::Vector2f updateAdaptive  (EnemyAIState&, const sf::Vector2f&, const PlayerState&, float dt);

    static void recordPlayerSample(EnemyAIState& ai, const PlayerState& player);
    static sf::Vector2f predictPlayerPosition(const EnemyAIState& ai, float secondsAhead);
};