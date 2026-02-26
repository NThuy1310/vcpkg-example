#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include "utils/Types.hpp"
#include "utils/Constants.hpp"

// ---------------------------------------------------------------------------
// InputHandler – translates SFML events to game Actions, implements DAS/ARR.
// ---------------------------------------------------------------------------
class InputHandler
{
public:
    InputHandler();

    // Feed raw SFML events (call once per event per frame).
    void processEvent(const sf::Event &event);

    // Update DAS/ARR timers; returns pending actions since last call.
    std::vector<Action> update(sf::Time dt);

    void reset();

private:
    // DAS/ARR state for left/right
    struct DirectionState
    {
        bool held = false;
        float dasAccum = 0.f; // seconds since key was pressed
        float arrAccum = 0.f; // seconds since last repeat fire
        bool dasTriggered = false;
    };

    DirectionState m_left;
    DirectionState m_right;

    // Actions queued from processEvent (instant-fire)
    std::vector<Action> m_queued;

    static constexpr float DAS = Constants::DAS_SECONDS;
    static constexpr float ARR = Constants::ARR_SECONDS;
};
