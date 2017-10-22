#pragma once
#include "Widget.h"
#include <string>

class TextEditor :
	public Widget
{
protected:
	std::string _empty_value;
	bool _selected;
	float _cursor_posxid;
///Átmeneti?
	sf::Font _font;
	sf::Text text;
	float mouse_x;
///
public:
	TextEditor(float x, float y, float size_x, float size_y, std::string value, std::string empty_value);
	~TextEditor();
	void draw(sf::RenderWindow & window);
	void handle(sf::Event event);
	bool isempty();
	std::string getValue();
};

