# Tetris – Game Design

## Overview
A classic Tetris clone using SFML 3 / C++23. Full Tetris Guideline mechanics: 7-bag
randomiser, SRS rotation with wall kicks, Guideline scoring, and six game screens.

---

## Screens

| Screen | Description |
|--------|-------------|
| **Main Menu** | Start Game, High Scores, Settings, Quit |
| **Play** | Core gameplay |
| **Pause** | Overlay over Play; Resume, Quit to Menu |
| **Game Over** | Shows final score; prompt to enter 3-char name |
| **High Score** | Top 10 list sorted by score |
| **Settings** | Board width (8–12), board height (16–24), start level (1–15), music volume (0–100) |

Settings are persisted to `settings.cfg` (plain `key=value` text file).

---

## Playfield

- Default: **10 columns × 20 rows** visible; 2 hidden rows above for spawning.
- Configurable via Settings (board width 8–12, board height 16–24).
- Tile size: **32 px**. Window is sized dynamically: `(boardW × 32 + 200) × (boardH × 32)`.

---

## Tetrominoes

Seven standard pieces (I, J, L, O, S, T, Z) with Guideline colours:

| Piece | Colour |
|-------|--------|
| I | Cyan |
| J | Blue |
| L | Orange |
| O | Yellow |
| S | Green |
| T | Purple |
| Z | Red |

Randomisation uses a **7-bag** system: all 7 pieces shuffled, then the next bag starts.

Rotation system: **SRS** (Super Rotation System) with standard JLSTZ and I wall-kick tables.

Spawn position: top-centre of the board, 1 row above the visible area.

---

## Controls

| Key | Action |
|-----|--------|
| `←` / `→` | Move left / right |
| `↓` | Soft drop (20× gravity) |
| `Space` | Hard drop (instant lock) |
| `Z` | Rotate counter-clockwise |
| `X` or `↑` | Rotate clockwise |
| `C` | Hold (one swap per lock) |
| `Esc` | Pause / back |
| `Enter` | Confirm / select |

### DAS / ARR
- **DAS** (Delayed Auto Shift): 167 ms before key-repeat starts.
- **ARR** (Auto Repeat Rate): 33 ms between repeated moves.

---

## Gravity

Gravity is expressed as **seconds per cell drop**.

| Level | Seconds/cell |
|-------|-------------|
| 1 | 1.000 |
| 2 | 0.793 |
| 3 | 0.618 |
| 4 | 0.473 |
| 5 | 0.355 |
| 6 | 0.262 |
| 7 | 0.190 |
| 8 | 0.135 |
| 9 | 0.094 |
| 10 | 0.064 |
| 11 | 0.042 |
| 12 | 0.028 |
| 13 | 0.017 |
| 14 | 0.011 |
| 15+ | 0.007 |

Soft drop forces one cell per frame (60 fps = ~16 ms per forced cell).

---

## Scoring

Guideline scoring (multiplied by current level):

| Lines cleared | Points × level |
|---------------|---------------|
| 1 (Single) | 100 |
| 2 (Double) | 300 |
| 3 (Triple) | 500 |
| 4 (Tetris) | 800 |

Hard drop: **+2 points per cell** dropped.  
Soft drop: **+1 point per cell** dropped.

Level increases by 1 every **10 lines** cleared.

---

## Lock Delay
A piece locks **0.5 s** after it touches the floor without being moved.  
Each successful move or rotation resets the lock timer (max 15 resets per piece).

---

## Hold Piece
- Press `C` to store the current piece into the hold slot.
- If a piece is already held, the two are swapped.
- Hold is reset to available on each new piece lock.

---

## Next Queue
- **3 next pieces** are displayed in the sidebar.

---

## Audio

| Event | Sound |
|-------|-------|
| Move | `move.wav` |
| Rotate | `rotate.wav` |
| Hard drop | `drop.wav` |
| Line clear | `clear.wav` |
| Level up | `levelup.wav` |
| Game over | `gameover.wav` |
| Background music | `theme.ogg` (looped) |

Music volume is configurable. Sound effects volume is fixed at 80%.

---

## Game Over
The game ends when a newly spawned piece immediately overlaps locked cells
(block-out condition).
