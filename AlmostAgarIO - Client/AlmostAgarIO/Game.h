#pragma once
#include <SFML/Graphics.hpp>
#include "FoodGenerator.h"
#include "Network.h"
#include "Player.h"
#include<SFML/Network.hpp>

class Network;

//#include "C:\SFML\include\SFML\Graphics\RenderWindow.hpp"
class Game :
	public sf::RenderWindow
{
public:
	Game();
	//Game(sf::WindowHandle 	handle, const sf::ContextSettings & = sf::ContextSettings());
	~Game();
	void draw(sf::RenderWindow & window);
	void resize(sf::Event::SizeEvent event_size, sf::Vector2u window_size);
	void connect();
	void disconnect();
	sf::Vector2f getCirclePos();
	void setIpAndWindowSize(sf::IpAddress _serverIp, sf::Vector2u window_size);
	void setName(sf::String _name);
	void counting(sf::RenderWindow & window);
	void func();
	void threadWait();
	void setFirst();
	void setFood(const std::vector<sf::Vector2f>& _food, float foodRadius);
	void updateFood(int index, sf::Vector2f newFood);

private:
	sf::Vector2f vec;
	sf::Vector2f movement;
	sf::Clock clock;
	bool first; //kamera nézetet reseteli, ha first=true, mindig meg kell hivni a setfirst-t hogy ha új játékot kezdünk
	sf::View view;

	std::vector<sf::CircleShape> food;
	//FoodGenerator gen;

	sf::Sprite map, background;
	sf::Texture texture, texture2;
	Network* network;
	Player player;
	bool finished;
	sf::Thread thread;
};