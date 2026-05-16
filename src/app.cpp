#include "app.hpp"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

static constexpr const char* COL_LABELS      = "ABCDEFGHIJKLMNOPQRS";
static constexpr const char* ENDGAME_MSG     =
    "Player aligned five stones — opponent may still break the line by capture.";

GomokuApp::GomokuApp()
    : m_window(sf::VideoMode({WINDOW_W, WINDOW_H}), WINDOW_TITLE)
    , m_palette(Palette::dark())
    , m_boardRenderer(m_palette)
    , m_panelRenderer(m_font, m_palette)
    , m_bottomBar(m_font, m_palette)
    , m_popupRenderer(m_font, m_palette)
    , m_startScreen(m_font, m_palette) {
    m_window.setFramerateLimit(FRAMERATE);
    loadFont();
    m_boardRenderer.setBoardOffset(BOARD_OFFSET_X, BOARD_OFFSET_Y);
}

void GomokuApp::run() {
    while (m_window.isOpen()) {
        processEvents();
        update();
        buildUIState();
        render();
    }
}

void GomokuApp::loadFont() {
    if (!m_font.openFromFile(FONT_PATH))
        std::cerr << "Warning: Could not load font\n";
}

void GomokuApp::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
            return;
        }

        if (const auto* re = event->getIf<sf::Event::Resized>()) {
            unsigned w = re->size.x;
            unsigned h = re->size.y;
            const float ratio = static_cast<float>(w) / static_cast<float>(h);
            if (ratio > ASPECT_RATIO + 0.01f)
                w = static_cast<unsigned>(h * ASPECT_RATIO);
            else if (ratio < ASPECT_RATIO - 0.01f)
                h = static_cast<unsigned>(w / ASPECT_RATIO);
            if (w != re->size.x || h != re->size.y)
                m_window.setSize({w, h});
        }

        if (m_appState == AppState::START_SCREEN) {
            m_startScreen.handleEvent(*event, m_window);
            if (m_startScreen.isStartRequested())
                startGame(m_startScreen.getConfig());
            continue;
        }

        if (m_uiState.activePopup != UIState::ActivePopup::NONE) {
            const bool isHistory = (m_uiState.activePopup == UIState::ActivePopup::HISTORY);
            if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
                if (isHistory && kp->code == sf::Keyboard::Key::Up) {
                    --m_historyScrollOffset;
                } else if (isHistory && kp->code == sf::Keyboard::Key::Down) {
                    ++m_historyScrollOffset;
                } else {
                    m_activePopup = UIState::ActivePopup::NONE;
                    m_popupMessage.clear();
                }
            } else if (const auto* ws = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (isHistory)
                    m_historyScrollOffset -= static_cast<int>(ws->delta);
            } else if (event->is<sf::Event::MouseButtonPressed>()) {
                m_activePopup = UIState::ActivePopup::NONE;
                m_popupMessage.clear();
            }
            continue;
        }

        if (const auto* mp = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mp->button == sf::Mouse::Button::Left)
                handleMouseClick(mp->position);
        }

        if (const auto* mm = event->getIf<sf::Event::MouseMoved>())
            handleMouseMove(mm->position);

        if (const auto* kp = event->getIf<sf::Event::KeyPressed>())
            handleKeyPress(kp->code);
    }
}

void GomokuApp::handleMouseClick(sf::Vector2i mousePos) {
    if (m_game.isGameOver()) return;

    const sf::Vector2f logicalPos = m_window.mapPixelToCoords(mousePos);
    const Position boardPos = m_boardRenderer.screenToBoard(logicalPos);
    if (!boardPos.isValid()) return;

    MoveResult result = m_game.makeMove(boardPos);

    if (!result.success) {
        m_activePopup  = UIState::ActivePopup::ILLEGAL_MOVE;
        m_popupMessage = failReasonStr(result.failReason);
    } else if (m_game.isGameOver()) {
        m_activePopup  = UIState::ActivePopup::GAME_OVER;
        m_popupMessage = buildGameOverMessage();
    } else if (result.endgameCaptureActive) {
        m_activePopup  = UIState::ActivePopup::ENDGAME_CAPTURE;
        m_popupMessage = ENDGAME_MSG;
    }
}

void GomokuApp::handleMouseMove(sf::Vector2i mousePos) {
    const sf::Vector2f logicalPos = m_window.mapPixelToCoords(mousePos);
    const Position boardPos = m_boardRenderer.screenToBoard(logicalPos);
    if (boardPos.isValid() && m_game.getBoard().getCell(boardPos) == Cell::EMPTY)
        m_hoverPos = boardPos;
    else
        m_hoverPos = std::nullopt;
}

