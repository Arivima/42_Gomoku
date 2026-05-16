#pragma once

#include <SFML/Graphics.hpp>
#include "core/board.hpp"
#include "ui/ui_state.hpp"
#include "ui/palette.hpp"

class BoardRenderer {
public:
    explicit BoardRenderer(const Palette& palette);

    void drawBoard(sf::RenderWindow& window, const sf::Font& font);
    void drawStones(sf::RenderWindow& window, const GomokuBoard& board,
                    const sf::Font& font, bool showMoveNumbers);
    void drawOverlays(sf::RenderWindow& window, const UIState& state);
    void drawHover(sf::RenderWindow& window, const UIState& state);

    Position     screenToBoard(sf::Vector2f screenPos) const;
    sf::Vector2f boardToScreen(Position boardPos) const;

    void         setCellSize(float size);
    float        getCellSize() const { return m_cellSize; }
    void         setBoardOffset(float offsetX, float offsetY);
    sf::Vector2f getBoardOffset() const { return {m_offsetX, m_offsetY}; }

    // ── Public layout constants (used by GomokuApp for offset computation) ────
    static constexpr float DEFAULT_CELL_SIZE    = 40.0f;
    static constexpr float PADDING              = 20.0f;

private:
    const Palette& m_palette;

    float m_cellSize;
    float m_offsetX;
    float m_offsetY;
    float m_stoneRadius;
    float m_lineWidth;

    // ── Default/fallback values ───────────────────────────────────────────────
    static constexpr float DEFAULT_OFFSET_X     = 50.0f;
    static constexpr float DEFAULT_OFFSET_Y     = 50.0f;
    static constexpr float DEFAULT_STONE_RADIUS = 13.0f;
    static constexpr float DEFAULT_LINE_WIDTH   = 1.5f;

    // ── Label rendering ───────────────────────────────────────────────────────
    static constexpr float        LABEL_OFFSET        = 18.0f;
    static constexpr unsigned int LABEL_FONT_SIZE      = 14u;
    static constexpr const char*  COL_LABELS           = "ABCDEFGHIJKLMNOPQRS";

    // ── Star points (0-indexed positions for 19x19: 4th, 10th, 16th lines) ───
    static constexpr int          STAR_POINTS[3]       = {3, 9, 15};
    static constexpr float        STAR_RADIUS           = 4.0f;

    // ── Stone geometry ────────────────────────────────────────────────────────
    static constexpr float        STONE_RADIUS_RATIO   = 0.43f;
    static constexpr float        STONE_OUTLINE_WIDTH  = 1.0f;
    static constexpr unsigned     GHOST_ALPHA          = 100u;

    // ── Move number text ──────────────────────────────────────────────────────
    static constexpr unsigned int MOVE_NUM_FONT_SIZE   = 10u;

    // ── Ring overlay ─────────────────────────────────────────────────────────
    static constexpr float        RING_EXTRA           = 1.0f;
    static constexpr float        RING_THICKNESS       = 3.0f;

    sf::CircleShape createStoneShape(Position pos, Cell cell) const;
    void            drawCoordinateLabels(sf::RenderWindow& window, const sf::Font& font) const;
    void            drawRing(sf::RenderWindow& window, Position pos, sf::Color color) const;
};
