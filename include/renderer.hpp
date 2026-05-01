#pragma once

#include <SFML/Graphics.hpp>
#include "board.hpp"

class Renderer {
public:
    Renderer();

    void drawBoard(sf::RenderWindow& window);
    void drawStones(sf::RenderWindow& window, const GomokuBoard& board);
    void drawLastMove(sf::RenderWindow& window, const GomokuBoard& board);

    Position screenToBoard(sf::Vector2f screenPos) const;
    sf::Vector2f boardToScreen(Position boardPos) const;

    void setCellSize(float size);
    float getCellSize() const { return m_cellSize; }

    void setBoardOffset(float offsetX, float offsetY);
    sf::Vector2f getBoardOffset() const { return {m_offsetX, m_offsetY}; }

private:
    float m_cellSize;
    float m_offsetX;
    float m_offsetY;
    float m_stoneRadius;
    float m_lineWidth;

    static constexpr float DEFAULT_CELL_SIZE = 40.0f;
    static constexpr float DEFAULT_OFFSET_X = 50.0f;
    static constexpr float DEFAULT_OFFSET_Y = 50.0f;
    static constexpr float DEFAULT_STONE_RADIUS = 13.0f;
    static constexpr float DEFAULT_LINE_WIDTH = 1.5f;
    static constexpr sf::Color BOARD_COLOR = sf::Color(240, 200, 150);
    static constexpr sf::Color LINE_COLOR = sf::Color(0, 0, 0);
    static constexpr float PADDING = 20.0f;

    static constexpr sf::Color LAST_MOVE_COLOR = sf::Color(255, 100, 100);
    static constexpr sf::Color GRID_COLOR = sf::Color(0, 0, 0);
    static constexpr sf::Color BACKGROUND_COLOR = sf::Color(222, 184, 135);
    
    sf::CircleShape createStoneShape(Position pos, Cell cell) const;
};
