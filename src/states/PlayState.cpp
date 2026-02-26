#include "states/PlayState.hpp"
#include "Game.hpp"
#include "states/PauseState.hpp"
#include "states/GameOverState.hpp"
#include "systems/Renderer.hpp"
#include <SFML/Window/Event.hpp>

PlayState::PlayState(Game &game)
    : IGameState(game), m_board(), m_score(), m_pieceCtrl(m_board, m_score), m_input()
{
}

void PlayState::onEnter()
{
    const Config &cfg = m_game.settings().get();
    m_board.reset(cfg.boardW, cfg.boardH);
    m_score.reset(cfg.startLevel);
    m_pieceCtrl.reset();
    m_pieceCtrl.spawnNext();
    m_input.reset();

    m_gravityAccum = sf::Time::Zero;
    m_lockAccum = sf::Time::Zero;
    m_touchingFloor = false;
    m_softDropActive = false;

    m_game.audio().playMusic();
}

void PlayState::onExit()
{
    m_game.audio().pauseMusic();
}

void PlayState::handleEvent(const sf::Event &event)
{
    m_input.processEvent(event);
}

sf::Vector2f PlayState::boardOrigin() const
{
    return {0.f, 0.f};
}

sf::Vector2f PlayState::sidebarOrigin() const
{
    const Config &cfg = m_game.settings().get();
    return {cfg.boardW * Constants::TILE_SIZE, 0.f};
}

void PlayState::processAction(Action a)
{
    switch (a)
    {
    case Action::MoveLeft:
    {
        bool moved = m_pieceCtrl.tryMove(-1);
        if (moved)
        {
            m_game.audio().playSound(SoundId::Move);
            if (m_touchingFloor && m_pieceCtrl.lockResets < Constants::MAX_LOCK_RESETS)
            {
                m_lockAccum = sf::Time::Zero;
                ++m_pieceCtrl.lockResets;
            }
        }
        break;
    }
    case Action::MoveRight:
    {
        bool moved = m_pieceCtrl.tryMove(+1);
        if (moved)
        {
            m_game.audio().playSound(SoundId::Move);
            if (m_touchingFloor && m_pieceCtrl.lockResets < Constants::MAX_LOCK_RESETS)
            {
                m_lockAccum = sf::Time::Zero;
                ++m_pieceCtrl.lockResets;
            }
        }
        break;
    }
    case Action::RotateCW:
    {
        bool rotated = m_pieceCtrl.tryRotate(+1);
        if (rotated)
        {
            m_game.audio().playSound(SoundId::Rotate);
            if (m_touchingFloor && m_pieceCtrl.lockResets < Constants::MAX_LOCK_RESETS)
            {
                m_lockAccum = sf::Time::Zero;
                ++m_pieceCtrl.lockResets;
            }
        }
        break;
    }
    case Action::RotateCCW:
    {
        bool rotated = m_pieceCtrl.tryRotate(-1);
        if (rotated)
        {
            m_game.audio().playSound(SoundId::Rotate);
            if (m_touchingFloor && m_pieceCtrl.lockResets < Constants::MAX_LOCK_RESETS)
            {
                m_lockAccum = sf::Time::Zero;
                ++m_pieceCtrl.lockResets;
            }
        }
        break;
    }
    case Action::HardDrop:
    {
        int lines = m_pieceCtrl.hardDrop();
        m_game.audio().playSound(SoundId::Drop);
        if (lines > 0)
            m_game.audio().playSound(SoundId::Clear);
        if (lines == -1)
        {
            m_game.audio().playSound(SoundId::GameOver);
            m_game.audio().stopMusic();
            m_game.replaceState(std::make_unique<GameOverState>(m_game, m_score.score()));
            return;
        }
        m_gravityAccum = sf::Time::Zero;
        m_lockAccum = sf::Time::Zero;
        m_touchingFloor = false;
        break;
    }
    case Action::SoftDrop:
        m_softDropActive = true;
        break;
    case Action::Hold:
        m_pieceCtrl.holdPiece();
        m_gravityAccum = sf::Time::Zero;
        m_lockAccum = sf::Time::Zero;
        m_touchingFloor = false;
        break;
    case Action::Pause:
        m_game.pushState(std::make_unique<PauseState>(m_game));
        break;
    default:
        break;
    }
}

void PlayState::update(sf::Time dt)
{
    m_softDropActive = false;
    auto actions = m_input.update(dt);
    for (auto a : actions)
        processAction(a);

    // Gravity
    sf::Time interval = m_score.gravityInterval();
    if (m_softDropActive)
        interval = sf::seconds(1.f / 60.f);

    m_gravityAccum += dt;
    while (m_gravityAccum >= interval)
    {
        m_gravityAccum -= interval;
        bool moved = m_pieceCtrl.softDropStep(m_softDropActive);
        if (!moved)
        {
            m_touchingFloor = true;
        }
        else
        {
            m_touchingFloor = false;
            m_lockAccum = sf::Time::Zero;
        }
    }

    // Lock delay
    if (m_touchingFloor)
    {
        m_lockAccum += dt;
        if (m_lockAccum >= sf::seconds(Constants::LOCK_DELAY_S))
        {
            int lines = m_pieceCtrl.lockPiece();
            if (lines > 0)
            {
                m_game.audio().playSound(
                    m_score.level() > /* previous level */ 0 ? SoundId::Clear : SoundId::Clear);
            }
            m_game.audio().playSound(SoundId::Drop);
            if (lines == -1)
            {
                m_game.audio().playSound(SoundId::GameOver);
                m_game.audio().stopMusic();
                m_game.replaceState(std::make_unique<GameOverState>(m_game, m_score.score()));
                return;
            }
            m_lockAccum = sf::Time::Zero;
            m_touchingFloor = false;
            m_gravityAccum = sf::Time::Zero;
        }
    }
}

void PlayState::render(sf::RenderTarget &target)
{
    float ts = Constants::TILE_SIZE;
    sf::Vector2f bo = boardOrigin();
    sf::Vector2f so = sidebarOrigin();
    const Config &cfg = m_game.settings().get();

    Renderer::drawBoard(target, m_board, bo, ts);
    Renderer::drawGhostPiece(target, m_pieceCtrl, bo, ts);
    Renderer::drawActivePiece(target, m_pieceCtrl, bo, ts);
    Renderer::drawBoardBorder(target, bo, ts, cfg.boardW, cfg.boardH);

    if (m_game.assets().hasFont(FontId::Main))
        Renderer::drawSidebar(target, m_pieceCtrl, m_score,
                              m_game.assets().getFont(FontId::Main), so, ts);
}
