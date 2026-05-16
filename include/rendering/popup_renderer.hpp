#pragma once

#include <SFML/Graphics.hpp>
#include "ui/ui_state.hpp"
#include "ui/palette.hpp"

class PopupRenderer {
public:
    PopupRenderer(const sf::Font& font, const Palette& palette);
    void draw(sf::RenderWindow& window, const UIState& state) const;

private:
    const sf::Font&  m_font;
    const Palette&   m_palette;

    // ── Box geometry ──────────────────────────────────────────────────────────
    static constexpr float POPUP_WIDTH  = 420.f;
    static constexpr float POPUP_HEIGHT = 180.f;
    static constexpr float PADDING      = 20.f;

    // ── Top accent bar ────────────────────────────────────────────────────────
    static constexpr float TOP_BAR_HEIGHT = 4.f;

    // ── Text layout (relative to box top-left + PADDING) ─────────────────────
    static constexpr float TITLE_Y             = 18.f;
    static constexpr float BODY_Y              = 42.f;
    static constexpr float BODY_Y2             = 64.f;
    static constexpr float HINT_BOTTOM_OFFSET  = 44.f;

    // ── Font sizes ────────────────────────────────────────────────────────────
    static constexpr unsigned int FONT_SIZE_TITLE = 20u;
    static constexpr unsigned int FONT_SIZE_BODY  = 15u;
    static constexpr unsigned int FONT_SIZE_HINT  = 12u;

    // ── Word wrap ─────────────────────────────────────────────────────────────
    static constexpr std::size_t BODY_WRAP_LENGTH = 45;

    // ── Horizontal centering on board area ───────────────────────────────────
    static constexpr float BOARD_AREA_RATIO = 0.6f;
    static constexpr float BOARD_AREA_MAX_X = 390.f;

    // ── Shared popup outline ──────────────────────────────────────────────────
    static constexpr float POPUP_OUTLINE_WIDTH = 2.f;

    // ── Rules popup geometry ──────────────────────────────────────────────────
    static constexpr float RULES_WIDTH    = 440.f;
    static constexpr float RULES_HEIGHT   = 310.f;
    static constexpr float RULES_LINE_H   = 22.f;
    static constexpr float RULES_VALUE_X  = 160.f;  // value column X offset from left edge

    // ── History popup geometry ────────────────────────────────────────────────
    static constexpr float HISTORY_WIDTH      = 620.f;
    static constexpr float HISTORY_HEIGHT     = 380.f;
    static constexpr float HISTORY_ROW_HEIGHT = 20.f;
    static constexpr float HISTORY_HEADER_Y   = 38.f;
    static constexpr float HISTORY_ROWS_Y     = 60.f;

public:
    static constexpr int   HISTORY_VISIBLE_ROWS = 12;

private:
    // ── History column X offsets from left edge ───────────────────────────────
    static constexpr float HISTORY_COL_NUMBER    =   0.f;
    static constexpr float HISTORY_COL_TIMESTAMP =  34.f;
    static constexpr float HISTORY_COL_PLAYER    = 170.f;
    static constexpr float HISTORY_COL_COLOR     = 330.f;
    static constexpr float HISTORY_COL_CELL      = 410.f;
    static constexpr float HISTORY_COL_EVENT     = 460.f;

    // ── History footer ────────────────────────────────────────────────────────
    static constexpr float HISTORY_FOOTER_CLOSE_X = 160.f;  // offset left from popup right edge

    void drawPopupBase(sf::RenderWindow& window,
                       const std::string& title, const std::string& body,
                       sf::Color accentColor) const;
    void drawRulesPopup(sf::RenderWindow& window, const UIState& state) const;
    void drawHistoryPopup(sf::RenderWindow& window, const UIState& state) const;
    void drawText(sf::RenderWindow& window, const std::string& text,
                  unsigned int size, sf::Vector2f pos, sf::Color color) const;
};
