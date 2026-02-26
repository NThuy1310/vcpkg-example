#pragma once
#include <array>
#include <vector>
#include "utils/Types.hpp"
#include "utils/Constants.hpp"
#include "core/Tetromino.hpp"

// ---------------------------------------------------------------------------
// Board – stores the locked cells and performs grid operations.
// Rows 0..(HIDDEN_ROWS-1) are invisible; rows HIDDEN_ROWS..(H-1) are visible.
// ---------------------------------------------------------------------------
class Board
{
public:
    Board();

    void reset(int w, int h);

    // Returns true if the given cells (world col/row) are all inside bounds
    // and occupy empty cells on the grid.
    [[nodiscard]] bool isValid(const CellArray &cells,
                               sf::Vector2i origin) const;

    // Locks a piece into the grid at the given origin.
    void lock(const CellArray &cells, sf::Vector2i origin, CellColor color);

    // Removes all full lines; returns the number of lines cleared.
    int clearLines();

    // Returns true if any cell in the top hidden rows is occupied.
    [[nodiscard]] bool isGameOver() const;

    // Cell query (returns CellColor::None if out of bounds)
    [[nodiscard]] CellColor cell(int col, int row) const;

    [[nodiscard]] int width() const { return m_w; }
    [[nodiscard]] int height() const { return m_h; } // total rows (visible + hidden)

private:
    int m_w = Constants::DEFAULT_BOARD_W;
    int m_h = Constants::DEFAULT_BOARD_H + Constants::HIDDEN_ROWS;

    // m_grid[row][col]
    std::vector<std::vector<CellColor>> m_grid;

    [[nodiscard]] bool inBounds(int col, int row) const;
};
