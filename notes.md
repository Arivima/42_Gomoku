Notes arielle
- revoir makefile explications




# Compréhension du sujet
## Termes
- Gomoku
- Goban
- Stone
- Intersection
- Move


## Regles
- **Board** : Goban 19x19
- **Stones** : 
    - 2 colors black and white
    - no limit to the number of stones
- **Game** : Two players alternate turns placing one stone of their color on an empty intersection. 
- **Starting condition:**
    - Base : black plays first.
    - Bonus : Standard, Pro, Swap, Swap2
- **End of game conditions :**
    - **Win condition :**
        - **5 in a row** : The winner is the first player to align five stones of their color in a row horizontally, vertically, or diagonally.
        - **5 or more** : An alignment of 5 or more stones is considered a win
        - **breakable alignment** : A 5-in-a-row wins only if the opponent cannot immediately break the line by capturing a pair from it. If they can, the game continues
        - **win by capture** : 10 stones captured from opponent is considered a win (5 pairs captured)
    - **Draw condition :**
        - If the board is completely filled and no one has made a line of 5 stones, then the game ends in a draw.

- **Illegal moves :**
    - **no double three** : no two free-three alignments, unless introduced by capture


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


