#pragma once
#include "states/IGameState.hpp"
#include <vector>
#include <string>

// ---------------------------------------------------------------------------
// MenuState – main menu with four items: Start, High Scores, Settings, Quit.
// ---------------------------------------------------------------------------
class MenuState : public IGameState
{
public:
    explicit MenuState(Game &game);

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderTarget &target) override;

private:
    int m_selected = 0;
    std::vector<std::string> m_items = {"Start Game", "High Scores", "Settings", "Quit"};

    void activate();
};
