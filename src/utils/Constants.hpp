#pragma once
#include <array>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include "Types.hpp"

namespace Constants
{

    // ---- Board defaults (can be overridden by Settings) --------------------
    constexpr int DEFAULT_BOARD_W = 10;
    constexpr int DEFAULT_BOARD_H = 20;
    constexpr int MAX_BOARD_W = 12;
    constexpr int MAX_BOARD_H = 24;
    constexpr int HIDDEN_ROWS = 2; // extra rows above visible area for spawning

    // ---- Rendering ---------------------------------------------------------
    constexpr float TILE_SIZE = 32.f;
    constexpr float SIDEBAR_W = 200.f;
    constexpr float BORDER = 2.f;

    // ---- Timing ------------------------------------------------------------
    constexpr float DAS_SECONDS = 0.167f;  // initial delay before key-repeat
    constexpr float ARR_SECONDS = 0.033f;  // interval between repeated moves
    constexpr float LOCK_DELAY_S = 0.500f; // seconds before piece locks on floor
    constexpr int MAX_LOCK_RESETS = 15;

    // ---- Target frame rate -------------------------------------------------
    constexpr int TARGET_FPS = 60;

    // ---- Next-piece queue size ---------------------------------------------
    constexpr int NEXT_QUEUE_SIZE = 3;

    // ---- High-score entries ------------------------------------------------
    constexpr int MAX_HIGH_SCORES = 10;

    // ---- Scoring -----------------------------------------------------------
    // Guideline base points indexed by lines cleared (0..4)
    constexpr std::array<int, 5> LINE_SCORE_BASE = {0, 100, 300, 500, 800};
    constexpr int HARD_DROP_PTS = 2; // per cell
    constexpr int SOFT_DROP_PTS = 1; // per cell

    // ---- Gravity table (seconds per one-cell drop, index = level-1) --------
    // Guideline standard formula approximated to 15 levels
    constexpr std::array<float, 15> GRAVITY_SECONDS = {
        1.000f, 0.793f, 0.618f, 0.473f, 0.355f,
        0.262f, 0.190f, 0.135f, 0.094f, 0.064f,
        0.042f, 0.028f, 0.017f, 0.011f, 0.007f};

    inline sf::Time gravityInterval(int level)
    {
        const int idx = std::min(level - 1, static_cast<int>(GRAVITY_SECONDS.size()) - 1);
        return sf::seconds(GRAVITY_SECONDS[static_cast<std::size_t>(idx)]);
    }

    // ---- Colours per CellColor ---------------------------------------------
    constexpr std::array<sf::Color, static_cast<std::size_t>(CellColor::Count)> CELL_COLORS = {{
        sf::Color(30, 30, 30),        // None  – dark background
        sf::Color(0, 240, 240),       // I     – cyan
        sf::Color(0, 0, 240),         // J     – blue
        sf::Color(240, 160, 0),       // L     – orange
        sf::Color(240, 240, 0),       // O     – yellow
        sf::Color(0, 240, 0),         // S     – green
        sf::Color(160, 0, 240),       // T     – purple
        sf::Color(240, 0, 0),         // Z     – red
        sf::Color(180, 180, 180, 80), // Ghost – translucent grey
    }};

} // namespace Constants
