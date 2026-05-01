#include "board.hpp"

GomokuBoard::GomokuBoard()
    : m_currentPlayer(Player::BLACK)
    , m_blackCaptures(0)
    , m_whiteCaptures(0)
    , m_moveCount(0) {
    m_board.fill(std::array<Cell, BOARD_SIZE>{});
    for (auto& row : m_board) {
        row.fill(Cell::EMPTY);
    }
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

bool GomokuBoard::isFull() const {
    return m_moveCount >= BOARD_SIZE * BOARD_SIZE;
}
