#pragma once
#include <SFML/Graphics.hpp>
#include "FoodGenerator.h"
#include "Network.h"
#include "Player.h"
#include<SFML/Network.hpp>
//#include "C:\SFML\include\SFML\Graphics\RenderWindow.hpp"
class Game :
	public sf::RenderWindow
{
public:
	Game(sf::VideoMode mode, const sf::String &title, sf::IpAddress _serverIp, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings &settings = sf::ContextSettings());
	//Game(sf::WindowHandle 	handle, const sf::ContextSettings & = sf::ContextSettings());
	~Game();
	void event_loop();
	void add();
private:
	sf::View view;
	std::vector<sf::CircleShape> food;
	//FoodGenerator gen;
	sf::CircleShape circle;
	sf::Sprite map, background;
	sf::Texture texture, texture2;
	Network *network;
	Player player;

};

