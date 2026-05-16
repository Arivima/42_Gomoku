#include "rendering/panel_renderer.hpp"
#include <sstream>
#include <iomanip>

PanelRenderer::PanelRenderer(const sf::Font& font, const Palette& palette)
    : m_font(font), m_palette(palette) {}

void PanelRenderer::draw(sf::RenderWindow& window, const UIState& state) const {
    drawBackground(window);
    drawTitle(window);
    drawSeparator(window, Y_MODE_SECTION - 8.f);
    drawModeSection(window, state);
    drawSeparator(window, Y_CAPTURE_SECTION - 8.f);
    drawCaptures(window, state);
    if (state.gameMode == GameMode::HUMAN_VS_AI) {
        drawSeparator(window, Y_STATS_SECTION - 8.f);
        drawStatsSection(window, state);
        drawSeparator(window, Y_TURN - 8.f);
    } else {
        drawSeparator(window, Y_TURN - 8.f);
    }
    drawTurnButton(window, state);
    if (state.warningMessage.has_value())
        drawWarningBox(window, state);
    if (state.hoveredCellInfo.has_value())
        drawCellInfo(window, state);
}

void PanelRenderer::drawBackground(sf::RenderWindow& window) const {
    auto winSize = window.getView().getSize();
    sf::RectangleShape bg({PANEL_WIDTH + PANEL_PADDING, winSize.y});
    bg.setPosition({PANEL_X - PANEL_PADDING, 0.f});
    bg.setFillColor(m_palette.surface);
    window.draw(bg);
}

void PanelRenderer::drawTitle(sf::RenderWindow& window) const {
    const float cx = PANEL_X + PANEL_WIDTH / 2.f;

    sf::Text title(m_font, TITLE_TEXT, FONT_SIZE_TITLE);
    auto bounds = title.getLocalBounds();
    title.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y});
    title.setPosition({cx, Y_TITLE});
    title.setFillColor(m_palette.text);
    window.draw(title);

    float tw = bounds.size.x;
    sf::RectangleShape underline({tw, 2.f});
    underline.setPosition({cx - tw / 2.f, Y_TITLE + bounds.size.y + 6.f});
    underline.setFillColor(m_palette.accent);
    window.draw(underline);
}

void PanelRenderer::drawSeparator(sf::RenderWindow& window, float y) const {
    sf::RectangleShape sep({PANEL_WIDTH, SEPARATOR_HEIGHT});
    sep.setPosition({PANEL_X, y});
    sep.setFillColor(m_palette.border);
    window.draw(sep);
}

void PanelRenderer::drawModeSection(sf::RenderWindow& window, const UIState& state) const {
    const float y = Y_MODE_SECTION;
    drawInfoRow(window, y,                "Mode :",         gameModeStr(state.gameMode),       m_palette.text);
    drawInfoRow(window, y + ROW_HEIGHT,   "Opening rule :", openingRuleStr(state.openingRule),  m_palette.text);
    drawInfoRow(window, y + ROW_HEIGHT*2, "AI Coach :",
        state.coachEnabled ? "Active" : "Inactive",
        state.coachEnabled ? m_palette.accentActive : m_palette.accentInactive);
    drawInfoRow(window, y + ROW_HEIGHT*3, "Debug :",
        state.debugEnabled ? "Active" : "Inactive",
        state.debugEnabled ? m_palette.accentActive : m_palette.accentInactive);
}

void PanelRenderer::drawCaptures(sf::RenderWindow& window, const UIState& state) const {
    const float y  = Y_CAPTURE_SECTION;
    const float cx = PANEL_X + CAPTURE_COL_X;
    drawText(window, "Captures", FONT_SIZE_LABEL, {PANEL_X, y}, m_palette.text);

    drawSmallCircle(window, {cx, y + CAPTURE_STONE_RADIUS + 1.f}, CAPTURE_STONE_RADIUS, sf::Color::White, sf::Color::Black);
    drawText(window, std::to_string(state.whiteCaptures), FONT_SIZE_VALUE,
             {cx + CAPTURE_TEXT_OFFSET, y}, m_palette.text);

    drawSmallCircle(window, {cx + CAPTURE_COL_GAP, y + CAPTURE_STONE_RADIUS + 1.f}, CAPTURE_STONE_RADIUS, sf::Color::Black, sf::Color(100,100,100));
    drawText(window, std::to_string(state.blackCaptures), FONT_SIZE_VALUE,
             {cx + CAPTURE_COL_GAP + CAPTURE_TEXT_OFFSET, y}, m_palette.text);
}

