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
}

Player::~Player()
{
}

sf::TcpSocket * Player::getTcpSocket()
{
	return tcpSocket;
}

sf::UdpSocket * Player::getUdpSocket()
{
	return udpSocket;
}

unsigned int Player::getId()
{
	return id;
}

sf::Vector2f Player::getPosition()
{
	return position;
}

void Player::setPosition(sf::Vector2f _position)
{
	position = _position;
}