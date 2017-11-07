#pragma once
#include<SFML/Network.hpp>
#include "Player.h"

class Network
{
	sf::IpAddress serverIp;
	unsigned short serverTcpPort = 50000;
	unsigned short serverUdpPortSend = 50005;
	unsigned short serverUdpPortReceive = 50006;
	sf::TcpSocket tcpSocket;
	sf::UdpSocket udpSocket;

	Player* player;
	sf::Vector2f position;

public:
	Network(sf::IpAddress _serverIp, Player* player);
	~Network();

	void setPosition(sf::Vector2f _position);

	void connectPlayer();
	void sendPosition(sf::Vector2f position);
	void sendKey(char key);

	void run();
};

