RichText
========

Rich text class for [SFML2](https://github.com/LaurentGomila/SFML/). Allows the
user to draw lines of text with different styles and colors.

License
-------

This code is licensed under public domain.

Authors
-------

* [Cristian Pallarés](https://github.com/Skyrpex) - Original code
* [Lukas Dürrenberger](https://github.com/eXpl0it3r/) - Conversion to the new SFML2 API

How to use
----------

1. Include the header and the source to your project.
2. Link to SFML2 (obviously :P!).
3. Use a C++11 ready compiler.

Repository
----------

You can get the current development version from the [git repository](https://github.com/Skyrpex/RichText).

Example
-------

    #include "RichText.hpp"
    #include <SFML/Graphics.hpp>
     
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