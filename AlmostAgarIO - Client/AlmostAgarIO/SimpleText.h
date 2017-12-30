#pragma once
#include "Widget.h"
class SimpleText :
	public Widget
{
protected:
	sf::Font _font;
	sf::Text text;
public:
	SimpleText(float x, float y, float size_x, float size_y, std::string value);
	~SimpleText();
	void setValue(std::string value);
	void draw(sf::RenderWindow & window);

	//Átmenetileg:
	void handle(sf::Event event);
	void setSelected(bool selected);
	bool isSelected();
};

