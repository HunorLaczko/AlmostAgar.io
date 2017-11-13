#include "stdafx.h"
#include "Player.h"
#include <SFML/Network.hpp>

Player::Player()
{
}

Player::Player(int _id, sf::Vector2f _position, sf::TcpSocket *_tcpSocket) 
{
	id = _id;
	position = _position;
	tcpSocket = _tcpSocket;
	radius = 30;
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
	return udpSocket;
}

unsigned int Player::getId() const
{
	return id;
}

sf::Vector2f Player::getPosition() const
{
	return position;
}

void Player::setPosition(sf::Vector2f _position)
{
	position = _position;
}

sf::Vector2f Player::getMapSize()
{
	return mapSize;
}

void Player::setMapSize(sf::Vector2f _mapSize)
{
	mapSize = _mapSize;
}

sf::Vector2f Player::getMapPosition()
{
	return mapPosition;
}

void Player::setMapPosition(sf::Vector2f _mapPosition)
{
	mapPosition = _mapPosition;
}

sf::Vector2f Player::getWindowSize()
{
	return windowSize;
}

void Player::setWindowSize(sf::Vector2f _windowSize)
{
	windowSize = _windowSize;
}

unsigned int Player::getRadius()
{
	return radius;
}

void Player::setRadius(unsigned int _radius)
{
	radius = _radius;
}

sf::IpAddress Player::getPlayerIp()
{
	return playerIp;
}

void Player::setPlayerIp(sf::IpAddress _playerIp)
{
	playerIp = _playerIp;
}

sf::Vector2f Player::getMousePosition()
{
	return mousePosition;
}

void Player::setMousePosition(sf::Vector2f _mousePosition)
{
	mousePosition = _mousePosition;
}

bool operator==(const Player &a, const Player &b)
{
	if (a.getId() == b.getId())
		return true;
	return false;
}