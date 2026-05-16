# Test Evaluation

## Current state

One test file exists: `tests/board_test.cpp` — 15 tests, all targeting `GomokuBoard`.  
`test_main.cpp` is a one-liner (`#include <gtest/gtest.h>`) — no additional tests there.  
**`Game`, `AiEngine`, and all rule logic are entirely untested.**

---

## Evaluation of existing tests

### What they cover well

| Test | Value |
|------|-------|
| `InitialBoardIsEmpty` | Exhaustive — checks all 361 cells |
| `InitialState` | Good baseline: player, captures, move count, full status |
| `PlaceStoneBlack/White` | Correct placement and move count increment |
| `CannotPlaceOnOccupiedCell` | Important guard — would catch a regression in the occupied check |
| `CannotPlaceOutsideBoard` | Tests all four boundary directions |
| `PlaceStoneViaPosition` | Validates the `Position` overload — good, it's the most-used call site |
| `RemoveStone` / `CannotRemoveEmptyCell` | Verifies removal and its guard |
| `ClearBoard` | Composite: checks cell state, player, captures, move count after clear |
| `SwitchPlayer` | Round-trip toggle test |
| `AddCaptures` / `SetCaptures` | Accumulation and override |
| `IsFull` | Fills the board systematically — tests the sentinel correctly |
| `GetCellOutOfBounds` | Important: confirms the method returns `EMPTY` rather than UB |

### Gaps in existing tests

- `RemoveStone` does not verify that `moveHistory` is updated (undo depends on this).
- `ClearBoard` does not test that `moveHistory` is also cleared.
- `AddCaptures` does not test WHITE side — only BLACK is exercised.
- `IsFull` fills with alternating BLACK/WHITE which calls `placeStone` for both players on the same board object; it implicitly tests that `placeStone` does not check `getCurrentPlayer()`, but this is not documented.
- No test for `removeCapturedStone` / `restoreCapturedStone` — these are the undo primitives and differ from `removeStone` in a subtle but critical way (they do not push to move history).

### Verdict

The existing tests are **correct and useful as a sanity baseline** for `GomokuBoard`. They protect against accidental regressions in the board primitives. However, they cover ≈4% of the non-trivial game logic surface. The classes that matter most for correctness — `Game` and `AiEngine` — have zero coverage.

---

## Missing tests — priority order

### 1. `Game` — capture detection (`game_capture_test.cpp`)

This is the highest-value target: capture logic is complex, directional, and easy to silently break.

```cpp
// Horizontal capture: B places at (6,9), pattern X O O X
// Place: W(3,9) W(4,9) B(5,9), then B places at (6,9) → captures (4,9)(5,9)? No—
// correct pattern: B(3,9) W(4,9) W(5,9) then B places at (6,9) → captures W(4,9) W(5,9)
TEST(GameCapture, HorizontalCapture)
TEST(GameCapture, VerticalCapture)
TEST(GameCapture, DiagonalCapture_TopLeftBottomRight)
TEST(GameCapture, DiagonalCapture_TopRightBottomLeft)
TEST(GameCapture, SimultaneousCaptureTwoDirections)   // single move captures 2 pairs
TEST(GameCapture, NoCaptureWhenNotFlanked)
TEST(GameCapture, PlacingBetweenTwoOpponents_IsNotCapture)  // "move into capture" is safe
TEST(GameCapture, CaptureCountIncrements)
TEST(GameCapture, CaptureWinAt10Stones)               // 5 pairs → BLACK_WINS
```

### 2. `Game` — win detection (`game_win_test.cpp`)

```cpp
TEST(GameWin, HorizontalWin)
TEST(GameWin, VerticalWin)
TEST(GameWin, DiagonalWin_NE)
TEST(GameWin, DiagonalWin_NW)
TEST(GameWin, SixInARow_StillWins)       // "5 or more", not exactly 5
TEST(GameWin, FourInARow_NoWin)
TEST(GameWin, WinNearBoardEdge)          // alignment touching row 0 or col 18
TEST(GameWin, WinInfo_StoresCorrectPositions)  // WinInfo.stones matches actual line
```

### 3. `Game` — double-three prohibition (`game_doublethree_test.cpp`)

This is the most complex rule and the most likely to have edge cases.

```cpp
// Type 1: .XXX.
TEST(DoubleThree, Type1_Rejected)
// Type 2: .XX.X.
TEST(DoubleThree, Type2_Rejected)
// Type 3: .X.XX.
TEST(DoubleThree, Type3_Rejected)
// Two free-threes in different directions
TEST(DoubleThree, TwoDirections_Rejected)
// Only one free-three → legal
TEST(DoubleThree, OneFreeThree_Allowed)
// Exception: double-three is allowed if move also captures
TEST(DoubleThree, DoubleThreeWithCapture_Allowed)
// Board edge: partial window should not count as free-three
TEST(DoubleThree, AtBoardEdge_NotFreeThree)
```

### 4. `Game` — endgame capture (`game_endgame_test.cpp`)

The most intricate rule; bugs here are catastrophic and invisible without tests.

