#include "states/HighScoreState.hpp"
#include "Game.hpp"
#include "systems/Renderer.hpp"
#include <SFML/Window/Event.hpp>
#include <format>

HighScoreState::HighScoreState(Game &game) : IGameState(game) {}

void HighScoreState::handleEvent(const sf::Event &event)
{
    if (const auto *kp = event.getIf<sf::Event::KeyPressed>())
    {
        if (kp->code == sf::Keyboard::Key::Escape ||
            kp->code == sf::Keyboard::Key::Enter)
            m_game.popState();
    }
}

void HighScoreState::update(sf::Time /*dt*/) {}

void HighScoreState::render(sf::RenderTarget &target)
{
    target.clear(sf::Color(10, 10, 10));
    if (!m_game.assets().hasFont(FontId::Main))
        return;

    auto &font = m_game.assets().getFont(FontId::Main);
    float h = static_cast<float>(target.getSize().y);

    Renderer::drawCenteredText(target, "HIGH SCORES", font, 38u, sf::Color::Cyan, h * 0.05f);

    const auto &scores = m_game.highScores();
    if (scores.empty())
    {
        Renderer::drawCenteredText(target, "No scores yet!", font, 20u,
                                   sf::Color(160, 160, 160), h * 0.30f);
    }
    else
    {
        float rowH = h * 0.07f;
        float startY = h * 0.18f;
        for (int i = 0; i < static_cast<int>(scores.size()); ++i)
        {
            const auto &e = scores[static_cast<std::size_t>(i)];
            std::string line = std::format("{:2}. {:3}   {:>7}", i + 1, e.name, e.score);
            sf::Color c = (i == 0) ? sf::Color::Yellow : (i == 1) ? sf::Color(200, 200, 200)
                                                     : (i == 2)   ? sf::Color(180, 130, 80)
                                                                  : sf::Color(160, 160, 160);
            Renderer::drawCenteredText(target, line, font, 20u, c, startY + i * rowH);
        }
    }
    Renderer::drawCenteredText(target, "Press Esc or Enter to go back",
                               font, 14u, sf::Color(120, 120, 120), h * 0.90f);
}
