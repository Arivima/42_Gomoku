# Code Review

## Summary

The codebase is well-structured for a game project of this scope. The layered architecture (core / app / rendering) is clean and the separation of concerns is largely respected. The most significant gap is the AI: the engine is currently a depth-1 heuristic stub, while the subject requires Minimax/Negamax at depth ≥ 10 within 500 ms. Everything else is either done or close to done.

---

## Strengths

- **Clear layered architecture.** Core logic has zero SFML dependencies. Renderers hold only `const&` to palette and font — no accidental coupling.
- **Immutable UIState pattern.** Rebuilding the frame snapshot every tick makes rendering deterministic and easy to reason about. No hidden shared state between renderers.
- **Undo/redo from day one.** `MoveRecord` stacks give correct full-state restoration including captured stones and player turn. The AI will benefit from this directly.
- **Palette abstraction.** Centralizing all colors in `Palette::dark()` / `Palette::light()` makes theming a one-liner and keeps renderers free of magic color literals.
- **`AiEngine` is stateless.** Pure function interface makes it trivially testable and reusable for both AI play and coach suggestions.
- **`MoveResult` encodes failure reason.** Returning `FailReason` instead of a bare bool lets the UI show informative messages without coupling the game logic to UI strings.

---

## Technical Debt

### Critical (blocks passing the subject)

**AI depth is 1 — subject requires ≥ 10**  
`AiEngine::rankMoves` scores positions using a depth-1 heuristic. There is no game tree search. The subject mandates Minimax (or equivalent) at depth ≥ 10 within 500 ms average, and the defense will specifically ask you to explain the search algorithm. This is the single highest-priority item.

**No AI timer**  
`AiStatsAccumulator` tracks timing data, but there is no time-budget enforcement during search. The subject states: "No timer = project fails. Non-negotiable."

### Medium

**`GomokuApp` is doing too much**  
`app.cpp` handles event routing, AI scheduling, UIState construction, popup state management, and rendering orchestration — ~600 lines. The AI scheduling and popup state machine would benefit from being extracted into dedicated classes once the Minimax search is added, otherwise `update()` will become very hard to maintain.

**`Game::makeMove` is a long function**  
The method handles validity check, placement, capture, win detection, endgame capture logic, and undo stack push in a single function. Each of these steps is already broken into private methods, but the sequencing logic inside `makeMove` is dense. Consider a named helper (`applyMove`) that does placement + side effects, leaving `makeMove` as pure validation + delegation.

**`AiEngine` is not a class with state — but it is instantiated as one**  
All methods in `AiEngine` are effectively free functions operating on `Game&`. The class adds no state. This is fine for now, but the Minimax implementation will need transient search state (nodes evaluated, depth reached, best path). Plan to add those members before implementing search.

**Undo/redo not available in AI mode**  
The undo stack is built but may not correctly handle AI moves (undoing one human move should also undo the subsequent AI reply). Verify the behavior before shipping.

### Low

**`ROADMAP.md` has many `PENDING` decisions**  
Many questions (Q3–Q14) are marked `PENDING` but the implementation has already resolved them. The roadmap should be updated to reflect the actual choices made.

**`countFreeThreesAt` pattern matching is hard to audit**  
The 7-cell window with numeric codes (1, 0, -1) and hardcoded index checks works, but is difficult to verify against the Gomoku rule book. Consider a brief comment naming each pattern (Type 1 / Type 2 / Type 3) inline so reviewers can cross-check without mental simulation.

---

## Inconsistencies

- `GomokuBoard::removeCapturedStone` / `restoreCapturedStone` exist alongside `removeStone` / `placeStone`. The distinction (captured removal does not push to move history) is not obvious from the name alone and could cause bugs if the wrong method is called.
- `AiEngine::rankMoves` has a `topN` parameter defaulting to 1, but callers for coach mode pass 3. This asymmetry is fine but the default is surprising — someone reading a call site with no argument would expect a single-move return.
- Several `std::optional` fields in `UIState` (e.g., `hoverPos`, `hoveredCellInfo`, `aiStats`) default to `std::nullopt` but are not consistently checked before access in `buildUIState`. A missed check under an unexpected code path would crash.
- `PopupRenderer` uses hardcoded pixel sizes for each popup variant. If font or window dimensions change, these will silently produce clipped or misaligned popups. A layout pass that derives sizes from content would be more robust.
- The `STAR_POINTS` constant `{3, 9, 15}` is 0-indexed but corresponds to the 4th, 10th, and 16th lines of a 19×19 board. This is correct but not obvious; a comment or a named constant (`TENGEN = 9`) would help.

