#pragma once
#include <string>
#include <expected>

// ---------------------------------------------------------------------------
// SettingsManager – reads/writes settings.cfg (plain key=value format).
// ---------------------------------------------------------------------------
struct Config
{
    int boardW = 10;
    int boardH = 20;
    int startLevel = 1;
    float musicVolume = 70.f;
};

class SettingsManager
{
public:
    SettingsManager() = default;

    // Load from file; returns the parsed config or an error message.
    std::expected<Config, std::string> load(const std::string &path);

    // Save current config to file; returns error message on failure.
    std::expected<void, std::string> save(const std::string &path) const;

    [[nodiscard]] Config &get() { return m_config; }
    [[nodiscard]] const Config &get() const { return m_config; }

private:
    Config m_config;
};
