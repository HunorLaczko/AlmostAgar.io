#pragma once
#include <SFML/Network.hpp>
#include <list>
#include "Player.h"

class Server
{
	sf::TcpListener listener;
	sf::UdpSocket udpSocket;
	std::vector<Player> players;
	sf::SocketSelector selector;
	unsigned short port = 50000;
	unsigned short udpPortReceive = 50005;
	unsigned short udpPortSend = 50006;
	unsigned int id;
	sf::Vector2f position;

	bool running;

public:
	Server();
	~Server();

	void run();
};

