#include "game.hpp"

Game::Game()
    : m_status(GameStatus::PLAYING) {
}

bool Game::makeMove(Position pos) {
    return makeMove(pos.x, pos.y);
}

bool Game::makeMove(int x, int y) {
    if (isGameOver()) return false;

    if (!isValidPosition(x, y)) return false;
    if (m_board.getCell(x, y) != Cell::EMPTY) return false;

    Player currentPlayer = m_board.getCurrentPlayer();
    if (!m_board.placeStone(x, y, currentPlayer)) return false;

    m_lastMove = {x, y};

    if (checkWin(currentPlayer)) {
        m_status = (currentPlayer == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
    } else if (m_board.isFull()) {
        m_status = GameStatus::DRAW;
    } else {
        m_board.switchPlayer();
    }

    return true;
}

void Game::reset() {
    m_board.clear();
    m_status = GameStatus::PLAYING;
    m_winInfo.reset();
    m_lastMove = {-1, -1};
}

bool Game::checkWin(Player player) {
    static const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            if (m_board.getCell(x, y) != playerToCell(player)) continue;

            for (auto& dir : directions) {
                std::vector<Position> stones;
                if (checkWinDirection(player, x, y, dir[0], dir[1], stones)) {
                    if (stones.size() >= 5) {
                        m_winInfo = WinInfo{
                            player,
                            stones.front(),
                            stones.back(),
                            stones
                        };
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Game::checkWinDirection(Player player, int x, int y, int dx, int dy, std::vector<Position>& stones) {
    stones.clear();
    Cell playerCell = playerToCell(player);

    int cx = x;
    int cy = y;
    while (isValidPosition(cx, cy) && m_board.getCell(cx, cy) == playerCell) {
        stones.push_back({cx, cy});
        cx += dx;
        cy += dy;
    }

    return stones.size() >= 5;
}

bool Game::isValidPosition(int x, int y) const {
    return x >= 0 && x < GomokuBoard::BOARD_SIZE && y >= 0 && y < GomokuBoard::BOARD_SIZE;
}
