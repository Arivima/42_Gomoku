# Compréhension du sujet
## Regles
- **Board** : Goban 19x19
- **Stones** : 
    - 2 colors black and white
    - no limit to the number of stones
- **Game** : Two players alternate turns placing one stone of their color on an empty intersection. 
- **Starting condition :**
    - Base : black plays first.
    - Bonus : Standard, Pro, Swap, Swap2
- **Play condition :**
    - capture : 
        - surrounding a pair in a line will capture the pair. 
        - The pair is removed from board and the player who captured gets 1 point per stone.
        - Capture is computed only during stone placement, cannot be retroactive.
- **End of game conditions :**
    - **Win condition :**
        - **5 in a row** : The winner is the first player to align five stones of their color in a row 3 patterns horizontally, vertically, or diagonally.
        - **5 or more** : An alignment of 5 or more stones is considered a win
        - **breakable alignment** : A 5-in-a-row wins only if the opponent cannot immediately break the line by capturing a pair from it. If they can, the game continues
        - **win by capture** : 10 stones captured from opponent is considered a win (5 pairs captured)
    - **Draw condition :**
        - If the board is completely filled and no one has made a line of 5 stones, then the game ends in a draw.

- **Illegal moves :**
    - **no double three** : 
        - no two free-three alignments
        - unless introduced by capture
        - 3 patterns horizontally, vertically, or diagonally.
    - **cannot move into capture**


**Additional context :**
- **Capture** (As in the Ninuki-renju or Pente variants) : 
    - You can remove a pair of your opponent’s stones from the board by flanking them with your own stones horizontally, vertically, diagonally. 
    - One can only capture PAIRS, not single stones, and not more than 2 stones in a row.
    - Active capture only, no traps possible : the pattern is not activated retroactively. BR.B: Red can play in a without losing the pair. However, if later Red takes one of the Blue stones, his position becomes vulnerable to capture.
    - This rule adds a win condition : If you manage to capture ten of your opponent’s stones, you win the game.
- **Endgame Capture**:
    - A player who manages to line up five stones wins only if the opponent cannot break this line by capturing a pair.
    - If the player has already lost four pairs and the opponent can capture one more, the opponent wins by capture.
    - If there is no possibility of this happening, there is no need to continue the game.
- **No double-threes** : 
    - It is forbidden to play a move that introduces two free-three alignments, which would guarantee a win by alignment.
    - A free-three is an alignement of three stones that, if not immediately blocked, allows for an indefendable alignment of four stones (that’s to say an alignment of four stones with two unobstructed extremities). examples : XX.X | .XXX.
    - A double-three is a move that introduces two simultaneous free-three alignments. You can’t block a double-three. 
    - it is not forbidden to introduce a double-three by capturing a pair.



