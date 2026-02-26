#include "states/PauseState.hpp"
#include "Game.hpp"
#include "systems/Renderer.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

PauseState::PauseState(Game &game) : IGameState(game)
{
    transparent = true; // render PlayState below
}

void PauseState::onEnter()
{
    m_game.audio().pauseMusic();
}

void PauseState::onExit()
{
    m_game.audio().playMusic();
}

void PauseState::handleEvent(const sf::Event &event)
{
    if (const auto *kp = event.getIf<sf::Event::KeyPressed>())
    {
        switch (kp->code)
        {
        case sf::Keyboard::Key::Up:
            m_selected = (m_selected - 1 + 2) % 2;
            break;
        case sf::Keyboard::Key::Down:
            m_selected = (m_selected + 1) % 2;
            break;
        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Escape:
            if (m_selected == 0)
            {
                m_game.popState(); // resume
            }
            else
            {
                m_game.popState(); // pop pause
                m_game.popState(); // pop play → back to menu
            }
            break;
        default:
            break;
        }
    }
}

void PauseState::update(sf::Time /*dt*/) {}

void PauseState::render(sf::RenderTarget &target)
{
    // Semi-transparent overlay
    sf::RectangleShape overlay(sf::Vector2f(target.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 140));
    target.draw(overlay);

    if (!m_game.assets().hasFont(FontId::Main))
        return;
    auto &font = m_game.assets().getFont(FontId::Main);
    float h = static_cast<float>(target.getSize().y);

    Renderer::drawCenteredText(target, "PAUSED", font, 40u, sf::Color::White, h / 2.f - 80.f);

    const std::array<std::string, 2> items = {"Resume", "Quit to Menu"};
    for (int i = 0; i < 2; ++i)
    {
        sf::Color c = (i == m_selected) ? sf::Color::Yellow : sf::Color(180, 180, 180);
        Renderer::drawCenteredText(target, items[static_cast<std::size_t>(i)],
                                   font, 22u, c, h / 2.f - 10.f + i * 44.f);
    }
}
