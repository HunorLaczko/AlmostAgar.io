#pragma once
#include<SFML/Network.hpp>
#include "Player.h"
#include "Game.h"

class Game;

class Network
{
	sf::IpAddress serverIp;
	unsigned short serverTcpPort = 50000;
	unsigned short serverUdpPortSend = 50015;
	unsigned short serverUdpPortReceive = 50016;
	sf::TcpSocket tcpSocket;
	sf::UdpSocket udpSocket;

	Player* player;
	Game* game;
	sf::Vector2f position;

public:
	Network();
	Network(Game *_game);
	~Network();

	void setPosition(sf::Vector2f _position);
	void setIp(sf::IpAddress _serverIp);

	void connectPlayer(Player *_player);
	void disconnectPlayer();
	void init(sf::Vector2f _mapSize, sf::Vector2f _mapPosition, sf::Vector2f _windowSize);
	void sendPosition(sf::Vector2f position);
	void getResponse();
	void sendKey(int key, bool active);

};

