#include "stdafx.h"
#include "TextEditor.h"
#include <iostream>

TextEditor::TextEditor(float x, float y, float size_x, float size_y, std::string value, std::string empty_value="")
	: Widget(x, y, size_x, size_y, value)
{
	_empty_value = empty_value;//Ha �res, sz�rk�tve ezt �rja ki
	_selected = false;//r� van-e kattintva
///JAV�TAND�: int k�ne legyen:
	_cursor_posxid = 0;//kurzor helye(hanyadik bet� m�g�tt van)
	mouse_x = 0;//Eg�r x pozici�ja(az�rt van r� szuks�g, mert a press eventn�l nem tudom lek�rni)

	_font.loadFromFile("arial.ttf");// Bet�tipus bet�lt�s
	text = sf::Text(_value, _font);// Sz�veg l�trehoz�sa
}

TextEditor::~TextEditor()
{
}

bool TextEditor::isempty()
{
	if (_value == "")
		return true;
	else
		false;
}

void TextEditor::draw(sf::RenderWindow & window)
{
	//Alap kocka l�trehoz�sa(a h�tt�r):
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(_size_x, _size_y));
	rectangle.setOutlineThickness(-_size_y*0.02);
	rectangle.setPosition(_x, _y);
	rectangle.setFillColor(sf::Color::White);
	//Alap sz�n(kocka szine):
	sf::Color szin(sf::Color::Cyan);
	 if (_selected) {
		szin = sf::Color(50, 50, 200);
	}
	else if (_mouseover) {
		szin = sf::Color(100, 100, 255);
	}
	rectangle.setOutlineColor(szin);

	/*
	///feljebb vittem a konstruktorba:
	// Bet�tipus bet�lt�s
	sf::Font font;
	font.loadFromFile("Lora-Regular.ttf");


	// Sz�veg l�trehoz�sa
	sf::Text text(_value, font);*/


// Bet�tipus bet�lt�s:
//sf::Font font;
//font.loadFromFile("Lora-Regular.ttf");
// Sz�veg l�trehoz�sa
//sf::Text _text(_value, font);

	//Sz�veg be�ll�t�sa:
	text.setString(_value);
	text.setCharacterSize(_size_y*0.5);
	text.setPosition(_x + _size_y*0.2, _y + _size_y*0.2);
	text.setStyle(sf::Text::Bold);

//_text.setCharacterSize(_size_y*0.5);
//_text.setPosition(_x + _size_y*0.2, _y + _size_y*0.2+5);
//_text.setStyle(sf::Text::Bold);

	// Kurzor l�trehoz�sa:
	sf::RectangleShape line(sf::Vector2f(_size_y*0.6, _size_y/40));
	line.rotate(90);
	line.setFillColor(sf::Color::Black);

	// Sz�veg �s a kurzor be�ll�t�sa(ha �res/nem �res):
	if (_value == "") {
//_text.setString(_empty_value);
		sf::Color szin(150, 150, 150);
//_text.setColor(szin);
		text.setString(_empty_value);
		text.setColor(szin); 
		line.setPosition(text.getPosition().x, text.getPosition().y);
	}
	else{
		sf::Color szin(0, 0, 0);
//_text.setColor(szin);
		text.setColor(szin);
		if (_cursor_posxid == 0) {
			//Kurzor az elej�re
			line.setPosition(text.getPosition().x + 3, text.getPosition().y);
		}
		else if (_cursor_posxid == _value.size()) {
			//Kurzor a v�g�re
			line.setPosition(text.getPosition().x + text.getLocalBounds().width + 7, text.getPosition().y);
		}
		else {
			//Kurzor egy bet� ut�n
			line.setPosition(text.findCharacterPos(_cursor_posxid).x , text.getPosition().y);
		}
	}
//std::cout << text.findCharacterPos(1).y << " , " << text.getLocalBounds().height << std::endl;
	

	// Kirajzol�s:
	window.draw(rectangle);
//window.draw(_text);
	if (_selected)
		window.draw(line);
	window.draw(text);
}

