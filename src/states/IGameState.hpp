#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Game; // forward declaration – states access Game via reference

// ---------------------------------------------------------------------------
// IGameState – abstract interface for all game screen/states.
// ---------------------------------------------------------------------------
class IGameState
{
public:
    explicit IGameState(Game &game) : m_game(game) {}
    virtual ~IGameState() = default;

    // Called after the state is pushed (or resumed after a pop above it).
    virtual void onEnter() {}
    // Called just before the state is popped or replaced.
    virtual void onExit() {}

    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void render(sf::RenderTarget &target) = 0;

    // If true, the state below this one in the stack is also rendered
    // (used by PauseState so the gameplay is visible behind the overlay).
    bool transparent = false;

protected:
    Game &m_game;
};
