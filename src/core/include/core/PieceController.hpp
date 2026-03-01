#pragma once
#include <optional>
#include <array>
#include <SFML/System/Vector2.hpp>
#include "core/Tetromino.hpp"
#include "core/Board.hpp"
#include "core/TetrominoFactory.hpp"
#include "core/ScoreManager.hpp"
#include "utils/Constants.hpp"

// ---------------------------------------------------------------------------
// PieceController – manages the active piece, ghost, hold slot, and next queue.
// ---------------------------------------------------------------------------
class PieceController
{
public:
    explicit PieceController(Board &board, ScoreManager &score);

    void reset();

    // Spawn the next piece from the queue; returns false if spawn position
    // is blocked (game over condition).
    bool spawnNext();

    // Movement (returns true on success)
    bool tryMove(int dcol);
    bool tryRotate(int dir); // +1 = CW, -1 = CCW

    // Hard drop: moves piece to ghost row, awards points, locks it.
    // Returns the number of lines cleared after lock.
    int hardDrop();

    // Soft-drop one step (returns true if moved, false if touching floor).
    bool softDropStep(bool awardPoint = true);

    // Lock the active piece into the board; spawns next piece.
    // Returns lines cleared. Returns -1 on game over.
    int lockPiece();

    // Hold piece (one swap per lock cycle)
    bool holdPiece();

    // Accessors
    [[nodiscard]] const Tetromino &activePiece() const { return m_active; }
    [[nodiscard]] sf::Vector2i activeOrigin() const { return m_origin; }
    [[nodiscard]] int activeRotation() const { return m_rotation; }
    [[nodiscard]] int ghostRow() const { return m_ghostRow; }
    [[nodiscard]] const std::optional<Tetromino> &holdSlot() const { return m_hold; }
    [[nodiscard]] bool holdUsed() const { return m_holdUsed; }

    // Next queue
    [[nodiscard]] Tetromino nextPiece(int n) const;

    // Lock-delay state (managed externally in PlayState)
    int lockResets = 0;

private:
    Board &m_board;
    ScoreManager &m_score;
    TetrominoFactory m_factory;

    Tetromino m_active;
    sf::Vector2i m_origin{};
    int m_rotation = 0;
    int m_ghostRow = 0;

    std::optional<Tetromino> m_hold;
    bool m_holdUsed = false;

    void recalcGhost();
};
