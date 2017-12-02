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
	Network();
	~Network();

	void setPosition(sf::Vector2f _position);
	void setIp(sf::IpAddress _serverIp);

	void connectPlayer(Player *_player);
	void disconnectPlayer();
	void init(sf::Vector2f _mapSize, sf::Vector2f _mapPosition, sf::Vector2f _windowSize);
	void sendPosition(sf::Vector2f position);
	void getResponse();
	void sendKey(char key);

	void run();
};

