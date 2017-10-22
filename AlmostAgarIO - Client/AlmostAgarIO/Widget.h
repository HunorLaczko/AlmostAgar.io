#pragma once
#include <string>
#include <SFML/Graphics.hpp>
class Widget
{
protected:
	bool _mouseover, _mousedown;
	float _x;
	float _y;
	float _size_x;
	float _size_y;
	bool _selected;
	std::string _value;
public:
	Widget(float x, float y, float size_x, float size_y,std::string value);
	~Widget();
	virtual void draw(sf::RenderWindow & window);
	virtual void handle(sf::Event event);
	//virtual bool is_selected(int mouse_x, int mouse_y) const;
};

