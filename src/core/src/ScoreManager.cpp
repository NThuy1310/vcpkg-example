#include "core/ScoreManager.hpp"
#include <algorithm>

void ScoreManager::reset(int startLevel)
{
    m_startLevel = startLevel;
    m_level = startLevel;
    m_score = 0;
    m_totalLines = 0;
    m_linesForNextLevel = startLevel * 10; // first level-up at 10*startLevel lines
}

void ScoreManager::addLinesCleared(int n)
{
    if (n <= 0)
        return;
    int idx = std::min(n, static_cast<int>(Constants::LINE_SCORE_BASE.size()) - 1);
    m_score += Constants::LINE_SCORE_BASE[static_cast<std::size_t>(idx)] * m_level;
    m_totalLines += n;

    while (m_totalLines >= m_linesForNextLevel)
    {
        ++m_level;
        m_linesForNextLevel += 10;
    }
}

void ScoreManager::addDropPoints(int cells, bool hardDrop)
{
    m_score += cells * (hardDrop ? Constants::HARD_DROP_PTS : Constants::SOFT_DROP_PTS);
}
