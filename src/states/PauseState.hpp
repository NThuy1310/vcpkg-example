#pragma once
#include "states/IGameState.hpp"

// ---------------------------------------------------------------------------
// PauseState – transparent overlay that pauses gameplay.
// ---------------------------------------------------------------------------
class PauseState : public IGameState
{
public:
    explicit PauseState(Game &game);

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderTarget &target) override;
    void onEnter() override;
    void onExit() override;

private:
    int m_selected = 0;
};
