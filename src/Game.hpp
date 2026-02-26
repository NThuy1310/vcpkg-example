#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include "systems/AssetManager.hpp"
#include "systems/AudioManager.hpp"
#include "systems/SettingsManager.hpp"
#include "states/IGameState.hpp"

// ---------------------------------------------------------------------------
// High-score entry
// ---------------------------------------------------------------------------
struct HighScoreEntry
{
    std::string name;
    int score = 0;
};

// ---------------------------------------------------------------------------
// Game – top-level owner: window, state stack, shared managers, high scores.
// ---------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game() = default;

    // Initialise assets, audio, settings, then run the main loop.
    void run();

    // ---- State stack management ----------------------------------------
    // These are queued and applied after update+render to avoid re-entrancy.
    void pushState(std::unique_ptr<IGameState> state);
    void popState();
    void replaceState(std::unique_ptr<IGameState> state);

    // ---- Shared resource accessors -------------------------------------
    [[nodiscard]] sf::RenderWindow &window() { return m_window; }
    [[nodiscard]] AssetManager &assets() { return m_assets; }
    [[nodiscard]] AudioManager &audio() { return m_audio; }
    [[nodiscard]] SettingsManager &settings() { return m_settings; }

    // ---- High scores ---------------------------------------------------
    [[nodiscard]] const std::vector<HighScoreEntry> &highScores() const { return m_highScores; }
    void addHighScore(const std::string &name, int score);
    void loadHighScores();
    void saveHighScores();

private:
    sf::RenderWindow m_window;
    AssetManager m_assets;
    AudioManager m_audio;
    SettingsManager m_settings;

    std::vector<HighScoreEntry> m_highScores;

    // ---- State stack ---------------------------------------------------
    std::vector<std::unique_ptr<IGameState>> m_states;

    enum class PendingOp
    {
        Push,
        Pop,
        Replace
    };
    struct PendingChange
    {
        PendingOp op;
        std::unique_ptr<IGameState> state; // only for Push/Replace
    };
    std::vector<PendingChange> m_pendingChanges;

    void applyPendingChanges();

    // ---- Helpers -------------------------------------------------------
    void handleEvents();
    void update(sf::Time dt);
    void render();
    void rebuildWindow();

    static constexpr const char *SETTINGS_FILE = "settings.cfg";
    static constexpr const char *HIGHSCORES_FILE = "highscores.dat";
    static constexpr int MAX_HIGH_SCORES = 10;
};
