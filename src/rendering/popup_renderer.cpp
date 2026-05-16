#include "rendering/popup_renderer.hpp"
#include <algorithm>
#include <string>

PopupRenderer::PopupRenderer(const sf::Font& font, const Palette& palette)
    : m_font(font), m_palette(palette) {}

void PopupRenderer::draw(sf::RenderWindow& window, const UIState& state) const {
    if (state.activePopup == UIState::ActivePopup::NONE) return;

    switch (state.activePopup) {
        case UIState::ActivePopup::ILLEGAL_MOVE:
            drawPopupBase(window, "Illegal move", state.popupMessage, m_palette.accentDanger);
            return;
        case UIState::ActivePopup::ENDGAME_CAPTURE:
            drawPopupBase(window, "Endgame capture rule", state.popupMessage, m_palette.accentWarning);
            return;
        case UIState::ActivePopup::GAME_OVER:
            drawPopupBase(window, "Game over", state.popupMessage, m_palette.accentActive);
            return;
        case UIState::ActivePopup::RULES:
            drawRulesPopup(window, state);
            return;
        case UIState::ActivePopup::HISTORY:
            drawHistoryPopup(window, state);
            return;
        default:
            return;
    }
}

void PopupRenderer::drawRulesPopup(sf::RenderWindow& window, const UIState& state) const {
    const auto  winSize = window.getView().getSize();
    const float wx = winSize.x;
    const float wy = winSize.y;

    sf::RectangleShape backdrop({wx, wy});
    backdrop.setFillColor(m_palette.popupBackdrop);
    window.draw(backdrop);

    const float cx = std::min(wx * BOARD_AREA_RATIO, BOARD_AREA_MAX_X);
    const float cy = wy / 2.f;
    const sf::Color accent = m_palette.accentActive;

    sf::RectangleShape box({RULES_WIDTH, RULES_HEIGHT});
    box.setOrigin({RULES_WIDTH / 2.f, RULES_HEIGHT / 2.f});
    box.setPosition({cx, cy});
    box.setFillColor(m_palette.surface);
    box.setOutlineColor(accent);
    box.setOutlineThickness(POPUP_OUTLINE_WIDTH);
    window.draw(box);

    sf::RectangleShape topBar({RULES_WIDTH, TOP_BAR_HEIGHT});
    topBar.setOrigin({RULES_WIDTH / 2.f, TOP_BAR_HEIGHT / 2.f});
    topBar.setPosition({cx, cy - RULES_HEIGHT / 2.f});
    topBar.setFillColor(accent);
    window.draw(topBar);

    const float lx  = cx - RULES_WIDTH / 2.f + PADDING;
    const float vx  = cx - RULES_WIDTH / 2.f + RULES_VALUE_X;
    float       y   = cy - RULES_HEIGHT / 2.f + TITLE_Y;

    drawText(window, "Rules", FONT_SIZE_TITLE, {lx, y}, accent);
    y += RULES_LINE_H + 8.f;

    // Game mode
    const bool isAI = (state.gameMode == GameMode::HUMAN_VS_AI);
    drawText(window, "Game mode:", FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
    drawText(window, isAI ? "Human vs AI" : "Human vs Human", FONT_SIZE_BODY, {vx, y}, m_palette.text);
    y += RULES_LINE_H;

    if (isAI) {
        const bool humanIsBlack = (state.humanColor == Player::BLACK);
        drawText(window, "Human plays:", FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
        drawText(window, humanIsBlack ? "Black (starts first)" : "White (starts second)",
                 FONT_SIZE_BODY, {vx, y}, m_palette.text);
        y += RULES_LINE_H;
        drawText(window, "AI plays:", FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
        drawText(window, humanIsBlack ? "White" : "Black (starts first)",
                 FONT_SIZE_BODY, {vx, y}, m_palette.text);
        y += RULES_LINE_H;
    } else {
        drawText(window, "Player 1:", FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
        drawText(window, "Black (starts first)", FONT_SIZE_BODY, {vx, y}, m_palette.text);
        y += RULES_LINE_H;
        drawText(window, "Player 2:", FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
        drawText(window, "White", FONT_SIZE_BODY, {vx, y}, m_palette.text);
        y += RULES_LINE_H;
    }

    // Opening rule
    const char* ruleNames[] = {"Standard", "Pro", "Swap", "Swap2"};
    drawText(window, "Opening rule:", FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
    drawText(window, ruleNames[static_cast<int>(state.openingRule)], FONT_SIZE_BODY, {vx, y}, m_palette.text);
    y += RULES_LINE_H + 6.f;

    // Fixed rules
    struct RuleRow { const char* label; const char* value; };
    const RuleRow rows[] = {
        {"Win condition:",  "5 or more stones in a row"},
        {"Captures:",       "Pairs (flanking)"},
        {"Capture win:",    "10 stones (5 pairs)"},
        {"Double-three:",   "Forbidden"},
        {"Endgame capture:","Enabled"},
    };
    for (const auto& r : rows) {
        drawText(window, r.label, FONT_SIZE_BODY, {lx, y}, m_palette.textSecondary);
        drawText(window, r.value, FONT_SIZE_BODY, {vx, y}, m_palette.text);
        y += RULES_LINE_H;
    }

    drawText(window, "Press any key to close", FONT_SIZE_HINT,
             {lx, cy + RULES_HEIGHT / 2.f - 28.f}, m_palette.textSecondary);
}

void PopupRenderer::drawHistoryPopup(sf::RenderWindow& window, const UIState& state) const {
    const auto  winSize = window.getView().getSize();
    const float wx = winSize.x;
    const float wy = winSize.y;

    sf::RectangleShape backdrop({wx, wy});
    backdrop.setFillColor(m_palette.popupBackdrop);
    window.draw(backdrop);

    const float cx = std::min(wx * BOARD_AREA_RATIO, BOARD_AREA_MAX_X);
    const float cy = wy / 2.f;
    const sf::Color accent = m_palette.accentWarning;

    sf::RectangleShape box({HISTORY_WIDTH, HISTORY_HEIGHT});
    box.setOrigin({HISTORY_WIDTH / 2.f, HISTORY_HEIGHT / 2.f});
    box.setPosition({cx, cy});
    box.setFillColor(m_palette.surface);
    box.setOutlineColor(accent);
    box.setOutlineThickness(POPUP_OUTLINE_WIDTH);
    window.draw(box);

    sf::RectangleShape topBar({HISTORY_WIDTH, TOP_BAR_HEIGHT});
    topBar.setOrigin({HISTORY_WIDTH / 2.f, TOP_BAR_HEIGHT / 2.f});
    topBar.setPosition({cx, cy - HISTORY_HEIGHT / 2.f});
    topBar.setFillColor(accent);
    window.draw(topBar);

    const float lx = cx - HISTORY_WIDTH / 2.f + PADDING;
    const float ty = cy - HISTORY_HEIGHT / 2.f;

    drawText(window, "Move History", FONT_SIZE_TITLE, {lx, ty + TITLE_Y}, accent);

    // Column headers
    const float c1 = lx + HISTORY_COL_NUMBER,    c2 = lx + HISTORY_COL_TIMESTAMP,
                c3 = lx + HISTORY_COL_PLAYER,    c4 = lx + HISTORY_COL_COLOR,
                c5 = lx + HISTORY_COL_CELL,      c6 = lx + HISTORY_COL_EVENT;
    const float headerY = ty + HISTORY_HEADER_Y;
    drawText(window, "#",         FONT_SIZE_HINT, {c1, headerY}, m_palette.textSecondary);
    drawText(window, "Timestamp", FONT_SIZE_HINT, {c2, headerY}, m_palette.textSecondary);
    drawText(window, "Player",    FONT_SIZE_HINT, {c3, headerY}, m_palette.textSecondary);
    drawText(window, "Color",     FONT_SIZE_HINT, {c4, headerY}, m_palette.textSecondary);
    drawText(window, "Cell",      FONT_SIZE_HINT, {c5, headerY}, m_palette.textSecondary);
    drawText(window, "Event",     FONT_SIZE_HINT, {c6, headerY}, m_palette.textSecondary);

    // Separator line
    sf::RectangleShape sep({HISTORY_WIDTH - PADDING * 2.f, 1.f});
    sep.setPosition({lx, ty + HISTORY_HEADER_Y + 16.f});
    sep.setFillColor(m_palette.textSecondary);
    window.draw(sep);

    // Rows
    const auto& hist = state.moveHistory;
    const int total  = static_cast<int>(hist.size());
    const int start  = state.historyScrollOffset;
    const int end    = std::min(start + HISTORY_VISIBLE_ROWS, total);

    for (int i = start; i < end; ++i) {
        const auto& e = hist[i];
        const float ry = ty + HISTORY_ROWS_Y + (i - start) * HISTORY_ROW_HEIGHT;
        const sf::Color col = m_palette.text;
        drawText(window, std::to_string(e.num),  FONT_SIZE_HINT, {c1, ry}, col);
        drawText(window, e.timestamp,            FONT_SIZE_HINT, {c2, ry}, col);
        drawText(window, e.playerLabel,          FONT_SIZE_HINT, {c3, ry}, col);
        drawText(window, e.color,                FONT_SIZE_HINT, {c4, ry}, col);
        drawText(window, e.coord,                FONT_SIZE_HINT, {c5, ry}, col);
        drawText(window, e.event,                FONT_SIZE_HINT, {c6, ry}, m_palette.accentDanger);
    }

    // Footer: scroll hint + count
    const float footerY = ty + HISTORY_HEIGHT - 26.f;
    std::string scrollHint = "↑/↓ or scroll to navigate";
    if (total > 0) scrollHint += "  |  " + std::to_string(std::min(end, total)) + "/" + std::to_string(total);
    drawText(window, scrollHint, FONT_SIZE_HINT, {lx, footerY}, m_palette.textSecondary);
    drawText(window, "Any key to close", FONT_SIZE_HINT, {cx + HISTORY_WIDTH / 2.f - HISTORY_FOOTER_CLOSE_X, footerY}, m_palette.textSecondary);
}

void PopupRenderer::drawPopupBase(sf::RenderWindow& window,
                                   const std::string& title, const std::string& body,
                                   sf::Color accentColor) const {
    const auto  winSize = window.getView().getSize();
    const float wx = winSize.x;
    const float wy = winSize.y;

    sf::RectangleShape backdrop({wx, wy});
    backdrop.setFillColor(m_palette.popupBackdrop);
    window.draw(backdrop);

    const float cx = std::min(wx * BOARD_AREA_RATIO, BOARD_AREA_MAX_X);
    const float cy = wy / 2.f;

    sf::RectangleShape box({POPUP_WIDTH, POPUP_HEIGHT});
    box.setOrigin({POPUP_WIDTH / 2.f, POPUP_HEIGHT / 2.f});
    box.setPosition({cx, cy});
    box.setFillColor(m_palette.surface);
    box.setOutlineColor(accentColor);
    box.setOutlineThickness(POPUP_OUTLINE_WIDTH);
    window.draw(box);

    sf::RectangleShape topBar({POPUP_WIDTH, TOP_BAR_HEIGHT});
    topBar.setOrigin({POPUP_WIDTH / 2.f, TOP_BAR_HEIGHT / 2.f});
    topBar.setPosition({cx, cy - POPUP_HEIGHT / 2.f});
    topBar.setFillColor(accentColor);
    window.draw(topBar);

    const float textX = cx - POPUP_WIDTH / 2.f + PADDING;
    const float textY = cy - POPUP_HEIGHT / 2.f + TITLE_Y;

    drawText(window, title, FONT_SIZE_TITLE, {textX, textY}, accentColor);

    if (body.size() <= BODY_WRAP_LENGTH) {
        drawText(window, body, FONT_SIZE_BODY, {textX, textY + BODY_Y}, m_palette.text);
    } else {
        auto mid = body.rfind(' ', BODY_WRAP_LENGTH);
        if (mid == std::string::npos) mid = BODY_WRAP_LENGTH;
        drawText(window, body.substr(0, mid),  FONT_SIZE_BODY, {textX, textY + BODY_Y},  m_palette.text);
        drawText(window, body.substr(mid + 1), FONT_SIZE_BODY, {textX, textY + BODY_Y2}, m_palette.text);
    }

    drawText(window, "Click or press any key to dismiss",
             FONT_SIZE_HINT, {textX, textY + POPUP_HEIGHT - HINT_BOTTOM_OFFSET}, m_palette.textSecondary);
}

void PopupRenderer::drawText(sf::RenderWindow& window, const std::string& text,
                              unsigned int size, sf::Vector2f pos, sf::Color color) const {
    sf::Text t(m_font, text, size);
    t.setPosition(pos);
    t.setFillColor(color);
    window.draw(t);
}
