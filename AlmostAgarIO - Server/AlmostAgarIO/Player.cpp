#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <SFML/Network.hpp>


Player::Player(int _id, sf::Vector2f _position, sf::TcpSocket *_tcpSocket)
	: id(_id), position(_position), tcpSocket(_tcpSocket), radius(80),
	name("unknown"), udpSocket(nullptr), playerIp(sf::IpAddress()),
	speed(sf::Vector2f(0, 0)), velocity(sf::Vector2f(0, 0)), points(0), 
	mapSize(sf::Vector2f(0, 0)), mapPosition(sf::Vector2f(0, 0)),
	windowSize(sf::Vector2f(0, 0)), mousePosition(sf::Vector2f(0, 0))
{
	
}

Player::~Player()
{
}

sf::TcpSocket * Player::getTcpSocket() const
{
	return tcpSocket;
}

sf::UdpSocket * Player::getUdpSocket() const
{
	if (udpSocket == nullptr) std::cout << "udpSocket hasn't been initialized\n";
	return udpSocket;
}

unsigned int Player::getId() const
{
	return id;
}

std::string Player::getName() const
{
	return name;
}

void Player::setName(std::string _name)
{
	name = _name;
}

sf::Vector2f Player::getPosition() const
{
	return position;
}

void Player::setPosition(sf::Vector2f _position)
{
	position = _position;
}

sf::Vector2f Player::getMapSize() const
{
	return mapSize;
}

void Player::setMapSize(sf::Vector2f _mapSize)
{
	mapSize = _mapSize;
}

sf::Vector2f Player::getMapPosition() const
{
	return mapPosition;
}

void Player::setMapPosition(sf::Vector2f _mapPosition)
{
	mapPosition = _mapPosition;
}

sf::Vector2f Player::getWindowSize() const
{
	return windowSize;
}

void Player::setWindowSize(sf::Vector2f _windowSize)
{
	windowSize = _windowSize;
}

float Player::getRadius() const
{
	return radius;
}

void Player::setRadius(float _radius)
{
	radius = _radius;
}

sf::IpAddress Player::getPlayerIp() const
{
	return playerIp;
}

void Player::setPlayerIp(sf::IpAddress _playerIp)
{
	playerIp = _playerIp;
}

sf::UdpSocket * Player::bindUdpSocket(unsigned short _udpPortReceive, unsigned short _udpPortSend)
{
	udpSocket = new sf::UdpSocket();
	udpSocket->bind(_udpPortReceive);
	udpPortReceive = _udpPortReceive;
	udpPortSend = _udpPortSend;
	return udpSocket;
}

unsigned short Player::getUdpPortSend() const
{
	return udpPortSend;
}

unsigned short Player::getUdpPortReceive() const
{
	return udpPortReceive;
}

sf::Vector2f Player::getMousePosition() const
{
	return mousePosition;
}

void Player::setMousePosition(sf::Vector2f _mousePosition)
{
	mousePosition = _mousePosition;
}
void Player::initPosition()
{
	//srand(time(0));
	position.x = rand() % (int)mapSize.x + mapPosition.x;
	position.y = rand() % (int)mapSize.y + mapPosition.y;
	//position = sf::Vector2f(2000, 2000);
}

sf::Color Player::getColor() const {
	return color;
}

void Player::setColor(sf::Color _color) {
	color = _color;
}


bool operator==(const Player &a, const Player &b)
{
	if (a.getId() == b.getId())
		return true;
	return false;
}

bool operator<(const Player &a, const Player &b)
{
	if (a.getRadius() < b.getRadius())
		return true;
	return false;
}


