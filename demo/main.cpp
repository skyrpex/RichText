#include <SFML/Graphics.hpp>
#include "../RichText.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "sfe::RichText");
    window.setFramerateLimit(30);

    sf::Font font;
    font.loadFromFile("FreeMono.ttf");

    sfe::RichText text(font);
    text << sf::Text::Bold       << sf::Color::Cyan  << "This "
         << sf::Text::Italic     << sf::Color::White << "is\nan\n"
         << sf::Text::Regular    << sf::Color::Green << "example"
         << sf::Color::White     << ".\n"
         << sf::Text::Underlined << "It looks good!\n" << sf::Text::StrikeThrough
         << sfe::Outline{ sf::Color::Blue, 3.f } << "Really good!";
 
    text.setCharacterSize(25);
    text.setPosition(400, 300);
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        window.display();
    }
}
