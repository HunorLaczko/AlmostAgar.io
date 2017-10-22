#include "stdafx.h"
#include "Widget.h"


Widget::Widget(float x,float y, float size_x,float size_y,std::string value)
{
	_x = x;
	_y = y;
	_size_x = size_x;
	_size_y = size_y;
	_value = value;
	_mouseover = false;
	_mousedown = false;
}


Widget::~Widget()
{
}

void Widget::draw(sf::RenderWindow & window)
{
}

void Widget::handle(sf::Event event)
{
}

/*bool Widget::is_selected(int mouse_x, int mouse_y) const
{
	return mouse_x>_x && mouse_x<_x + _size_x && mouse_y>_y && mouse_y<_y + _size_y;
}*/