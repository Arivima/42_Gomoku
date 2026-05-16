# Architecture

## Overview

The project is split into three layers with clear ownership boundaries:

```
┌─────────────────────────────────────────────────────────┐
│                   Rendering Layer (SFML)                 │
│  BoardRenderer  PanelRenderer  PopupRenderer  StartScreen│
│  BottomBarRenderer                                       │
└──────────────────────────┬──────────────────────────────┘
                           │ reads UIState (immutable snapshot)
┌──────────────────────────▼──────────────────────────────┐
│               Application Layer (orchestration)          │
│  GomokuApp — event loop, AI scheduling, UIState builder  │
│  UIState — immutable frame snapshot passed to renderers  │
│  Palette — centralized color scheme (dark / light)       │
│  AiStatsAccumulator — timing metrics                     │
└────────────────┬──────────────────────────┬─────────────┘
                 │ owns & drives            │ calls
┌────────────────▼──────────┐  ┌───────────▼─────────────┐
│      Game (rules engine)  │  │    AiEngine (stateless)  │
│  ┌──────────────────────┐ │  │  evalCell()              │
│  │   GomokuBoard        │ │  │  rankMoves()             │
│  │   19×19 cell array   │ │  │  countInLine()           │
│  │   move history       │ │  └─────────────────────────┘
│  │   capture counts     │ │
│  └──────────────────────┘ │
│  makeMove / undo / redo   │
│  win / capture detection  │
│  double-three check       │
└───────────────────────────┘
```

---

## Directory structure

```
42_Gomoku/
├── include/
│   ├── app.hpp                     # GomokuApp — main controller
│   ├── core/
│   │   ├── board.hpp               # GomokuBoard — raw board state
│   │   ├── game.hpp                # Game — rules & state machine
│   │   ├── ai_engine.hpp           # AiEngine — heuristic evaluation
│   │   └── constants.hpp           # BOARD_SIZE, WIN_LENGTH, etc.
│   ├── rendering/
│   │   ├── board_renderer.hpp
│   │   ├── panel_renderer.hpp
│   │   ├── bottom_bar_renderer.hpp
│   │   ├── popup_renderer.hpp
│   │   └── start_screen.hpp
│   └── ui/
│       ├── ui_state.hpp            # UIState — frame snapshot for renderers
│       ├── palette.hpp             # Palette — color scheme
│       └── ai_stats.hpp            # AiStatsAccumulator
├── src/                            # Mirrors include/ structure
├── tests/
│   ├── board_test.cpp
│   └── test_main.cpp
└── assets/fonts/                   # Roboto
```

---

## Class responsibilities

### Core layer

**`GomokuBoard`** — raw board state only; no rules  
- 19×19 `Cell` array (`EMPTY` / `BLACK` / `WHITE`)  
- `placeStone` / `removeStone` / `removeCapturedStone` / `restoreCapturedStone`  
- `moveHistory` vector for undo support  
- `blackCaptures` / `whiteCaptures` counters  

**`Game`** — rules engine and game state machine  
- `makeMove(pos)` → `MoveResult` (success or reason for rejection)  
- `detectCaptures` / `applyCaptures`  
- `checkWin` / `checkCaptureWin` / `canBreakByCapture`  
- `countFreeThreesAt` — double-three prohibition  
- `undoMove` / `redoMove` with full `MoveRecord` stack  
- `GameStatus` enum: `PLAYING` / `BLACK_WINS` / `WHITE_WINS` / `DRAW`  

**`AiEngine`** — stateless; no persistent state  
- `evalCell(board, pos, aiPlayer)` — heuristic score for one position  
- `rankMoves(game, aiPlayer, topN)` — score all legal moves, return top N  
- `countInLine` — consecutive own/opponent stones in one direction  

**`Constants`**

| Constant | Value | Meaning |
|----------|-------|---------|
| `BOARD_SIZE` | 19 | Board dimension |
| `WIN_LENGTH` | 5 | Stones in a row to win |
| `CAPTURE_WIN` | 10 | Stones captured to win (5 pairs) |
| `MIN_THREAT_RUN` | 3 | Minimum run length shown as threat overlay |

### Application layer

**`GomokuApp`** — main event loop and orchestrator  
- `run()` → `processEvents()` → `update()` → `buildUIState()` → `render()`  
- AI turn: calls `AiEngine::rankMoves()` then `Game::makeMove()`  
- Builds `UIState` snapshot every frame for the rendering layer  
- Manages `AppState`: `START_SCREEN` / `PLAYING`  

**`UIState`** — immutable snapshot consumed by all renderers  
- Cell overlays: last move (yellow), forbidden (red), threat (orange), suggestion (green)  
- Hover position, capture counts, turn label, warning messages  
- Active popup type and content  
- Move history entries, AI stats snapshot  

