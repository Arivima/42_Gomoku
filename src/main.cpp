// STATUS
// Board ok
// event mouse click ok
// event keyboard R to reset ok
// move ok
// UI WIP
// win check NOK
// AI NOK

#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "game.hpp"
#include "renderer.hpp"
#include "game_ui.hpp"

class GomokuApp {
public:
    GomokuApp()
        : m_window(sf::VideoMode({1000, 700}), "Gomoku")
        , m_game()
        , m_renderer()
        , m_gameUI()
        , m_font()
        , m_aiTimeMs(0)
        {

        m_window.setFramerateLimit(60);

        loadFont();

    }

    void run() {
        while (m_window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:

    void loadFont() {
        if (!m_font.openFromFile("assets/fonts/Roboto-VariableFont_wdth,wght.ttf")) {
            std::cerr << "Warning: Could not load font" << std::endl;
        }
    }

    void processEvents() {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
            
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    handleMouseClick(mousePressed->position);
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    m_game.reset();
                }
            }
        }
    }


    void handleMouseClick(sf::Vector2i mousePos) {
        if (m_game.isGameOver()) return;

        Position boardPos = m_renderer.screenToBoard(sf::Vector2f(mousePos));
        
        if (boardPos.isValid()) {
            m_game.makeMove(boardPos);
        }
    }

    void update() {
        // AI
    }

    void render() {
        m_window.clear(sf::Color(50, 50, 50));

        m_renderer.drawBoard(m_window);
        m_renderer.drawStones(m_window, m_game.getBoard());
        m_renderer.drawLastMove(m_window, m_game.getBoard());

        m_gameUI.drawUI(m_window, m_game, m_font, m_aiTimeMs);

        m_window.display();
    }

    sf::RenderWindow m_window;
    Game m_game;
    Renderer m_renderer;
    GameUI m_gameUI;
    sf::Font m_font;
    float m_aiTimeMs;
};

int main() {
    GomokuApp app;
    app.run();
    return 0;
}

