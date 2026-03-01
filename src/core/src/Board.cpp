#include "core/Board.hpp"
#include <algorithm>

Board::Board()
{
    reset(Constants::DEFAULT_BOARD_W, Constants::DEFAULT_BOARD_H);
}

void Board::reset(int w, int h)
{
    m_w = w;
    m_h = h + Constants::HIDDEN_ROWS;
    m_grid.assign(static_cast<std::size_t>(m_h),
                  std::vector<CellColor>(static_cast<std::size_t>(m_w), CellColor::None));
}

bool Board::inBounds(int col, int row) const
{
    return col >= 0 && col < m_w && row >= 0 && row < m_h;
}

bool Board::isValid(const CellArray &cells, sf::Vector2i origin) const
{
    for (const auto &c : cells)
    {
        int col = origin.x + c.x;
        int row = origin.y + c.y;
        if (!inBounds(col, row))
            return false;
        if (m_grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] != CellColor::None)
            return false;
    }
    return true;
}

void Board::lock(const CellArray &cells, sf::Vector2i origin, CellColor color)
{
    for (const auto &c : cells)
    {
        int col = origin.x + c.x;
        int row = origin.y + c.y;
        if (inBounds(col, row))
            m_grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] = color;
    }
}

int Board::clearLines()
{
    int cleared = 0;
    for (int r = m_h - 1; r >= 0;)
    {
        const auto &row = m_grid[static_cast<std::size_t>(r)];
        bool full = std::all_of(row.begin(), row.end(),
                                [](CellColor c)
                                { return c != CellColor::None; });
        if (full)
        {
            m_grid.erase(m_grid.begin() + r);
            m_grid.insert(m_grid.begin(),
                          std::vector<CellColor>(static_cast<std::size_t>(m_w), CellColor::None));
            ++cleared;
            // Don't decrement r – the row that dropped down is now at index r
        }
        else
        {
            --r;
        }
    }
    return cleared;
}

bool Board::isGameOver() const
{
    for (int r = 0; r < Constants::HIDDEN_ROWS; ++r)
    {
        for (int c = 0; c < m_w; ++c)
        {
            if (m_grid[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)] != CellColor::None)
                return true;
        }
    }
    return false;
}

CellColor Board::cell(int col, int row) const
{
    if (!inBounds(col, row))
        return CellColor::None;
    return m_grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
}
