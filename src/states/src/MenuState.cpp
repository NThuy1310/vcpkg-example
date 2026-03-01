#include "states/MenuState.hpp"
#include "Game.hpp"
#include "states/PlayState.hpp"
#include "states/HighScoreState.hpp"
#include "states/SettingsState.hpp"
#include "systems/Renderer.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

MenuState::MenuState(Game &game) : IGameState(game) {}

void MenuState::onEnter()
{
    m_game.audio().playMusic();
}

void MenuState::onExit()
{
    // Music continues into PlayState (which manages its own music)
    // No need to stop here — PlayState.onEnter() will handle it
}

void MenuState::handleEvent(const sf::Event &event)
{
    if (const auto *kp = event.getIf<sf::Event::KeyPressed>())
    {
        switch (kp->code)
        {
        case sf::Keyboard::Key::Up:
            m_selected = (m_selected - 1 + static_cast<int>(m_items.size())) % static_cast<int>(m_items.size());
            break;
        case sf::Keyboard::Key::Down:
            m_selected = (m_selected + 1) % static_cast<int>(m_items.size());
            break;
        case sf::Keyboard::Key::Enter:
            activate();
            break;
        case sf::Keyboard::Key::Escape:
            m_game.window().close();
            break;
        default:
            break;
        }
    }
}

void MenuState::update(sf::Time /*dt*/) {}

void MenuState::activate()
{
    switch (m_selected)
    {
    case 0:
        m_game.pushState(std::make_unique<PlayState>(m_game));
        break;
    case 1:
        m_game.pushState(std::make_unique<HighScoreState>(m_game));
        break;
    case 2:
        m_game.pushState(std::make_unique<SettingsState>(m_game));
        break;
    case 3:
        m_game.window().close();
        break;
    default:
        break;
    }
}

void MenuState::render(sf::RenderTarget &target)
{
    auto &font = m_game.fonts().getFont(FontId::Main);
    auto size = target.getSize();
    float cx = size.x / 2.f;
    float cy = size.y / 2.f;

    // Title
    Renderer::drawCenteredText(target, "TETRIS", font, 48u, sf::Color::Cyan, cy - 140.f);

    // Menu items
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        bool sel = (i == m_selected);
        sf::Color c = sel ? sf::Color::Yellow : sf::Color(180, 180, 180);
        float y = cy - 40.f + i * 50.f;
        if (sel)
        {
            // Highlight bar
            sf::RectangleShape bar({size.x * 0.5f, 38.f});
            bar.setFillColor(sf::Color(60, 60, 60));
            bar.setPosition({cx - size.x * 0.25f, y - 2.f});
            target.draw(bar);
        }
        Renderer::drawCenteredText(target, m_items[static_cast<std::size_t>(i)],
                                   font, 24u, c, y);
    }
}
