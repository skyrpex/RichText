////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "RichText.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace sfe
{

RichText::RichText(const sf::Font &font)
    : myFont(&font),
      myCurrentColor(sf::Color::White),
      myCurrentStyle(sf::Text::Regular),
      mySizeUpdated(false)
{

}

////////////////////////////////////////////////////////////////////////////////
// Operator << sf::Color
////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const sf::Color &color)
{
    myCurrentColor = color;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Operator << sf::Text::Style
////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (sf::Text::Style style)
{
    myCurrentStyle = style;
    return *this;
}

//std::vector<std::string> explode(const std::string &string, char delim)
//{
//    std::vector<std::string> result;
//    std::istringstream iss(string);

//    for (std::string token; std::getline(iss, token, delim); ) {
//        result.push_back(std::move(token));
//    }

//    return result;
//}

std::vector<std::string> explode(const std::string &string, char delimiter) {
    std::string next;
    std::vector<std::string> result;

    // For each character in the string
    for (auto it = string.begin(); it != string.end(); it++) {
        // If we've hit the terminal character
        if (*it == delimiter) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }

    if (!next.empty())
         result.push_back(next);

    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Operator << sf::String
////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const sf::String &string)
{
    // It is not updated
    mySizeUpdated = false;

    // Add lines
    std::vector<std::string> lines = explode(string, '\n');
    for (const std::string &line : lines) {
        sf::Text text(line, *myFont);
        text.setFont(*myFont);
        text.setColor(myCurrentColor);
        text.setStyle(myCurrentStyle);
        myTexts.push_back(std::move(text));
    }

    // Return
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Set size
////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacterSize(unsigned int size)
{
    // Set character size
    for (auto it = myTexts.begin(); it != myTexts.end(); ++it) {
        it->setCharacterSize(size);
    }

    // It is not updated
    mySizeUpdated = false;
}

////////////////////////////////////////////////////////////////////////////////
// Set font
////////////////////////////////////////////////////////////////////////////////
void RichText::setFont(const sf::Font &font)
{
    myFont = &font;

    // Set character size
    for (auto it = myTexts.begin(); it != myTexts.end(); ++it) {
        it->setFont(font);
    }

    // It is not updated
    mySizeUpdated = false;
}

////////////////////////////////////////////////////////////////////////////////
// Clear
////////////////////////////////////////////////////////////////////////////////
void RichText::clear()
{
    // Clear text list
    myTexts.clear();

    // Reset size
    mySize = sf::Vector2f(0.f, 0.f);

    // It is updated
    mySizeUpdated = true;
}

////////////////////////////////////////////////////////////////////////////////
// Get text list
////////////////////////////////////////////////////////////////////////////////
const std::vector<sf::Text> & RichText::getTextList() const
{
    return myTexts;
}

////////////////////////////////////////////////////////////////////////////////
// Get character size
////////////////////////////////////////////////////////////////////////////////
unsigned int RichText::getCharacterSize() const
{
    if (myTexts.size())
        return myTexts.begin()->getCharacterSize();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Get font
////////////////////////////////////////////////////////////////////////////////
const sf::Font *RichText::getFont() const
{
    return myFont;
}

////////////////////////////////////////////////////////////////////////////////
// Get width
////////////////////////////////////////////////////////////////////////////////
float RichText::getWidth() const
{
    updateSize();
    return mySize.x;
}

////////////////////////////////////////////////////////////////////////////////
// Get height
////////////////////////////////////////////////////////////////////////////////
float RichText::getHeight() const
{
    updateSize();
    return mySize.y;
}

////////////////////////////////////////////////////////////////////////////////
// Render
////////////////////////////////////////////////////////////////////////////////
void RichText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const sf::Text &text : myTexts) {
        int height =  text.getGlobalBounds().height;
        target.draw(text, states);

        states.transform.translate(0, height);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Update size
////////////////////////////////////////////////////////////////////////////////
void RichText::updateSize() const
{
    // Return if updated
    if (mySizeUpdated)
        return;

    // Return if empty
    if (myTexts.begin() == myTexts.end())
        return;

    // It is updated
    mySizeUpdated = true;

    // Sum all sizes (height not implemented)
    mySize.x = 0.f;
    mySize.y = myTexts.begin()->getGlobalBounds().height;
    for (auto it = myTexts.begin(); it != myTexts.end(); ++it) {
        // Update width
        mySize.x += it->getGlobalBounds().width;
    }
}

}
