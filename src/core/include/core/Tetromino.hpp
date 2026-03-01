#pragma once
#include <array>
#include <span>
#include <SFML/System/Vector2.hpp>
#include "utils/Types.hpp"
#include "utils/Constants.hpp"

// ---------------------------------------------------------------------------
// Tetromino – header-only value type
// Contains all SRS shape data and wall-kick tables as constexpr arrays.
// Cell offsets are stored as {col, row} relative to the piece's spawn origin.
// Positive col = right, positive row = down.
// ---------------------------------------------------------------------------

// Number of rotation states and cells per state
inline constexpr int NUM_ROTATIONS = 4;
inline constexpr int CELLS_PER_PIECE = 4;
inline constexpr int NUM_KICK_TESTS = 5;

// ---- Piece shapes (col, row offsets from top-left of bounding box) ---------
// Index order: [type][rotation][cell] = {col, row}
// Types ordered to match CellColor: I=0, J=1, L=2, O=3, S=4, T=5, Z=6
using CellArray = std::array<sf::Vector2i, CELLS_PER_PIECE>;
using RotArray = std::array<CellArray, NUM_ROTATIONS>;

inline constexpr std::array<RotArray, 7> PIECE_CELLS = {{
    // ---- I piece (4-wide bounding box) -----------------------------------
    {{
        {{sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{3, 1}}}, // R0
        {{sf::Vector2i{2, 0}, sf::Vector2i{2, 1}, sf::Vector2i{2, 2}, sf::Vector2i{2, 3}}}, // R1
        {{sf::Vector2i{0, 2}, sf::Vector2i{1, 2}, sf::Vector2i{2, 2}, sf::Vector2i{3, 2}}}, // R2
        {{sf::Vector2i{1, 0}, sf::Vector2i{1, 1}, sf::Vector2i{1, 2}, sf::Vector2i{1, 3}}}, // R3
    }},
    // ---- J piece ---------------------------------------------------------
    {{
        {{sf::Vector2i{0, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}}}, // R0
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{0, 2}}}, // R1
        {{sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{2, 2}}}, // R2
        {{sf::Vector2i{1, 0}, sf::Vector2i{1, 1}, sf::Vector2i{0, 2}, sf::Vector2i{1, 2}}}, // R3
    }},
    // ---- L piece ---------------------------------------------------------
    {{
        {{sf::Vector2i{2, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}}}, // R0
        {{sf::Vector2i{0, 0}, sf::Vector2i{0, 1}, sf::Vector2i{0, 2}, sf::Vector2i{1, 2}}}, // R1
        {{sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{0, 2}}}, // R2
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{1, 1}, sf::Vector2i{1, 2}}}, // R3
    }},
    // ---- O piece (2-wide; all rotations identical) -----------------------
    {{
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}}},
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}}},
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}}},
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}}},
    }},
    // ---- S piece ---------------------------------------------------------
    {{
        {{sf::Vector2i{1, 0}, sf::Vector2i{2, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}}}, // R0
        {{sf::Vector2i{0, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{1, 2}}}, // R1
        {{sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{0, 2}, sf::Vector2i{1, 2}}}, // R2
        {{sf::Vector2i{1, 0}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{2, 2}}}, // R3
    }},
    // ---- T piece ---------------------------------------------------------
    {{
        {{sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}}}, // R0
        {{sf::Vector2i{0, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{0, 2}}}, // R1
        {{sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{1, 2}}}, // R2
        {{sf::Vector2i{1, 0}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{1, 2}}}, // R3
    }},
    // ---- Z piece ---------------------------------------------------------
    {{
        {{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}}}, // R0
        {{sf::Vector2i{2, 0}, sf::Vector2i{1, 1}, sf::Vector2i{2, 1}, sf::Vector2i{1, 2}}}, // R1
        {{sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{1, 2}, sf::Vector2i{2, 2}}}, // R2
        {{sf::Vector2i{1, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}, sf::Vector2i{0, 2}}}, // R3
    }},
}};

