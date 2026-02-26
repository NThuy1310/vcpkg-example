#include "Game.hpp"
#include "utils/Constants.hpp"
#include "states/MenuState.hpp"
#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>

// ---- Construction / init ---------------------------------------------------

Game::Game()
{
    // Load settings first (so we know the board dimensions for the window)
    auto result = m_settings.load(SETTINGS_FILE);
    if (!result)
        std::cerr << "Settings: " << result.error() << " (using defaults)\n";

    rebuildWindow();

    // Load font
    if (!m_assets.loadFont(FontId::Main, "assets/fonts/mono.ttf"))
        std::cerr << "Warning: main font not found - text will be blank\n";

    // Load audio (missing files are non-fatal)
    m_audio.loadAll();
    m_audio.setMusicVolume(m_settings.get().musicVolume);

    loadHighScores();
}

void Game::rebuildWindow()
{
    const Config &cfg = m_settings.get();
    unsigned int w = static_cast<unsigned int>(
        cfg.boardW * Constants::TILE_SIZE + Constants::SIDEBAR_W);
    unsigned int h = static_cast<unsigned int>(
        cfg.boardH * Constants::TILE_SIZE);

    if (m_window.isOpen())
        m_window.close();
    m_window.create(sf::VideoMode({w, h}), "Tetris",
                    sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(Constants::TARGET_FPS);
}

// ---- Main loop -------------------------------------------------------------

void Game::run()
{
    pushState(std::make_unique<MenuState>(*this));
    applyPendingChanges();

    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Time dt = clock.restart();
        // Clamp dt to avoid spiral-of-death on hitches
        if (dt > sf::milliseconds(100))
            dt = sf::milliseconds(100);

        handleEvents();
        update(dt);
        render();
        applyPendingChanges();
    }
    m_settings.save(SETTINGS_FILE);
    saveHighScores();
}

void Game::handleEvents()
{
    while (const auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
            return;
        }
        if (!m_states.empty())
            m_states.back()->handleEvent(*event);
    }
}

void Game::update(sf::Time dt)
{
    if (!m_states.empty())
        m_states.back()->update(dt);
}

void Game::render()
{
    m_window.clear(sf::Color(10, 10, 10));

    // Render transparent states bottom-up
    // Find first non-transparent state from the top
    int start = static_cast<int>(m_states.size()) - 1;
    while (start > 0 && m_states[static_cast<std::size_t>(start)]->transparent)
        --start;

    for (int i = start; i < static_cast<int>(m_states.size()); ++i)
        m_states[static_cast<std::size_t>(i)]->render(m_window);

    m_window.display();
}

// ---- State stack management ------------------------------------------------

void Game::pushState(std::unique_ptr<IGameState> state)
{
    m_pendingChanges.push_back({PendingOp::Push, std::move(state)});
}

void Game::popState()
{
    m_pendingChanges.push_back({PendingOp::Pop, nullptr});
}

void Game::replaceState(std::unique_ptr<IGameState> state)
{
    m_pendingChanges.push_back({PendingOp::Replace, std::move(state)});
}

void Game::applyPendingChanges()
{
    for (auto &change : m_pendingChanges)
    {
        switch (change.op)
        {
        case PendingOp::Push:
            if (!m_states.empty())
                m_states.back()->onExit();
            change.state->onEnter();
            m_states.push_back(std::move(change.state));
            break;
        case PendingOp::Pop:
            if (!m_states.empty())
            {
                m_states.back()->onExit();
                m_states.pop_back();
            }
            if (!m_states.empty())
                m_states.back()->onEnter();
            break;
        case PendingOp::Replace:
            if (!m_states.empty())
            {
                m_states.back()->onExit();
                m_states.pop_back();
            }
            change.state->onEnter();
            m_states.push_back(std::move(change.state));
            break;
        }
    }
    m_pendingChanges.clear();
}

// ---- High scores -----------------------------------------------------------

void Game::addHighScore(const std::string &name, int score)
{
    m_highScores.push_back({name, score});
    std::ranges::sort(m_highScores,
                      [](const HighScoreEntry &a, const HighScoreEntry &b)
                      { return a.score > b.score; });
    if (static_cast<int>(m_highScores.size()) > MAX_HIGH_SCORES)
        m_highScores.resize(static_cast<std::size_t>(MAX_HIGH_SCORES));
}

void Game::loadHighScores()
{
    std::ifstream file(HIGHSCORES_FILE);
    if (!file.is_open())
        return;
    std::string name;
    int sc;
    while (file >> name >> sc)
        m_highScores.push_back({name, sc});
    std::ranges::sort(m_highScores,
                      [](const HighScoreEntry &a, const HighScoreEntry &b)
                      { return a.score > b.score; });
}

void Game::saveHighScores()
{
    std::ofstream file(HIGHSCORES_FILE);
    if (!file.is_open())
        return;
    for (const auto &e : m_highScores)
        file << e.name << ' ' << e.score << '\n';
}
