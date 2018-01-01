#pragma once
#include "Widget.h"
#include <string>
#include <functional>

class TextEditor :
	public Widget
{
protected:
	std::string _empty_value;
	int _cursor_posxid;
///Átmeneti?
	sf::Font _font;
	sf::Text text;
	float mouse_x;
	std::function<void()> _tab;
	std::function<void()> _enter;
	bool _selected;
	bool _previousSelected;
	int _lowerLimit;
	int _upperLimit;
	std::map<int, char> _filter;
///
public:
	TextEditor(float x, float y, float size_x, float size_y, std::string value, std::string empty_value, int lowerLimit,int upperLimit,std::map<int, char> filter, std::function<void()> tab_f, std::function<void()> enter_f);
	~TextEditor();
	void setValue(std::string value);
	void draw(sf::RenderWindow & window);
	void handle(sf::Event event);
	bool isempty();
	void setTabFunc(std::function<void()> tab_f);
	void setEnterFunc(std::function<void()> enter_f);
	void setSelected(bool selected);
	bool isSelected();
};

