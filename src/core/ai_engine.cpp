#include "core/ai_engine.hpp"
#include "core/constants.hpp"
#include <algorithm>
#include <cmath>

static constexpr int DIRS[4][2] = {{1,0},{0,1},{1,1},{1,-1}};

// Attack/defense weights — tweak when a real evaluator is added.
static constexpr float WEIGHT_ATTACK  = 10.f;
static constexpr float WEIGHT_DEFENSE = 14.f;
static constexpr float WEIGHT_CENTER  = 1.5f;

int AiEngine::countInLine(const GomokuBoard& board, Position pos,
                           int dx, int dy, Player player) const {
    Cell cell = playerToCell(player);
    int count = 0;
    for (int step = 1; step < Gomoku::WIN_LENGTH; ++step) {
        Position p{pos.x + dx * step, pos.y + dy * step};
        if (!p.isValid() || board.getCell(p) != cell) break;
        ++count;
    }
    for (int step = 1; step < Gomoku::WIN_LENGTH; ++step) {
        Position p{pos.x - dx * step, pos.y - dy * step};
        if (!p.isValid() || board.getCell(p) != cell) break;
        ++count;
    }
    return count;
}

float AiEngine::evalCell(const GomokuBoard& board, Position pos, Player aiPlayer) const {
    Player opp = (aiPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    float score = 0.f;

    for (auto& d : DIRS) {
        int own      = countInLine(board, pos, d[0], d[1], aiPlayer);
        int oppCount = countInLine(board, pos, d[0], d[1], opp);

        // Quadratic: each additional aligned stone is much more valuable.
        score += static_cast<float>(own * own) * WEIGHT_ATTACK;

        // Defense is weighted slightly higher to encourage blocking.
        score += static_cast<float>(oppCount * oppCount) * WEIGHT_DEFENSE;
    }

    // Prefer cells closer to the centre of the board.
    static constexpr float CENTER = (Gomoku::BOARD_SIZE - 1) / 2.f;
    float dx = pos.x - CENTER, dy = pos.y - CENTER;
    float dist = std::sqrt(dx * dx + dy * dy);
    score += std::max(0.f, CENTER - dist) * WEIGHT_CENTER;

    return score;
}

std::vector<std::pair<Position, float>>
AiEngine::rankMoves(const Game& game, Player aiPlayer, int topN) const {
    const GomokuBoard& board = game.getBoard();
    std::vector<std::pair<Position, float>> scored;
    scored.reserve(GomokuBoard::BOARD_SIZE * GomokuBoard::BOARD_SIZE);

    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            Position p{x, y};
            if (board.getCell(p) != Cell::EMPTY) continue;
            if (game.checkMoveValidity(p) != MoveFailReason::NONE) continue;
            scored.push_back({p, evalCell(board, p, aiPlayer)});
        }
    }

    std::sort(scored.begin(), scored.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    if (topN > 0 && static_cast<int>(scored.size()) > topN)
        scored.resize(static_cast<std::size_t>(topN));

    return scored;
}
