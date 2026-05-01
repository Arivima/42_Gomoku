
//TODO refaire AI thinking

#include "game_ui.hpp"


void GameUI::drawUI(
    sf::RenderWindow& window,
    const Game& game,
    const sf::Font& font,
    float aiTimeMs
) const {
    drawTitle(window, font);
    drawStatus(window, game, font);
    drawStats(window, game, font);
    drawAIInfo(window, font, aiTimeMs);
    drawHelp(window, font);
}

void GameUI::drawTitle(sf::RenderWindow& window, const sf::Font& font) const {
    drawText(window, font, "Gomoku", FONT_SIZE_TITLE, {UI_X, UI_TITLE_Y}, sf::Color::White);
}

void GameUI::drawStatus(
    sf::RenderWindow& window,
    const Game& game,
    const sf::Font& font
) const {
    std::string statusStr;
    sf::Color statusColor = sf::Color::White;

    if (game.isGameOver()) {
        if (game.getStatus() == GameStatus::BLACK_WINS) {
            statusStr = "Noirs gagnants!";
            statusColor = sf::Color::White;
        } else if (game.getStatus() == GameStatus::WHITE_WINS) {
            statusStr = "Blancs gagnants!";
            statusColor = sf::Color::White;
        } else {
            statusStr = "Match nul!";
            statusColor = sf::Color::Yellow;
        }
    } else {
        statusStr = game.getCurrentPlayer() == Player::BLACK
            ? "Au tour des noirs de jouer"
            : "Au tour des blancs de jouer";
    }

    drawText(window, font, statusStr, FONT_SIZE_STATUS, {UI_X, UI_STATUS_Y}, statusColor);
}

void GameUI::drawStats(
    sf::RenderWindow& window,
    const Game& game,
    const sf::Font& font
) const {
    const GomokuBoard& board = game.getBoard();

    drawText(window, font,
        "Captures Noires: " + std::to_string(board.getBlackCaptures()),
        FONT_SIZE_STATS, {UI_X, UI_STATS_BLACK_Y}, sf::Color::White);

    drawText(window, font,
        "Captures Blanches: " + std::to_string(board.getWhiteCaptures()),
        FONT_SIZE_STATS, {UI_X, UI_STATS_WHITE_Y}, sf::Color::White);

    drawText(window, font,
        "Coups: " + std::to_string(board.getMoveCount()),
        FONT_SIZE_STATS, {UI_X, UI_STATS_MOVES_Y}, sf::Color::White);
}

// a refaire AI thinking
void GameUI::drawAIInfo(
    sf::RenderWindow& window,
    const sf::Font& font,
    float aiTimeMs
) const {

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << aiTimeMs << " ms";

    drawText(window, font,
        "Temps AI: " + oss.str(),
        FONT_SIZE_AI_INFO,
        {UI_X, UI_AI_INFO_Y},
        sf::Color::White
    );

}

void GameUI::drawHelp(sf::RenderWindow& window, const sf::Font& font) const {
    drawText(window, font, "Touches clavier:", FONT_SIZE_HELP, {UI_X, UI_HELP_Y}, sf::Color::White);
    drawText(window, font, "R : reinitialiser le jeu", FONT_SIZE_HELP, {UI_X, UI_HELP_Y + 20}, sf::Color::White);
}

void GameUI::drawText(
    sf::RenderWindow& window,
    const sf::Font& font,
    const std::string& text,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color
) const {
    sf::Text bodyText(font, text, size);
    bodyText.setPosition(position);
    bodyText.setFillColor(color);
    window.draw(bodyText);
}