void TextEditor::handle(sf::Event event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		mouse_x = event.mouseMove.x;
		//Az adott objektum felett van-e az eg�r
		if (_x < event.mouseMove.x && event.mouseMove.x < (_x + _size_x) && _y < event.mouseMove.y && event.mouseMove.y < (_y + _size_y)) {
			_mouseover = true;
		}
		else {
			_mouseover = false;
		}
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		//R�kattintott-e az objektumra(bal klikk)
		if (_mouseover && event.mouseButton.button == sf::Mouse::Left)
		{
			if (_value.size() == 0) {
				//Ha nincs sz�veg, akkor az elej�re tegye a kurzort
				_cursor_posxid = 0;
			}
			else if ((text.findCharacterPos(0).x + text.findCharacterPos(1).x) / 2 > mouse_x) {
				//Ha a sz�veg el� kettint, akkor az elej�re tegye a kurzort(ki kellett venni a for ciklusb�l, mert 3 elemet vizsg�l a ciklus)
				_cursor_posxid = 0;
			}
			else if ((text.findCharacterPos(_value.size() - 1).x + text.getPosition().x + text.getLocalBounds().width) / 2 < mouse_x) {
				//Ha a sz�veg m�g� kettint, akkor a v�g�re tegye a kurzort(szint�n kiindexel�s lenne, ha a ciklusban n�zn�m)
				_cursor_posxid = _value.size();
			}
			else {
				for (unsigned i = 0; i < _value.size() - 1; i++) {
					//Melyik bet�h�z, illetve a bet� bal vagy jobb oldal�ra tegye a kurzort:
					///Feloszt�s: 60-40
					///if ((text.findCharacterPos(_value.size() - i - 2).x + 11*(text.findCharacterPos(_value.size() - i - 1).x-text.findCharacterPos(_value.size() - i - 2).x)/ 20) < mouse_x && (text.findCharacterPos(_value.size() - i - 1).x +11*(text.findCharacterPos(_value.size() - i).x-text.findCharacterPos(_value.size() - i - 1).x)/ 20) > mouse_x) {
					///Feloszt�s: 50-50
					if ((text.findCharacterPos(_value.size() - i - 1).x + text.findCharacterPos(_value.size() - i - 2).x) / 2 < mouse_x && (text.findCharacterPos(_value.size() - i).x + text.findCharacterPos(_value.size() - i - 1).x) / 2 > mouse_x) {
						_cursor_posxid = _value.size() -i - 1;
						break;
					}
				}
			}
//std::cout << mouse_x << " vs " << (text.findCharacterPos(_value.size() - 1).x + text.getPosition().x + text.getLocalBounds().width) / 2 << " , " << _cursor_posxid << std::endl;
			_selected = true;
			_mousedown = true;
		}
		else if (event.mouseButton.button == sf::Mouse::Left) {
			///m�r nem tudom mi�rt �rtam ide :D
			_selected = false;
		}
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		//Felengedte-e az egeret(bal klikk)
		if (_mousedown && event.mouseButton.button == sf::Mouse::Left)
		{
			_mousedown = false;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && _cursor_posxid > 0)
	{
		//bal nyilra menjen balra a kurzor
		_cursor_posxid--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && _cursor_posxid < _value.size())
	{
		//jobb nyilra menjen jobbra a kurzor
		_cursor_posxid++;
	}
	// Delete le�t�sre ker�lt
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && _cursor_posxid < _value.size())
	{
		//jobbr�l t�r�l, ha van ott
		if (_cursor_posxid == 0 && _value.length() == 1) {
			_value = "";
		}
		else if (_cursor_posxid == 0) {
			_value = _value.substr(1, _value.size() - 1);
		}
		else {
			_value = _value.substr(0, _cursor_posxid) + _value.substr(_cursor_posxid + 1, _value.length() - 1);
		}
	}
	if (event.type == sf::Event::TextEntered && _selected)
	{
		// BackSpace le�t�sre ker�lt
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && _cursor_posxid > 0)
		{
			//balr�l t�r�l, ha van ott
			if (_cursor_posxid == 1 && _value.length()==1) {
				_value = "";
			}
			else if(_value.length() == _cursor_posxid){
				_value = _value.substr(0, _cursor_posxid-1);
			}
			else {
				_value = _value.substr(0, _cursor_posxid-1) + _value.substr(_cursor_posxid, _value.length() - 1);
			}
			_cursor_posxid--;
		}
		// Sz�vegbevitel t�rt�nt �s nem BackSpace
		else if (event.type == sf::Event::TextEntered && event.text.unicode!=8)
		{
//std::cout << event.text.unicode << std::endl;
			///szab�lyozni k�ne, hogy mi j�het, �s mi nem
			///de �ssze vissza van, �s pl �,� nincs
			///if ((event.text.unicode < 0x12C))
			///{
			if (_cursor_posxid == 0) {
				_value = (char)event.text.unicode + _value.substr(0, _value.length());
			}
			else if (_cursor_posxid == _value.size()) {
				_value = _value.substr(0, _cursor_posxid) + (char)event.text.unicode;
			}
			else {
				_value = _value.substr(0, _cursor_posxid) + (char)event.text.unicode + _value.substr(_cursor_posxid, _value.length());
			}
			_cursor_posxid++;
//_value += (char)event.text.unicode;
			///}
		}
	}
}

