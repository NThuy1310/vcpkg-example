#pragma once
#include <SFML/System/Time.hpp>
#include "utils/Constants.hpp"

// ---------------------------------------------------------------------------
// ScoreManager – tracks score, level, total lines and gravity speed.
// ---------------------------------------------------------------------------
class ScoreManager
{
public:
    ScoreManager() = default;

    void reset(int startLevel = 1);

    // Called when n lines are cleared simultaneously; updates score + level.
    void addLinesCleared(int n);

    // Called per-cell for hard/soft drops.
    void addDropPoints(int cells, bool hardDrop);

    [[nodiscard]] int score() const { return m_score; }
    [[nodiscard]] int level() const { return m_level; }
    [[nodiscard]] int totalLines() const { return m_totalLines; }

    // Returns the time per one-cell gravity drop for the current level.
    [[nodiscard]] sf::Time gravityInterval() const
    {
        return Constants::gravityInterval(m_level);
    }

private:
    int m_score = 0;
    int m_level = 1;
    int m_totalLines = 0;
    int m_startLevel = 1;
    // Lines needed to reach the next level (every 10 lines from start)
    int m_linesForNextLevel = 10;
};
