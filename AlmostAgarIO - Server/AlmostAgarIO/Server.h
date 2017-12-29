#pragma once
#include <SFML/Network.hpp>
#include <SFML\System.hpp>
#include <list>
#include <map>
#include <unordered_map>
#include "Player.h"
#include "FoodGenerator.h"

class Server
{
	sf::TcpListener listener;
	sf::UdpSocket udpSocket;
	std::unordered_map<int, Player> players;
	sf::SocketSelector selector;
	unsigned short port = 50000;
	//unsigned short udpPortReceive = 50015;
	//unsigned short udpPortSend = 50016;
	unsigned int id;
	sf::Vector2f position;
	sf::Clock clock;
	FoodGenerator foodGenerator;
	std::vector<sf::Vector2f> food;
	std::unordered_map<int, sf::Vector2f> foodToUpdate;
	std::vector<Player> ranking;

	bool running;
	bool rankingChanged;

	void updatePlayerPosition(int id, sf::Vector2f pos);
	void setFood(unsigned int id);
	void updateFood(unsigned int id);
	void checkRanking();
	void deletePlayerFromRanking(int id);
	void playerDied(unsigned int id);
public:
	Server();
	~Server();

	void run();
};

