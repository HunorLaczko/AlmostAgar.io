#pragma once
#include "Widget.h"

class Letter :
	public Widget
{
	//enum Status { Active, Inactive, Loading };
	enum Status;
	sf::Font _font;
	sf::Text _text;
	Status _status;
	sf::Color _color;
	int _id;
public:
	Letter(float x, float y, float size_x, float size_y, std::string letter, sf::Color color, int id);
	~Letter();
	enum Status { Active, Inactive, Loading };
	void changeStatus(Status new_status);
	void draw(sf::RenderWindow & window);
	void handle(sf::Event event);
	bool isSelected();
	void setSelected(bool selected);
	void setValue(std::string value);
};

