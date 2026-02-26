#pragma once
#include "states/IGameState.hpp"
#include "core/Board.hpp"
#include "core/PieceController.hpp"
#include "core/ScoreManager.hpp"
#include "systems/InputHandler.hpp"
#include <SFML/System/Time.hpp>

// ---------------------------------------------------------------------------
// PlayState – core gameplay state.
// ---------------------------------------------------------------------------
class PlayState : public IGameState
{
public:
    explicit PlayState(Game &game);

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderTarget &target) override;

    // Accessors for GameOverState
    [[nodiscard]] int finalScore() const { return m_score.score(); }

private:
    Board m_board;
    ScoreManager m_score;
    PieceController m_pieceCtrl;
    InputHandler m_input;

    sf::Time m_gravityAccum;
    sf::Time m_lockAccum;
    bool m_touchingFloor = false;
    bool m_softDropActive = false;

    void processAction(Action a);
    void tryLock(sf::Time dt);
    sf::Vector2f boardOrigin() const;
    sf::Vector2f sidebarOrigin() const;
};
