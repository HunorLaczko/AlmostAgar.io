#pragma once
#include <SFML/Network.hpp>
#include <SFML\System.hpp>
#include <list>
#include <map>
#include "Player.h"

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

	bool running;



public:
	Server();
	~Server();

	void run();
};