## UI

  * **Keyboard shortcuts**
    * When are shortcut shown 
        | shortcut  | action                                       | HvH | HvH w/ Coach | HvAI |
        | ---       | ---                                          | --- | ---          | ---  |
        | R Restart | erase history and start a new game           | ✅  | ✅            | ✅   |
        | Q Quit    | exit the game gracefully                     | ✅  | ✅            | ✅   |
        | D Debug   |                                              | ✅  | ✅            | ✅   |
        | Z Undo*   | undo last (HvH) or last 2 (HvAI steps)       | ✅  | ✅            | ✅   |
        | A Redo*   | redo last (HvH) or last 2 (HvAI steps)       | ✅  | ✅            | ✅   |
        | C Coach*  | several bonus added - see below              | ✅  | ✅            | ❌   |
        | I Info*   | shows rules chosen on a pop up               | ✅  | ✅            | ✅   |
        | H History*| shows scrolling history                      | ✅  | ✅            | ✅   |
        * mode HvAI w/ Coach does not exist

    * Z Undo / A Redo
        - In Human vs AI, undo will undo both the human and AI stones at the same time, redo will redo the same human + AI moves. it does not compute the step again
        - In human vs human, undo/redo is one stone at a time


  * **Board**
    * 19x19 Gomoku board
    * Stones on intersection : Empty clickable, Occupied cells cannot be played
    * Captures : removed from board, become playable again, added to capture counters
    * Last move highlight on cell color (yellow border)
    * IF Human vs Human
        * Show top move suggestion on cell color (green): AI can suggest a move without automatically playing it

    * (bonus) IF AI coach option activated
        * Show coordinates
        * Show move history number on stones
        * Best moves - highlight 3 best moves in stone ring color (green) +  scores when passing the mouse on the cell
        * Threat visualization - highlight cells stone ring color (orange)
            * highlight open threes
            * highlight open fours
            * highlight capture threats
            * highlight vulnerable pairs
        * forbidden cell visualization - highlight cells color (red)
            * highlight Double-three illegal moves
            * suicide by capture
        * info cell visualization - hover on a cell
            * Coordinate
            * Legal or illegal
            * Reason if illegal
            * AI score eval

    * When are features shown 
        | Board features      | description                   | HvH      | HvH w/ Coach | HvAI |
        | ---                 | ---                           | ---      | ---          | ---  |
        | stone ring green    | top1/top3 AI move suggestion  | ✅ top 1  | ✅ top 3     | ❌   |
        | stone ring yellow   | last move                     | ✅        | ✅           | ✅   |
        | stone ring orange   | threats (4 types)             | ❌        | ✅           | ❌   |
        | stone ring red      | forbidden moves (2 types)     | ❌        | ✅           | ❌   |
        | board coordinates   | alpha numerical scale         | ❌        | ✅           | ❌   |
        | number on stone     | move history                  | ❌        | ✅           | ❌   |
        | cell info on hoover | info (4 types)                | ❌        | ✅           | ❌   |
        * mode HvAI w/ Coach does not exist


  * **Game info UI**
    * **Game start**
        * Choose game mode (human vs human, human vs AI)
            * IF Human vs AI : Choose who starts : AI or human
        * Choose opening rule : Standard, Pro, Swap, Swap2

    * **Game UI**
        * Show Game mode : Human vs AI, Human vs Human / hotseat
        * Show whose turn it is (Player 1 / Player 2 or Humain / AI) with the color of current player
            * IF Human vs AI, show stats
                * AI last timer (+ min/max/avg)
                * AI last depth search (+ min/max/avg)
        * Show if AI coach is active
        * Show if debug is active
        * Show keyboard keys

        * When are features shown 
            | Game UI features     | description                   | HvH | HvH w/ Coach | HvAI |
            | ---                  | ---                           | --- | ---          | ---  |
            | Game mode : HvH HvAI | Show game mode                | ✅  | ✅            | ✅   |
            | Opening rule         | Show active opening rules     | ✅  | ✅            | ✅   |
            | AI coach             | Show if coach is active       | ✅  | ✅            | ✅   |
            | Debug                | Show debug info               | ✅  | ✅            | ✅   |
            | Turn                 | Show current player           | ✅  | ✅            | ✅   |
            | Stats                | Show AI timer and depth       | ❌  | ❌            | ✅   |
            | Cell info            | Show info on cell.            | ❌  | ✅            | ❌   |
            * mode HvAI w/ Coach does not exist

    * **Pop ups**

        * IF Illegal move
            * POPUP Show error message : `Illegal move: double-three is forbidden.`
                * Cell already occupied
                * Move outside the board
                * Move creates a forbidden double-three
        * ELIF endgame capture rule activated
            * POPUP Show Player has aligned five stones but opponent may still break the line by capture.
        * ELIF Game over
            * POPUP Show the game is over
            * POPUP Show the winner
            * POPUP Show the win reason
                * Alignment of 5 or more stones
                * Capture victory

        * if I pressed, show Rules:
                * Board size: 19x19
                * Win condition: 5 or more
                * Captures (pairs) : enabled
                * Capture victory: 10 stones / 5 pairs
                * Double-three: forbidden
                * Endgame capture: enabled
        * if H pressed, display scrollable history with timestamp and events
            * timestamp, move number, player, coordinate, event (capture coordinates, 5 or more, game ending)
            * scrollable
        * if D pressed, show debug panel
         WIP
            * AI debug panel ? rerun eval on current board
            * AI eval panel
            * game state values


        * When are features shown 
            | Pop up events            | description                    | HvH | HvH w/ Coach | HvAI |
            | ---                      | ---                            | --- | ---          | ---  |
            | Illegal move (red)       | Show why illegal (3 types)     | ✅  | ✅            | ✅   |
            | Game over (red)          | Show game over, winner, reason | ✅  | ✅            | ✅   |
            | Endgame capture (orange) | Show warn rule activated       | ✅  | ✅            | ✅   |
            | Info / rules (blue)      | Show list of active rules      | ✅  | ✅            | ✅   |
            | History (blue)           | Show scrollable history        | ✅  | ✅            | ✅   |
            | Debug (purple)           | Show debug info                | ✅  | ✅            | ✅   |
            * mode HvAI w/ Coach does not exist

 

