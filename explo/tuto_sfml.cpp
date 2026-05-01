#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    // Load a sprite to display
    const sf::Texture texture("cute_image.jpg");
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    const sf::Font font("arial.ttf");
    sf::Text text(font, "Hello SFML", 50);

    // Load a music to play
    sf::Music music("nice_music.ogg");

    // Play the music
    music.play();

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
    }
}


if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
{
    // move left...
}
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
{
    // move right...
}
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
{
    // quit...
}
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Grave))
{
    // open in-game command line (if it's not already open)
}

if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
{
    // left click...
}

// get global mouse position
sf::Vector2i position = sf::Mouse::getPosition();

// set mouse position relative to a window
sf::Mouse::setPosition(sf::Vector2i(100, 200), window);