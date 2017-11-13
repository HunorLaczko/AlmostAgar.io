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
	sf::IpAddress playerIp;
	unsigned int port;
	sf::Vector2f position;  //player position, in world coordinates (absolute coordinates)
	sf::Vector2f speed;     //player speed
	sf::Vector2f velocity;  //player velocity
	unsigned int radius;    //player's circle's radius
	int points;				//player's points
	sf::Vector2f mapSize;   //player's map size
	sf::Vector2f mapPosition;//player's map's location relative to world coordinates
	sf::Vector2f windowSize;//player's window size
	sf::Vector2f mousePosition;


public:
	Player();
	Player(int id, sf::Vector2f _position, sf::TcpSocket *tcpSocket);
	~Player();

	sf::TcpSocket* getTcpSocket() const;
	sf::UdpSocket* getUdpSocket() const;

	unsigned int getId() const;

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f _position);

	sf::Vector2f getMapSize();
	void setMapSize(sf::Vector2f _mapSize);

	sf::Vector2f getMapPosition();
	void setMapPosition(sf::Vector2f _mapPosition);

	sf::Vector2f getWindowSize();
	void setWindowSize(sf::Vector2f _windowSize);

	unsigned int getRadius();
	void setRadius(unsigned int _radius);

	sf::IpAddress getPlayerIp();
	void setPlayerIp(sf::IpAddress _playerIp);

	sf::Vector2f getMousePosition();
	void setMousePosition(sf::Vector2f _mousePosition);

	//void movePlayer()
};

