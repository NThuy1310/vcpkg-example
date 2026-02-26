#pragma once
#include <cstdint>

// ---- Cell colour (also used as tetromino type index) -------------------
enum class CellColor : uint8_t
{
    None = 0,
    I,     // cyan
    J,     // blue
    L,     // orange
    O,     // yellow
    S,     // green
    T,     // purple
    Z,     // red
    Ghost, // semi-transparent grey used for ghost piece
    Count
};

// ---- Player actions (input abstraction layer) ---------------------------
enum class Action
{
    MoveLeft,
    MoveRight,
    SoftDrop,
    HardDrop,
    RotateCW,
    RotateCCW,
    Hold,
    Pause,
    Select,
    Back,
    Quit
};

// ---- Sound identifiers --------------------------------------------------
enum class SoundId
{
    Move,
    Rotate,
    Drop,
    Clear,
    LevelUp,
    GameOver,
    Count
};

// ---- Font identifiers ---------------------------------------------------
enum class FontId
{
    Main,
    Count
};
