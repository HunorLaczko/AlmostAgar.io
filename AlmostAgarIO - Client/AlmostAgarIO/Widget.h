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
	std::string _value;
public:
	Widget(float x, float y, float size_x, float size_y,std::string value);
	~Widget();
	virtual void draw(sf::RenderWindow & window) = 0;
	virtual void handle(sf::Event event) = 0;
	virtual bool isSelected() = 0;
	virtual void setSelected(bool selected)=0;
	std::string getValue();
	virtual void setValue(std::string value)=0;
	void resize(float r);
};

