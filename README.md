# RichText

Rich text class for [SFML 2](https://github.com/SFML/SFML/). Allows the
user to draw lines of text with different styles and colors.

## Authors

* [Cristian Pallarés](https://github.com/Skyrpex/) - Original code
* [Lukas Dürrenberger](https://github.com/eXpl0it3r/) - Conversion to the new SFML 2 API

## How to use

1. Include the header and the source to your project.
2. Link to SFML 2.4.x.
3. Use a C++11 ready compiler.

## Support branches

**Notice:** There's no guarantee that these branches are fully updated.

* For a non C++11 ready compilers, there is a [support branch](https://github.com/Skyrpex/RichText/tree/support/no-c%2B%2B11).
* For earlier SFML versions than 2.4.x, see the [support branch](https://github.com/Skyrpex/RichText/tree/support/pre-sfml-2.4).

## Repository

You can get the current development version from the [git repository](https://github.com/Skyrpex/RichText).

## Example

```cpp
#include <SFML/Graphics.hpp>
#include "RichText.hpp"

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
         << sf::Text::Underlined << "It looks good!";

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
```