---

## Improvement suggestions

### Short-term (before AI work)

1. **Add `makeMove` / `undoMove` symmetry tests.** The endgame capture state is complex — add unit tests that verify undo fully restores `m_endgamePending`, `m_endgameWinner`, and `m_endgameWinInfo`.
2. **Extract AI scheduling from `GomokuApp::update()`.** Move the AI call into a small `AiController` that owns the timing budget check. This will make the Minimax integration much cleaner.
3. **Add `const` to `AiEngine` methods.** `rankMoves` and `evalCell` do not mutate state — marking them `const` makes the intent explicit and catches accidental writes.

### AI implementation

4. **Implement Negamax with alpha-beta pruning.** Negamax (single function, negated child score) is simpler than two-function Minimax and equally explainable. Start with depth 5, verify correctness, then tune for depth 10 / 500 ms.
5. **Add iterative deepening.** Run depth 1, 2, 3… up to the time budget. This gives a best move at every prefix depth and makes time control trivial — return the last completed depth's result.
6. **Candidate move pruning.** Instead of evaluating all 361 positions, restrict candidates to a radius of 2 around existing stones. This alone reduces the branching factor by ~10× with no quality loss at depth 10+.
7. **Transposition table with Zobrist hashing.** If iterative deepening + candidate pruning still doesn't hit depth 10 in 500 ms, a transposition table will recover 2–4× speed. Implement Zobrist hashing on `GomokuBoard` (incremental update on place/remove) and cache `(hash, depth, score)` in a fixed-size table.

### Medium-term (polish & correctness)

8. **Timer in the UI sidebar is required.** Confirm the displayed AI time is per-move, not cumulative. The subject evaluator will check this.
9. **Heuristic calibration.** The current weights (attack ×10, defense ×14, center ×1.5) were chosen without benchmarking. Once Minimax is in, run self-play tests with different weight sets and pick the strongest.
10. **Cover more rules edge cases in tests.** `board_test.cpp` exists but the coverage of `Game` rules (endgame capture, double-three with capture exception, redo after win) is unknown. Add property-based or table-driven tests for the most subtle interactions.

### Long-term

11. **Bitboard representation for AI search.** The 2D array is correct and readable. If the Minimax search still exceeds 500 ms at depth 10 after the above optimizations, a bitboard (two `uint64_t[4]` for rows) would make pattern matching branchless and cache-friendly. This is an optimization, not a correctness change — do it last.
12. **Window resize support.** Currently fixed at compile time. A resize-aware layout pass would require propagating the new cell size to all renderers, but would make the UI more usable on different screen resolutions.

---

## Best practices to apply

- **Name magic numbers.** `WEIGHT_ATTACK = 10.0`, `WEIGHT_DEFENSE = 14.0`, `WEIGHT_CENTER = 1.5` are in `ai_engine.cpp` but not in `constants.hpp`. Move them there so they are visible in one place and can be tuned without reading the algorithm.
- **Use `[[nodiscard]]` on `makeMove` and `checkMoveValidity`.** Silently discarding the return value of `makeMove` (which carries the failure reason) is a common source of hard-to-find bugs.
- **Mark terminal `enum class` variants explicitly.** `GameStatus::DRAW` is defined but currently unreachable (Gomoku cannot draw under standard rules — the board can fill but captures prevent it). Either remove it or add a comment explaining the intent.
- **Avoid raw pointer members.** Currently the codebase uses references and value members correctly. Keep it that way — no raw `new`/`delete`.
- **CI / automated tests.** The Google Test suite is wired into the Makefile but is not run in a CI pipeline. Adding a simple GitHub Actions workflow (build + `make test`) would catch regressions automatically.

---

## Next steps (priority order)

| Priority | Task |
|----------|------|
| 1 | Implement Negamax + alpha-beta pruning |
| 2 | Add iterative deepening with time budget |
| 3 | Restrict candidate moves to radius-2 around existing stones |
| 4 | Verify AI move timer displays correctly in the sidebar |
| 5 | Add `[[nodiscard]]` to `makeMove` / `checkMoveValidity` |
| 6 | Unit tests for undo/redo across endgame capture state |
| 7 | Move AI weights to `constants.hpp` |
| 8 | Update `ROADMAP.md` decisions (Q3–Q14 are all resolved) |
| 9 | Transposition table + Zobrist hashing (if performance still insufficient) |
| 10 | CI pipeline for build + tests |
