#pragma once
#include <SFML/Graphics.hpp>
#include "Network.h"
#include "Player.h"
#include "Letter.h"
#include<SFML/Network.hpp>
#include <functional>

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
	void setLeaderboard(std::vector<unsigned int> _leaderboard);
	bool isOver();
	void gameOver();
	void init(sf::IpAddress _serverIp, sf::Vector2u window_size,sf::String _playerName);
	void counting(sf::RenderWindow & window);
	void func();
	void threadWait();
	void setFirst();
	void setFood(const std::vector<sf::Vector2f>& _food, float foodRadius);
	void updateFood(int index, sf::Vector2f newFood);
	void keyPressed(int key);
	void keyReleased(int key);
	bool testServer();
	bool isLoeaded();
	void setLoeadedFunc(std::function<void()> loaded);
private:
	sf::Font _font;
	sf::Vector2f vec;
	sf::Vector2f movement;
	sf::Clock clock;
	bool first; //kamera nézetet reseteli, ha first=true, mindig meg kell hivni a setfirst-t hogy ha új játékot kezdünk
	sf::View view;
	int zoom_count;
	std::vector<unsigned int> leaderboard;
	bool gameover;
	std::map<int, bool> specalkeys;
	std::vector<Letter*> specalkeys_value;
	std::function<void()> _loaded;

	std::vector<sf::CircleShape> food;
	sf::Vector2f oldPos;
	float ZOOM;

	sf::Sprite map, background;
	sf::Texture texture, texture2;
	Network* network;
	Player player;
	bool finished;
	sf::Thread thread;
};