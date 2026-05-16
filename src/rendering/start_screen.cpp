#include "rendering/start_screen.hpp"

StartScreen::StartScreen(const sf::Font& font, const Palette& palette)
    : m_font(font), m_palette(palette) {}

void StartScreen::reset() {
    m_selectedMode  = GameMode::HUMAN_VS_HUMAN;
    m_selectedColor = Player::BLACK;
    m_selectedRule  = OpeningRule::STANDARD;
    m_startRequested = false;
}

StartConfig StartScreen::getConfig() const {
    return {m_selectedMode, m_selectedColor, m_selectedRule};
}

void StartScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    const auto* click = event.getIf<sf::Event::MouseButtonPressed>();
    if (!click || click->button != sf::Mouse::Button::Left) return;

    sf::Vector2f pos = window.mapPixelToCoords(click->position);

    int idx = -1;
    if (hitTest(pos, m_modeRects, idx)) {
        m_selectedMode  = (idx == 0) ? GameMode::HUMAN_VS_HUMAN : GameMode::HUMAN_VS_AI;
        m_selectedColor = Player::BLACK; // reset to default whenever mode changes
        return;
    }
    if (m_selectedMode == GameMode::HUMAN_VS_AI && hitTest(pos, m_colorRects, idx)) {
        m_selectedColor = (idx == 0) ? Player::BLACK : Player::WHITE;
        return;
    }
    if (hitTest(pos, m_ruleRects, idx)) {
        static const OpeningRule rules[] = {
            OpeningRule::STANDARD, OpeningRule::PRO,
            OpeningRule::SWAP,     OpeningRule::SWAP2
        };
        m_selectedRule = rules[idx];
        return;
    }
    if (m_startRect.contains(pos)) {
        m_startRequested = true;
    }
}

void StartScreen::draw(sf::RenderWindow& window) const {
    drawBackground(window);

    auto winSize = window.getView().getSize();
    float cx = winSize.x / 2.f;
    float cy = winSize.y / 2.f;

    drawTitle(window, {cx, cy + GROUP_Y_TITLE});

    float groupX = cx - (BUTTON_WIDTH + BUTTON_GAP / 2.f);

    drawGroup(window, {groupX, cy + GROUP_Y_MODE}, "Game mode",
              {"Human vs Human", "Human vs AI"},
              m_modeRects,
              m_selectedMode == GameMode::HUMAN_VS_HUMAN ? 0 : 1);

    if (m_selectedMode == GameMode::HUMAN_VS_AI) {
        drawGroup(window, {groupX, cy + GROUP_Y_COLOR}, "Who starts?",
                  {"Human first", "AI first"},
                  m_colorRects,
                  m_selectedColor == Player::BLACK ? 0 : 1);
    } else {
        m_colorRects.clear(); // ensure no stale hit-rects in HvH mode
    }

    drawGroup(window, {cx - (BUTTON_WIDTH * 2 + BUTTON_GAP * 1.5f), cy + GROUP_Y_RULES}, "Opening rule",
              {"Standard", "Pro", "Swap", "Swap2"},
              m_ruleRects,
              static_cast<int>(m_selectedRule));

    drawStartButton(window, {cx, cy + GROUP_Y_START});
}

void StartScreen::drawBackground(sf::RenderWindow& window) const {
    auto winSize = window.getView().getSize();
    sf::RectangleShape bg({winSize.x, winSize.y});
    bg.setFillColor(m_palette.surface);
    window.draw(bg);
}

void StartScreen::drawTitle(sf::RenderWindow& window, sf::Vector2f pos) const {
    sf::Text title(m_font, TITLE_TEXT, FONT_SIZE_TITLE);
    title.setFillColor(m_palette.text);
    auto bounds = title.getLocalBounds();
    title.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y});
    title.setPosition(pos);
    window.draw(title);
}

void StartScreen::drawGroup(sf::RenderWindow& window, sf::Vector2f origin,
                             const std::string& groupLabel,
                             const std::vector<std::string>& labels,
                             std::vector<sf::FloatRect>& rects,
                             int selected) const {
    sf::Text lbl(m_font, groupLabel, FONT_SIZE_GROUP_LABEL);
    lbl.setFillColor(m_palette.textSecondary);
    lbl.setPosition({origin.x, origin.y});
    window.draw(lbl);

    rects.clear();
    for (int i = 0; i < static_cast<int>(labels.size()); ++i) {
        sf::FloatRect rect{
            {origin.x + i * (BUTTON_WIDTH + BUTTON_GAP), origin.y + GROUP_BUTTON_Y_OFFSET},
            {BUTTON_WIDTH, BUTTON_HEIGHT}
        };
        rects.push_back(rect);
        drawButton(window, rect, labels[i], i == selected);
    }
}

void StartScreen::drawButton(sf::RenderWindow& window, sf::FloatRect rect,
                              const std::string& label, bool selected) const {
    sf::RectangleShape btn(rect.size);
    btn.setPosition(rect.position);
    btn.setFillColor(selected ? m_palette.accent : m_palette.border);
    window.draw(btn);

    sf::Text text(m_font, label, FONT_SIZE_BUTTON);
    text.setFillColor(m_palette.text);
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({rect.position.x + rect.size.x / 2.f,
                      rect.position.y + rect.size.y / 2.f});
    window.draw(text);
}

void StartScreen::drawStartButton(sf::RenderWindow& window, sf::Vector2f center) const {
    m_startRect = {{center.x - START_BUTTON_WIDTH / 2.f, center.y - START_BUTTON_HEIGHT / 2.f},
                   {START_BUTTON_WIDTH, START_BUTTON_HEIGHT}};

    sf::RectangleShape btn(m_startRect.size);
    btn.setPosition(m_startRect.position);
    btn.setFillColor(m_palette.accentActive);
    window.draw(btn);

    sf::Text text(m_font, START_BUTTON_LABEL, FONT_SIZE_START_BUTTON);
    text.setFillColor(sf::Color::White);
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f});
    text.setPosition(center);
    window.draw(text);
}

bool StartScreen::hitTest(sf::Vector2f pos, const std::vector<sf::FloatRect>& rects, int& idx) const {
    for (int i = 0; i < static_cast<int>(rects.size()); ++i) {
        if (rects[i].contains(pos)) {
            idx = i;
            return true;
        }
    }
    return false;
}
