#pragma once
#include "states/IGameState.hpp"

// ---------------------------------------------------------------------------
// HighScoreState – displays the top-10 high score list.
// ---------------------------------------------------------------------------
class HighScoreState : public IGameState
{
public:
    explicit HighScoreState(Game &game);

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderTarget &target) override;
};
