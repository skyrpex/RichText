////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "RichText.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <SFML/System/String.hpp>

namespace sfe
{

////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setCharacterSize(unsigned int size)
{
    for (std::size_t i = 0; i < m_texts.size(); ++i)
        m_texts[i].setCharacterSize(size);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setFont(const sf::Font &font)
{
    for (std::size_t i = 0; i < m_texts.size(); ++i)
        m_texts[i].setFont(font);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<sf::Text> &RichText::Line::getTexts() const
{
    return m_texts;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::appendText(sf::Text text)
{
    // Set text offset
    text.setPosition(m_bounds.width, 0.f);

    // Update bounds
    m_bounds.height = std::max(m_bounds.height, text.getGlobalBounds().height);
    m_bounds.width += text.getGlobalBounds().width;

    // Push back
    m_texts.push_back(text);
}


////////////////////////////////////////////////////////////////////////////////
sf::FloatRect RichText::Line::getLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////////////////////////
sf::FloatRect RichText::Line::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (std::size_t i = 0; i < m_texts.size(); ++i)
        target.draw(m_texts[i], states);
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::updateGeometry() const
{
    m_bounds = sf::FloatRect();

    for (std::size_t i = 0; i < m_texts.size(); ++i) {
        sf::Text &text = m_texts[i];
        text.setPosition(m_bounds.width, 0.f);

        m_bounds.height = std::max(m_bounds.height, text.getGlobalBounds().height);
        m_bounds.width += text.getGlobalBounds().width;
    }
}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText()
    : m_font(NULL),
      m_characterSize(30),
      m_currentColor(sf::Color::White),
      m_currentStyle(sf::Text::Regular)
{

}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText(const sf::Font &font)
    : m_font(&font),
      m_characterSize(30),
      m_currentColor(sf::Color::White),
      m_currentStyle(sf::Text::Regular)
{

}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const sf::Color &color)
{
    m_currentColor = color;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (sf::Text::Style style)
{
    m_currentStyle = style;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
std::vector<sf::String> explode(const sf::String &string, sf::Uint32 delimiter) {
    if (string.isEmpty())
        return std::vector<sf::String>();

    // For each character in the string
    std::vector<sf::String> result;
    sf::String buffer;
    for (std::size_t i = 0; i < string.getSize(); ++i) {
        // If we've hit the delimiter character
        sf::Uint32 character = string[i];
        if (character == delimiter) {
            // Add them to the result vector
            result.push_back(buffer);
            buffer.clear();
        } else {
            // Accumulate the next character into the sequence
            buffer += character;
        }
    }

    // Add to the result if buffer still has contents or if the last character is a delimiter
    if (!buffer.isEmpty() || string[string.getSize() - 1] == delimiter)
        result.push_back(buffer);

    return result;
}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const sf::String &string)
{
    // Maybe skip
    if (string.isEmpty())
        return *this;

    // Explode into substrings
    std::vector<sf::String> subStrings = explode(string, '\n');

    // Append first substring using the last line
    std::vector<sf::String>::iterator it = subStrings.begin();
    if (it != subStrings.end()) {
        // If there isn't any line, just create it
        if (m_lines.empty())
            m_lines.resize(1);

        // Remove last line's height
        Line &line = m_lines.back();
        m_bounds.height -= line.getGlobalBounds().height;

        // Append text
        line.appendText(createText(*it));

        // Update bounds
        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }

    // Append the rest of substrings as new lines
    while (++it != subStrings.end()) {
        Line line;
        line.setPosition(0.f, m_bounds.height);
        line.appendText(createText(*it));
        m_lines.push_back(line);

        // Update bounds
        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }

    // Return
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacterSize(unsigned int size)
{
    // Maybe skip
    if (m_characterSize == size)
        return;

    // Update character size
    m_characterSize = size;

    // Set texts character size
    for (std::size_t i = 0; i < m_lines.size(); ++i)
        m_lines[i].setCharacterSize(size);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setFont(const sf::Font &font)
{
    // Maybe skip
    if (m_font == &font)
        return;

    // Update font
    m_font = &font;

    // Set texts font
    for (std::size_t i = 0; i < m_lines.size(); ++i)
            m_lines[i].setFont(font);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::clear()
{
    // Clear texts
    m_lines.clear();

    // Reset bounds
    m_bounds = sf::FloatRect();
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<RichText::Line> &RichText::getLines() const
{
    return m_lines;
}


////////////////////////////////////////////////////////////////////////////////
unsigned int RichText::getCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////////////////////////
const sf::Font *RichText::getFont() const
{
    return m_font;
}


////////////////////////////////////////////////////////////
sf::FloatRect RichText::getLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////
sf::FloatRect RichText::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////////////////////////
void RichText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (std::size_t i = 0; i < m_lines.size(); ++i)
        target.draw(m_lines[i], states);
}


////////////////////////////////////////////////////////////////////////////////
sf::Text RichText::createText(const sf::String &string) const
{
    sf::Text text;
    text.setString(string);
    text.setColor(m_currentColor);
    text.setStyle(m_currentStyle);
    if (m_font)
        text.setFont(*m_font);

    return text;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::updateGeometry() const
{
    m_bounds = sf::FloatRect();

    for (std::size_t i = 0; i < m_lines.size(); ++i) {
        Line &line = m_lines[i];
        line.setPosition(0.f, m_bounds.height);

        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }
}

}
