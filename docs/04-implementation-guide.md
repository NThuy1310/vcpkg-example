# Tetris – Implementation Guide

## Build Order (recommended)

Implement files in this sequence to keep each step compilable:

1. `utils/Types.hpp` – enums and aliases used everywhere
2. `utils/Constants.hpp` – numeric constants, gravity table, colours
3. `core/Tetromino.hpp` – piece shapes and kick tables (header-only)
4. `core/Board` – grid operations
5. `core/TetrominoFactory` – 7-bag RNG
6. `core/PieceController` – active piece management
7. `core/ScoreManager` – scoring and level
8. `systems/InputHandler` – key → action mapping, DAS/ARR
9. `systems/Renderer` – drawing helpers
10. `systems/AssetManager` – font loading
11. `systems/AudioManager` – sound/music
12. `systems/SettingsManager` – config file I/O
13. `states/IGameState.hpp`
14. `states/PlayState` – implement first; this drives all core classes
15. `states/PauseState`
16. `states/GameOverState` / `HighScoreState`
17. `states/MenuState` / `SettingsState`
18. `Game` – wire everything together
19. `main.cpp` – call `Game::run()`

---

## CMakeLists.txt Changes

Replace the single-source `set(SOURCES ...)` with glob + asset copy:

```cmake
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Copy assets to the build output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
```

---

## SFML 3.0.2 API Notes

### Window and events
```cpp
// Window must be RenderWindow for drawing
sf::RenderWindow window(sf::VideoMode({w, h}), "Tetris");

// Event loop returns std::optional<sf::Event>
while (const auto event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) window.close();

    if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
        // kp->code, kp->scancode, kp->alt, kp->shift ...
}
```

### Loading resources (return std::optional)
```cpp
// Font
std::optional<sf::Font> font = sf::Font::openFromFile("assets/fonts/mono.ttf");
if (!font) { /* handle error */ }

// SoundBuffer
std::optional<sf::SoundBuffer> buf = sf::SoundBuffer::loadFromFile("assets/sounds/move.wav");

// Music (stream, not optional – returns bool via openFromFile on the object)
sf::Music music;
if (!music.openFromFile("assets/music/theme.ogg")) { /* error */ }
```

### Text
```cpp
sf::Text text(*font, "Hello", 24);   // font ref, string, character size
text.setFillColor(sf::Color::White);
text.setPosition({x, y});
window.draw(text);
```

### Drawing shapes
```cpp
sf::RectangleShape tile({32.f, 32.f});
tile.setFillColor(sf::Color::Cyan);
tile.setOutlineColor(sf::Color(0, 0, 0, 80));
tile.setOutlineThickness(-1.f);
tile.setPosition({col * 32.f, row * 32.f});
window.draw(tile);
```

---

## C++23 Features

| Feature | Example usage |
|---------|---------------|
| `std::expected<T,E>` | `SettingsManager::load()` returns `std::expected<Config, std::string>` |
| `std::println` | `std::println("Level up: {}", level);` (requires `<print>`) |
| `std::ranges::shuffle` | `std::ranges::shuffle(bag, rng);` |
| `std::optional` | Hold piece: `std::optional<Tetromino> holdPiece` |
| Structured bindings | `auto [row, col] = cell;` |
| `constexpr` arrays | Kick tables and piece shapes in `Tetromino.hpp` |
| `std::to_underlying` | Converting `CellColor` enum to array index |

---

## Settings File Format

`settings.cfg` uses plain `key=value` lines (one per line, `#` for comments):

```
boardW=10
boardH=20
startLevel=1
musicVolume=70
```

Parsing:
```cpp
std::getline(file, line);
auto eq = line.find('=');
auto key = line.substr(0, eq);
auto val = line.substr(eq + 1);
```

---

## High Score File Format

`highscores.dat` – plain text, one entry per line: `NAME SCORE`

```
AAA 12400
BBB 9800
```

Up to 10 entries, sorted descending by score. Managed by `Game` directly.

---

## SRS Wall-Kick Tables

Two kick tables are needed:

- **JLSTZ kicks** (used for J, L, S, T, Z pieces):  
  5 test offsets per rotation transition `(from → to)`.
- **I kicks** (used for I piece):  
  Different offset set due to the 4×1 shape.

Both tables are stored as `constexpr` arrays in `Tetromino.hpp`.  
When rotating, `Board::isValid()` is tested for each kick offset in order.  
The first passing offset is used; if none pass, rotation is denied.

---

## Gravity Timer

In `PlayState::update(dt)`:

```cpp
gravityAccum += dt;
sf::Time interval = scoreManager.getGravityInterval();
if (softDropActive) interval /= 20.f;

while (gravityAccum >= interval) {
    gravityAccum -= interval;
    if (!pieceController.softDropStep()) {
        lockTimer += interval;  // touching floor
    }
}
if (lockTimer >= LOCK_DELAY) lockPiece();
```

---

## Fixed Timestep

`Game::run()` uses variable timestep clamped to 100 ms to prevent spiral-of-death:

```cpp
sf::Clock clock;
while (window.isOpen()) {
    sf::Time dt = std::min(clock.restart(), sf::milliseconds(100));
    // handleEvents → update(dt) → render
}
```

---

## Verifying the Build

```bash
# Configure
cmake --preset linux-debug -DCMAKE_CXX_COMPILER=/usr/bin/g++-11

# Build
cmake --build --preset linux-debug

# Run
./build/linux-debug/Tetris
```

Expected sequence:
1. Main Menu appears.
2. Navigate to Settings – change board size / music volume – saved to `settings.cfg`.
3. Start Game – gameplay works (pieces fall, rotate, lines clear, level increases).
4. Game Over – enter name – appears in High Score list.
5. Pause (Esc) – resume works; Quit to Menu works.