```cpp
// Scenario: Black aligns 5; White has a breakable pair in the line
TEST(EndgameCapture, BreakableAlignment_EndgamePending)
// White captures the pair → alignment gone → game continues
TEST(EndgameCapture, OpponentBreaks_GameContinues)
// White fails to break → Black wins
TEST(EndgameCapture, OpponentFailsToBreak_AlignerWins)
// Unbreakable alignment (no flanking opportunity) → immediate win
TEST(EndgameCapture, UnbreakableAlignment_ImmediateWin)
// White reaches 10 captures during the endgame response turn → White wins
TEST(EndgameCapture, ResponderWinsByCapture)
// Double endgame: responder also aligns 5 while breaking
TEST(EndgameCapture, ResponderAlignsWhileBreaking)
```

### 5. `Game` — undo/redo (`game_undo_test.cpp`)

Undo is load-bearing for both the UI and the future Minimax search.

```cpp
TEST(Undo, UndoPlacement)               // stone removed, player switched back
TEST(Undo, UndoCapture)                 // captured stones restored, count reverted
TEST(Undo, UndoAfterWin)               // game returns to PLAYING, winInfo cleared
TEST(Undo, UndoEndgamePending)         // endgamePending / endgameWinner restored
TEST(Undo, UndoRedoRoundTrip)          // make N moves, undo N, redo N → same state
TEST(Undo, RedoClearedAfterNewMove)    // redo stack is invalidated after a new move
TEST(Undo, UndoOnEmptyStack)           // returns false, no crash
TEST(Undo, RedoOnEmptyStack)           // returns false, no crash
```

### 6. `Game` — move validity (`game_validity_test.cpp`)

```cpp
TEST(MoveValidity, OutOfBounds)
TEST(MoveValidity, Occupied)
TEST(MoveValidity, DoubleThreeRejected)
TEST(MoveValidity, LegalMove)
TEST(MoveValidity, MoveAfterGameOver_Rejected)
```

### 7. `AiEngine` — evaluation and ranking (`ai_engine_test.cpp`)

```cpp
// evalCell returns a finite float for all 361 cells on an empty board
TEST(AiEngine, EvalCell_FiniteOnEmptyBoard)
// Center cell scores higher than a corner on an empty board
TEST(AiEngine, EvalCell_CenterHigherThanCorner)
// Placing own stone adjacent to existing own stone raises evalCell score
TEST(AiEngine, EvalCell_AlignedStoneIncreasesScore)
// rankMoves returns no occupied or forbidden positions
TEST(AiEngine, RankMoves_NoneOccupied)
TEST(AiEngine, RankMoves_NoneDoubleThree)
// rankMoves prefers blocking an opponent 4-in-a-row over an open center cell
TEST(AiEngine, RankMoves_BlocksOpponentFour)
// topN=1 returns exactly one result; topN=0 returns all
TEST(AiEngine, RankMoves_TopNRespected)
// On a near-full board, rankMoves returns only the remaining empty cells
TEST(AiEngine, RankMoves_NearFullBoard)
```

### 8. `GomokuBoard` — missing coverage (`board_test.cpp` additions)

```cpp
TEST(Board, RemoveCapturedStone_DoesNotDecrementMoveCount)
TEST(Board, RestoreCapturedStone_RestoressCell)
TEST(Board, AddCapture_WhitePlayer)
TEST(Board, ClearBoard_AlsoClearsMoveHistory)
TEST(Board, MoveHistoryTracksOrder)
```

---

## Suggested file layout

```
tests/
├── test_main.cpp           # existing (keep as-is)
├── board_test.cpp          # existing — add 5 missing cases above
├── game_capture_test.cpp   # new
├── game_win_test.cpp       # new
├── game_doublethree_test.cpp  # new
├── game_endgame_test.cpp   # new
├── game_undo_test.cpp      # new
├── game_validity_test.cpp  # new
└── ai_engine_test.cpp      # new
```

The Makefile test target already links all core `.o` files and `tests/*.cpp`, so adding new test files requires no Makefile changes.

---

## Test writing tips for this codebase

**Board setup helper** — most `Game` tests need a specific board state. Write a free function:
```cpp
void placeMoves(Game& g, std::initializer_list<Position> blacks,
                          std::initializer_list<Position> whites) {
    // alternates BLACK/WHITE to satisfy turn order, or uses getBoard().placeStone directly
}
```

**Direct board manipulation for `Game` tests** — `Game::getBoard()` returns a non-const reference. Use `game.getBoard().placeStone(pos, player)` to set up complex positions without going through move validation, then call `game.makeMove()` for the move under test. This avoids having to construct a legal move sequence just to reach a specific state.

**`checkWin` is private** — test it through `makeMove` return value and `game.getStatus()`.

**`countFreeThreesAt` is private** — test it through `checkMoveValidity` (which returns `MoveFailReason::DOUBLE_THREE`) or through `makeMove`.

**Parameterized tests** for directional cases — use `INSTANTIATE_TEST_SUITE_P` to run the same win/capture test in all 4 directions without copy-paste.

---

## Priority summary

| Priority | File | Reason |
|----------|------|--------|
| 1 | `game_capture_test.cpp` | Core mechanic; most bugs here are silent |
| 2 | `game_undo_test.cpp` | Required by Minimax search; broken undo = broken AI |
| 3 | `game_win_test.cpp` | Victory condition — must be correct before AI work |
| 4 | `game_doublethree_test.cpp` | Most complex rule; tricky edge cases |
| 5 | `game_endgame_test.cpp` | Critical for correctness but rarer in play |
| 6 | `ai_engine_test.cpp` | Helps tune and validate heuristic changes |
| 7 | `game_validity_test.cpp` | Quick wins; already partially covered indirectly |
| 8 | `board_test.cpp` additions | Low risk, fills obvious gaps |
