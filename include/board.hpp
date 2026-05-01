#pragma once

#include <array>
#include <cstdint>
#include <optional>

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

    bool isValid() const { return x >= 0 && x < 19 && y >= 0 && y < 19; }
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

class GomokuBoard {
public:
    static constexpr int BOARD_SIZE = 19;

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

    int getMoveCount() const { return m_moveCount; }

    bool isFull() const;

private:
    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> m_board;
    Player m_currentPlayer;
    int m_blackCaptures;
    int m_whiteCaptures;
    int m_moveCount;
};

inline Cell playerToCell(Player player) {
    return static_cast<Cell>(player);
}

inline Player cellToPlayer(Cell cell) {
    return static_cast<Player>(cell);
}
