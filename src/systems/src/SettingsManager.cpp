#include "systems/SettingsManager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

std::expected<Config, std::string> SettingsManager::load(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return std::unexpected("Cannot open settings file: " + path);

    Config cfg;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line.front() == '#')
            continue;
        auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;
        auto key = line.substr(0, eq);
        auto val = line.substr(eq + 1);
        // Trim whitespace
        auto trim = [](std::string &s)
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                            [](unsigned char c)
                                            { return !std::isspace(c); }));
            s.erase(std::find_if(s.rbegin(), s.rend(),
                                 [](unsigned char c)
                                 { return !std::isspace(c); })
                        .base(),
                    s.end());
        };
        trim(key);
        trim(val);
        try
        {
            if (key == "boardW")
                cfg.boardW = std::stoi(val);
            else if (key == "boardH")
                cfg.boardH = std::stoi(val);
            else if (key == "startLevel")
                cfg.startLevel = std::stoi(val);
            else if (key == "musicVolume")
                cfg.musicVolume = std::stof(val);
        }
        catch (...)
        { /* ignore malformed values */
        }
    }
    m_config = cfg;
    return cfg;
}

std::expected<void, std::string> SettingsManager::save(const std::string &path) const
{
    std::ofstream file(path);
    if (!file.is_open())
        return std::unexpected("Cannot write settings file: " + path);

    file << "boardW=" << m_config.boardW << '\n'
         << "boardH=" << m_config.boardH << '\n'
         << "startLevel=" << m_config.startLevel << '\n'
         << "musicVolume=" << m_config.musicVolume << '\n';

    return {};
}