void GomokuApp::handleKeyPress(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::R:
            m_appState = AppState::START_SCREEN;
            m_startScreen.reset();
            m_activePopup = UIState::ActivePopup::NONE;
            break;
        case sf::Keyboard::Key::Q:
            m_window.close();
            break;
        case sf::Keyboard::Key::C:
            m_coachEnabled = !m_coachEnabled;
            break;
        case sf::Keyboard::Key::D:
            m_debugEnabled = !m_debugEnabled;
            break;
        case sf::Keyboard::Key::Z:
            m_game.undoMove();
            if (m_game.getGameMode() == GameMode::HUMAN_VS_AI)
                m_game.undoMove();
            m_activePopup = UIState::ActivePopup::NONE;
            break;
        case sf::Keyboard::Key::A:
            m_game.redoMove();
            if (m_game.getGameMode() == GameMode::HUMAN_VS_AI)
                m_game.redoMove();
            m_activePopup = UIState::ActivePopup::NONE;
            break;
        case sf::Keyboard::Key::P:
            m_isDark  = !m_isDark;
            m_palette = m_isDark ? Palette::dark() : Palette::light();
            break;
        case sf::Keyboard::Key::I:
            m_activePopup = UIState::ActivePopup::RULES;
            break;
        case sf::Keyboard::Key::H:
            m_activePopup = UIState::ActivePopup::HISTORY;
            m_historyScrollOffset = 0;
            break;
        default:
            break;
    }
}

void GomokuApp::startGame(const StartConfig& cfg) {
    m_game.setGameMode(cfg.mode);
    m_game.setOpeningRule(cfg.openingRule);
    m_humanColor = cfg.humanColor;
    m_game.reset();
    m_aiStats.reset();
    m_coachEnabled = false;
    m_debugEnabled = false;
    m_hoverPos     = std::nullopt;
    m_activePopup  = UIState::ActivePopup::NONE;
    m_popupMessage.clear();
    m_appState = AppState::PLAYING;
}

void GomokuApp::update() {
    if (m_appState != AppState::PLAYING) return;
    if (m_game.isGameOver()) return;
    if (m_activePopup != UIState::ActivePopup::NONE) return;
    if (m_game.getGameMode() != GameMode::HUMAN_VS_AI) return;
    if (m_game.getCurrentPlayer() == m_humanColor) return;

    // Ask the AI for the best legal move (depth-1 heuristic).
    // rankMoves already filters illegal positions (double-three, occupied, etc.)
    // so the first result is always safe to play.
    Player aiPlayer = m_game.getCurrentPlayer();

    auto t0 = std::chrono::steady_clock::now();
    auto ranked = m_aiEngine.rankMoves(m_game, aiPlayer, 1);
    auto t1 = std::chrono::steady_clock::now();

    if (ranked.empty()) return;

    MoveResult result = m_game.makeMove(ranked[0].first);
    if (!result.success) return;

    float ms = std::chrono::duration<float, std::milli>(t1 - t0).count();
    m_aiStats.record(ms, 1);

    if (m_game.isGameOver()) {
        m_activePopup  = UIState::ActivePopup::GAME_OVER;
        m_popupMessage = buildGameOverMessage();
    } else if (result.endgameCaptureActive) {
        m_activePopup  = UIState::ActivePopup::ENDGAME_CAPTURE;
        m_popupMessage = ENDGAME_MSG;
    }
}

