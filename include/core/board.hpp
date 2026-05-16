#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "core/constants.hpp"

enum class Player : std::uint8_t {
    NONE = 0,
    BLACK = 1,
    WHITE = 2
};

enum class Cell : std::uint8_t {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

struct Position {
    int x;
    int y;

    bool isValid() const {
        return x >= 0 && x < Gomoku::BOARD_SIZE && y >= 0 && y < Gomoku::BOARD_SIZE;
    }
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

class GomokuBoard {
public:
    static constexpr int BOARD_SIZE = Gomoku::BOARD_SIZE;

    GomokuBoard();

    Cell getCell(int x, int y) const;
    Cell getCell(Position pos) const;

    bool placeStone(int x, int y, Player player);
    bool placeStone(Position pos, Player player);

    bool removeStone(int x, int y);
    bool removeStone(Position pos);

    void clear();

    Player getCurrentPlayer() const { return m_currentPlayer; }
    void switchPlayer() { m_currentPlayer = (m_currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK; }

    int getBlackCaptures() const { return m_blackCaptures; }
    int getWhiteCaptures() const { return m_whiteCaptures; }
    void addCapture(Player player, int count);
    void setCaptures(Player player, int count);

    // Capture-specific: bypass move history (captured stones are not player moves)
    void removeCapturedStone(Position pos);
    void restoreCapturedStone(Position pos, Player player);

    int getMoveCount() const { return m_moveCount; }

    bool isFull() const;

    std::optional<Position> getLastMove() const;
    int getMoveNumber(Position pos) const;
    const std::vector<Position>& getMoveHistory() const { return m_moveHistory; }

private:
    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> m_board;
    Player m_currentPlayer;
    int m_blackCaptures;
    int m_whiteCaptures;
    int m_moveCount;
    std::vector<Position> m_moveHistory;
};

inline Cell playerToCell(Player player) {
    return static_cast<Cell>(player);
}

inline Player cellToPlayer(Cell cell) {
    return static_cast<Player>(cell);
}
