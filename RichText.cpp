////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "RichText.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace sfe
{

RichText::RichText()
  : myCurrentColor(sf::Color::White),
    myCurrentStyle(sf::Text::Regular),
    mySizeUpdated(false),
    myPositionUpdated(false)
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
    std::cerr << "sfe::RichtText: Oops, character \n found."
                 "You will get visual errors." << std::endl;
                 
  //String cannot be void
  //assert(string != "");
	
  // Add string
  myTexts.push_back(sf::Text(string));

  // Setup string
  sf::Text &text = myTexts.back();
  text.setColor(myCurrentColor);
  text.setStyle(myCurrentStyle);

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
    it->setCharacterSize(size);
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
  // Set character size
  for(collection_type::iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    it->setFont(font);
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
  if(myTexts.size()) return myTexts.begin()->getCharacterSize();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Get font
////////////////////////////////////////////////////////////////////////////////
const sf::Font & RichText::getFont() const
{
  if(myTexts.size()) return myTexts.begin()->getFont();
  return sf::Font::getDefaultFont();
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

    // Draw text
    target.draw(*it, states);
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

  // Sum all sizes (height not implemented)
  mySize.x = 0.f;
  mySize.y = myTexts.begin()->getGlobalBounds().height;
  for(collection_type::const_iterator it = myTexts.begin();
      it != myTexts.end(); ++it)
  {
    // Update width
    mySize.x += it->getGlobalBounds().width;
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
    // Set all the origins to the first one
    it->setOrigin(it->getPosition() - myTexts.begin()->getPosition() - offset);

    // Set offset
    const sf::FloatRect rect = it->getGlobalBounds();
    offset.x += rect.width;
  }
}

}
