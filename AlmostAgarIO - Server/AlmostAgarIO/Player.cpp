#include "stdafx.h"
#include "Player.h"
#include <SFML/Network.hpp>


Player::Player(int _id, sf::Vector2f _position, sf::TcpSocket *_tcpSocket)
	: id(_id), position(_position), tcpSocket(_tcpSocket), radius(30),
	name("unknown"), udpSocket(nullptr), playerIp(sf::IpAddress()),port(0),
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

float Player::getRadius()
{
	return radius;
}

void Player::setRadius(float _radius)
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

void Player::init(std::vector<sf::Vector2f>& _food)
{
	/*
	for (sf::Vector2f f : _food)
	{
		if (f.x > position.x - (windowSize.x / 2 - 50) && f.x < position.x + (windowSize.x / 2 + 50) &&
			f.y > position.y - (windowSize.y / 2 - 50) && f.y < position.y + (windowSize.y / 2 + 50))
		{
			food.insert(f);
		}
	}
	*/
	food = _food;
}

void Player::updateFood(int index, sf::Vector2f newPosition)
{
	/*
	food.erase(food.find(toDelete));
	if (toAdd.x > position.x - (windowSize.x / 2 - 50) && toAdd.x < position.x + (windowSize.x / 2 + 50) &&
		toAdd.y > position.y - (windowSize.y / 2 - 50) && toAdd.y < position.y + (windowSize.y / 2 + 50))
	{
		food.insert(toAdd);
	}*/

}

bool operator==(const Player &a, const Player &b)
{
	if (a.getId() == b.getId())
		return true;
	return false;
}


