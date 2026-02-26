# Tetris – Architecture

## High-Level Overview

```
main()
  └─ Game
       ├─ sf::RenderWindow
       ├─ StateStack
       │    └─ IGameState  [MenuState | PlayState | PauseState |
       │                    GameOverState | HighScoreState | SettingsState]
       ├─ AssetManager      (fonts cache)
       ├─ AudioManager      (sounds + music)
       ├─ SettingsManager   (settings.cfg ↔ Config struct)
       └─ HighScoreList     (vector of {name, score})
```

---

## Main Loop

`Game::run()` runs at **60 fps** using a fixed-timestep update:

```
while (window.isOpen()):
    handleEvents()   → forward sf::Event to top state
    update(dt)       → update top state (non-blocking states also update below)
    render()         → clear → top state renders → display
    processStateQueue()   → apply pending push/pop/replace
```

---

## State Stack

`StateStack` manages a `vector<unique_ptr<IGameState>>`. Operations (push, pop,
replace) are queued to avoid mutation during iteration.

Transparency flag: `PauseState` is *transparent* (the Play state below still renders
and, optionally, updates in a frozen state).

State transitions:

```
MenuState ──[Start]──► PlayState ──[Esc]──► PauseState
                            │                   │[Resume]
                            │                   └──────────► PlayState (resume)
                            │                   │[Quit Menu]
                       [Game Over]               └──────────► MenuState
                            ▼
                       GameOverState ──[Enter]──► HighScoreState ──[Esc]──► MenuState
MenuState ──[Settings]──► SettingsState ──[Esc]──► MenuState
MenuState ──[Scores]────► HighScoreState ──[Esc]──► MenuState
```

---

## Class Responsibilities

### `Game`
- Owns `sf::RenderWindow`, `StateStack`, shared managers.
- Pushes `MenuState` at startup.
- Provides accessor methods to managers so states can use them without coupling.

### `IGameState` (abstract interface)
```cpp
virtual void handleEvent(const sf::Event&) = 0;
virtual void update(sf::Time dt) = 0;
virtual void render(sf::RenderTarget&) = 0;
virtual void onEnter() {}
virtual void onExit()  {}
bool transparent = false;  // render below state when true
```

### `Board`
- Holds a 2-D grid: `array<array<CellColor, maxW>, maxH>`.
- `isValid(Tetromino, pos, rot) → bool`
- `lock(Tetromino, pos, rot)`
- `clearLines() → int`   (returns count of lines cleared)
- `isGameOver() → bool`  (any cell in the top hidden rows)
- `reset()`

### `Tetromino` (header-only value type)
- Stores: `TetrominoType type`
- Static data: `constexpr` arrays of rotation states (4 cells × 4 rotations).
- Static data: SRS wall-kick tables for JLSTZ and I.
- `getCells(int rotation) → array<sf::Vector2i, 4>`
- `getKicks(int fromRot, int toRot) → span<sf::Vector2i>`

### `TetrominoFactory`
- 7-bag shuffler using `std::mt19937` seeded with `std::random_device`.
- `next() → Tetromino`

### `PieceController`
- Owns: active piece, active position, active rotation, ghost row, hold piece,
  next queue (3 pieces), hold-used flag.
- `tryMove(dx) → bool`
- `tryRotate(dir) → bool`   (applies SRS kicks via Board)
- `hardDrop() → int`        (returns cells dropped)
- `softDropStep() → bool`   (moves down one cell)
- `lockPiece() → int`       (locks piece, refills queue, returns lines cleared via Board)
- `reset()`

### `ScoreManager`
- Tracks score, level, totalLines.
- `addLinesCleared(int n)`  (applies Guideline table; increments level every 10 lines)
- `addDropPoints(int cells, bool hard)`
- `getGravityInterval() → sf::Time`
- `reset()`

### `InputHandler`
- Maps `sf::Keyboard::Key` → `Action` enum.
- Tracks DAS/ARR state per direction key using `sf::Clock`.
- `processEvent(sf::Event) → void`
- `update(sf::Time dt) → vector<Action>`
- `reset()`

### `Renderer`
- Stateless free functions (or a class of static methods).
- `drawBoard(target, board, tileSize, origin)`
- `drawPiece(target, cells, color, tileSize, origin)`  (active + ghost)
- `drawSidebar(target, pieceCtrl, scoreManager, font, origin)`
- `drawOverlay(target, text, font)`

### `AudioManager`
- Loads `sf::SoundBuffer` objects at startup; plays via `sf::Sound`.
- Holds `sf::Music` for background track.
- `playSound(SoundId id)`
- `playMusic() / pauseMusic() / stopMusic()`
- `setMusicVolume(float v)`

### `AssetManager`
- `loadFont(FontId, path) → bool`
- `getFont(FontId) → sf::Font&`

### `SettingsManager`
- Config struct: `{ int boardW, boardH, startLevel; float musicVolume; }`
- `load(path) → bool`
- `save(path) → bool`
- `get() → Config&`

---

## Data Flow (Play state one tick)

```
InputHandler::update(dt)
  └─ [Action::MoveLeft]
       └─ PieceController::tryMove(-1)
            └─ Board::isValid(...)
PieceController → gravity timer expires
  └─ softDropStep() / lockPiece()
       └─ Board::lock() → Board::clearLines() → int n
            └─ ScoreManager::addLinesCleared(n)
                 └─ AudioManager::playSound(Clear/LevelUp)
Renderer::drawBoard + drawPiece + drawSidebar
```

---

## Key C++23 Features Used

| Feature | Where |
|---------|-------|
| `std::expected<T,E>` | File I/O in `SettingsManager`, `AssetManager` |
| `std::println` | Debug logging |
| `std::ranges::shuffle` | 7-bag in `TetrominoFactory` |
| `std::optional` | `pollEvent`, hold piece, wall-kick results |
| Structured bindings | Row/col iteration |
| `constexpr` arrays | SRS kick tables in `Tetromino.hpp` |
