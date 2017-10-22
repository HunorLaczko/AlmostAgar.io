#pragma once
#include "Widget.h"

#include "iostream"

#include "Button.h"
#include "TextEditor.h"
#include <SFML/Graphics.hpp>
//#include "C:\SFML\include\SFML\Graphics\RenderWindow.hpp"
class Windows :
	public sf::RenderWindow
{
public:
	Windows();
	Windows(sf::VideoMode mode, const sf::String &title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings &settings = sf::ContextSettings());
	Windows(sf::WindowHandle 	handle, const sf::ContextSettings & = sf::ContextSettings());
	~Windows();
	void event_loop();
	void add(Widget *w);
private:
	std::vector<Widget*> widgets;
	unsigned int size_x, size_y;
};

