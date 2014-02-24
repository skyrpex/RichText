#include <SFML/Graphics.hpp>

#include "../RichText.hpp"

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "sfe::RichText");
    window.setFramerateLimit(30);

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf");

    sfe::RichText text(font);
    text << sf::Text::Bold       << sf::Color::Cyan  << "This "
         << sf::Text::Italic     << sf::Color::White << "is\ncool\n"
         << sf::Text::Regular    << sf::Color::Green << "mate"
         << sf::Color::White     << ".\n"
         << sf::Text::Underlined << "I wish I could lick it!";
 
    text.setCharacterSize(25);
    text.setPosition(400, 300);
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}
