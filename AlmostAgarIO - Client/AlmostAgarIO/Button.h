#pragma once

#include "Widget.h"
#include <string>
#include <functional>

class Button : public Widget
{
	std::function<void()> _f;
	std::function<void()> _tab;
	bool _selected;
	bool _previousSelected;
public:
	Button(float x, float y, float size_x, float size_y, std::string value, std::function<void()> f, std::function<void()> tab_f);
	void draw(sf::RenderWindow & window);
	void handle(sf::Event event);
	void action();
	void setValue(std::string value);
	void setSelected(bool selected);
	bool isSelected();
	void setTabFunc(std::function<void()> tab_f);
};
