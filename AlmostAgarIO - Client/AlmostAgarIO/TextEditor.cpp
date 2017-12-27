#include "stdafx.h"
#include "TextEditor.h"
#include <iostream>

TextEditor::TextEditor(float x, float y, float size_x, float size_y, std::string value, std::string empty_value="")
	: Widget(x, y, size_x, size_y, value)
{
	_empty_value = empty_value;//Ha üres, szürkítve ezt írja ki
	_selected = false;//rá van-e kattintva
///JAVÍTANDÓ: int kéne legyen:
	_cursor_posxid = 0;//kurzor helye(hanyadik betü mögött van)
	mouse_x = 0;//Egér x poziciója(azért van rá szukség, mert a press eventnél nem tudom lekérni)

	_font.loadFromFile("arial.ttf");// Betûtipus betöltés
	text = sf::Text(_value, _font);// Szöveg létrehozása
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
	//Alap kocka létrehozása(a háttér):
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(_size_x, _size_y));
	rectangle.setOutlineThickness(-_size_y*0.02);
	rectangle.setPosition(_x, _y);
	rectangle.setFillColor(sf::Color::White);
	//Alap szín(kocka szine):
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
	// Betûtipus betöltés
	sf::Font font;
	font.loadFromFile("Lora-Regular.ttf");


	// Szöveg létrehozása
	sf::Text text(_value, font);*/


// Betûtipus betöltés:
//sf::Font font;
//font.loadFromFile("Lora-Regular.ttf");
// Szöveg létrehozása
//sf::Text _text(_value, font);

	//Szöveg beállítása:
	text.setString(_value);
	text.setCharacterSize(_size_y*0.5);
	text.setPosition(_x + _size_y*0.2, _y + _size_y*0.2);
	text.setStyle(sf::Text::Bold);

//_text.setCharacterSize(_size_y*0.5);
//_text.setPosition(_x + _size_y*0.2, _y + _size_y*0.2+5);
//_text.setStyle(sf::Text::Bold);

	// Kurzor létrehozása:
	sf::RectangleShape line(sf::Vector2f(_size_y*0.6, _size_y/40));
	line.rotate(90);
	line.setFillColor(sf::Color::Black);

	// Szöveg és a kurzor beállítása(ha üres/nem üres):
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
			//Kurzor az elejére
			line.setPosition(text.getPosition().x + 3, text.getPosition().y);
		}
		else if (_cursor_posxid == _value.size()) {
			//Kurzor a végére
			line.setPosition(text.getPosition().x + text.getLocalBounds().width + 7, text.getPosition().y);
		}
		else {
			//Kurzor egy betü után
			line.setPosition(text.findCharacterPos(_cursor_posxid).x , text.getPosition().y);
		}
	}
//std::cout << text.findCharacterPos(1).y << " , " << text.getLocalBounds().height << std::endl;
	

	// Kirajzolás:
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
		//Az adott objektum felett van-e az egér
		if (_x < event.mouseMove.x && event.mouseMove.x < (_x + _size_x) && _y < event.mouseMove.y && event.mouseMove.y < (_y + _size_y)) {
			_mouseover = true;
		}
		else {
			_mouseover = false;
		}
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		//Rákattintott-e az objektumra(bal klikk)
		if (_mouseover && event.mouseButton.button == sf::Mouse::Left)
		{
			if (_value.size() == 0) {
				//Ha nincs szöveg, akkor az elejére tegye a kurzort
				_cursor_posxid = 0;
			}
			else if ((text.findCharacterPos(0).x + text.findCharacterPos(1).x) / 2 > mouse_x) {
				//Ha a szöveg elé kettint, akkor az elejére tegye a kurzort(ki kellett venni a for ciklusból, mert 3 elemet vizsgál a ciklus)
				_cursor_posxid = 0;
			}
			else if ((text.findCharacterPos(_value.size() - 1).x + text.getPosition().x + text.getLocalBounds().width) / 2 < mouse_x) {
				//Ha a szöveg mögé kettint, akkor a végére tegye a kurzort(szintén kiindexelés lenne, ha a ciklusban nézném)
				_cursor_posxid = _value.size();
			}
			else {
				for (unsigned i = 0; i < _value.size() - 1; i++) {
					//Melyik betühöz, illetve a betü bal vagy jobb oldalára tegye a kurzort:
					///Felosztás: 60-40
					///if ((text.findCharacterPos(_value.size() - i - 2).x + 11*(text.findCharacterPos(_value.size() - i - 1).x-text.findCharacterPos(_value.size() - i - 2).x)/ 20) < mouse_x && (text.findCharacterPos(_value.size() - i - 1).x +11*(text.findCharacterPos(_value.size() - i).x-text.findCharacterPos(_value.size() - i - 1).x)/ 20) > mouse_x) {
					///Felosztás: 50-50
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
			///már nem tudom miért írtam ide :D
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
	// Delete leütésre került
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && _cursor_posxid < _value.size())
	{
		//jobbról töröl, ha van ott
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
		// BackSpace leütésre került
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && _cursor_posxid > 0)
		{
			//balról töröl, ha van ott
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
		// Szövegbevitel történt és nem BackSpace
		else if (event.type == sf::Event::TextEntered && event.text.unicode!=8)
		{
//std::cout << event.text.unicode << std::endl;
			///szabályozni kéne, hogy mi jöhet, és mi nem
			///de össze vissza van, és pl õ,û nincs
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

