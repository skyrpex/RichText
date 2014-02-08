////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "RichText.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace sfe
{

RichText::RichText(std::string filename)
  : myCurrentColor(sf::Color::White),
    myCurrentStyle(sf::Text::Regular),
    mySizeUpdated(false),
    myPositionUpdated(false),
    myCurrentLine(0)
{
  //Check if we have our font
  if(!myFont.loadFromFile(filename))
  {
    std::cout << "Font " << filename << " not found!\n";
  }
  
  //Set up our collection
  line_type val;
  myTexts.push_back(val);
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

////////////////////////////////////////////////////////////////////////////////
// Operator << sf::String
////////////////////////////////////////////////////////////////////////////////
/*
  **  Must parse the strings to look for '\n' characters. If found, we break
  **  the string into two pieces.
  */
RichText & RichText::operator << (const sf::String &string)
{
  // It is not updated
  mySizeUpdated = false;
  myPositionUpdated = false;

  // Find \n characters (assert)
  //assert(string.Find('\n') == std::string::npos);
  if(string.find('\n') != std::string::npos)
  {
    std::cerr << "sfe::RichtText: Oops, character \\n found."
                 "Let's see what happens ;)." << std::endl;
    size_t newline = string.find('\n');
    operator<<(string.toWideString().substr(0, newline-1));
    line_type val;
    myTexts.push_back(val);
    myCurrentLine++;
    operator<<(string.toWideString().substr(newline+1));
  }
  else
  {
    //String cannot be void
    //assert(string != "");

    // Setup string
    sf::Text text;
    text.setString(string);
    text.setColor(myCurrentColor);
    text.setStyle(myCurrentStyle);
    text.setFont(myFont);

    // Add string
    myTexts[myCurrentLine].push_back(text);

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
  for(collection_type::iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    for(line_type::iterator it2 = it->begin();
      it2 != it->end(); ++it2)
    {
      it2->setCharacterSize(size);
    }
  }

  // It is not updated
  mySizeUpdated = false;
  myPositionUpdated = false;
}

////////////////////////////////////////////////////////////////////////////////
// Set font
////////////////////////////////////////////////////////////////////////////////
void RichText::setFont(const sf::Font &font)
{
  //Set the object's font for new strings
  myFont = font;

  // Set character size
  for(collection_type::iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    for(line_type::iterator it2 = it->begin();
        it2 != it->end(); ++it)
    {
      it2->setFont(myFont);
    }
  }

  // It is not updated
  mySizeUpdated = false;
  myPositionUpdated = false;
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
  myPositionUpdated = true;
}

////////////////////////////////////////////////////////////////////////////////
// Get text list
////////////////////////////////////////////////////////////////////////////////
const RichText::collection_type & RichText::getTextList() const
{
  return myTexts;
}

////////////////////////////////////////////////////////////////////////////////
// Get character size
////////////////////////////////////////////////////////////////////////////////
unsigned int RichText::getCharacterSize() const
{
  if(myTexts.size()) return myTexts.begin()->begin()->getCharacterSize();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Get font
////////////////////////////////////////////////////////////////////////////////
const sf::Font & RichText::getFont() const
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
  // Update position
  updatePosition();

  states.transform *= getTransform();

  // Draw
  for(collection_type::const_iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    // Add transformation
    //it->setT
    for(line_type::const_iterator it2 = it->begin();
        it2 != it->end(); ++it2)
    {
      // Draw text
      target.draw(*it2, states);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Update size
////////////////////////////////////////////////////////////////////////////////
void RichText::updateSize() const
{
  // Return if updated
  if(mySizeUpdated) return;

  // Return if empty
  if(myTexts.begin() == myTexts.end()) return;

  // It is updated
  mySizeUpdated = true;

  // Sum all sizes
  mySize.x = 0.f;
  mySize.y = myTexts.begin()->begin()->getGlobalBounds().height*myCurrentLine;
  for(collection_type::const_iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    float lineWidth = 0.0f;
    for(line_type::const_iterator it2 = it->begin();
        it2 != it->end(); ++it2)
    {
      // Update width
      lineWidth += it2->getGlobalBounds().width;
    }
    if(mySize.x < lineWidth)
    {
    	mySize.x = lineWidth;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Update position
////////////////////////////////////////////////////////////////////////////////
void RichText::updatePosition() const
{
  // Return if updated
  if(myPositionUpdated) return;

  // Return if empty
  if(myTexts.begin() == myTexts.end()) return;

  // It is updated
  myPositionUpdated = true;

  // Get starting position
  sf::Vector2f offset;

  // Draw
  for(collection_type::iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    for(line_type::iterator it2 = it->begin();
      it2 != it->end(); ++it2)
    {
      // Set all the origins to the first one
      it2->setOrigin(it2->getPosition() - myTexts.begin()->begin()->getPosition() - offset);

      // Set offset
      const sf::FloatRect rect = it2->getGlobalBounds();
      offset.x += rect.width;
    }
    offset.y += it->begin()->getGlobalBounds().height;
    offset.x = 0.0f;
  }
}

}
