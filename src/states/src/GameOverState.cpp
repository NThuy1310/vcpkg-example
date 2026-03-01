#include "states/GameOverState.hpp"
#include "Game.hpp"
#include <SFML/Graphics/Text.hpp>
#include "states/HighScoreState.hpp"
#include "systems/Renderer.hpp"
#include <SFML/Window/Event.hpp>
#include <algorithm>

GameOverState::GameOverState(Game &game, int finalScore)
    : IGameState(game), m_finalScore(finalScore)
{
}

void GameOverState::handleEvent(const sf::Event &event)
{
    if (m_submitted)
        return;

    if (const auto *kp = event.getIf<sf::Event::KeyPressed>())
    {
        switch (kp->code)
        {
        case sf::Keyboard::Key::Up:
        {
            char c = m_name[static_cast<std::size_t>(m_cursor)];
            c = (c == 'Z') ? 'A' : static_cast<char>(c + 1);
            m_name[static_cast<std::size_t>(m_cursor)] = c;
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            char c = m_name[static_cast<std::size_t>(m_cursor)];
            c = (c == 'A') ? 'Z' : static_cast<char>(c - 1);
            m_name[static_cast<std::size_t>(m_cursor)] = c;
            break;
        }
        case sf::Keyboard::Key::Right:
            m_cursor = std::min(m_cursor + 1, 2);
            break;
        case sf::Keyboard::Key::Left:
            m_cursor = std::max(m_cursor - 1, 0);
            break;
        case sf::Keyboard::Key::Enter:
            submit();
            break;
        default:
            break;
        }
    }
}

void GameOverState::submit()
{
    m_submitted = true;
    m_game.addHighScore(m_name, m_finalScore);
    m_game.saveHighScores();
    m_game.replaceState(std::make_unique<HighScoreState>(m_game));
}

void GameOverState::update(sf::Time /*dt*/) {}

void GameOverState::render(sf::RenderTarget &target)
{
    target.clear(sf::Color(10, 10, 10));
    if (!m_game.fonts().hasFont(FontId::Main))
        return;

    auto &font = m_game.fonts().getFont(FontId::Main);
    float h = static_cast<float>(target.getSize().y);
    float w = static_cast<float>(target.getSize().x);

    Renderer::drawCenteredText(target, "GAME OVER", font, 44u, sf::Color::Red, h * 0.15f);
    Renderer::drawCenteredText(target, "Score: " + std::to_string(m_finalScore),
                               font, 26u, sf::Color::White, h * 0.30f);
    Renderer::drawCenteredText(target, "Enter your name:", font, 20u,
                               sf::Color(200, 200, 200), h * 0.45f);

    // Name entry: 3 characters with cursor highlight
    float charSpacing = 40.f;
    float startX = w / 2.f - charSpacing;
    for (int i = 0; i < 3; ++i)
    {
        sf::Color c = (i == m_cursor) ? sf::Color::Yellow : sf::Color::White;
        std::string ch(1, m_name[static_cast<std::size_t>(i)]);

        sf::Text t(font, ch, 36u);
        t.setFillColor(c);
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition({startX + i * charSpacing - b.size.x / 2.f - b.position.x, h * 0.55f});
        target.draw(t);
    }
    Renderer::drawCenteredText(target, "Arrow keys to change  Enter to confirm",
                               font, 14u, sf::Color(140, 140, 140), h * 0.70f);
}