void GomokuApp::buildUIState() {
    m_uiState = UIState{};
    const GomokuBoard& board = m_game.getBoard();

    if (auto last = board.getLastMove())
        m_uiState.overlays.push_back({*last, UIState::OverlayKind::LAST_MOVE});

    // Coach mode: threat, forbidden, and suggestion overlays
    if (m_coachEnabled && !m_game.isGameOver()) {
        // Orange rings: opponent threats + vulnerable pairs
        for (const auto& p : m_game.getThreatCells(m_game.getCurrentPlayer()))
            m_uiState.overlays.push_back({p, UIState::OverlayKind::THREAT});

        // Red rings: cells where current player's move would be a double-three
        for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
            for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
                Position p{x, y};
                if (board.getCell(p) == Cell::EMPTY &&
                    m_game.checkMoveValidity(p) == MoveFailReason::DOUBLE_THREE)
                    m_uiState.overlays.push_back({p, UIState::OverlayKind::FORBIDDEN});
            }
        }

        // Green rings: top 3 suggested moves for the current player
        auto top3 = m_aiEngine.rankMoves(m_game, m_game.getCurrentPlayer(), 3);
        for (const auto& [pos, score] : top3)
            m_uiState.overlays.push_back({pos, UIState::OverlayKind::COACH_SUGGESTION});
    }

    m_uiState.hoverPos      = m_hoverPos;
    m_uiState.gameMode      = m_game.getGameMode();
    m_uiState.openingRule   = m_game.getOpeningRule();
    m_uiState.humanColor    = m_humanColor;
    m_uiState.coachEnabled  = m_coachEnabled;
    m_uiState.debugEnabled  = m_debugEnabled;
    m_uiState.currentPlayer = m_game.getCurrentPlayer();
    m_uiState.blackCaptures = board.getBlackCaptures();
    m_uiState.whiteCaptures = board.getWhiteCaptures();

    if (m_game.getGameMode() == GameMode::HUMAN_VS_AI && m_aiStats.snapshot().lastMs > 0.f)
        m_uiState.aiStats = m_aiStats.snapshot();

    m_uiState.isGameOver = m_game.isGameOver();
    m_uiState.turnLabel  = buildTurnLabel();

    if (m_activePopup == UIState::ActivePopup::ENDGAME_CAPTURE)
        m_uiState.warningMessage = "Endgame capture rule activated!";

    if (m_coachEnabled && m_hoverPos.has_value())
        m_uiState.hoveredCellInfo = buildCellInfo(*m_hoverPos);

    // Build move history entries for the History popup
    {
        const bool isAiMode = (m_game.getGameMode() == GameMode::HUMAN_VS_AI);
        auto raw = m_game.getMoveHistory();
        m_uiState.moveHistory.reserve(raw.size());
        for (const auto& e : raw) {
            // Timestamp string "YYYYMMDD HH:MM"
            auto tt = std::chrono::system_clock::to_time_t(e.timestamp);
            std::tm tm{};
#ifdef _WIN32
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif
            std::ostringstream oss;
            oss << std::put_time(&tm, TIMESTAMP_FMT);

            const bool isBlack = (e.player == Player::BLACK);
            std::string playerLabel;
            if (isAiMode)
                playerLabel = (e.player == m_humanColor) ? "Human" : "AI";
            else
                playerLabel = isBlack ? "Player 1" : "Player 2";

            std::string coord(1, COL_LABELS[e.pos.x]);
            coord += std::to_string(e.pos.y + 1);

            std::string event;
            if (e.capturedCount > 0)
                event = "captures " + std::to_string(e.capturedCount);

            m_uiState.moveHistory.push_back({
                e.moveNumber, oss.str(), playerLabel,
                isBlack ? "Black" : "White",
                coord, event
            });
        }
        // Clamp scroll offset
        const int maxOffset = std::max(0, static_cast<int>(m_uiState.moveHistory.size()) - PopupRenderer::HISTORY_VISIBLE_ROWS);
        m_historyScrollOffset = std::max(0, std::min(m_historyScrollOffset, maxOffset));
        m_uiState.historyScrollOffset = m_historyScrollOffset;
    }

    m_uiState.activePopup  = m_activePopup;
    m_uiState.popupMessage = m_popupMessage;
}

UIState::CellInfo GomokuApp::buildCellInfo(Position pos) const {
    std::string coord(1, COL_LABELS[pos.x]);
    coord += std::to_string(pos.y + 1);

    auto reason = m_game.checkMoveValidity(pos);
    bool legal  = (reason == MoveFailReason::NONE);

    float score = m_aiEngine.evalCell(m_game.getBoard(), pos, m_game.getCurrentPlayer());

    return {coord, legal, legal ? "" : failReasonStr(reason), score};
}

std::string GomokuApp::buildTurnLabel() const {
    if (m_game.isGameOver()) {
        if (m_game.getStatus() == GameStatus::DRAW) return "Draw!";
        return std::string(m_game.getStatus() == GameStatus::BLACK_WINS ? "Black" : "White") + " wins!";
    }
    const bool isBlack = m_game.getCurrentPlayer() == Player::BLACK;
    if (m_game.getGameMode() == GameMode::HUMAN_VS_HUMAN)
        return isBlack ? "Player 1 (Black)" : "Player 2 (White)";
    const bool isAiTurn = (m_game.getCurrentPlayer() != m_humanColor);
    return std::string(isAiTurn ? "AI" : "Human") + (isBlack ? " (Black)" : " (White)");
}

std::string GomokuApp::failReasonStr(MoveFailReason r) const {
    switch (r) {
        case MoveFailReason::OCCUPIED:      return "That cell is already occupied.";
        case MoveFailReason::OUT_OF_BOUNDS: return "Move is outside the board.";
        case MoveFailReason::DOUBLE_THREE:  return "Forbidden: double-three.";
        default:                            return "Illegal move.";
    }
}

std::string GomokuApp::buildGameOverMessage() const {
    const auto& info = m_game.getWinInfo();
    if (!info.has_value()) return "The board is full — draw!";
    const std::string winner = (info->winner == Player::BLACK) ? "Black" : "White";
    if (info->byCaptureWin) return winner + " wins by capture! (10 stones)";
    return winner + " wins by alignment!";
}

void GomokuApp::render() {
    m_window.clear(m_palette.boardBackground);

    if (m_appState == AppState::START_SCREEN) {
        m_startScreen.draw(m_window);
    } else {
        m_boardRenderer.drawBoard(m_window, m_font);
        m_boardRenderer.drawStones(m_window, m_game.getBoard(), m_font, m_coachEnabled);
        m_boardRenderer.drawOverlays(m_window, m_uiState);
        m_boardRenderer.drawHover(m_window, m_uiState);
        m_panelRenderer.draw(m_window, m_uiState);
        m_bottomBar.draw(m_window);
        m_popupRenderer.draw(m_window, m_uiState);
    }

    m_window.display();
}
