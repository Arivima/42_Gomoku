# Gomoku — Glossary & Concept Cheatsheet

Quick reference for every key concept in the 42 Gomoku project (subject v3.3).

---

## Game & Board

| Term | Definition |
|---|---|
| **Gomoku** | A strategy board game where two players compete to align stones. Also known as "Five in a Row". |
| **Goban** | The 19x19 grid board on which the game is played. Stones are placed on **intersections**, not inside squares. |
| **Stone** | A game piece. Each player has their own color (typically black and white). There is no limit to the number of stones. |
| **Intersection** | A point where two lines of the Goban cross. This is where stones are placed. |
| **Move** | Placing a single stone on an empty intersection. Players alternate turns. |

---

## Win Conditions

| Condition | Description |
|---|---|
| **Alignment win** | A player aligns **5 or more** stones in a row (horizontal, vertical, or diagonal). Subject to the *endgame capture* rule. |
| **Capture win** | A player captures **10 opponent stones** (5 pairs). This is an alternative win condition introduced by the capture rule. |

---

## Captures (Ninuki-renju / Pente Variant)

| Term | Definition |
|---|---|
| **Pair capture** | Removing exactly **2** opponent stones that are adjacent in a line by placing your stone so that your stones **flank** them on both ends. Pattern: `X O O X` — placing the second `X` captures the two `O` stones. |
| **Flanking** | Placing a stone so that a pair of opponent stones is enclosed between two of your stones along a line. |
| **Freed intersection** | After a capture, the two intersections become empty and can be played on as if they were never occupied. |
| **Move-into-capture (safe)** | Placing your stone **between** two opponent stones does **not** result in your own capture. Example: `O _ O` — playing `X` in the middle is safe. However, if the opponent later removes an adjacent stone, the position may become vulnerable. |
| **Only pairs** | You can only capture exactly 2 stones in a row. Single stones and groups of 3+ cannot be captured. |

---

## Endgame Capture

These rules modify the alignment win condition:

| Rule | Description |
|---|---|
| **Breakable alignment** | A 5-in-a-row wins **only if** the opponent cannot immediately break the line by capturing a pair from it. If they can, the game continues. |
| **4-pair vulnerability** | If the aligning player has already lost 4 pairs (8 stones) and the opponent can capture a 5th pair, the **opponent wins by capture** instead. |
| **Immediate end** | If neither of the above applies (alignment is unbreakable and no 5th-pair capture is possible), the game ends immediately in favor of the aligning player. |

---

## Free-Threes & Double-Threes

| Term | Definition |
|---|---|
| **Free-three** | An alignment of 3 stones that, if not immediately blocked, leads to an **indefendable four** — 4 stones in a row with both ends open. Can include a gap (e.g., `X _ X X`). |
| **Open/unobstructed extremity** | An end of an alignment that is not blocked by an opponent stone or the board edge. A free-three requires that the resulting four would have **two** open ends. |
| **Double-three** | A single move that creates **two free-threes simultaneously**. This guarantees a win because the opponent cannot block both. |
| **Double-three prohibition** | It is **forbidden** to play a move that introduces a double-three. The move is simply illegal and cannot be made. |
| **Capture exception** | It is **not** forbidden to introduce a double-three if the move also captures a pair. Captures override the prohibition. |

---

## AI Concepts

| Term | Definition |
|---|---|
| **Min-Max (Minimax)** | A decision-making algorithm for two-player zero-sum games. It explores a tree of possible moves, alternating between maximizing (AI's turn) and minimizing (opponent's turn) the evaluated score. |
| **Alpha-Beta pruning** | An optimization of Minimax that skips branches of the game tree that cannot influence the final decision. Dramatically reduces the number of nodes evaluated. |
| **Negamax** | A variant of Minimax that simplifies implementation by negating scores at each level instead of alternating between max and min. Functionally equivalent. |
| **Game tree** | The tree of all possible move sequences from the current board state. Each node is a board position; each edge is a move. |
| **Search depth** | How many moves ahead the AI looks. The subject requires **at least 10 levels deep**. Lower depth = lower grade. |
| **Terminal node (leaf)** | A node at the maximum search depth (or a game-ending state). Its value is determined by the heuristic function. |
| **Heuristic function** | A function that evaluates a board position and returns a numerical score estimating how favorable it is for the AI. This is the **hardest and most critical part** of the project. Must be accurate, fast, and explainable. |
| **Move generation** | The process of listing all legal candidate moves from a given board state. Efficiency here directly impacts AI speed. |
| **Possible-solution tree** | The game tree as explored by the AI. The AI generates this tree and picks the best move according to the evaluated scores. |

---

## Performance Constraints

| Constraint | Requirement |
|---|---|
| **Time limit** | The AI must take **< 500ms on average** per move. Exceeding this fails the project. |
| **Minimum depth** | The game tree must be searched **at least 10 levels deep**. Lower depth prevents reaching maximum grade. |
| **Heuristic speed** | The heuristic function runs at every terminal node — it must be lightweight with minimal heap allocations. |

---

## Game Modes

| Mode | Description |
|---|---|
| **Human vs AI** | The primary mode. The AI must be capable of winning without the human intentionally losing. It must adapt to the player's moves. |
| **Human vs Human (Hotseat)** | Two players on the same computer, taking turns. Must include a **move-suggestion** feature powered by the AI. |
| **Move suggestion** | In hotseat mode, the AI suggests the best move for the current player. Leverages the same AI engine used in Human vs AI mode. |

---

## UI & Debugging

| Term | Definition |
|---|---|
| **Graphical interface** | A mandatory visual interface (graphical or ncurses-like) that allows playing the game. Must be "at least vaguely pleasing to the eye" and easily playable. |
| **AI timer** | A visible timer in the UI showing how long the AI takes to compute each move. **No timer = no project validation.** |
| **AI debugging/visualization** | A way to inspect the AI's reasoning process at runtime (e.g., evaluated scores, explored branches, chosen path). Required for development and expected during defense. |

---

## Build & Delivery

| Term | Definition |
|---|---|
| **Executable name** | Must be `Gomoku` (case-sensitive). |
| **Makefile** | Required build system. Must not relink. |
| **`all` / `$(NAME)`** | Builds the executable. |
| **`clean`** | Removes object files. |
| **`fclean`** | Removes object files and the executable. |
| **`re`** | Full rebuild (`fclean` + `all`). |
| **No-crash rule** | The program must **never** crash or quit unexpectedly, including on out-of-memory. Any crash = grade 0. |

---

## Bonus Concepts

| Term | Definition |
|---|---|
| **Rule selection** | Ability to choose which game rules apply at the start of a match. |
| **Opening rules** | Predefined starting conditions to make the game fairer. Examples below. |
| **Standard** | Default opening — no restrictions on the first moves. |
| **Pro** | First player's second move must be placed at least 3 intersections from the center. |
| **Swap** | After the first player places 3 stones (2 black, 1 white), the second player chooses which color to play. |
| **Swap2** | Extended swap: after 3 initial stones, the second player can choose color, place 2 more stones for the first player to choose, or simply play. |

---

## Defense Keywords

These are things the evaluator will specifically test:

- **Explain Minimax** — You must be able to explain your algorithm in detail to someone who knows nothing about it.
- **Explain heuristic** — You must articulate why your heuristic works, what it measures, and how it scores positions.
- **Rule correctness** — Captures, endgame capture, double-three prohibition must all be demonstrably correct.
- **Live play** — The AI plays against the evaluator. It should not get "completely wiped out."
