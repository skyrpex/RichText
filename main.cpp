#include <SFML/Graphics.hpp>

#include "RichText.hpp"

int main(int, char *[])
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Test");
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf");
    sfe::RichText text(font);
    text << sf::Color::Red << "Test\nbla" << sf::Color::Blue << "Lol";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            }
        }

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}

