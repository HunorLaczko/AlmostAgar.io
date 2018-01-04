
#include "stdafx.h"
#include "Button.h"
#include <iostream>

Button::Button(float x, float y, float size_x, float size_y, std::string value, std::function<void()> f, std::function<void()> tab_f)
	: Widget(x, y, size_x, size_y, value)
{
	_f = f;
	_tab = tab_f;
	_selected = false;
	_previousSelected = false;
}

void Button::draw(sf::RenderWindow & window)
{
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(_size_x, _size_y));
	//rectangle.setOutlineColor(sf::Color::Red);
	//rectangle.setOutlineThickness(-5);
	rectangle.setPosition(_x, _y);


	// Declare and load a font
	sf::Font font;
	font.loadFromFile("arial.ttf");
	// Create a text
	sf::Text text(_value, font);
	//text.setString(_value);
	text.setCharacterSize(_size_y*0.5);
	float x_padding = rectangle.getLocalBounds().width - text.getLocalBounds().width;
	text.setPosition(_x + x_padding/2, _y + _size_y*0.25);
	/*if (_mousedown) {
		text.setCharacterSize(40);
		text.setPosition(_x + 5, _y + 5);
	}
	else{
		text.setCharacterSize(30);
		text.setPosition(_x+10,_y+10);
	}*/
	//text.setStyle(sf::Text::Bold);
	text.setColor(sf::Color::White);
	if (_selected) {
		//text.setColor(sf::Color::Blue);
		rectangle.setFillColor(sf::Color(215, 88, 0));
		//rectangle.setOutlineColor(sf::Color::Cyan);
	}
	else if(_mousedown || _mouseover){
		//text.setColor(sf::Color::Blue);
		rectangle.setFillColor(sf::Color(245, 118, 0));
		//rectangle.setOutlineColor(sf::Color::Cyan);
	}
	else {
		rectangle.setFillColor(sf::Color(255,128,0));
		//rectangle.setOutlineColor(sf::Color::White);
	}
	// Draw it

	window.draw(rectangle);
	window.draw(text);
}


void Button::handle(sf::Event event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		if (_x < event.mouseMove.x && event.mouseMove.x < (_x + _size_x) && _y < event.mouseMove.y && event.mouseMove.y < (_y + _size_y)) {
			_mouseover = true;
		}
		else{
			_mouseover = false;
		}
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (_mouseover && event.mouseButton.button == sf::Mouse::Left)
		{
			_mousedown = true;
		}
		else if(event.mouseButton.button == sf::Mouse::Left) {
			_previousSelected = false;
			_selected = false;
		}
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (_mousedown && event.mouseButton.button == sf::Mouse::Left)
		{
			_mousedown = false;
			if(_mouseover)
				action();
		}
	}

	if (event.type == sf::Event::TextEntered && _selected)
	{
		// Tab leütésre került
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
			if (_previousSelected)
				_tab();
		}
		// Enter leütésre került
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			_f();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && (_selected != _previousSelected)) {
		_previousSelected = _selected;
	}
}

void Button::action() {
	_f();
}

void Button::setValue(std::string value)
{
	_value = value;
}

void Button::setSelected(bool selected)
{
	_selected = selected;
}

bool Button::isSelected()
{
	return _selected;
}

void Button::setTabFunc(std::function<void()> tab_f)
{
	_tab = tab_f;
}
