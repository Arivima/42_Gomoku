#pragma once

#include "core/board.hpp"
#include "core/game.hpp"
#include <optional>
#include <string>
#include <vector>

struct UIState {
    enum class OverlayKind { LAST_MOVE, FORBIDDEN, THREAT, COACH_SUGGESTION };

    struct CellOverlay {
        Position pos;
        OverlayKind kind;
    };

    struct AiStats {
        float lastMs = 0, minMs = 0, maxMs = 0, avgMs = 0;
        int lastDepth = 0, minDepth = 0, maxDepth = 0, avgDepth = 0;
    };

    struct CellInfo {
        std::string coordLabel;
        bool isLegal = true;
        std::string illegalReason;
        std::optional<float> aiScore;
    };

    enum class ActivePopup { NONE, ILLEGAL_MOVE, ENDGAME_CAPTURE, GAME_OVER, RULES, HISTORY };

    struct MoveHistoryEntry {
        int         num;
        std::string timestamp;   // "YYYYMMDD HH:MM"
        std::string playerLabel; // "Player 1", "Player 2", "Human", "AI"
        std::string color;       // "Black" / "White"
        std::string coord;       // "H12"
        std::string event;       // "" / "captures 2" / "wins by alignment" / ...
    };

    // Board overlays
    std::vector<CellOverlay> overlays;
    std::optional<Position> hoverPos;

    // Panel info
    GameMode gameMode = GameMode::HUMAN_VS_HUMAN;
    OpeningRule openingRule = OpeningRule::STANDARD;
    bool coachEnabled = false;
    bool debugEnabled = false;
    Player currentPlayer = Player::BLACK;
    int blackCaptures = 0;
    int whiteCaptures = 0;
    std::optional<AiStats> aiStats;

    std::string turnLabel;
    bool isGameOver = false;
    std::optional<std::string> warningMessage;
    std::optional<CellInfo> hoveredCellInfo;

    // Player assignment (used by Rules popup)
    Player humanColor = Player::BLACK;

    // History popup data
    std::vector<MoveHistoryEntry> moveHistory;
    int historyScrollOffset = 0;

    // Popups
    ActivePopup activePopup = ActivePopup::NONE;
    std::string popupMessage;
};
