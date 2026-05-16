#pragma once

#include <SFML/Graphics.hpp>
#include "ui/ui_state.hpp"
#include "ui/palette.hpp"
#include <cstddef>
#include <string>

class PanelRenderer {
public:
    PanelRenderer(const sf::Font& font, const Palette& palette);

    void draw(sf::RenderWindow& window, const UIState& state) const;

    // ── Public layout constant (used by GomokuApp for board centering) ────────
    static constexpr float PANEL_X = 810.f;

private:
    const sf::Font&  m_font;
    const Palette&   m_palette;

    // ── Panel text ────────────────────────────────────────────────────────────
    static constexpr const char* TITLE_TEXT = "Gomoku";

    // ── Panel geometry ────────────────────────────────────────────────────────
    static constexpr float PANEL_WIDTH   = 380.f;
    static constexpr float PANEL_PADDING = 15.f;

    // ── Section Y positions ───────────────────────────────────────────────────
    static constexpr float Y_TITLE           = 20.f;
    static constexpr float Y_MODE_SECTION    = 70.f;
    static constexpr float Y_CAPTURE_SECTION = 220.f;
    static constexpr float Y_STATS_SECTION   = 290.f;
    static constexpr float Y_TURN            = 420.f;
    static constexpr float Y_WARNING         = 490.f;
    static constexpr float Y_CELL_INFO       = 620.f;

    static constexpr float ROW_HEIGHT       = 28.f;
    static constexpr float SEPARATOR_HEIGHT = 1.f;

    // ── Font sizes ────────────────────────────────────────────────────────────
    static constexpr unsigned int FONT_SIZE_TITLE = 26u;
    static constexpr unsigned int FONT_SIZE_LABEL = 15u;
    static constexpr unsigned int FONT_SIZE_VALUE = 15u;
    static constexpr unsigned int FONT_SIZE_SMALL = 13u;
    static constexpr unsigned int FONT_SIZE_TURN  = 16u;

    // ── Turn button ───────────────────────────────────────────────────────────
    static constexpr float TURN_BUTTON_HEIGHT = 44.f;
    static constexpr float TURN_STONE_RADIUS  = 10.f;
    static constexpr float TURN_STONE_PAD     = 20.f;

    // ── Captures section ──────────────────────────────────────────────────────
    static constexpr float CAPTURE_STONE_RADIUS = 7.f;
    static constexpr float CAPTURE_COL_X        = 110.f;
    static constexpr float CAPTURE_COL_GAP      = 55.f;
    static constexpr float CAPTURE_TEXT_OFFSET  = 12.f;

    // ── Warning box ───────────────────────────────────────────────────────────
    static constexpr float       WARNING_HEIGHT        = 110.f;
    static constexpr float       WARNING_PADDING_X     = 10.f;
    static constexpr float       WARNING_PADDING_TOP   = 8.f;
    static constexpr float       WARNING_BODY_Y        = 32.f;
    static constexpr float       WARNING_BODY_Y2       = 52.f;
    static constexpr float       WARNING_OUTLINE_WIDTH = 2.f;
    static constexpr std::size_t WARNING_WRAP_LENGTH   = 40u;

    // ── Info row label column width ───────────────────────────────────────────
    static constexpr float INFO_COLUMN_X = 150.f;

    // ── Stats table column X positions (offset from PANEL_X) ─────────────────
    static constexpr float STATS_COL_LAST = 70.f;
    static constexpr float STATS_COL_MIN  = 140.f;
    static constexpr float STATS_COL_MAX  = 210.f;
    static constexpr float STATS_COL_AVG  = 280.f;

    void drawBackground(sf::RenderWindow& window) const;
    void drawTitle(sf::RenderWindow& window) const;
    void drawSeparator(sf::RenderWindow& window, float y) const;
    void drawModeSection(sf::RenderWindow& window, const UIState& state) const;
    void drawCaptures(sf::RenderWindow& window, const UIState& state) const;
    void drawStatsSection(sf::RenderWindow& window, const UIState& state) const;
    void drawTurnButton(sf::RenderWindow& window, const UIState& state) const;
    void drawWarningBox(sf::RenderWindow& window, const UIState& state) const;
    void drawCellInfo(sf::RenderWindow& window, const UIState& state) const;

    void drawInfoRow(sf::RenderWindow& window, float y,
                     const std::string& label, const std::string& value,
                     sf::Color valueColor) const;
    void drawText(sf::RenderWindow& window, const std::string& text,
                  unsigned int size, sf::Vector2f pos, sf::Color color) const;
    void drawSmallCircle(sf::RenderWindow& window, sf::Vector2f center,
                         float radius, sf::Color fill, sf::Color outline) const;

    std::string gameModeStr(GameMode mode) const;
    std::string openingRuleStr(OpeningRule rule) const;
};
