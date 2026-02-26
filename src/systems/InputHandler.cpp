#include "systems/InputHandler.hpp"

InputHandler::InputHandler()
{
    reset();
}

void InputHandler::reset()
{
    m_left = {};
    m_right = {};
    m_queued.clear();
}

void InputHandler::processEvent(const sf::Event &event)
{
    if (const auto *kp = event.getIf<sf::Event::KeyPressed>())
    {
        switch (kp->code)
        {
        case sf::Keyboard::Key::Left:
            if (!m_left.held)
            {
                m_left = {true, 0.f, 0.f, false};
                m_queued.push_back(Action::MoveLeft);
            }
            break;
        case sf::Keyboard::Key::Right:
            if (!m_right.held)
            {
                m_right = {true, 0.f, 0.f, false};
                m_queued.push_back(Action::MoveRight);
            }
            break;
        case sf::Keyboard::Key::Down:
            m_queued.push_back(Action::SoftDrop);
            break;
        case sf::Keyboard::Key::Space:
            m_queued.push_back(Action::HardDrop);
            break;
        case sf::Keyboard::Key::X:
        case sf::Keyboard::Key::Up:
            m_queued.push_back(Action::RotateCW);
            break;
        case sf::Keyboard::Key::Z:
            m_queued.push_back(Action::RotateCCW);
            break;
        case sf::Keyboard::Key::C:
            m_queued.push_back(Action::Hold);
            break;
        case sf::Keyboard::Key::Escape:
            m_queued.push_back(Action::Pause);
            break;
        case sf::Keyboard::Key::Enter:
            m_queued.push_back(Action::Select);
            break;
        default:
            break;
        }
    }
    if (const auto *kr = event.getIf<sf::Event::KeyReleased>())
    {
        if (kr->code == sf::Keyboard::Key::Left)
            m_left = {};
        if (kr->code == sf::Keyboard::Key::Right)
            m_right = {};
    }
}

std::vector<Action> InputHandler::update(sf::Time dt)
{
    std::vector<Action> actions = std::move(m_queued);
    m_queued.clear();

    float secs = dt.asSeconds();

    auto processDAS = [&](DirectionState &state, Action action)
    {
        if (!state.held)
            return;
        state.dasAccum += secs;
        if (!state.dasTriggered)
        {
            if (state.dasAccum >= DAS)
            {
                state.dasTriggered = true;
                state.arrAccum = 0.f;
                actions.push_back(action);
            }
        }
        else
        {
            state.arrAccum += secs;
            while (state.arrAccum >= ARR)
            {
                state.arrAccum -= ARR;
                actions.push_back(action);
            }
        }
    };

    processDAS(m_left, Action::MoveLeft);
    processDAS(m_right, Action::MoveRight);

    return actions;
}
