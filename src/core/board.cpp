#include "core/board.hpp"

GomokuBoard::GomokuBoard()
    : m_currentPlayer(Player::BLACK)
    , m_blackCaptures(0)
    , m_whiteCaptures(0)
    , m_moveCount(0) {
    for (auto& row : m_board)
        row.fill(Cell::EMPTY);
}

Cell GomokuBoard::getCell(int x, int y) const {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return Cell::EMPTY;
    }
    return m_board[y][x];
}

Cell GomokuBoard::getCell(Position pos) const {
    return getCell(pos.x, pos.y);
}

bool GomokuBoard::placeStone(int x, int y, Player player) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false;
    }
    if (m_board[y][x] != Cell::EMPTY) {
        return false;
    }
    m_board[y][x] = static_cast<Cell>(player);
    m_moveCount++;
    m_moveHistory.push_back({x, y});
    return true;
}

bool GomokuBoard::placeStone(Position pos, Player player) {
    return placeStone(pos.x, pos.y, player);
}

bool GomokuBoard::removeStone(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false;
    }
    if (m_board[y][x] == Cell::EMPTY) {
        return false;
    }
    m_board[y][x] = Cell::EMPTY;
    m_moveCount--;
    if (!m_moveHistory.empty() && m_moveHistory.back() == Position{x, y}) {
        m_moveHistory.pop_back();
    }
    return true;
}

bool GomokuBoard::removeStone(Position pos) {
    return removeStone(pos.x, pos.y);
}

void GomokuBoard::clear() {
    for (auto& row : m_board) {
        row.fill(Cell::EMPTY);
    }
    m_currentPlayer = Player::BLACK;
    m_blackCaptures = 0;
    m_whiteCaptures = 0;
    m_moveCount = 0;
    m_moveHistory.clear();
}

void GomokuBoard::addCapture(Player player, int count) {
    if (player == Player::BLACK) {
        m_blackCaptures += count;
    } else if (player == Player::WHITE) {
        m_whiteCaptures += count;
    }
}

void GomokuBoard::setCaptures(Player player, int count) {
    if (player == Player::BLACK) {
        m_blackCaptures = count;
    } else if (player == Player::WHITE) {
        m_whiteCaptures = count;
    }
}

void GomokuBoard::removeCapturedStone(Position pos) {
    if (!pos.isValid()) return;
    if (m_board[pos.y][pos.x] == Cell::EMPTY) return;
    m_board[pos.y][pos.x] = Cell::EMPTY;
    m_moveCount--;
}

void GomokuBoard::restoreCapturedStone(Position pos, Player player) {
    if (!pos.isValid()) return;
    if (m_board[pos.y][pos.x] != Cell::EMPTY) return;
    m_board[pos.y][pos.x] = playerToCell(player);
    m_moveCount++;
}

bool GomokuBoard::isFull() const {
    return m_moveCount >= BOARD_SIZE * BOARD_SIZE;
}

std::optional<Position> GomokuBoard::getLastMove() const {
    if (m_moveHistory.empty()) return std::nullopt;
    return m_moveHistory.back();
}

int GomokuBoard::getMoveNumber(Position pos) const {
    for (int i = 0; i < static_cast<int>(m_moveHistory.size()); ++i) {
        if (m_moveHistory[i] == pos) return i + 1;
    }
    return 0;
}
