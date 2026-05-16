#pragma once

#include "core/board.hpp"
#include <chrono>
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

enum class OpeningRule {
    STANDARD,
    PRO,
    SWAP,
    SWAP2
};

enum class MoveFailReason {
    NONE,
    OCCUPIED,
    OUT_OF_BOUNDS,
    DOUBLE_THREE
};

struct MoveResult {
    bool success;
    MoveFailReason failReason = MoveFailReason::NONE;
    bool captureOccurred = false;
    bool endgameCaptureActive = false;
};

struct WinInfo {
    Player winner;
    Position startPos;
    Position endPos;
    std::vector<Position> stones;
    bool byCaptureWin = false;
};

class Game {
public:
    Game();

    MoveResult makeMove(Position pos);

    GomokuBoard& getBoard() { return m_board; }
    const GomokuBoard& getBoard() const { return m_board; }

    Player getCurrentPlayer() const { return m_board.getCurrentPlayer(); }

    GameStatus getStatus() const { return m_status; }
    bool isGameOver() const { return m_status != GameStatus::PLAYING; }

    const std::optional<WinInfo>& getWinInfo() const { return m_winInfo; }

    GameMode getGameMode() const { return m_gameMode; }
    void setGameMode(GameMode mode) { m_gameMode = mode; }

    OpeningRule getOpeningRule() const { return m_openingRule; }
    void setOpeningRule(OpeningRule rule) { m_openingRule = rule; }

    bool undoMove();
    bool redoMove();

    MoveFailReason checkMoveValidity(Position pos) const;

    // Public move history entry (exposed for UI/history popup)
    struct MoveEntry {
        int       moveNumber;
        Position  pos;
        Player    player;
        int       capturedCount;
        std::chrono::system_clock::time_point timestamp;
    };
    std::vector<MoveEntry> getMoveHistory() const;

    // Returns positions of stones that form dangerous patterns for `perspective`:
    // opponent's open-threes and open-fours, plus perspective's immediately
    // vulnerable pairs (opponent stone on one flank, empty on the other).
    std::vector<Position> getThreatCells(Player perspective) const;

    void reset();

private:
    // Full context of one move, used for undo/redo
    struct MoveRecord {
        Position pos;
        Player player;
        std::vector<Position> captured;  // opponent stones removed this move
        int capturesBefore;              // player's capture count before this move
        // Endgame state snapshot so undo can restore it exactly
        bool endgamePendingBefore = false;
        Player endgameWinnerBefore = Player::NONE;
        WinInfo endgameWinInfoBefore;
        std::chrono::system_clock::time_point timestamp;
    };

    GomokuBoard m_board;
    GameStatus m_status;
    std::optional<WinInfo> m_winInfo;
    GameMode m_gameMode = GameMode::HUMAN_VS_HUMAN;
    OpeningRule m_openingRule = OpeningRule::STANDARD;
    std::vector<MoveRecord> m_undoStack;
    std::vector<MoveRecord> m_redoStack;

    // Endgame capture state: set when a player aligns 5 but opponent may break it
    bool m_endgamePending = false;
    Player m_endgameWinner = Player::NONE;
    WinInfo m_endgameWinInfo;

    std::vector<Position> detectCaptures(Position pos, Player player) const;
    void applyCaptures(Player player, const std::vector<Position>& captured);
    bool checkCaptureWin(Player player);
    bool checkWin(Player player);
    bool checkWinDirection(Player player, int x, int y, int dx, int dy, std::vector<Position>& stones);
    bool isValidPosition(int x, int y) const;
    bool canBreakByCapture(Player opponent, const WinInfo& win) const;
    bool captureWouldBreak(const WinInfo& win, const std::vector<Position>& pair) const;
    int  countFreeThreesAt(Position pos, Player player) const;
};