# Gomoku — Game Design Notes

---

## 1. Game Rules

### Board
- 19×19 Goban — stones are placed on **intersections**, not inside squares
- No limit to the total number of stones

### Turns
- 2 players: Black (Player 1) and White (Player 2)
- Players alternate placing **one stone** per turn on any empty intersection
- Black plays first by default (may be overridden by the opening rule)

---

### Win Conditions

| Condition | Description |
| --- | --- |
| **Alignment** | First to align **5 or more** stones horizontally, vertically, or diagonally |
| **Capture** | Capturing **10 opponent stones** (5 pairs) wins immediately |

> The alignment win is subject to the **Endgame Capture** rule (see below).

---

### Captures (Ninuki-renju / Pente variant)

- **How to capture**: flank a pair of opponent stones with your own along any line → `X O O X`
- Only **exactly 2** adjacent opponent stones can be captured — not 1, not 3+
- Captured stones are removed; their intersections become playable again immediately
- Capture is triggered **only when you place** a stone — never retroactively
- Capture counter: each captured pair = +1 point (displayed in the UI)

**Moving into a "would-be-captured" position is SAFE (not illegal):**
Placing your stone between two opponent stones (`O _ O → O X O`) does **not** get you captured.
However, if the opponent later removes an adjacent stone, your stone may become flanked and vulnerable.

---

### Endgame Capture

Triggered when a player aligns 5+ stones. Before declaring a win, check in order:

1. Can the opponent **immediately break the alignment** by capturing a pair from it?
   - **Yes** → game continues; the 5-in-a-row is not a win yet
2. Has the aligning player already lost **4 pairs** and can the opponent capture a 5th?
   - **Yes** → **opponent wins by capture**
3. Neither applies → **aligning player wins immediately**

---

### No Double-Threes (Forbidden Move)

| Term | Definition |
| --- | --- |
| **Free-three** | 3 aligned stones that, if unblocked, allow an indefendable 4 (4 stones with both ends open). Examples: `_ X X X _` or `_ X _ X X _` |
| **Double-three** | A single move that simultaneously creates **two free-threes** — cannot be blocked |
| **Prohibition** | Playing a double-three is **illegal** and rejected |
| **Exception** | A double-three introduced by **capturing a pair** is **legal** |

---

### Other Illegal Moves

| Illegal move | Reason |
| --- | --- |
| Occupied intersection | Cell already has a stone |
| Outside the board | Coordinates out of the 19×19 grid |
| Double-three | Creates two simultaneous free-threes (unless via capture) |

---

### Draw

If the board is completely filled with no winner (no 5+ alignment, no capture victory) → **draw**.

---

## 2. Opening Rules (Bonus)

| Rule | Description |
| --- | --- |
| **Standard** | No restrictions — Black plays first anywhere |
| **Pro** | Black's **second** stone must be placed **≥3 intersections** from the center |
| **Swap** | Black places 2 black + 1 white stone. White then **chooses which color** to play as |
| **Swap2** | Black places 3 stones (2B + 1W). White may: (a) choose a color, (b) place 2 more stones for Black to choose color, or (c) play as White |

---

## 3. Game Modes

| Mode | Description |
| --- | --- |
| **Human vs AI (HvAI)** | Player faces the AI. AI must win without the human letting it. Cannot use Coach mode. |
| **Human vs Human (HvH)** | Hotseat — two players on the same machine. AI provides move suggestions. |
| **HvH + Coach** | HvH with full AI coaching overlay enabled. |

> HvAI + Coach does not exist as a mode.

---

## 4. UI

### 4.1 Keyboard Shortcuts

