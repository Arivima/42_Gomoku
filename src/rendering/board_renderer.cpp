#include "rendering/board_renderer.hpp"
#include <cstdint>
#include <string>

BoardRenderer::BoardRenderer(const Palette& palette)
    : m_palette(palette)
    , m_cellSize(DEFAULT_CELL_SIZE)
    , m_offsetX(DEFAULT_OFFSET_X)
    , m_offsetY(DEFAULT_OFFSET_Y)
    , m_stoneRadius(DEFAULT_STONE_RADIUS)
    , m_lineWidth(DEFAULT_LINE_WIDTH) {
}

void BoardRenderer::drawBoard(sf::RenderWindow& window, const sf::Font& font) {
    const float gridSize = m_cellSize * (GomokuBoard::BOARD_SIZE - 1);

    sf::RectangleShape boardBg({gridSize + PADDING * 2.f, gridSize + PADDING * 2.f});
    boardBg.setPosition({m_offsetX - PADDING, m_offsetY - PADDING});
    boardBg.setFillColor(m_palette.boardBackground);
    window.draw(boardBg);

    sf::RectangleShape line;
    line.setFillColor(m_palette.boardLine);

    for (int i = 0; i < GomokuBoard::BOARD_SIZE; ++i) {
        const float x = m_offsetX + i * m_cellSize;
        const float y = m_offsetY + i * m_cellSize;

        line.setSize({gridSize, m_lineWidth});
        line.setPosition({m_offsetX, y - m_lineWidth / 2.f});
        window.draw(line);

        line.setSize({m_lineWidth, gridSize});
        line.setPosition({x - m_lineWidth / 2.f, m_offsetY});
        window.draw(line);
    }

    sf::CircleShape star(STAR_RADIUS);
    star.setOrigin({STAR_RADIUS, STAR_RADIUS});
    star.setFillColor(m_palette.boardLine);

    for (int sx : STAR_POINTS) {
        for (int sy : STAR_POINTS) {
            star.setPosition({m_offsetX + sx * m_cellSize, m_offsetY + sy * m_cellSize});
            window.draw(star);
        }
    }

    drawCoordinateLabels(window, font);
}

void BoardRenderer::drawCoordinateLabels(sf::RenderWindow& window, const sf::Font& font) const {
    for (int i = 0; i < GomokuBoard::BOARD_SIZE; ++i) {
        sf::Text colLabel(font, std::string(1, COL_LABELS[i]), LABEL_FONT_SIZE);
        colLabel.setFillColor(m_palette.boardCoordinate);
        auto cb = colLabel.getLocalBounds();
        colLabel.setOrigin({cb.position.x + cb.size.x / 2.f, cb.position.y + cb.size.y / 2.f});
        colLabel.setPosition({m_offsetX + i * m_cellSize, m_offsetY - LABEL_OFFSET});
        window.draw(colLabel);

        sf::Text rowLabel(font, std::to_string(i + 1), LABEL_FONT_SIZE);
        rowLabel.setFillColor(m_palette.boardCoordinate);
        auto rb = rowLabel.getLocalBounds();
        rowLabel.setOrigin({rb.position.x + rb.size.x / 2.f, rb.position.y + rb.size.y / 2.f});
        rowLabel.setPosition({m_offsetX - LABEL_OFFSET, m_offsetY + i * m_cellSize});
        window.draw(rowLabel);
    }
}

void BoardRenderer::drawStones(sf::RenderWindow& window, const GomokuBoard& board,
                                const sf::Font& font, bool showMoveNumbers) {
    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            const Cell cell = board.getCell(x, y);
            if (cell == Cell::EMPTY) continue;

            window.draw(createStoneShape({x, y}, cell));

            if (showMoveNumbers) {
                int num = board.getMoveNumber({x, y});
                if (num > 0) {
                    sf::Text numText(font, std::to_string(num), MOVE_NUM_FONT_SIZE);
                    numText.setFillColor(cell == Cell::BLACK ? sf::Color::White : sf::Color::Black);
                    auto bounds = numText.getLocalBounds();
                    numText.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                                       bounds.position.y + bounds.size.y / 2.f});
                    auto [px, py] = boardToScreen({x, y});
                    numText.setPosition({px, py});
                    window.draw(numText);
                }
            }
        }
    }
}

void BoardRenderer::drawOverlays(sf::RenderWindow& window, const UIState& state) {
    for (const auto& overlay : state.overlays) {
        sf::Color color;
        switch (overlay.kind) {
            case UIState::OverlayKind::LAST_MOVE:        color = m_palette.ringLastMove;   break;
            case UIState::OverlayKind::FORBIDDEN:        color = m_palette.ringForbidden;  break;
            case UIState::OverlayKind::THREAT:           color = m_palette.ringThreat;     break;
            case UIState::OverlayKind::COACH_SUGGESTION: color = m_palette.ringSuggestion; break;
        }
        drawRing(window, overlay.pos, color);
    }
}

void BoardRenderer::drawHover(sf::RenderWindow& window, const UIState& state) {
    if (!state.hoverPos.has_value()) return;

    const auto [px, py] = boardToScreen(*state.hoverPos);
    const auto alpha = static_cast<std::uint8_t>(GHOST_ALPHA);

    sf::Color ghostColor = (state.currentPlayer == Player::BLACK)
        ? sf::Color(0, 0, 0, alpha)
        : sf::Color(255, 255, 255, alpha);

    sf::CircleShape ghost(m_stoneRadius);
    ghost.setOrigin({m_stoneRadius, m_stoneRadius});
    ghost.setPosition({px, py});
    ghost.setFillColor(ghostColor);
    window.draw(ghost);
}

void BoardRenderer::drawRing(sf::RenderWindow& window, Position pos, sf::Color color) const {
    const float r = m_stoneRadius + RING_EXTRA;
    sf::CircleShape ring(r);
    ring.setOrigin({r, r});
    auto [px, py] = boardToScreen(pos);
    ring.setPosition({px, py});
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineColor(color);
    ring.setOutlineThickness(RING_THICKNESS);
    window.draw(ring);
}

sf::CircleShape BoardRenderer::createStoneShape(Position pos, Cell cell) const {
    const auto [px, py] = boardToScreen(pos);

    sf::CircleShape stone(m_stoneRadius);
    stone.setOrigin({m_stoneRadius, m_stoneRadius});
    stone.setPosition({px, py});

    if (cell == Cell::BLACK) {
        stone.setFillColor(sf::Color::Black);
    } else {
        stone.setFillColor(sf::Color::White);
        stone.setOutlineThickness(STONE_OUTLINE_WIDTH);
        stone.setOutlineColor(sf::Color::Black);
    }

    return stone;
}

Position BoardRenderer::screenToBoard(sf::Vector2f screenPos) const {
    const int x = static_cast<int>((screenPos.x - m_offsetX + m_cellSize / 2.f) / m_cellSize);
    const int y = static_cast<int>((screenPos.y - m_offsetY + m_cellSize / 2.f) / m_cellSize);
    return {x, y};
}

sf::Vector2f BoardRenderer::boardToScreen(Position boardPos) const {
    return {m_offsetX + boardPos.x * m_cellSize, m_offsetY + boardPos.y * m_cellSize};
}

void BoardRenderer::setCellSize(float size) {
    m_cellSize    = size;
    m_stoneRadius = size * STONE_RADIUS_RATIO;
}

void BoardRenderer::setBoardOffset(float offsetX, float offsetY) {
    m_offsetX = offsetX;
    m_offsetY = offsetY;
}