void PanelRenderer::drawStatsSection(sf::RenderWindow& window, const UIState& state) const {
    const float y   = Y_STATS_SECTION;
    const float x0  = PANEL_X;
    const float xL  = x0 + STATS_COL_LAST;
    const float xMn = x0 + STATS_COL_MIN;
    const float xMx = x0 + STATS_COL_MAX;
    const float xAv = x0 + STATS_COL_AVG;

    drawText(window, "Stats", FONT_SIZE_SMALL, {x0,  y}, m_palette.textSecondary);
    drawText(window, "last",  FONT_SIZE_SMALL, {xL,  y}, m_palette.textSecondary);
    drawText(window, "min",   FONT_SIZE_SMALL, {xMn, y}, m_palette.textSecondary);
    drawText(window, "max",   FONT_SIZE_SMALL, {xMx, y}, m_palette.textSecondary);
    drawText(window, "avg",   FONT_SIZE_SMALL, {xAv, y}, m_palette.textSecondary);

    if (!state.aiStats.has_value()) return;

    auto fmtMs = [](float ms) {
        std::ostringstream ss;
        ss << static_cast<int>(ms) << "ms";
        return ss.str();
    };

    const auto& s  = *state.aiStats;
    const float y2 = y + ROW_HEIGHT;
    drawText(window, "Timer",         FONT_SIZE_SMALL, {x0,  y2}, m_palette.text);
    drawText(window, fmtMs(s.lastMs), FONT_SIZE_SMALL, {xL,  y2}, m_palette.text);
    drawText(window, fmtMs(s.minMs),  FONT_SIZE_SMALL, {xMn, y2}, m_palette.text);
    drawText(window, fmtMs(s.maxMs),  FONT_SIZE_SMALL, {xMx, y2}, m_palette.text);
    drawText(window, fmtMs(s.avgMs),  FONT_SIZE_SMALL, {xAv, y2}, m_palette.text);

    const float y3 = y + ROW_HEIGHT * 2;
    drawText(window, "Depth",                     FONT_SIZE_SMALL, {x0,  y3}, m_palette.text);
    drawText(window, std::to_string(s.lastDepth), FONT_SIZE_SMALL, {xL,  y3}, m_palette.text);
    drawText(window, std::to_string(s.minDepth),  FONT_SIZE_SMALL, {xMn, y3}, m_palette.text);
    drawText(window, std::to_string(s.maxDepth),  FONT_SIZE_SMALL, {xMx, y3}, m_palette.text);
    drawText(window, std::to_string(s.avgDepth),  FONT_SIZE_SMALL, {xAv, y3}, m_palette.text);
}

void PanelRenderer::drawTurnButton(sf::RenderWindow& window, const UIState& state) const {
    sf::RectangleShape btn({PANEL_WIDTH, TURN_BUTTON_HEIGHT});
    btn.setPosition({PANEL_X, Y_TURN});
    btn.setFillColor(m_palette.accent);
    window.draw(btn);

    const float cy = Y_TURN + TURN_BUTTON_HEIGHT / 2.f;

    if (!state.isGameOver) {
        const bool    isBlack = state.currentPlayer == Player::BLACK;
        const sf::Color fill    = isBlack ? sf::Color::Black : sf::Color::White;
        const sf::Color outline = isBlack ? sf::Color(130, 130, 130) : sf::Color::Black;
        drawSmallCircle(window, {PANEL_X + TURN_STONE_PAD, cy}, TURN_STONE_RADIUS, fill, outline);
    }

    sf::Text label(m_font, state.turnLabel, FONT_SIZE_TURN);
    auto bounds = label.getLocalBounds();
    label.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                     bounds.position.y + bounds.size.y / 2.f});
    label.setPosition({PANEL_X + PANEL_WIDTH / 2.f, cy});
    label.setFillColor(sf::Color::White);
    window.draw(label);
}

