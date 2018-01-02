#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <memory>
#include <utility>
#include <SFML/Graphics.hpp>


class Player
{
	unsigned int id;
	std::string name;
	sf::TcpSocket* tcpSocket;
	sf::UdpSocket* udpSocket;
	sf::IpAddress playerIp;
	unsigned short udpPortReceive;
	unsigned short udpPortSend;
	sf::Vector2f position;  //player position, in world coordinates (absolute coordinates)
	sf::Vector2f speed;     //player speed
	sf::Vector2f velocity;  //player velocity
	float defRadius;    //default radius set in Player constructor
	float radius;    //player's circle's radius
	float points;				//player's points above 330f.0 to reduce movement lag in client
	sf::Vector2f mapSize;   //player's map size
	sf::Vector2f mapPosition;//player's map's location relative to world coordinates
	sf::Vector2f windowSize;//player's window size
	sf::Vector2f mousePosition;
	sf::Color color; //player's color
	bool initReady;

public:
	Player(int id, sf::Vector2f _position, sf::TcpSocket *tcpSocket);
	~Player();

	sf::TcpSocket* getTcpSocket() const;
	sf::UdpSocket* getUdpSocket() const;

	unsigned int getId() const;

	std::string getName() const;
	void setName(std::string _name);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f _position);

	sf::Vector2f getMapSize() const;
	void setMapSize(sf::Vector2f _mapSize);

	sf::Vector2f getMapPosition() const;
	void setMapPosition(sf::Vector2f _mapPosition);

	sf::Vector2f getWindowSize() const;
	void setWindowSize(sf::Vector2f _windowSize);

	float getPoints() const;

	float getRadius() const;
	void setRadius(float _radius);

	sf::IpAddress getPlayerIp() const;
	void setPlayerIp(sf::IpAddress _playerIp);

	sf::UdpSocket* bindUdpSocket(unsigned short _udpPortReceive, unsigned short _udpPortSend);
	unsigned short getUdpPortSend() const;
	unsigned short getUdpPortReceive() const;

	sf::Vector2f getMousePosition() const;
	void setMousePosition(sf::Vector2f _mousePosition);

	void initPosition();

	sf::Color getColor() const;
	void setColor(sf::Color _color);

	bool getInitReady() const;
	void setInitReady(bool _initReady);

};

