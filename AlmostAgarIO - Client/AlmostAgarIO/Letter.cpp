#include "stdafx.h"
#include "Letter.h"
#include <iostream>


Letter::Letter(float x, float y, float size_x, float size_y, std::string letter, sf::Color color, int id)
	: Widget(x, y, size_x, size_y, letter)
{
	_font.loadFromFile("arial.ttf");// Betûtipus betöltés
	_text = sf::Text(_value, _font);// Szöveg létrehozása
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
	//Szöveg beállítása:
	float zoom_factor = window.getView().getSize().y / window.getDefaultView().getSize().y;
	_text.setString(_value);
	_text.setCharacterSize(_size_y*zoom_factor);;
	_text.setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2 + 10 * zoom_factor, window.getView().getCenter().y - window.getView().getSize().y / 2 + _id*_size_y*zoom_factor + 10 * zoom_factor);
	
	_text.setStyle(sf::Text::Bold);
	sf::Color color = _color;

	if (_status == Loading) {
		color.a = 50;
	}
	else if (_status == Active) {
		color.a = 255;
	}
	// Szöveg beállítása:
	_text.setColor(color);

	// Kirajzolás:
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
