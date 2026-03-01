#pragma once
#include <array>
#include <optional>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include "utils/Types.hpp"

// ---------------------------------------------------------------------------
// AudioManager – loads and plays sound effects and background music.
// Sound files are loaded from the assets/sounds/ and assets/music/ directories
// relative to the executable.
// ---------------------------------------------------------------------------
class AudioManager
{
public:
    AudioManager();

    // Load all audio assets; returns false if any critical file is missing.
    bool loadAll();

    void playSound(SoundId id);
    void playMusic();
    void pauseMusic();
    void stopMusic();
    void setMusicVolume(float volume); // 0–100

    [[nodiscard]] float musicVolume() const { return m_musicVolume; }

private:
    static constexpr int NUM_SOUNDS = static_cast<int>(SoundId::Count);

    std::array<std::optional<sf::SoundBuffer>, NUM_SOUNDS> m_buffers;
    std::array<std::optional<sf::Sound>, NUM_SOUNDS> m_sounds;

    sf::Music m_music;
    float m_musicVolume = 70.f;
    bool m_musicLoaded = false;

    static const char *soundPath(SoundId id);
};
