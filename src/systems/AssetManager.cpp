#include "systems/AssetManager.hpp"
#include <iostream>
#include <cassert>

bool AssetManager::loadFont(FontId id, const std::string &path)
{
    auto idx = static_cast<std::size_t>(id);
    sf::Font font;
    if (!font.openFromFile(path))
    {
        std::cerr << "AssetManager: failed to load font '" << path << "'\n";
        return false;
    }
    m_fonts[idx] = std::move(font);
    return true;
}

sf::Font &AssetManager::getFont(FontId id)
{
    auto idx = static_cast<std::size_t>(id);
    assert(m_fonts[idx].has_value() && "Font not loaded");
    return *m_fonts[idx];
}

const sf::Font &AssetManager::getFont(FontId id) const
{
    auto idx = static_cast<std::size_t>(id);
    assert(m_fonts[idx].has_value() && "Font not loaded");
    return *m_fonts[idx];
}

bool AssetManager::hasFont(FontId id) const
{
    return m_fonts[static_cast<std::size_t>(id)].has_value();
}
