#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include "core/Board.hpp"
#include "core/PieceController.hpp"
#include "core/ScoreManager.hpp"

// ---------------------------------------------------------------------------
// Renderer – stateless drawing helpers for all gameplay elements.
// All positions are in screen pixels.
// ---------------------------------------------------------------------------
class Renderer
{
public:
    // Draw the locked cells of the board.
    // boardOrigin = top-left pixel of the visible play area.
    static void drawBoard(sf::RenderTarget &target,
                          const Board &board,
                          sf::Vector2f boardOrigin,
                          float tileSize);

    // Draw the active piece.
    static void drawActivePiece(sf::RenderTarget &target,
                                const PieceController &pc,
                                sf::Vector2f boardOrigin,
                                float tileSize);

    // Draw the ghost piece (faded drop preview).
    static void drawGhostPiece(sf::RenderTarget &target,
                               const PieceController &pc,
                               sf::Vector2f boardOrigin,
                               float tileSize);

    // Draw the sidebar: hold, next queue, score, level, lines.
    static void drawSidebar(sf::RenderTarget &target,
                            const PieceController &pc,
                            const ScoreManager &score,
                            const sf::Font &font,
                            sf::Vector2f sidebarOrigin,
                            float tileSize);

    // Draw a board border rectangle.
    static void drawBoardBorder(sf::RenderTarget &target,
                                sf::Vector2f boardOrigin,
                                float tileSize,
                                int boardW, int boardH);

    // Draw a centred text overlay (e.g. "PAUSED").
    static void drawCenteredText(sf::RenderTarget &target,
                                 const std::string &text,
                                 const sf::Font &font,
                                 unsigned int size,
                                 sf::Color color,
                                 float y);

private:
    static void drawTile(sf::RenderTarget &target,
                         int col, int row,
                         CellColor color,
                         sf::Vector2f origin,
                         float tileSize);

    static void drawMiniPiece(sf::RenderTarget &target,
                              const Tetromino &piece,
                              sf::Vector2f topLeft,
                              float tileSize);
};
