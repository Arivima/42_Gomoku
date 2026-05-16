#include "rendering/bottom_bar_renderer.hpp"

BottomBarRenderer::BottomBarRenderer(const sf::Font& font, const Palette& palette)
    : m_font(font), m_palette(palette) {}

void BottomBarRenderer::draw(sf::RenderWindow& window) const {
    auto winSize = window.getView().getSize();
    float barY   = winSize.y - BAR_HEIGHT;

    sf::RectangleShape bg({winSize.x, BAR_HEIGHT});
    bg.setPosition({0.f, barY});
    bg.setFillColor(m_palette.surfaceSecondary);
    window.draw(bg);

    sf::RectangleShape border({winSize.x, 1.f});
    border.setPosition({0.f, barY});
    border.setFillColor(m_palette.border);
    window.draw(border);

    drawLegend(window, barY);
    drawShortcuts(window, barY, winSize.x);
}

void BottomBarRenderer::drawLegend(sf::RenderWindow& window, float barY) const {
    float cy = barY + BAR_HEIGHT / 2.f;
    float x  = PADDING_X;

    struct LegendItem { sf::Color color; const char* label; };
    const LegendItem items[] = {
        {m_palette.ringLastMove,  "last move"},
        {m_palette.ringForbidden, "forbidden"},
        {m_palette.ringThreat,    "threat"},
    };

    for (const auto& item : items) {
        drawRingGlyph(window, {x + RING_RADIUS, cy}, item.color);

        sf::Text label(m_font, item.label, TEXT_SIZE);
        auto bounds = label.getLocalBounds();
        label.setOrigin({bounds.position.x, bounds.position.y + bounds.size.y / 2.f});
        label.setPosition({x + RING_RADIUS * 2.f + LABEL_TEXT_GAP, cy});
        label.setFillColor(m_palette.textSecondary);
        window.draw(label);

        x += LEGEND_ITEM_W;
    }
}

void BottomBarRenderer::drawShortcuts(sf::RenderWindow& window, float barY, float winWidth) const {
    sf::Text text(m_font, SHORTCUTS_TEXT, TEXT_SIZE);
    auto bounds = text.getLocalBounds();
    float x = winWidth - bounds.size.x - PADDING_X;
    float y = barY + PADDING_Y;
    text.setPosition({x, y});
    text.setFillColor(m_palette.textSecondary);
    window.draw(text);
}

void BottomBarRenderer::drawRingGlyph(sf::RenderWindow& window, sf::Vector2f center, sf::Color color) const {
    sf::CircleShape ring(RING_RADIUS);
    ring.setOrigin({RING_RADIUS, RING_RADIUS});
    ring.setPosition(center);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineColor(color);
    ring.setOutlineThickness(2.f);
    window.draw(ring);
}
