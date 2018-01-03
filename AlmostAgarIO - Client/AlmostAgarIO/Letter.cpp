#include "stdafx.h"
#include "Letter.h"
#include <iostream>


Letter::Letter(float x, float y, float size_x, float size_y, std::string letter, sf::Color color, int id)
	: Widget(x, y, size_x, size_y, letter)
{
	_font.loadFromFile("arial.ttf");// Bet�tipus bet�lt�s
	_text = sf::Text(_value, _font);// Sz�veg l�trehoz�sa
	_status = Loading;
	_color = color;
	_id = id;
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
	/*
	//Sz�veg be�ll�t�sa:
	text.setString(_value);
	text.setCharacterSize(_size_y*0.5);
	text.setPosition(_x + (_size_y - text.getGlobalBounds().width) / 2 + text.getGlobalBounds().width / 2, _y + _size_y*0.2);
	text.setStyle(sf::Text::Bold);

	// Sz�veg be�ll�t�sa:
	sf::Color szin(255, 255, 255);
	text.setColor(szin);
	
	// Kirajzol�s:
	window.draw(text);*/

	//Sz�veg be�ll�t�sa:
	float zoom_factor = window.getView().getSize().y / window.getDefaultView().getSize().y;
	_text.setString(_value);
	_text.setCharacterSize(_size_y*zoom_factor);;
	//_text.setPosition(_x + _size_y*0.2, _y + _size_y*0.2);
	//_text.setPosition(_x + (_size_y - _text.getLocalBounds().width) / 2 + _text.getLocalBounds().width / 2, _y + _size_y*0.2);
	
	//_text.setPosition(window.getView().getCenter().x + _id*_size_x, window.getView().getCenter().y + window.getView().getSize().y/2 - _size_y-10);
	_text.setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2 + 10 * zoom_factor, window.getView().getCenter().y - window.getView().getSize().y / 2 + _id*_size_y*zoom_factor + 10 * zoom_factor);
	
	_text.setStyle(sf::Text::Bold);
	sf::Color color = _color;
	//color.a = 80;

	if (_status == Loading) {
		color.a = 50;
	}
	else if (_status == Active) {
		/*sf::RectangleShape background;
		background.setSize(sf::Vector2f(_size_x, _size_y));
		background.setPosition(_text.getPosition().x, _text.getPosition().y);
		background.setFillColor(sf::Color(0, 0, 0, 80));
		window.draw(background);*/
		color.a = 255;
	}
	else if (_status == Inactive) {

	}
	
	// Sz�veg be�ll�t�sa:
	_text.setColor(color);

	// Kirajzol�s:
	window.draw(_text);
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
