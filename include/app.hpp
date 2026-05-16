#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

#include "core/ai_engine.hpp"
#include "core/game.hpp"
#include "ui/ui_state.hpp"
#include "ui/ai_stats.hpp"
#include "ui/palette.hpp"
#include "rendering/board_renderer.hpp"
#include "rendering/panel_renderer.hpp"
#include "rendering/bottom_bar_renderer.hpp"
#include "rendering/popup_renderer.hpp"
#include "rendering/start_screen.hpp"

enum class AppState { START_SCREEN, PLAYING };

class GomokuApp {
public:
    GomokuApp();
    void run();

private:
    // ── Window / framerate ────────────────────────────────────────────────────
    static constexpr unsigned    WINDOW_W       = 1200u;
    static constexpr unsigned    WINDOW_H       = 820u;
    static constexpr unsigned    FRAMERATE      = 60u;
    static constexpr const char* WINDOW_TITLE   = "Gomoku";
    static constexpr const char* FONT_PATH      = "assets/fonts/Roboto-VariableFont_wdth,wght.ttf";
    static constexpr const char* TIMESTAMP_FMT  = "%Y%m%d %H:%M";

    // ── Board layout (derived from renderer constants) ────────────────────────
    static constexpr float GRID_SIZE      = BoardRenderer::DEFAULT_CELL_SIZE * (GomokuBoard::BOARD_SIZE - 1);
    static constexpr float BOARD_PADDING  = BoardRenderer::PADDING;
    static constexpr float BOARD_OFFSET_X =
        (PanelRenderer::PANEL_X - GRID_SIZE - 2.f * BOARD_PADDING) / 2.f + BOARD_PADDING;
    static constexpr float BOARD_OFFSET_Y =
        (static_cast<float>(WINDOW_H) - BottomBarRenderer::BAR_HEIGHT - GRID_SIZE - 2.f * BOARD_PADDING) / 2.f + BOARD_PADDING;
    static constexpr float ASPECT_RATIO =
        static_cast<float>(WINDOW_W) / static_cast<float>(WINDOW_H);

    // ── Lifecycle ─────────────────────────────────────────────────────────────
    void loadFont();
    void processEvents();
    void update();
    void buildUIState();
    void render();

    // ── Event handlers ────────────────────────────────────────────────────────
    void handleMouseClick(sf::Vector2i mousePos);
    void handleMouseMove(sf::Vector2i mousePos);
    void handleKeyPress(sf::Keyboard::Key key);
    void startGame(const StartConfig& cfg);

    // ── UIState builders ──────────────────────────────────────────────────────
    UIState::CellInfo buildCellInfo(Position pos) const;
    std::string       buildTurnLabel() const;
    std::string       failReasonStr(MoveFailReason r) const;
    std::string       buildGameOverMessage() const;

    // ── SFML ──────────────────────────────────────────────────────────────────
    sf::RenderWindow m_window;
    sf::Font         m_font;

    // ── Game state ────────────────────────────────────────────────────────────
    Game               m_game;
    AiEngine           m_aiEngine;
    UIState            m_uiState;
    AiStatsAccumulator m_aiStats;

    // m_palette must be declared before renderers (they hold a const ref to it)
    Palette m_palette;
    bool    m_isDark = true;

    // ── Renderers ─────────────────────────────────────────────────────────────
    BoardRenderer     m_boardRenderer;
    PanelRenderer     m_panelRenderer;
    BottomBarRenderer m_bottomBar;
    PopupRenderer     m_popupRenderer;
    StartScreen       m_startScreen;

    // ── App state ─────────────────────────────────────────────────────────────
    AppState m_appState   = AppState::START_SCREEN;
    Player   m_humanColor = Player::BLACK;
    bool     m_coachEnabled = false;
    bool     m_debugEnabled = false;
    std::optional<Position> m_hoverPos;

    UIState::ActivePopup m_activePopup = UIState::ActivePopup::NONE;
    std::string          m_popupMessage;
    int                  m_historyScrollOffset = 0;
};
