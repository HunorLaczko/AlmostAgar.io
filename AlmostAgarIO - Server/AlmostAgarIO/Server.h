#pragma once
#include <SFML/Network.hpp>
#include <SFML\System.hpp>
#include <list>
#include <map>
#include "Player.h"
#include "FoodGenerator.h"

class Server
{
	sf::TcpListener listener;
	sf::UdpSocket udpSocket;
	std::map<int, Player> players;
	sf::SocketSelector selector;
	unsigned short port = 50000;
	unsigned short udpPortReceive = 50005;
	unsigned short udpPortSend = 50006;
	unsigned int id;
	sf::Vector2f position;
	sf::Clock clock;
	FoodGenerator foodGenerator;
	std::vector<sf::Vector2f> food;

	bool running;

	void updatePlayerPosition(int id, sf::Vector2f pos);
	void setFood(unsigned int id);
	void updateFood(unsigned int id);
public:
	Server();
	~Server();

	void run();
};