| Key | Action | HvH | HvH+Coach | HvAI |
| --- | --- | --- | --- | --- |
| **R** — Restart | Erase history and start a new game | ✅ | ✅ | ✅ |
| **Q** — Quit | Exit the game gracefully | ✅ | ✅ | ✅ |
| **D** — Debug | Toggle debug panel | ✅ | ✅ | ✅ |
| **Z** — Undo | Undo 1 stone (HvH) or 1 full round / 2 stones (HvAI) | ✅ | ✅ | ✅ |
| **A** — Redo | Redo mirroring Undo behavior | ✅ | ✅ | ✅ |
| **C** — Coach | Toggle AI coach overlay | ✅ | ✅ | ❌ |
| **I** — Info | Show active rules in a popup | ✅ | ✅ | ✅ |
| **H** — History | Show scrollable move log | ✅ | ✅ | ✅ |

**Undo / Redo detail:**
- **HvH**: 1 stone per action
- **HvAI**: undoes/redoes the full Human + AI round (2 stones) together; does **not** recompute the AI move

---

### 4.2 Board Features

| Feature | Description | HvH | HvH+Coach | HvAI |
| --- | --- | --- | --- | --- |
| Stone ring — **yellow** | Last move highlight | ✅ | ✅ | ✅ |
| Stone ring — **green** | AI move suggestion: top-1 (HvH) or top-3 (Coach) | top 1 | top 3 | ❌ |
| Stone ring — **orange** | Threats: open threes, open fours, capture threats, vulnerable pairs | ❌ | ✅ | ❌ |
| Stone ring — **red** | Forbidden moves: double-three, suicide-by-capture | ❌ | ✅ | ❌ |
| Board coordinates | Alphanumeric scale (A–S / 1–19) | ❌ | ✅ | ❌ |
| Move numbers on stones | Order of placement history | ❌ | ✅ | ❌ |
| Cell info on hover | Coordinate, legal/illegal, reason, AI score eval | ❌ | ✅ | ❌ |

---

### 4.3 Side Panel (Game Info)

| Feature | Description | HvH | HvH+Coach | HvAI |
| --- | --- | --- | --- | --- |
| Game mode | HvH or HvAI label | ✅ | ✅ | ✅ |
| Opening rule | Active opening rule name | ✅ | ✅ | ✅ |
| Current player | Player name + stone color indicator | ✅ | ✅ | ✅ |
| Coach status | Whether AI coach is active | ✅ | ✅ | ✅ |
| Debug status | Whether debug mode is active | ✅ | ✅ | ✅ |
| Keyboard hints | Visible shortcut keys | ✅ | ✅ | ✅ |
| AI timer | Last move time + min/max/avg | ❌ | ❌ | ✅ |
| AI search depth | Last depth + min/max/avg | ❌ | ❌ | ✅ |
| Cell info | Coordinate, legality, AI eval (hover) | ❌ | ✅ | ❌ |

---

### 4.4 Pop-ups

| Pop-up | Trigger | Color | HvH | HvH+Coach | HvAI |
| --- | --- | --- | --- | --- | --- |
| **Illegal move** | Invalid placement attempt | Red | ✅ | ✅ | ✅ |
| **Endgame capture** | 5-in-a-row but opponent may break it | Orange | ✅ | ✅ | ✅ |
| **Game over** | Win or draw | Red | ✅ | ✅ | ✅ |
| **Info (I)** | Key pressed | Blue | ✅ | ✅ | ✅ |
| **History (H)** | Key pressed | Blue | ✅ | ✅ | ✅ |
| **Debug (D)** | Key pressed | Purple | ✅ | ✅ | ✅ |

**Illegal move popup** — shows one of:
- `Cell already occupied`
- `Move outside the board`
- `Illegal move: double-three is forbidden`

**Endgame capture popup** — warns:
- Player has aligned five stones but opponent may still break the line by capture

**Game over popup** — shows:
- Winner (Player 1 / Player 2 / Draw)
- Win reason: `Alignment of 5 or more stones` or `Capture victory (10 stones)`

**Info popup (I)** — shows active rules:
- Board size: 19×19
- Win condition: 5 or more
- Captures (pairs): enabled
- Capture victory: 10 stones / 5 pairs
- Double-three: forbidden
- Endgame capture: enabled

**History popup (H)** — scrollable log, one entry per move:
- Timestamp · Move number · Player · Coordinate · Event (capture coords / 5+ alignment / game end)

**Debug panel (D)** *(WIP)*:
- AI eval panel
- Rerun eval on current board
- Game state values
