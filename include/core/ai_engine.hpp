#pragma once

#include "core/board.hpp"
#include "core/game.hpp"
#include <utility>
#include <vector>

// Stub AI engine with a depth-1 positional heuristic.
// Replace evalCell() with a real minimax/alpha-beta later.
class AiEngine {
public:
    // Score of placing a stone at pos for aiPlayer (higher = better).
    // Does NOT place the stone — pure board-read, safe to call any time.
    float evalCell(const GomokuBoard& board, Position pos, Player aiPlayer) const;

    // Return the top `topN` legal moves for aiPlayer, sorted best-first.
    // Skips illegal positions (occupied, out-of-bounds, double-three).
    std::vector<std::pair<Position, float>> rankMoves(const Game& game, Player aiPlayer, int topN) const;

private:
    // Count consecutive stones of `player` through pos in direction ±(dx,dy),
    // not counting pos itself (assumes pos is not yet occupied).
    int countInLine(const GomokuBoard& board, Position pos,
                    int dx, int dy, Player player) const;
};
