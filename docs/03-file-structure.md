# Tetris – File Structure

```
vcpkg-example/
├── CMakeLists.txt              # Build config – lists all .cpp sources, copies assets
├── CMakePresets.json           # Debug/Release presets (Linux/Windows/macOS)
├── vcpkg.json                  # Dependencies: sfml, fmt, spdlog
├── vcpkg-configuration.json    # vcpkg registry baseline
├── settings.cfg                # Runtime-generated user settings (key=value)
│
├── assets/
│   ├── fonts/
│   │   └── mono.ttf            # Monospace pixel font for all UI text
│   ├── sounds/
│   │   ├── move.wav
│   │   ├── rotate.wav
│   │   ├── drop.wav
│   │   ├── clear.wav
│   │   ├── levelup.wav
│   │   └── gameover.wav
│   └── music/
│       └── theme.ogg           # Background music (looped)
│
├── docs/
│   ├── 01-game-design.md       # Rules, controls, scoring, screens
│   ├── 02-architecture.md      # Class diagram, state machine, data flow
│   ├── 03-file-structure.md    # This file
│   └── 04-implementation-guide.md
│
└── src/
    ├── main.cpp                # Entry point: constructs Game, calls run()
    ├── Game.hpp                # Game class declaration
    ├── Game.cpp                # Main loop, state stack owner, shared resources
    │
    ├── utils/
    │   ├── Types.hpp           # CellColor enum, Action enum, using aliases
    │   └── Constants.hpp       # Tile size, gravity table, DAS/ARR timing, colours
    │
    ├── core/
    │   ├── Tetromino.hpp       # Header-only: piece shapes, SRS kick tables
    │   ├── Board.hpp
    │   ├── Board.cpp           # Grid, isValid, lock, clearLines, isGameOver
    │   ├── TetrominoFactory.hpp
    │   ├── TetrominoFactory.cpp # 7-bag RNG
    │   ├── PieceController.hpp
    │   ├── PieceController.cpp  # Active piece, ghost, hold, next queue
    │   ├── ScoreManager.hpp
    │   └── ScoreManager.cpp    # Score, level, lines, gravity interval
    │
    ├── systems/
    │   ├── InputHandler.hpp
    │   ├── InputHandler.cpp    # sf::Event → Action, DAS/ARR
    │   ├── Renderer.hpp
    │   ├── Renderer.cpp        # Draw board, pieces, sidebar, overlays
    │   ├── AudioManager.hpp
    │   ├── AudioManager.cpp    # Sounds + music
    │   ├── AssetManager.hpp
    │   ├── AssetManager.cpp    # Font cache
    │   ├── SettingsManager.hpp
    │   └── SettingsManager.cpp # Read/write settings.cfg
    │
    └── states/
        ├── IGameState.hpp      # Abstract interface
        ├── MenuState.hpp
        ├── MenuState.cpp       # Navigation menu (Start, Scores, Settings, Quit)
        ├── PlayState.hpp
        ├── PlayState.cpp       # Core gameplay loop
        ├── PauseState.hpp
        ├── PauseState.cpp      # Pause overlay (transparent)
        ├── GameOverState.hpp
        ├── GameOverState.cpp   # Score entry
        ├── HighScoreState.hpp
        ├── HighScoreState.cpp  # Top-10 display
        ├── SettingsState.hpp
        └── SettingsState.cpp   # Settings editor
```

---

## Notable Relationships

```
PlayState
  owns ──► PieceController
              owns ──► Board
                        uses ──► Tetromino (value type)
              owns ──► TetrominoFactory
              owns ──► ScoreManager
  owns ──► InputHandler
  uses ──► Renderer       (via static calls)
  uses ──► AudioManager   (via Game& ref)
  uses ──► AssetManager   (via Game& ref)
```

All states hold a `Game&` reference to access shared systems (window size, managers,
push/pop state stack).

---

## Runtime Files

| File | Location | Notes |
|------|----------|-------|
| `settings.cfg` | working directory (next to executable) | Created on first run |
| `highscores.dat` | working directory | Plain text; 10 lines of `NAME SCORE` |
| `assets/` | copied to build output by CMake | `file(COPY assets ...)` |
