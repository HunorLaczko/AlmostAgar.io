#include "stdafx.h"
#include "Letter.h"
#include <iostream>


Letter::Letter(float x, float y, float size_x, float size_y, std::string letter, sf::Color color)
	: Widget(x, y, size_x, size_y, letter)
{
	_font.loadFromFile("arial.ttf");// Bet�tipus bet�lt�s
	text = sf::Text(_value, _font);// Sz�veg l�trehoz�sa
	_status = Loading;
	_color = color;
}

Letter::~Letter()
{
}

void Letter::changeStatus(Status new_status)
{
	_status = new_status;
}

void Letter::draw(sf::RenderWindow & window)
{

	//Sz�veg be�ll�t�sa:
	text.setString(_value);
	text.setCharacterSize(_size_y*0.5);
	text.setPosition(_x + (_size_y - text.getGlobalBounds().width) / 2 + text.getGlobalBounds().width / 2, _y + _size_y*0.2);
	text.setStyle(sf::Text::Bold);

	// Sz�veg be�ll�t�sa:
	sf::Color szin(255, 255, 255);
	text.setColor(szin);

	// Kirajzol�s:
	window.draw(text);
	/*//Sz�veg be�ll�t�sa:
	_text.setString(_value);
	_text.setCharacterSize(_size_y*0.5);
	_text.setPosition(_x + _size_y*0.2, _y + _size_y*0.2);
	//_text.setPosition(_x + (_size_y - _text.getLocalBounds().width) / 2 + _text.getLocalBounds().width / 2, _y + _size_y*0.2);
	_text.setStyle(sf::Text::Bold);
	sf::Color color = _color;*/
	//color.a = 80;

	/*if (_status == Loading) {
		color.a = 50;
	}
	else if (_status == Active) {
		sf::RectangleShape background;
		background.setSize(sf::Vector2f(_size_x, _size_y));
		background.setPosition(_x, _y);
		background.setFillColor(sf::Color(0, 0, 0, 80));
		window.draw(background);
	}
	else if (_status == Inactive) {

	}*/
	
	/*std::cout << "asd11" << std::endl;
	// Sz�veg be�ll�t�sa:
	_text.setColor(color);

	std::cout << "asd2" << std::endl;
	// Kirajzol�s:
	window.draw(_text);
	std::cout << "asd3" << std::endl;*/
}

void Letter::handle(sf::Event event)
{
}

bool Letter::isSelected()
{
	return false;
}

void Letter::setSelected(bool selected)
{
}

void Letter::setValue(std::string value)
{
	_value = value;
}