**`Palette`** — factory for `dark()` / `light()` color schemes  
- All color decisions are centralized here; renderers hold `const Palette&`  

**`AiStatsAccumulator`** — running statistics over AI moves  
- `record(ms, depth)` / `snapshot()` / `reset()`  
- Tracks last / min / max / avg time and depth  

### Rendering layer

All renderers hold `const Font&` and `const Palette&` — no mutable shared state.

| Renderer | Output |
|----------|--------|
| `BoardRenderer` | 19×19 grid, stones (with optional move numbers), overlays, hover ghost |
| `PanelRenderer` | Right sidebar: mode, captures, AI stats, turn indicator, cell info |
| `BottomBarRenderer` | Bottom strip: overlay legend, keyboard shortcut list |
| `PopupRenderer` | Modal dialogs: illegal move, endgame notice, game over, rules, history |
| `StartScreen` | Config screen: mode, human color, opening rule, Start button |

---

## Data flow (main loop)

```
SFML Event
    │
    ▼
GomokuApp::processEvents()
    │  mouse click → Game::makeMove()
    │  key press  → undo / redo / toggle
    │
    ▼
GomokuApp::update()                 ← runs only on AI turn
    │  AiEngine::rankMoves()
    │  Game::makeMove(bestPos)
    │  AiStatsAccumulator::record()
    │
    ▼
GomokuApp::buildUIState()           ← rebuilds UIState every frame
    │  reads Game state
    │  resolves overlays, labels, popup state
    │
    ▼
GomokuApp::render()
    │  BoardRenderer::draw*(uiState)
    │  PanelRenderer::draw(uiState)
    │  BottomBarRenderer::draw(uiState)
    │  PopupRenderer::draw(uiState)   ← only if activePopup != NONE
    ▼
window.display()
```

---

## Key algorithms

### Move validation — double-three detection

`Game::countFreeThreesAt(pos, player)`

For each of the 4 directions, builds a 7-cell window centered on the candidate position and matches three patterns:

```
Type 1:  . X X X .      (three consecutive, open both ends)
Type 2:  . X X . X .    (two + gap + one, open both ends)
Type 3:  . X . X X .    (one + gap + two, open both ends)
```

A move is rejected if it creates ≥ 2 free-threes **and** does not simultaneously capture a pair.

### Capture detection

`Game::detectCaptures(pos, player)`

For each of 4 axes, checks both signs for the pattern `own — opp — opp — [candidate]`:

```
X  O  O  [new stone]  →  capture the two O's
```

All captured positions are collected, deduplicated, and removed atomically.

### Win detection

`Game::checkWin()` / `checkWinDirection(pos, player, dx, dy)`

Scans the full board; for each own stone, counts consecutive stones in the (+dx,+dy) direction. Returns a `WinInfo` with the start position and length when count ≥ `WIN_LENGTH`.

### Endgame capture rule

`Game::canBreakByCapture()`

When a player aligns 5+ stones, checks if the opponent can immediately remove a pair from that alignment to reduce the run below 5. If so, sets `m_endgamePending` and gives the opponent one extra turn to attempt the capture. Win is only confirmed after that turn passes without a break.

### AI heuristic

`AiEngine::evalCell(board, pos, aiPlayer)`

```
score = 0
for direction in [H, V, D↘, D↗]:
    own = countInLine(pos, direction, aiPlayer)
    opp = countInLine(pos, direction, opponent)
    score += own²  × 10.0   (attack weight)
    score += opp²  × 14.0   (defense weight — slightly higher to prioritize blocking)

center_dist = Chebyshev distance from (9, 9)
score += max(0, 9 − center_dist) × 1.5
```

Quadratic weighting means a 4-in-a-row scores far more than two separate 2-in-a-rows, correctly capturing urgency of near-wins.

`AiEngine::rankMoves(game, aiPlayer, topN)` filters out all illegal positions (occupied, double-three) before scoring, then returns the top N by descending score.

---

## Constants and sizing

| Constant | Value |
|----------|-------|
| `DEFAULT_CELL_SIZE` | 40 px |
| `STONE_RADIUS_RATIO` | 0.43 × cell size |
| `PADDING` | 20 px |
| `STAR_POINTS` | {3, 9, 15} (0-indexed) |
| Panel width | 380 px |
| Bottom bar height | 36 px |

---

## Build system

The Makefile auto-detects SFML via `brew --prefix sfml` and generates `.d` dependency files for incremental builds.

```
src/**/*.cpp  →  obj/**/*.o  →  bin/Gomoku
                 ↑ also generates obj/**/*.d (header deps)
```

Test build links all core objects (excluding `main.cpp`) with `tests/*.cpp` and `libgtest`.
