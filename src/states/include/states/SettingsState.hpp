#pragma once
#include "states/IGameState.hpp"
#include "systems/SettingsManager.hpp"
#include <vector>
#include <string>

// ---------------------------------------------------------------------------
// SettingsState – editable settings: board size, start level, music volume.
// ---------------------------------------------------------------------------
class SettingsState : public IGameState
{
public:
    explicit SettingsState(Game &game);

    void handleEvent(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render(sf::RenderTarget &target) override;
    void onExit() override;

private:
    Config m_cfg; // working copy; saved on exit
    int m_selected = 0;

    struct Setting
    {
        std::string label;
        int *iVal = nullptr;
        float *fVal = nullptr;
        int min, max;
        float step = 1.f;
    };
    std::vector<Setting> m_settings;

    void buildSettings();
    void changeValue(int dir);
};