// ---- Spawn column offset from board left (before centering) ---------------
// I uses 4-wide bounding box (spawn at col = boardW/2 - 2)
// O uses 2-wide (spawn at col = boardW/2 - 1)
// All others use 3-wide (spawn at col = boardW/2 - 1)
inline constexpr std::array<int, 7> PIECE_BB_WIDTH = {4, 3, 3, 2, 3, 3, 3};

// ---- SRS Wall-kick tables ------------------------------------------------
// Encoded as 8 transition groups (CW/CCW for each of 4 states),
// each with 5 test offsets in {dcol, drow} (screen coordinates, row down).
//
// Transition index:
//   0: 0→1 (CW)   1: 1→0 (CCW)
//   2: 1→2 (CW)   3: 2→1 (CCW)
//   4: 2→3 (CW)   5: 3→2 (CCW)
//   6: 3→0 (CW)   7: 0→3 (CCW)

using KickList = std::array<sf::Vector2i, NUM_KICK_TESTS>;

// JLSTZ kicks (screen coords: positive row = down)
inline constexpr std::array<KickList, 8> KICKS_JLSTZ = {{
    /* 0→1 */ {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
    /* 1→0 */ {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},
    /* 1→2 */ {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},
    /* 2→1 */ {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
    /* 2→3 */ {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},
    /* 3→2 */ {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
    /* 3→0 */ {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
    /* 0→3 */ {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},
}};

// I piece kicks (screen coords)
inline constexpr std::array<KickList, 8> KICKS_I = {{
    /* 0→1 */ {{{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}}},
    /* 1→0 */ {{{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}}},
    /* 1→2 */ {{{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}}},
    /* 2→1 */ {{{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}}},
    /* 2→3 */ {{{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}}},
    /* 3→2 */ {{{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}}},
    /* 3→0 */ {{{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}}},
    /* 0→3 */ {{{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}}},
}};

// O piece: no kicks needed (kicks are all {0,0})
inline constexpr std::array<KickList, 8> KICKS_O = {{
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
}};

// Returns kick index (0-7) for a given from→to transition
// CW rotation: to = (from+1)%4  → from*2
// CCW rotation: to = (from+3)%4 → from*2+1
inline constexpr int kickIndex(int fromRot, int toRot)
{
    // CW: from*2  (from + 1) % 4
    // CCW: from*2 + 1
    int cwTo = (fromRot + 1) % 4;
    if (toRot == cwTo)
        return fromRot * 2;
    return fromRot * 2 + 1;
}

// Helper to get the correct kick table span for a type + transition
inline std::span<const sf::Vector2i, NUM_KICK_TESTS>
getKicks(CellColor type, int fromRot, int toRot)
{
    int ki = kickIndex(fromRot, toRot);
    switch (type)
    {
    case CellColor::I:
        return KICKS_I[static_cast<std::size_t>(ki)];
    case CellColor::O:
        return KICKS_O[static_cast<std::size_t>(ki)];
    default:
        return KICKS_JLSTZ[static_cast<std::size_t>(ki)];
    }
}

// ---------------------------------------------------------------------------
// Tetromino – the runtime value type
// ---------------------------------------------------------------------------
struct Tetromino
{
    CellColor type = CellColor::I;

    // Returns the 4 cell offsets for the given rotation state
    [[nodiscard]] const CellArray &cells(int rotation) const
    {
        int ti = static_cast<int>(type) - 1; // CellColor::I=1 → index 0
        return PIECE_CELLS[static_cast<std::size_t>(ti)]
                          [static_cast<std::size_t>(rotation % NUM_ROTATIONS)];
    }

    // Spawn column offset (centres piece on the board given boardW)
    [[nodiscard]] int spawnCol(int boardW) const
    {
        int ti = static_cast<int>(type) - 1;
        int bbW = PIECE_BB_WIDTH[static_cast<std::size_t>(ti)];
        return (boardW - bbW) / 2;
    }

    // Spawn row (first visible row in the board grid, so pieces appear immediately)
    [[nodiscard]] static int spawnRow() { return Constants::HIDDEN_ROWS; }
};
