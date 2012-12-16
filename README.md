RichText
========

Rich text class for [SFML2](https://github.com/LaurentGomila/SFML/). Allows the
user to draw lines of text with different styles and colors.

License
-------

This code is licensed under public domain.

Author
------

* [Cristian Pallarés](https://github.com/Skyrpex) - Original code
* [Lukas Dürrenberger](https://github.com/eXpl0it3r/) - Conversion to the new SFML2 API

How to use
----------

1. Include the header and the source to your project.
2. Link to SFML2 (obviously :P!).

Repository
----------

You can get the current development version from the [git repository](https://github.com/Skyrpex/RichText).

Example
-------

    #include "RichText.hpp"
    #include <SFML/Graphics.hpp>
     
    int main()
    {
      sfe::RichText text;
      text << sf::Text::Bold << sf::Color::Cyan << "This"
        << sf::Text::Italic << sf::Color::White << " is cool "
        << sf::Text::Regular << sf::Color::Green << "mate"
        << sf::Color::White << ". "
        << sf::Text::Underlined << "I wish I could lick it!";
     
      text.setCharacterSize(25);
      text.setPosition(400, 300);
      text.setOrigin(text.getWidth()/2.f, text.getHeight()/2.f);
     
      sf::RenderWindow window;
      window.create(sf::VideoMode(800, 600), "Rich text");
     
      while(window.isOpen())
      {
        sf::Event event;
        while(window.pollEvent(event))
        {
          if(event.type == sf::Event::Closed)
            window.close();
        }
     
        window.clear();
        window.draw(text);
        window.display();
      }
    }
