#pragma once
#include <array>
#include <random>
#include "core/Tetromino.hpp"
#include "utils/Types.hpp"

// ---------------------------------------------------------------------------
// TetrominoFactory – 7-bag randomiser
// Guarantees that each of the 7 tetromino types appears exactly once per bag.
// ---------------------------------------------------------------------------
class TetrominoFactory
{
public:
    TetrominoFactory();

    // Returns the next tetromino from the bag (refills when empty).
    [[nodiscard]] Tetromino next();

    // Peek at the n-th upcoming piece (0 = next, 1 = one after, …)
    [[nodiscard]] Tetromino peek(int n);

private:
    static constexpr int BAG_SIZE = 7;

    std::mt19937 m_rng;
    std::array<CellColor, BAG_SIZE> m_bag{};
    int m_bagIndex = BAG_SIZE; // start at end so first call refills

    // Refills and shuffles the bag
    void refill();

    // Ensure at least (count) pieces are available in the lookahead buffer
    std::array<CellColor, BAG_SIZE * 2> m_lookahead{};
    int m_lookaheadSize = 0;

    void ensureLookahead(int count);
};
