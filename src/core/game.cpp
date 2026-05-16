#include "core/game.hpp"
#include "core/constants.hpp"
#include <algorithm>
#include <chrono>

Game::Game()
    : m_status(GameStatus::PLAYING) {
}

MoveResult Game::makeMove(Position pos) {
    if (isGameOver()) return {false, MoveFailReason::NONE};

    if (!isValidPosition(pos.x, pos.y))
        return {false, MoveFailReason::OUT_OF_BOUNDS};

    if (m_board.getCell(pos) != Cell::EMPTY)
        return {false, MoveFailReason::OCCUPIED};

    Player currentPlayer = m_board.getCurrentPlayer();

    // Double-three check: place tentatively to evaluate the resulting position,
    // then reject if it creates 2+ free-threes without any capture (the capture
    // exception allows a capture-induced double-three).
    m_board.placeStone(pos, currentPlayer);
    bool wouldCapture = !detectCaptures(pos, currentPlayer).empty();
    if (!wouldCapture && countFreeThreesAt(pos, currentPlayer) >= 2) {
        m_board.removeStone(pos);
        return {false, MoveFailReason::DOUBLE_THREE};
    }
    m_board.removeStone(pos);

    int capturesBefore = (currentPlayer == Player::BLACK)
        ? m_board.getBlackCaptures()
        : m_board.getWhiteCaptures();

    // Snapshot endgame state before modifying anything (needed for undo)
    MoveRecord record{pos, currentPlayer, {}, capturesBefore,
                      m_endgamePending, m_endgameWinner, m_endgameWinInfo,
                      std::chrono::system_clock::now()};

    m_board.placeStone(pos, currentPlayer);
    m_redoStack.clear();

    MoveResult result{true};

    record.captured = detectCaptures(pos, currentPlayer);
    applyCaptures(currentPlayer, record.captured);
    result.captureOccurred = !record.captured.empty();

    m_undoStack.push_back(record);

    // ── Endgame response path ────────────────────────────────────────────────
    // The pending winner aligned 5 last turn; now the opponent responds.
    if (record.endgamePendingBefore) {
        m_endgamePending = false;
        m_endgameWinner  = Player::NONE;

        Player pendingWinner = record.endgameWinnerBefore;

        // Responder wins by capture first (reaching 10)?
        if (checkCaptureWin(currentPlayer)) {
            m_status = (currentPlayer == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
            return result;
        }

        // Does the pending winner still have a 5-in-a-row on the updated board?
        if (checkWin(pendingWinner)) {
            m_status = (pendingWinner == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
            return result;
        }

        // Alignment was broken — game continues.
        // Check if the responder themselves won by alignment.
        if (checkWin(currentPlayer)) {
            // Could itself be another endgame capture situation, handled below.
            Player opp = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
            if (canBreakByCapture(opp, *m_winInfo)) {
                m_endgamePending = true;
                m_endgameWinner  = currentPlayer;
                m_endgameWinInfo = *m_winInfo;
                m_winInfo.reset();
                m_board.switchPlayer();
                result.endgameCaptureActive = true;
            } else {
                m_status = (currentPlayer == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
            }
        } else if (m_board.isFull()) {
            m_status = GameStatus::DRAW;
        } else {
            m_board.switchPlayer();
        }
        return result;
    }

    // ── Normal move path ─────────────────────────────────────────────────────
    if (checkCaptureWin(currentPlayer)) {
        m_status = (currentPlayer == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
    } else if (checkWin(currentPlayer)) {
        Player opponent = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
        if (canBreakByCapture(opponent, *m_winInfo)) {
            // Opponent gets a chance to break the alignment next turn
            m_endgamePending = true;
            m_endgameWinner  = currentPlayer;
            m_endgameWinInfo = *m_winInfo;
            m_winInfo.reset();
            m_board.switchPlayer();
            result.endgameCaptureActive = true;
        } else {
            m_status = (currentPlayer == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
        }
    } else if (m_board.isFull()) {
        m_status = GameStatus::DRAW;
    } else {
        m_board.switchPlayer();
    }

    return result;
}

bool Game::undoMove() {
    if (m_undoStack.empty()) return false;

    bool wasOver = (m_status != GameStatus::PLAYING);
    if (wasOver) {
        m_status = GameStatus::PLAYING;
        m_winInfo.reset();
    }

    auto record = m_undoStack.back();
    m_undoStack.pop_back();

    // Restore endgame state exactly as it was before this move
    m_endgamePending = record.endgamePendingBefore;
    m_endgameWinner  = record.endgameWinnerBefore;
    m_endgameWinInfo = record.endgameWinInfoBefore;

    // switchPlayer was only called when the game continued after the move
    if (!wasOver) m_board.switchPlayer();

    // Restore captured opponent stones and rewind capture count
    if (!record.captured.empty()) {
        Player opponent = (record.player == Player::BLACK) ? Player::WHITE : Player::BLACK;
        for (const auto& capturedPos : record.captured)
            m_board.restoreCapturedStone(capturedPos, opponent);
        m_board.setCaptures(record.player, record.capturesBefore);
    }

    m_board.removeStone(record.pos);
    m_redoStack.push_back(record);
    return true;
}

bool Game::redoMove() {
    if (m_redoStack.empty()) return false;

    auto record = m_redoStack.back();
    m_redoStack.pop_back();

    m_board.placeStone(record.pos, record.player);
    applyCaptures(record.player, record.captured);
    m_undoStack.push_back(record);

    if (checkCaptureWin(record.player)) {
        m_endgamePending = false;
        m_status = (record.player == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
    } else if (checkWin(record.player)) {
        Player opponent = (record.player == Player::BLACK) ? Player::WHITE : Player::BLACK;
        if (canBreakByCapture(opponent, *m_winInfo)) {
            m_endgamePending = true;
            m_endgameWinner  = record.player;
            m_endgameWinInfo = *m_winInfo;
            m_winInfo.reset();
            m_board.switchPlayer();
        } else {
            m_endgamePending = false;
            m_status = (record.player == Player::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
        }
    } else if (m_board.isFull()) {
        m_endgamePending = false;
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
    m_undoStack.clear();
    m_redoStack.clear();
    m_endgamePending = false;
    m_endgameWinner  = Player::NONE;
}

// Returns the positions of all opponent stones captured by placing pos for player.
// Pattern: player [opp opp] player — the newly placed stone is one flanking stone.
// Deduplicates in case a stone is flanked in two crossing directions simultaneously.
std::vector<Position> Game::detectCaptures(Position pos, Player player) const {
    static const int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    Player opponent = (player == Player::BLACK) ? Player::WHITE : Player::BLACK;
    Cell oppCell    = playerToCell(opponent);
    Cell ownCell    = playerToCell(player);

    std::vector<Position> captured;

    for (auto& d : dirs) {
        for (int sign : {1, -1}) {
            int dx = d[0] * sign;
            int dy = d[1] * sign;
            Position p1 = {pos.x + dx,   pos.y + dy};
            Position p2 = {pos.x + 2*dx, pos.y + 2*dy};
            Position p3 = {pos.x + 3*dx, pos.y + 3*dy};

            if (!p1.isValid() || !p2.isValid() || !p3.isValid()) continue;
            if (m_board.getCell(p1) == oppCell &&
                m_board.getCell(p2) == oppCell &&
                m_board.getCell(p3) == ownCell) {
                captured.push_back(p1);
                captured.push_back(p2);
            }
        }
    }

    // Deduplicate: a stone may be flanked in two crossing directions simultaneously
    std::sort(captured.begin(), captured.end(),
              [](const Position& a, const Position& b) {
                  return a.x != b.x ? a.x < b.x : a.y < b.y;
              });
    captured.erase(std::unique(captured.begin(), captured.end()), captured.end());

    return captured;
}

void Game::applyCaptures(Player player, const std::vector<Position>& captured) {
    if (captured.empty()) return;
    for (const auto& capturedPos : captured)
        m_board.removeCapturedStone(capturedPos);
    m_board.addCapture(player, static_cast<int>(captured.size()));
}

bool Game::checkCaptureWin(Player player) {
    int captures = (player == Player::BLACK)
        ? m_board.getBlackCaptures()
        : m_board.getWhiteCaptures();
    if (captures >= Gomoku::CAPTURE_WIN) {
        m_winInfo = WinInfo{player, {-1,-1}, {-1,-1}, {}, true};
        return true;
    }
    return false;
}

// Checks whether the opponent can immediately place one stone that would capture
// a pair from the winning line AND leave fewer than 5 consecutive stones.
bool Game::canBreakByCapture(Player opponent, const WinInfo& win) const {
    if (win.stones.size() < 2) return false;

    int dx = win.stones[1].x - win.stones[0].x;
    int dy = win.stones[1].y - win.stones[0].y;
    Cell oppCell = playerToCell(opponent);

    for (size_t i = 0; i + 1 < win.stones.size(); ++i) {
        const Position& a = win.stones[i];
        const Position& b = win.stones[i + 1];
        if (b.x - a.x != dx || b.y - a.y != dy) continue; // non-adjacent (safety)

        Position before = {a.x - dx, a.y - dy};
        Position after  = {b.x + dx, b.y + dy};

        if (!before.isValid() || !after.isValid()) continue;

        bool p1 = (m_board.getCell(before) == oppCell && m_board.getCell(after)  == Cell::EMPTY);
        bool p2 = (m_board.getCell(after)  == oppCell && m_board.getCell(before) == Cell::EMPTY);
        if ((p1 || p2) && captureWouldBreak(win, {a, b})) return true;
    }
    return false;
}

// Returns true if removing 'pair' from the winning line leaves fewer than 5 consecutive.
bool Game::captureWouldBreak(const WinInfo& win, const std::vector<Position>& pair) const {
    int maxRun = 0, curRun = 0;
    for (const auto& s : win.stones) {
        bool removed = (s == pair[0] || s == pair[1]);
        if (removed) {
            if (curRun > maxRun) maxRun = curRun;
            curRun = 0;
        } else {
            ++curRun;
        }
    }
    if (curRun > maxRun) maxRun = curRun;
    return maxRun < 5;
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
                    m_winInfo = WinInfo{player, stones.front(), stones.back(), stones, false};
                    return true;
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

    return static_cast<int>(stones.size()) >= Gomoku::WIN_LENGTH;
}

bool Game::isValidPosition(int x, int y) const {
    return x >= 0 && x < GomokuBoard::BOARD_SIZE && y >= 0 && y < GomokuBoard::BOARD_SIZE;
}

// Counts free-three alignments that pass through pos (which must already be placed).
// Checks 4 directions; at most 1 free-three per direction is counted.
//
// Free-three patterns (S=own stone, E=empty, B=opponent/OOB):
//   Type 1: E S S S E         (.XXX.)
//   Type 2: E S S E S E       (.XX.X.)
//   Type 3: E S E S S E       (.X.XX.)
//
// For each direction a 7-cell window is built around pos (index 3).
// Each cell is encoded: 1=own, 0=empty, -1=opponent or out-of-bounds.
int Game::countFreeThreesAt(Position pos, Player player) const {
    static const int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    Cell ownCell = playerToCell(player);

    int freeThrees = 0;

    for (auto& d : dirs) {
        // Build 7-cell window: indices 0..6, pos at index 3
        int line[7];
        for (int i = 0; i < 7; ++i) {
            int offset = i - 3;
            Position p = {pos.x + d[0] * offset, pos.y + d[1] * offset};
            if (!p.isValid())            line[i] = -1;
            else if (m_board.getCell(p) == ownCell) line[i] =  1;
            else if (m_board.getCell(p) == Cell::EMPTY) line[i] =  0;
            else                         line[i] = -1;
        }

        bool found = false;

        // Type 1: E S S S E  — 5-cell pattern starting at s (s in 0..2)
        // Stone positions within the pattern: s+1, s+2, s+3; pos at index 3 must be one of them.
        for (int s = 0; s <= 2 && !found; ++s) {
            if (line[s] == 0 && line[s+1] == 1 && line[s+2] == 1 &&
                line[s+3] == 1 && line[s+4] == 0)
                found = true;
        }

        // Type 2: E S S E S E  — 6-cell pattern (s in 0..1)
        // Stone positions: s+1, s+2, s+4; pos (3) must be one of them.
        for (int s = 0; s <= 1 && !found; ++s) {
            if (3 != s+1 && 3 != s+2 && 3 != s+4) continue;
            if (line[s] == 0 && line[s+1] == 1 && line[s+2] == 1 &&
                line[s+3] == 0 && line[s+4] == 1 && line[s+5] == 0)
                found = true;
        }

        // Type 3: E S E S S E  — 6-cell pattern (s in 0..1)
        // Stone positions: s+1, s+3, s+4; pos (3) must be one of them.
        for (int s = 0; s <= 1 && !found; ++s) {
            if (3 != s+1 && 3 != s+3 && 3 != s+4) continue;
            if (line[s] == 0 && line[s+1] == 1 && line[s+2] == 0 &&
                line[s+3] == 1 && line[s+4] == 1 && line[s+5] == 0)
                found = true;
        }

        if (found) ++freeThrees;
    }

    return freeThrees;
}

MoveFailReason Game::checkMoveValidity(Position pos) const {
    if (!isValidPosition(pos.x, pos.y))
        return MoveFailReason::OUT_OF_BOUNDS;
    if (m_board.getCell(pos) != Cell::EMPTY)
        return MoveFailReason::OCCUPIED;

    Player current = m_board.getCurrentPlayer();

    // Use a const_cast trick: temporarily place, count free-threes, then remove.
    // We need a non-const board view for placeStone/removeStone.
    GomokuBoard& board = const_cast<GomokuBoard&>(m_board);
    board.placeStone(pos, current);
    bool wouldCapture = !detectCaptures(pos, current).empty();
    int freeThrees = (!wouldCapture) ? countFreeThreesAt(pos, current) : 0;
    board.removeStone(pos);

    if (freeThrees >= 2)
        return MoveFailReason::DOUBLE_THREE;
    return MoveFailReason::NONE;
}

std::vector<Position> Game::getThreatCells(Player perspective) const {
    static const int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    Player opponent = (perspective == Player::BLACK) ? Player::WHITE : Player::BLACK;
    Cell   oppCell  = playerToCell(opponent);
    Cell   ownCell  = playerToCell(perspective);

    bool seen[Gomoku::BOARD_SIZE][Gomoku::BOARD_SIZE] = {};
    std::vector<Position> result;
    auto addIfNew = [&](Position p) {
        if (!seen[p.y][p.x]) {
            seen[p.y][p.x] = true;
            result.push_back(p);
        }
    };

    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {

            // ── Opponent open-threes and open-fours ──────────────────────────
            if (m_board.getCell(x, y) == oppCell) {
                for (auto& d : dirs) {
                    // Skip if not the start of a run in this direction
                    if (isValidPosition(x - d[0], y - d[1]) &&
                        m_board.getCell(x - d[0], y - d[1]) == oppCell)
                        continue;

                    // Collect consecutive opponent stones
                    std::vector<Position> run;
                    int cx = x, cy = y;
                    while (isValidPosition(cx, cy) && m_board.getCell(cx, cy) == oppCell) {
                        run.push_back({cx, cy});
                        cx += d[0]; cy += d[1];
                    }
                    if (static_cast<int>(run.size()) < Gomoku::MIN_THREAT_RUN) continue;

                    bool openAfter  = isValidPosition(cx, cy) &&
                                      m_board.getCell(cx, cy) == Cell::EMPTY;
                    bool openBefore = isValidPosition(x - d[0], y - d[1]) &&
                                      m_board.getCell(x - d[0], y - d[1]) == Cell::EMPTY;

                    bool isOpenFour  = (run.size() == 4) && (openAfter || openBefore);
                    bool isOpenThree = (run.size() == 3) && openAfter && openBefore;

                    if (isOpenFour || isOpenThree)
                        for (const auto& p : run) addIfNew(p);
                }
            }

            // ── Immediately vulnerable pairs of `perspective` ─────────────
            // Pattern: OppStone  Own Own  EmptyCell  (or mirrored)
            if (m_board.getCell(x, y) == ownCell) {
                for (auto& d : dirs) {
                    // Only from the start of a pair
                    if (isValidPosition(x - d[0], y - d[1]) &&
                        m_board.getCell(x - d[0], y - d[1]) == ownCell)
                        continue;

                    int nx = x + d[0], ny = y + d[1];
                    if (!isValidPosition(nx, ny) || m_board.getCell(nx, ny) != ownCell)
                        continue;

                    // Must be exactly a pair (not a longer run)
                    int nnx = nx + d[0], nny = ny + d[1];
                    if (isValidPosition(nnx, nny) && m_board.getCell(nnx, nny) == ownCell)
                        continue;

                    Position before = {x - d[0], y - d[1]};
                    Position after  = {nnx, nny};
                    bool beforeOpp = isValidPosition(before.x, before.y) &&
                                     m_board.getCell(before) == oppCell;
                    bool afterOpp  = isValidPosition(after.x, after.y) &&
                                     m_board.getCell(after) == oppCell;
                    bool beforeFree = isValidPosition(before.x, before.y) &&
                                      m_board.getCell(before) == Cell::EMPTY;
                    bool afterFree  = isValidPosition(after.x, after.y) &&
                                      m_board.getCell(after) == Cell::EMPTY;

                    // Highlight if opponent can capture from either side on next move
                    if ((beforeOpp && afterFree) || (afterOpp && beforeFree)) {
                        addIfNew({x, y});
                        addIfNew({nx, ny});
                    }
                }
            }
        }
    }
    return result;
}

std::vector<Game::MoveEntry> Game::getMoveHistory() const {
    std::vector<MoveEntry> result;
    result.reserve(m_undoStack.size());
    for (int i = 0; i < static_cast<int>(m_undoStack.size()); ++i) {
        const auto& r = m_undoStack[i];
        result.push_back({i + 1, r.pos, r.player,
                          static_cast<int>(r.captured.size()),
                          r.timestamp});
    }
    return result;
}
