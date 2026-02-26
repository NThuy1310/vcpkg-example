#include "systems/AudioManager.hpp"
#include <iostream>

AudioManager::AudioManager() = default;

const char *AudioManager::soundPath(SoundId id)
{
    switch (id)
    {
    case SoundId::Move:
        return "assets/sounds/move.wav";
    case SoundId::Rotate:
        return "assets/sounds/rotate.wav";
    case SoundId::Drop:
        return "assets/sounds/drop.wav";
    case SoundId::Clear:
        return "assets/sounds/clear.wav";
    case SoundId::LevelUp:
        return "assets/sounds/levelup.wav";
    case SoundId::GameOver:
        return "assets/sounds/gameover.wav";
    default:
        return "";
    }
}

bool AudioManager::loadAll()
{
    bool allOk = true;
    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        auto id = static_cast<SoundId>(i);
        sf::SoundBuffer buf;
        if (buf.loadFromFile(soundPath(id)))
        {
            m_buffers[static_cast<std::size_t>(i)] = std::move(buf);
            m_sounds[static_cast<std::size_t>(i)].emplace(
                *m_buffers[static_cast<std::size_t>(i)]);
            m_sounds[static_cast<std::size_t>(i)]->setVolume(80.f);
        }
        else
        {
            std::cerr << "AudioManager: failed to load '" << soundPath(id) << "'\n";
            allOk = false;
        }
    }

    m_musicLoaded = m_music.openFromFile("assets/music/theme.ogg");
    if (!m_musicLoaded)
        std::cerr << "AudioManager: failed to load music 'assets/music/theme.ogg'\n";

    if (m_musicLoaded)
    {
        m_music.setLooping(true);
        m_music.setVolume(m_musicVolume);
    }
    return allOk;
}

void AudioManager::playSound(SoundId id)
{
    auto &s = m_sounds[static_cast<std::size_t>(id)];
    if (s.has_value())
        s->play();
}

void AudioManager::playMusic()
{
    if (m_musicLoaded && m_music.getStatus() != sf::SoundSource::Status::Playing)
        m_music.play();
}

void AudioManager::pauseMusic()
{
    if (m_musicLoaded)
        m_music.pause();
}

void AudioManager::stopMusic()
{
    if (m_musicLoaded)
        m_music.stop();
}

void AudioManager::setMusicVolume(float volume)
{
    m_musicVolume = volume;
    m_music.setVolume(volume);
}
