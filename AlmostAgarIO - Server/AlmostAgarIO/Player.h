#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <memory>
#include <utility>

class Player
{
	unsigned int id;
	std::string name;
	sf::TcpSocket* tcpSocket;
	sf::UdpSocket* udpSocket;
	sf::IpAddress serverIP;
	unsigned int port;
	sf::Vector2f position;  //player position, in world coordinates (absolute coordinates)
	sf::Vector2f speed;     //player speed
	sf::Vector2f velocity;  //player velocity
	unsigned int radius;    //player's circle's radius
	int points;				//player's points

public:
	Player();
	Player(int id, sf::Vector2f _position, sf::TcpSocket *tcpSocket);
	~Player();

	sf::TcpSocket* getTcpSocket();
	sf::UdpSocket* getUdpSocket();

	unsigned int getId();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f _position);
};

