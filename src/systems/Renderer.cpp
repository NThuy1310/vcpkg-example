#include "systems/Renderer.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "utils/Constants.hpp"
#include <string>
#include <format>

// Helper: look up colour for a CellColor enum value
static sf::Color colorFor(CellColor c)
{
    auto idx = static_cast<std::size_t>(c);
    if (idx < Constants::CELL_COLORS.size())
        return Constants::CELL_COLORS[idx];
    return sf::Color::White;
}

void Renderer::drawTile(sf::RenderTarget &target,
                        int col, int row,
                        CellColor color,
                        sf::Vector2f origin,
                        float tileSize)
{
    sf::RectangleShape tile({tileSize - 1.f, tileSize - 1.f});
    tile.setFillColor(colorFor(color));
    tile.setPosition(origin + sf::Vector2f{col * tileSize, row * tileSize});
    target.draw(tile);
}

void Renderer::drawBoard(sf::RenderTarget &target,
                         const Board &board,
                         sf::Vector2f boardOrigin,
                         float tileSize)
{
    // Draw background
    sf::RectangleShape bg({board.width() * tileSize, (board.height() - Constants::HIDDEN_ROWS) * tileSize});
    bg.setFillColor(sf::Color(20, 20, 20));
    bg.setPosition(boardOrigin);
    target.draw(bg);

    // Draw locked cells (skip hidden rows)
    for (int r = Constants::HIDDEN_ROWS; r < board.height(); ++r)
    {
        for (int c = 0; c < board.width(); ++c)
        {
            CellColor cc = board.cell(c, r);
            if (cc != CellColor::None)
            {
                drawTile(target, c, r - Constants::HIDDEN_ROWS, cc, boardOrigin, tileSize);
            }
        }
    }
}

void Renderer::drawBoardBorder(sf::RenderTarget &target,
                               sf::Vector2f boardOrigin,
                               float tileSize,
                               int boardW, int boardH)
{
    sf::RectangleShape border({boardW * tileSize + 4.f, boardH * tileSize + 4.f});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(100, 100, 100));
    border.setOutlineThickness(2.f);
    border.setPosition(boardOrigin - sf::Vector2f{2.f, 2.f});
    target.draw(border);
}

void Renderer::drawActivePiece(sf::RenderTarget &target,
                               const PieceController &pc,
                               sf::Vector2f boardOrigin,
                               float tileSize)
{
    const auto &cells = pc.activePiece().cells(pc.activeRotation());
    sf::Vector2i origin = pc.activeOrigin();
    for (const auto &c : cells)
    {
        int row = origin.y + c.y - Constants::HIDDEN_ROWS;
        int col = origin.x + c.x;
        if (row >= 0)
            drawTile(target, col, row, pc.activePiece().type, boardOrigin, tileSize);
    }
}

void Renderer::drawGhostPiece(sf::RenderTarget &target,
                              const PieceController &pc,
                              sf::Vector2f boardOrigin,
                              float tileSize)
{
    const auto &cells = pc.activePiece().cells(pc.activeRotation());
    int ghostRow = pc.ghostRow();
    int originCol = pc.activeOrigin().x;

    for (const auto &c : cells)
    {
        int row = ghostRow + c.y - Constants::HIDDEN_ROWS;
        int col = originCol + c.x;
        if (row >= 0)
            drawTile(target, col, row, CellColor::Ghost, boardOrigin, tileSize);
    }
}

void Renderer::drawMiniPiece(sf::RenderTarget &target,
                             const Tetromino &piece,
                             sf::Vector2f topLeft,
                             float tileSize)
{
    const auto &cells = piece.cells(0);
    for (const auto &c : cells)
        drawTile(target, c.x, c.y, piece.type, topLeft, tileSize * 0.75f);
}

void Renderer::drawSidebar(sf::RenderTarget &target,
                           const PieceController &pc,
                           const ScoreManager &score,
                           const sf::Font &font,
                           sf::Vector2f sidebarOrigin,
                           float tileSize)
{
    float x = sidebarOrigin.x + 8.f;
    float y = sidebarOrigin.y + 8.f;
    unsigned int textSize = 16u;

    auto drawLabel = [&](const std::string &label, float &yRef)
    {
        sf::Text t(font, label, textSize);
        t.setFillColor(sf::Color(180, 180, 180));
        t.setPosition({x, yRef});
        target.draw(t);
        yRef += static_cast<float>(textSize) + 4.f;
    };
    auto drawValue = [&](const std::string &val, float &yRef)
    {
        sf::Text t(font, val, textSize + 4u);
        t.setFillColor(sf::Color::White);
        t.setPosition({x, yRef});
        target.draw(t);
        yRef += static_cast<float>(textSize) + 12.f;
    };

    // --- HOLD ---
    drawLabel("HOLD", y);
    if (pc.holdSlot().has_value())
    {
        drawMiniPiece(target, pc.holdSlot().value(), {x, y}, tileSize);
    }
    y += tileSize * 1.8f;

    // --- NEXT ---
    drawLabel("NEXT", y);
    for (int i = 0; i < Constants::NEXT_QUEUE_SIZE; ++i)
    {
        drawMiniPiece(target, pc.nextPiece(i), {x, y}, tileSize);
        y += tileSize * 1.6f;
    }
    y += 8.f;

    // --- SCORE ---
    drawLabel("SCORE", y);
    drawValue(std::to_string(score.score()), y);

    // --- LEVEL ---
    drawLabel("LEVEL", y);
    drawValue(std::to_string(score.level()), y);

    // --- LINES ---
    drawLabel("LINES", y);
    drawValue(std::to_string(score.totalLines()), y);
}

void Renderer::drawCenteredText(sf::RenderTarget &target,
                                const std::string &text,
                                const sf::Font &font,
                                unsigned int size,
                                sf::Color color,
                                float y)
{
    sf::Text t(font, text, size);
    t.setFillColor(color);
    sf::FloatRect bounds = t.getLocalBounds();
    float winW = static_cast<float>(target.getSize().x);
    t.setPosition({(winW - bounds.size.x) / 2.f - bounds.position.x, y});
    target.draw(t);
}
