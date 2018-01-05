#include "stdafx.h"
#include "SimpleText.h"


SimpleText::SimpleText(float x, float y, float size_x, float size_y, std::string value)
	: Widget(x, y, size_x, size_y, value)
{
	_font.loadFromFile("arial.ttf");// Bet�tipus bet�lt�s
	text = sf::Text(_value, _font);// Sz�veg l�trehoz�sa
}

SimpleText::~SimpleText()
{
}

void SimpleText::setValue(std::string value)
{
	_value = value;
}

void SimpleText::draw(sf::RenderWindow & window)
{
	//Sz�veg be�ll�t�sa:
	text.setString(_value);
	text.setCharacterSize(_size_y*0.5);
	text.setPosition(_x + (_size_x- text.getGlobalBounds().width)/2, _y + (_size_y - text.getGlobalBounds().height)/2);
	text.setStyle(sf::Text::Bold);

	// Sz�veg be�ll�t�sa:
	sf::Color szin(255, 255, 255);
	text.setColor(szin);

	// Kirajzol�s:
	window.draw(text);
}


//�tmenetileg:
void SimpleText::handle(sf::Event event)
{
}

void SimpleText::setSelected(bool selected)
{
}

bool SimpleText::isSelected()
{
	return false;
}
