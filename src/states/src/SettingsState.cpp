#include "states/SettingsState.hpp"
#include "Game.hpp"
#include "systems/Renderer.hpp"
#include <SFML/Window/Event.hpp>
#include <format>

SettingsState::SettingsState(Game &game) : IGameState(game)
{
    m_cfg = m_game.settings().get();
    buildSettings();
}

void SettingsState::buildSettings()
{
    m_settings.clear();
    m_settings.push_back({"Board Width", &m_cfg.boardW, nullptr, 8, 12});
    m_settings.push_back({"Board Height", &m_cfg.boardH, nullptr, 16, 24});
    m_settings.push_back({"Start Level", &m_cfg.startLevel, nullptr, 1, 15});
    m_settings.push_back({"Music Volume", nullptr, &m_cfg.musicVolume, 0, 100, 5.f});
}

void SettingsState::onExit()
{
    const Config &old = m_game.settings().get();
    bool boardChanged = (old.boardW != m_cfg.boardW || old.boardH != m_cfg.boardH);

    m_game.settings().get() = m_cfg;
    m_game.settings().save("settings.cfg");
    m_game.audio().setMusicVolume(m_cfg.musicVolume);

    if (boardChanged)
        m_game.rebuildWindow();
}

void SettingsState::handleEvent(const sf::Event &event)
{
    if (const auto *kp = event.getIf<sf::Event::KeyPressed>())
    {
        switch (kp->code)
        {
        case sf::Keyboard::Key::Up:
            m_selected = (m_selected - 1 + static_cast<int>(m_settings.size())) % static_cast<int>(m_settings.size());
            break;
        case sf::Keyboard::Key::Down:
            m_selected = (m_selected + 1) % static_cast<int>(m_settings.size());
            break;
        case sf::Keyboard::Key::Left:
            changeValue(-1);
            break;
        case sf::Keyboard::Key::Right:
            changeValue(+1);
            break;
        case sf::Keyboard::Key::Escape:
        case sf::Keyboard::Key::Enter:
            m_game.popState();
            break;
        default:
            break;
        }
    }
}

void SettingsState::changeValue(int dir)
{
    auto &s = m_settings[static_cast<std::size_t>(m_selected)];
    if (s.iVal)
    {
        *s.iVal = std::clamp(*s.iVal + dir, s.min, s.max);
    }
    else if (s.fVal)
    {
        *s.fVal = std::clamp(*s.fVal + dir * s.step,
                             static_cast<float>(s.min),
                             static_cast<float>(s.max));
    }
}

void SettingsState::update(sf::Time /*dt*/) {}

void SettingsState::render(sf::RenderTarget &target)
{
    target.clear(sf::Color(10, 10, 10));
    if (!m_game.fonts().hasFont(FontId::Main))
        return;

    auto &font = m_game.fonts().getFont(FontId::Main);
    float h = static_cast<float>(target.getSize().y);

    Renderer::drawCenteredText(target, "SETTINGS", font, 38u, sf::Color::Cyan, h * 0.05f);

    float rowH = h * 0.10f;
    float startY = h * 0.22f;

    for (int i = 0; i < static_cast<int>(m_settings.size()); ++i)
    {
        bool sel = (i == m_selected);
        auto &s = m_settings[static_cast<std::size_t>(i)];
        sf::Color c = sel ? sf::Color::Yellow : sf::Color(180, 180, 180);

        std::string valStr;
        if (s.iVal)
            valStr = std::to_string(*s.iVal);
        else if (s.fVal)
            valStr = std::format("{:.0f}", *s.fVal);

        std::string line = std::format("{}  <  {}  >", s.label, valStr);
        Renderer::drawCenteredText(target, line, font, 22u, c, startY + i * rowH);
    }

    Renderer::drawCenteredText(target, "Arrow keys to change  Esc to save & back",
                               font, 14u, sf::Color(120, 120, 120), h * 0.90f);
}
