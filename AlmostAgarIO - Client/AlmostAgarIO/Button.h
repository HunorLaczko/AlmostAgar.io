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
	bool isSelected()=0;
	//virtual void action();
	//string adat();
};

#include <functional>

class lambdaButton : public Button {
	void draw(sf::RenderWindow & window);
	std::function<void()> _f;
	std::function<void()> _tab;
	bool _selected;
	bool _previousSelected;
public:
	void handle(sf::Event event);
	lambdaButton(float x, float y, float size_x, float size_y, std::string value, std::function<void()> f, std::function<void()> tab_f);
	void action();
	void setValue(std::string value);
	void setSelected(bool selected);
	bool isSelected();
	void setTabFunc(std::function<void()> tab_f);
}; 