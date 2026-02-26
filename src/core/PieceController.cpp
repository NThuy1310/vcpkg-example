#include "core/PieceController.hpp"

PieceController::PieceController(Board &board, ScoreManager &score)
    : m_board(board), m_score(score)
{
    reset();
}

void PieceController::reset()
{
    m_factory = TetrominoFactory{};
    m_hold = std::nullopt;
    m_holdUsed = false;
    m_rotation = 0;
    lockResets = 0;
}

bool PieceController::spawnNext()
{
    m_active = m_factory.next();
    m_rotation = 0;
    m_holdUsed = false;
    lockResets = 0;

    int spawnC = m_active.spawnCol(m_board.width());
    int spawnR = Tetromino::spawnRow(); // -2 (above visible)
    m_origin = {spawnC, spawnR};

    // Try to push down if needed
    while (!m_board.isValid(m_active.cells(m_rotation), m_origin) && m_origin.y < 0)
        ++m_origin.y;

    recalcGhost();
    return m_board.isValid(m_active.cells(m_rotation), m_origin);
}

bool PieceController::tryMove(int dcol)
{
    sf::Vector2i newOrigin = m_origin + sf::Vector2i{dcol, 0};
    if (m_board.isValid(m_active.cells(m_rotation), newOrigin))
    {
        m_origin = newOrigin;
        recalcGhost();
        return true;
    }
    return false;
}

bool PieceController::tryRotate(int dir)
{
    int newRot = (m_rotation + dir + 4) % 4;
    const auto &kicks = getKicks(m_active.type, m_rotation, newRot);

    for (const auto &kick : kicks)
    {
        sf::Vector2i testOrigin = m_origin + kick;
        if (m_board.isValid(m_active.cells(newRot), testOrigin))
        {
            m_origin = testOrigin;
            m_rotation = newRot;
            recalcGhost();
            return true;
        }
    }
    return false;
}

bool PieceController::softDropStep(bool awardPoint)
{
    sf::Vector2i newOrigin = m_origin + sf::Vector2i{0, 1};
    if (m_board.isValid(m_active.cells(m_rotation), newOrigin))
    {
        m_origin = newOrigin;
        if (awardPoint)
            m_score.addDropPoints(1, false);
        recalcGhost();
        return true;
    }
    return false; // touching floor
}

int PieceController::hardDrop()
{
    int cells = m_ghostRow - m_origin.y;
    m_score.addDropPoints(cells, true);
    m_origin.y = m_ghostRow;
    return lockPiece();
}

int PieceController::lockPiece()
{
    m_board.lock(m_active.cells(m_rotation), m_origin, m_active.type);
    int lines = m_board.clearLines();
    m_score.addLinesCleared(lines);

    m_holdUsed = false;
    lockResets = 0;

    if (!spawnNext())
        return -1; // game over
    return lines;
}

bool PieceController::holdPiece()
{
    if (m_holdUsed)
        return false;

    Tetromino previous = m_active;
    if (m_hold.has_value())
    {
        m_active = m_hold.value();
        m_hold = previous;
        m_rotation = 0;
        m_holdUsed = true;
        lockResets = 0;

        int spawnC = m_active.spawnCol(m_board.width());
        m_origin = {spawnC, Tetromino::spawnRow()};
        while (!m_board.isValid(m_active.cells(m_rotation), m_origin) && m_origin.y < 0)
            ++m_origin.y;

        recalcGhost();
    }
    else
    {
        m_hold = previous;
        m_holdUsed = true;
        // Spawn the next piece
        m_active = m_factory.next();
        m_rotation = 0;
        lockResets = 0;

        int spawnC = m_active.spawnCol(m_board.width());
        m_origin = {spawnC, Tetromino::spawnRow()};
        while (!m_board.isValid(m_active.cells(m_rotation), m_origin) && m_origin.y < 0)
            ++m_origin.y;

        recalcGhost();
    }
    return true;
}

Tetromino PieceController::nextPiece(int n) const
{
    return const_cast<TetrominoFactory &>(m_factory).peek(n);
}

void PieceController::recalcGhost()
{
    m_ghostRow = m_origin.y;
    while (m_board.isValid(m_active.cells(m_rotation), {m_origin.x, m_ghostRow + 1}))
        ++m_ghostRow;
}
