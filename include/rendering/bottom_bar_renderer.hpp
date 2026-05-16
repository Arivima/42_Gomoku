#pragma once

#include <SFML/Graphics.hpp>
#include "ui/palette.hpp"

class BottomBarRenderer {
public:
    BottomBarRenderer(const sf::Font& font, const Palette& palette);
    void draw(sf::RenderWindow& window) const;

    // ── Public layout constant (used by GomokuApp for board centering) ────────
    static constexpr float BAR_HEIGHT = 36.f;

private:
    const sf::Font&  m_font;
    const Palette&   m_palette;

    static constexpr const char*  SHORTCUTS_TEXT =
        "R Restart  |  Q Quit  |  Z Undo  |  A Redo  |  C Coach  |  D Debug  |  I Rules  |  H History  |  P Theme";

    static constexpr float        RING_RADIUS    = 7.f;
    static constexpr unsigned int TEXT_SIZE      = 13u;
    static constexpr float        PADDING_X      = 20.f;
    static constexpr float        PADDING_Y      = 9.f;
    static constexpr float        LEGEND_ITEM_W  = 110.f;  // width per legend entry
    static constexpr float        LABEL_TEXT_GAP = 6.f;    // gap between ring glyph and label

    void drawLegend(sf::RenderWindow& window, float barY) const;
    void drawShortcuts(sf::RenderWindow& window, float barY, float winWidth) const;
    void drawRingGlyph(sf::RenderWindow& window, sf::Vector2f center, sf::Color color) const;
};
