#pragma once

#include "board.hpp"
#include <optional>
#include <vector>

enum class GameMode {
    HUMAN_VS_AI,
    HUMAN_VS_HUMAN
};

enum class GameStatus {
    PLAYING,
    BLACK_WINS,
    WHITE_WINS,
    DRAW
};

struct WinInfo {
    Player winner;
    Position startPos;
    Position endPos;
    std::vector<Position> stones;
};

class Game {
public:
    Game();

    bool makeMove(Position pos);
    bool makeMove(int x, int y);

    GomokuBoard& getBoard() { return m_board; }
    const GomokuBoard& getBoard() const { return m_board; }

    Player getCurrentPlayer() const { return m_board.getCurrentPlayer(); }
    void switchPlayer() { m_board.switchPlayer(); }

    GameStatus getStatus() const { return m_status; }
    bool isGameOver() const { return m_status != GameStatus::PLAYING; }

    const std::optional<WinInfo>& getWinInfo() const { return m_winInfo; }

    void reset();

private:
    GomokuBoard m_board;
    GameStatus m_status;
    std::optional<WinInfo> m_winInfo;
    Position m_lastMove;

    bool checkWin(Player player);
    bool checkWinDirection(Player player, int x, int y, int dx, int dy, std::vector<Position>& stones);
    bool isValidPosition(int x, int y) const;
};
