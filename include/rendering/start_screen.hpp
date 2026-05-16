#pragma once

#include <SFML/Graphics.hpp>
#include "core/game.hpp"
#include "ui/palette.hpp"
#include <vector>
#include <string>

struct StartConfig {
    GameMode mode = GameMode::HUMAN_VS_HUMAN;
    Player humanColor = Player::BLACK;
    OpeningRule openingRule = OpeningRule::STANDARD;
};

class StartScreen {
public:
    StartScreen(const sf::Font& font, const Palette& palette);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    bool isStartRequested() const { return m_startRequested; }
    StartConfig getConfig() const;
    void reset();

private:
    const sf::Font&    m_font;
    const Palette&     m_palette;

    GameMode    m_selectedMode  = GameMode::HUMAN_VS_HUMAN;
    Player      m_selectedColor = Player::BLACK;
    OpeningRule m_selectedRule  = OpeningRule::STANDARD;
    bool        m_startRequested = false;

    mutable std::vector<sf::FloatRect> m_modeRects;
    mutable std::vector<sf::FloatRect> m_colorRects;
    mutable std::vector<sf::FloatRect> m_ruleRects;
    mutable sf::FloatRect m_startRect;

    // ── Button dimensions ─────────────────────────────────────────────────────
    static constexpr float BUTTON_WIDTH  = 160.f;
    static constexpr float BUTTON_HEIGHT = 42.f;
    static constexpr float BUTTON_GAP   = 14.f;

    // ── Start button ──────────────────────────────────────────────────────────
    static constexpr float START_BUTTON_WIDTH  = 220.f;
    static constexpr float START_BUTTON_HEIGHT = 54.f;

    // ── Group layout (Y relative to window centre) ────────────────────────────
    static constexpr float GROUP_Y_TITLE         = -240.f;
    static constexpr float GROUP_Y_MODE          = -140.f;
    static constexpr float GROUP_Y_COLOR         =  -40.f;
    static constexpr float GROUP_Y_RULES         =   60.f;
    static constexpr float GROUP_Y_START         =  180.f;
    static constexpr float GROUP_BUTTON_Y_OFFSET =   22.f;  // button top below group label

    // ── Font sizes ────────────────────────────────────────────────────────────
    static constexpr unsigned int FONT_SIZE_TITLE        = 48u;
    static constexpr unsigned int FONT_SIZE_GROUP_LABEL  = 14u;
    static constexpr unsigned int FONT_SIZE_BUTTON       = 15u;
    static constexpr unsigned int FONT_SIZE_START_BUTTON = 20u;

    // ── Labels ────────────────────────────────────────────────────────────────
    static constexpr const char* TITLE_TEXT         = "Gomoku";
    static constexpr const char* START_BUTTON_LABEL = "Start game";

    void drawBackground(sf::RenderWindow& window) const;
    void drawTitle(sf::RenderWindow& window, sf::Vector2f center) const;
    void drawGroup(sf::RenderWindow& window, sf::Vector2f origin,
                   const std::string& groupLabel,
                   const std::vector<std::string>& labels,
                   std::vector<sf::FloatRect>& rects,
                   int selected) const;
    void drawButton(sf::RenderWindow& window, sf::FloatRect rect,
                    const std::string& label, bool selected) const;
    void drawStartButton(sf::RenderWindow& window, sf::Vector2f center) const;

    bool hitTest(sf::Vector2f pos, const std::vector<sf::FloatRect>& rects, int& idx) const;
};
