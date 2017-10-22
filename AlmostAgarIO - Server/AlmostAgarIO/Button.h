#pragma once

#include "Widget.h"

#include <string>

class Button : public Widget
{
public:
	Button(float x, float y, float size_x, float size_y, std::string value);
	void draw(sf::RenderWindow & window);
	void handle(sf::Event event);
	virtual void action();
	//virtual void action();
	//string adat();
};

#include <functional>

class lambdaButton : public Button {
	std::function<void()> _f;
public:
	lambdaButton(float x, float y, float size_x, float size_y, std::string value, std::function<void()> f);
	void action();
};