void PanelRenderer::drawWarningBox(sf::RenderWindow& window, const UIState& state) const {
    sf::RectangleShape box({PANEL_WIDTH, WARNING_HEIGHT});
    box.setPosition({PANEL_X, Y_WARNING});
    box.setFillColor(m_palette.warningBackground);
    box.setOutlineColor(m_palette.accentWarning);
    box.setOutlineThickness(WARNING_OUTLINE_WIDTH);
    window.draw(box);

    drawText(window, "! Warning", FONT_SIZE_LABEL,
             {PANEL_X + WARNING_PADDING_X, Y_WARNING + WARNING_PADDING_TOP}, m_palette.warningText);

    const std::string& msg = *state.warningMessage;
    if (msg.size() <= WARNING_WRAP_LENGTH) {
        drawText(window, msg, FONT_SIZE_SMALL,
                 {PANEL_X + WARNING_PADDING_X, Y_WARNING + WARNING_BODY_Y}, m_palette.text);
    } else {
        auto mid = msg.rfind(' ', WARNING_WRAP_LENGTH);
        if (mid == std::string::npos) mid = 40;
        drawText(window, msg.substr(0, mid),  FONT_SIZE_SMALL,
                 {PANEL_X + WARNING_PADDING_X, Y_WARNING + WARNING_BODY_Y},  m_palette.text);
        drawText(window, msg.substr(mid + 1), FONT_SIZE_SMALL,
                 {PANEL_X + WARNING_PADDING_X, Y_WARNING + WARNING_BODY_Y2}, m_palette.text);
    }
}

void PanelRenderer::drawCellInfo(sf::RenderWindow& window, const UIState& state) const {
    const float y    = state.warningMessage.has_value() ? Y_CELL_INFO : Y_WARNING;
    const auto& info = *state.hoveredCellInfo;

    drawText(window, "Cell info",     FONT_SIZE_LABEL, {PANEL_X, y},                    m_palette.accent);
    drawText(window, info.coordLabel, FONT_SIZE_SMALL, {PANEL_X, y + ROW_HEIGHT},        m_palette.text);

    const std::string legalStr = info.isLegal ? "Legal" : "Illegal: " + info.illegalReason;
    const sf::Color   legalCol = info.isLegal ? m_palette.accentActive : m_palette.accentDanger;
    drawText(window, legalStr, FONT_SIZE_SMALL, {PANEL_X, y + ROW_HEIGHT * 2}, legalCol);

    if (info.aiScore.has_value()) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << *info.aiScore;
        drawText(window, "Score: " + ss.str(), FONT_SIZE_SMALL,
                 {PANEL_X, y + ROW_HEIGHT * 3}, m_palette.textSecondary);
    }
}

void PanelRenderer::drawInfoRow(sf::RenderWindow& window, float y,
                                 const std::string& label, const std::string& value,
                                 sf::Color valueColor) const {
    drawText(window, label, FONT_SIZE_LABEL, {PANEL_X, y},                   m_palette.textSecondary);
    drawText(window, value, FONT_SIZE_VALUE, {PANEL_X + INFO_COLUMN_X, y},   valueColor);
}

void PanelRenderer::drawText(sf::RenderWindow& window, const std::string& text,
                               unsigned int size, sf::Vector2f pos, sf::Color color) const {
    sf::Text t(m_font, text, size);
    t.setPosition(pos);
    t.setFillColor(color);
    window.draw(t);
}

void PanelRenderer::drawSmallCircle(sf::RenderWindow& window, sf::Vector2f center,
                                     float radius, sf::Color fill, sf::Color outline) const {
    sf::CircleShape c(radius);
    c.setOrigin({radius, radius});
    c.setPosition(center);
    c.setFillColor(fill);
    c.setOutlineColor(outline);
    c.setOutlineThickness(1.f);
    window.draw(c);
}

std::string PanelRenderer::gameModeStr(GameMode mode) const {
    return mode == GameMode::HUMAN_VS_AI ? "Human vs AI" : "Human vs Human";
}

std::string PanelRenderer::openingRuleStr(OpeningRule rule) const {
    switch (rule) {
        case OpeningRule::STANDARD: return "Standard";
        case OpeningRule::PRO:      return "Pro";
        case OpeningRule::SWAP:     return "Swap";
        case OpeningRule::SWAP2:    return "Swap2";
    }
    return "Standard";
}
