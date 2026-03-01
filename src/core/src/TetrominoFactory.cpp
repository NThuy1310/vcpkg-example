#include "core/TetrominoFactory.hpp"
#include <algorithm>
#include <ranges>

TetrominoFactory::TetrominoFactory()
    : m_rng(std::random_device{}())
{
    // Pre-populate the lookahead so peek() always works from the start
    ensureLookahead(BAG_SIZE);
}

void TetrominoFactory::refill()
{
    constexpr std::array<CellColor, BAG_SIZE> types = {
        CellColor::I, CellColor::J, CellColor::L, CellColor::O,
        CellColor::S, CellColor::T, CellColor::Z};
    m_bag = types;
    std::ranges::shuffle(m_bag, m_rng);
    m_bagIndex = 0;
}

void TetrominoFactory::ensureLookahead(int count)
{
    while (m_lookaheadSize < count)
    {
        if (m_bagIndex >= BAG_SIZE)
            refill();
        m_lookahead[static_cast<std::size_t>(m_lookaheadSize++)] =
            m_bag[static_cast<std::size_t>(m_bagIndex++)];
    }
}

Tetromino TetrominoFactory::next()
{
    ensureLookahead(1);
    CellColor type = m_lookahead[0];
    // Shift lookahead left
    for (int i = 0; i < m_lookaheadSize - 1; ++i)
        m_lookahead[static_cast<std::size_t>(i)] =
            m_lookahead[static_cast<std::size_t>(i + 1)];
    --m_lookaheadSize;
    return Tetromino{type};
}

Tetromino TetrominoFactory::peek(int n)
{
    ensureLookahead(n + 1);
    return Tetromino{m_lookahead[static_cast<std::size_t>(n)]};
}
