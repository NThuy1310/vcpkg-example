#pragma once
#include "states/IGameState.hpp"
#include <string>

// ---------------------------------------------------------------------------
// GameOverState – shows final score and lets the player enter a 3-char name.
// ---------------------------------------------------------------------------
class GameOverState : public IGameState
{
public:
    GameOverState(Game &game, int finalScore);

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderTarget &target) override;

private:
    int m_finalScore;
    std::string m_name = "AAA";
    int m_cursor = 0; // 0-2 (which character is being edited)
    bool m_submitted = false;

    void submit();
};
