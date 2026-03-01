#pragma once
#include <array>
#include <optional>
#include <string>
#include <SFML/Graphics/Font.hpp>
#include "utils/Types.hpp"

// ---------------------------------------------------------------------------
// FontManager – loads and caches sf::Font objects by FontId.
// ---------------------------------------------------------------------------
class FontManager
{
public:
    // Load a font and store it under the given id.
    bool loadFont(FontId id, const std::string &path);

    // Returns a reference to the loaded font. Behaviour is undefined if not loaded.
    [[nodiscard]] sf::Font &getFont(FontId id);
    [[nodiscard]] const sf::Font &getFont(FontId id) const;

    [[nodiscard]] bool hasFont(FontId id) const;

private:
    static constexpr int NUM_FONTS = static_cast<int>(FontId::Count);
    std::array<std::optional<sf::Font>, NUM_FONTS> m_fonts;
};
