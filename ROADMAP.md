# Gomoku — Roadmap & Open Decisions

This file tracks the project phases, next steps, and decisions still to be made.

Mark decisions with `[DECIDED]` once resolved, and update AGENTS.md accordingly.

---

## Phase 1: Foundation

### 1.1 Project Skeleton & Build System
- [x] Create directory structure: `src/`, `include/`, `tests/`, `assets/`
- [x] Write Makefile with required targets (`all`, `clean`, `fclean`, `re`)
- [x] Add dev targets (`debug`, `test`, `format`, `lint`)
- [x] Install SFML (`brew install sfml`)
- [x] Verify: blank SFML window compiles and opens

### 1.2 Board Representation
- [x] Implement 19x19 board data structure
- [x] Stone placement and removal
- [x] State queries (what's at position X,Y?)
- [x] Unit tests for basic board operations

#### Decisions — Board Representation

> **Q1: How to represent the board internally?**
>
> Options:
> - **2D array of enums** (`enum Cell { EMPTY, BLACK, WHITE }`) — simple, readable, easy to debug.
> - **Bitboard** (two `uint64_t` arrays, one per player) — fast for pattern matching, but harder to read and debug. Better for high-performance AI.
> - **Hybrid** — use a simple 2D array for the game layer, convert to bitboard only inside the AI search loop.
>
> Recommendation: Start with 2D array. Optimize to bitboard later only if performance requires it. Clean code matters more than premature optimization.

> **Q2: Does the board support undo/redo?**
>
> Not required by the subject, but useful for:
> - AI search (minimax needs to make/unmake moves)
> - Potential undo feature in the UI
>
> Recommendation: Yes. Implement a `makeMove` / `undoMove` pair from the start. The AI will need it anyway.

---

## Phase 2: Game Rules

### 2.1 Win Detection (5+ alignment)
- [ ] Check horizontal, vertical, and both diagonal directions
- [ ] Handle "5 or more" (not exactly 5)
- [ ] Unit tests for all directions and edge cases (board edges, corners)

### 2.2 Capture Logic
- [ ] Detect flanking pattern (`X O O X`) in all 4 axes (8 directions)
- [ ] Remove captured stones from the board
- [ ] Track capture count per player
- [ ] Win-by-capture condition (10 stones / 5 pairs)
- [ ] "Move into capture" safety (placing between two opponent stones is safe)
- [ ] Unit tests for captures in all directions, multiple simultaneous captures

### 2.3 Endgame Capture
- [ ] When 5-in-a-row occurs, check if opponent can break it by capturing a pair from the line
- [ ] 4-pair vulnerability: if aligning player lost 4 pairs and opponent can capture a 5th, opponent wins
- [ ] Unit tests for breakable/unbreakable alignments

### 2.4 Double-Three Prohibition
- [ ] Free-three detection (including gapped free-threes like `X . X X`)
- [ ] Forbid moves that create two simultaneous free-threes
- [ ] Exception: allow double-three if the move also captures a pair
- [ ] Unit tests: legal vs. forbidden positions, edge cases with captures

#### Decisions — Game Rules

> **Q3: Order of implementation?**
>
> Recommendation: Implement in exactly this order (2.1 → 2.2 → 2.3 → 2.4).
> Each rule builds on the previous one. Double-three is the hardest and depends on the capture exception.
> Write tests for each before moving to the next.

---

## Phase 3: Minimal Playable Game (UI)

### 3.1 SFML Rendering
- [ ] Draw the Goban (19x19 grid with labeled coordinates)
- [ ] Draw stones (filled circles, black and white)
- [ ] Mouse input: detect which intersection the cursor is on, place stone on click
- [ ] Display current player indicator
- [ ] Display capture count per player
- [ ] Display game status (ongoing, win by alignment, win by capture)

### 3.2 Human vs Human (Hotseat)
- [ ] Two players alternate turns, all rules enforced
- [ ] Forbidden move feedback (visual cue when a move is rejected)
- [ ] Game over screen / restart option

**Checkpoint: At this point the game is fully playable with no AI.**

#### Decisions — UI

> **Q4: Window size and layout?**
>
> Options:
> - **Board only** — the board fills the window, info overlaid or minimal.
> - **Board + sidebar** — board on the left, info panel on the right (capture count, timer, current player, game status).
>
> Recommendation: Board + sidebar. Gives clear space for the AI timer, capture counts, and debug info later. Easier to extend.

> **Q5: Fixed or resizable window?**
>
> Options:
> - **Fixed size** — simpler, no layout math, consistent appearance.
> - **Resizable** — more polished, but requires recalculating all positions on resize.
>
> Recommendation: Fixed size. Not worth the effort for this project. Pick a reasonable resolution (e.g., 1000x800 — 800 for the board, 200 for the sidebar).

> **Q6: Stone colors?**
>
> The subject says "two different colors." Traditional is black/white.
> Do you want classic black/white, or something custom? This is purely aesthetic.

> **Q7: Board appearance?**
>
> Options:
> - **Flat grid** — colored background + lines. Simple, clean.
> - **Wood texture** — load a background image for the board. More visual polish, requires an asset.
>
> Recommendation: Start with flat grid. Add a wood texture later as polish if time allows.

> **Q8: How to show forbidden moves?**
>
> When a player tries to place a stone on a double-three:
> - Flash the intersection red briefly?
> - Show a text message ("Forbidden: double-three")?
> - Play a sound?
> - Just do nothing (stone doesn't place, no feedback)?
>
> Recommendation: Brief visual flash + text message in the sidebar. Silent failures confuse users.

> **Q9: Hover preview?**
>
> Show a semi-transparent "ghost" stone where the cursor is hovering?
> Nice for UX, trivial to implement with SFML. Recommended.

> **Q10: Restart / New game flow?**
>
> Options:
> - **Keyboard shortcut** (e.g., `R` to restart, `Esc` to quit) — minimal, fast.
> - **Menu screen** — a proper start screen with mode selection (vs AI, vs Human).
> - **Both** — menu at launch, keyboard shortcuts during game.
>
> Recommendation: Start with keyboard shortcuts only. Add a menu screen later if time allows.

---

## Phase 4: AI

### 4.1 Move Generation
- [ ] Generate candidate moves (only near existing stones, not all 361 squares)
- [ ] Order candidates by likely quality (improves alpha-beta pruning efficiency)

### 4.2 Heuristic Function
- [ ] Evaluate board positions numerically
- [ ] Consider: alignment length, open ends, capture threats, capture count, center control
- [ ] Must be fast (runs at every leaf node of the search tree)
- [ ] Must be accurate (this is the hardest part of the project)
- [ ] Iterate and refine through testing

### 4.3 Minimax with Alpha-Beta Pruning
- [ ] Implement minimax (or negamax variant)
- [ ] Add alpha-beta pruning
- [ ] Reach depth 10+ within 500ms average
- [ ] Handle terminal states correctly (wins, captures, draws)

### 4.4 AI Timer
- [ ] Measure AI computation time per move
- [ ] Display in the UI sidebar
- [ ] **No timer = project fails. Non-negotiable.**

### 4.5 Move Suggestion (Hotseat Mode)
- [ ] In Human vs Human mode, show the AI's recommended move
- [ ] Same engine, just surface the best move visually

#### Decisions — AI

> **Q11: Minimax or Negamax?**
>
> Options:
> - **Minimax** — classic two-function approach (maximize for AI, minimize for opponent). Easier to understand and explain during defense.
> - **Negamax** — single-function variant that negates scores. Cleaner code, fewer bugs, same result.
>
> Recommendation: Negamax. Less code, fewer places for bugs. Easy to explain: "it's minimax where we negate the score at each level instead of switching between max and min."

> **Q12: Additional search optimizations?**
>
> Alpha-beta pruning is the baseline. Other options (decide later based on performance):
> - **Iterative deepening** — search depth 1, then 2, then 3... up to time limit. Gives a best move at every depth and enables time control.
> - **Transposition table** — cache evaluated positions to avoid re-computation. Significant speedup.
> - **Killer move heuristic** — remember moves that caused beta cutoffs. Improves move ordering.
> - **Zobrist hashing** — fast incremental hash for board positions. Needed for transposition tables.
>
> Recommendation: Start with plain alpha-beta. Add iterative deepening next (it's nearly free and gives time control). Add transposition table only if you can't hit depth 10 in 500ms.

> **Q13: How to display the move suggestion in hotseat?**
>
> Options:
> - **Always visible** — show the AI's suggestion as a highlighted/pulsing intersection.
> - **On request** — player presses a key (e.g., `H` for hint) to see the suggestion.
>
> Recommendation: On request. Always-visible suggestions would spoil the game for players who don't want help.

---

## Phase 5: Polish & Stability

### 5.1 AI Debugging Visualization
- [ ] Show AI reasoning during or after each move
- [ ] Options: heatmap overlay, text panel with scores, toggle-able debug mode
- [ ] Needed for defense — evaluator will ask you to explain what the AI is thinking

### 5.2 Stability Hardening
- [ ] No crash under any circumstance (out of memory, rapid clicking, window events)
- [ ] Graceful error handling for asset loading failures
- [ ] Test edge cases: full board, immediate captures, games that last 361 moves

### 5.3 Performance Tuning
- [ ] Profile AI search (where is time spent?)
- [ ] Optimize heuristic (no heap allocations in the hot path)
- [ ] Verify: < 500ms average, depth >= 10

#### Decisions — Debug UI

> **Q14: How to show AI debug info?**
>
> Options:
> - **Sidebar text** — after each AI move, show: depth reached, nodes evaluated, best score, time taken. Simple, sufficient.
> - **Board heatmap** — color each intersection by its evaluated score. Impressive during defense, more work to implement.
> - **Toggle mode** — press `D` to enable/disable debug overlay.
>
> Recommendation: Sidebar text as baseline (easy, always useful). Add heatmap later if time allows.

---

## Phase 6: Bonuses (Optional)

Only if the mandatory part is **perfect**.

- [ ] Rule selection screen at game start
- [ ] Opening rules: Standard, Pro, Swap, Swap2
- [ ] Other ideas: undo/redo in UI, move history, save/load game, sound effects

---

## Decision Summary

| # | Question | Status | Choice |
|---|---|---|---|
| Q1 | Board representation | `DECIDED` | 2D array of enums |
| Q2 | Undo/redo support | `DECIDED` | Yes, makeMove/undoMove pair |
| Q3 | Rule implementation order | `PENDING` | |
| Q4 | Window layout | `PENDING` | |
| Q5 | Fixed or resizable window | `PENDING` | |
| Q6 | Stone colors | `PENDING` | |
| Q7 | Board appearance | `PENDING` | |
| Q8 | Forbidden move feedback | `PENDING` | |
| Q9 | Hover preview | `PENDING` | |
| Q10 | Restart / new game flow | `PENDING` | |
| Q11 | Minimax or Negamax | `PENDING` | |
| Q12 | Additional search optimizations | `PENDING` | |
| Q13 | Move suggestion display | `PENDING` | |
| Q14 | AI debug info display | `PENDING` | |
