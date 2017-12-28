#pragma once
#include "Widget.h"
#include "iostream"
#include "Button.h"
#include "TextEditor.h"
#include <SFML/Graphics.hpp>
#include "FoodGenerator.h"
#include "Network.h"
#include "Player.h"
#include "Game.h"
#include<SFML/Network.hpp>
//#include "C:\SFML\include\SFML\Graphics\RenderWindow.hpp"
enum Views { main_menu_view, game_view, game_over_menu_view };

class Windows :
	public sf::RenderWindow
{
public:
	Windows();
	Windows(sf::VideoMode mode, const sf::String &title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings &settings = sf::ContextSettings());
	Windows(sf::WindowHandle 	handle, const sf::ContextSettings & = sf::ContextSettings());
	~Windows();
	void event_loop();
	//void add(Widget *w);
	void set(std::vector<Widget*> menu, std::vector<Widget*> game_over);
	void Windows::changeview(Views view);
	void initGame(sf::IpAddress _serverIp,sf::String _playerName);
	std::string validate(std::string _serverIp, std::string _playerName);
	void threadWait();
	void load();
	void save();

private:
	Views viewid;
	bool viewChanged;
	std::vector<Widget*> menu, game_over;
	unsigned int size_x, size_y;

	Game game;
};

