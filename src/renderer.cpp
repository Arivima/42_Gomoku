// TODO attention quand on change la taille de la fenêtre, il faut recalculer les offsets et la taille des cellules

#include "renderer.hpp"

Renderer::Renderer()
    : m_cellSize(DEFAULT_CELL_SIZE)
    , m_offsetX(DEFAULT_OFFSET_X)
    , m_offsetY(DEFAULT_OFFSET_Y)
    , m_stoneRadius(DEFAULT_STONE_RADIUS)
    , m_lineWidth(DEFAULT_LINE_WIDTH) {
}

void Renderer::drawBoard(sf::RenderWindow& window) {
    const float gridSize = m_cellSize * (GomokuBoard::BOARD_SIZE - 1);
    const float padding = PADDING;

    sf::RectangleShape boardBg({gridSize + padding * 2.0f, gridSize + padding * 2.0f});
    boardBg.setPosition({m_offsetX - padding, m_offsetY - padding});
    boardBg.setFillColor(BOARD_COLOR);
    window.draw(boardBg);

    sf::RectangleShape line;
    line.setFillColor(LINE_COLOR);

    for (int i = 0; i < GomokuBoard::BOARD_SIZE; ++i) {
        const float x = m_offsetX + i * m_cellSize;
        const float y = m_offsetY + i * m_cellSize;

        // Ligne horizontale
        line.setSize({gridSize, m_lineWidth});
        line.setPosition({m_offsetX, y - m_lineWidth / 2.0f});
        window.draw(line);

        // Ligne verticale
        line.setSize({m_lineWidth, gridSize});
        line.setPosition({x - m_lineWidth / 2.0f, m_offsetY});
        window.draw(line);
    }

    // Points
    static const int starPoints[] = {3, 9, 15};

    sf::CircleShape star(4.0f);
    star.setOrigin({4.0f, 4.0f});
    star.setFillColor(sf::Color::Black);

    for (int x : starPoints) {
        for (int y : starPoints) {
            star.setPosition({
                m_offsetX + x * m_cellSize,
                m_offsetY + y * m_cellSize
            });
            window.draw(star);
        }
    }
}

void Renderer::drawStones(sf::RenderWindow& window, const GomokuBoard& board) {
    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            const Cell cell = board.getCell(x, y);

            if (cell != Cell::EMPTY) {
                const auto stone = createStoneShape({x, y}, cell);
                window.draw(stone);
            }
        }
    }
}

void Renderer::drawLastMove(sf::RenderWindow& window, const GomokuBoard& board) {
    // WIP
    // Position lastMove = board.getLastMove();

    (void)window;
    (void)board;
}

sf::CircleShape Renderer::createStoneShape(Position pos, Cell cell) const {
    const float px = m_offsetX + pos.x * m_cellSize;
    const float py = m_offsetY + pos.y * m_cellSize;

    sf::CircleShape stone(m_stoneRadius);
    stone.setOrigin({m_stoneRadius, m_stoneRadius});
    stone.setPosition({px, py});

    if (cell == Cell::BLACK) {
        stone.setFillColor(sf::Color::Black);
    } else {
        stone.setFillColor(sf::Color::White);
        stone.setOutlineThickness(1.0f);
        stone.setOutlineColor(sf::Color::Black);
    }

    return stone;
}

Position Renderer::screenToBoard(sf::Vector2f screenPos) const {
    const int x = static_cast<int>((screenPos.x - m_offsetX + m_cellSize / 2.0f) / m_cellSize);
    const int y = static_cast<int>((screenPos.y - m_offsetY + m_cellSize / 2.0f) / m_cellSize);

    return {x, y};
}

sf::Vector2f Renderer::boardToScreen(Position boardPos) const {
    return {
        m_offsetX + boardPos.x * m_cellSize,
        m_offsetY + boardPos.y * m_cellSize
    };
}

void Renderer::setCellSize(float size) {
    m_cellSize = size;
    m_stoneRadius = size * 0.43f;
}

void Renderer::setBoardOffset(float offsetX, float offsetY) {
    m_offsetX = offsetX;
    m_offsetY = offsetY;
}