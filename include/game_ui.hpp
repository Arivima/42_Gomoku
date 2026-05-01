#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iomanip>
#include <sstream>

#include "game.hpp"
#include "board.hpp"

class GameUI {
public:
    void drawUI(sf::RenderWindow& window, const Game& game, const sf::Font& font, float aiTimeMs) const;

private:
    static constexpr float UI_X = 650.f;
    static constexpr float UI_TITLE_Y = 20.f;
    static constexpr float UI_STATUS_Y = 70.f;
    static constexpr float UI_STATS_BLACK_Y = 110.f;
    static constexpr float UI_STATS_WHITE_Y = 135.f;
    static constexpr float UI_STATS_MOVES_Y = 165.f;
    static constexpr float UI_AI_INFO_Y = 210.f;
    static constexpr float UI_AI_TIME_Y = 235.f;
    static constexpr float UI_HELP_Y = 300.f;

    static constexpr int FONT_SIZE_TITLE = 28;
    static constexpr int FONT_SIZE_STATUS = 20;
    static constexpr int FONT_SIZE_STATS = 16;
    static constexpr int FONT_SIZE_AI_INFO = 16;
    static constexpr int FONT_SIZE_AI_TIME = 16;
    static constexpr int FONT_SIZE_HELP = 14;


    void drawTitle(sf::RenderWindow& window, const sf::Font& font) const;

    void drawStatus(sf::RenderWindow& window, const Game& game, const sf::Font& font) const;

    void drawStats(sf::RenderWindow& window, const Game& game, const sf::Font& font) const;

    void drawAIInfo(sf::RenderWindow& window, const sf::Font& font, float aiTimeMs) const;

    void drawHelp(sf::RenderWindow& window, const sf::Font& font) const;

    void drawText(
        sf::RenderWindow& window,
        const sf::Font& font,
        const std::string& text,
        unsigned int size,
        sf::Vector2f position,
        sf::Color color
    ) const;
};