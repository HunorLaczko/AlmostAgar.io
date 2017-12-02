#include "stdafx.h"
#include "Network.h"
#include<iostream>
#include <thread>

Network::Network()
{
	
}


Network::~Network()
{
	delete player;
}

void Network::setPosition(sf::Vector2f _position)
{
	position = _position;
}

void Network::setIp(sf::IpAddress _serverIp)
{
	serverIp = _serverIp;
}

void Network::connectPlayer(Player *_player)
{
	player = _player;
	udpSocket.bind(serverUdpPortReceive, serverIp);
	udpSocket.setBlocking(false);

	sf::Socket::Status status = tcpSocket.connect(serverIp, serverTcpPort);
	//udpSocket.bind(serverUdpPortReceive);
	if (status != sf::Socket::Done)
	{
		std::cout << "Player could not connect\n";
	}
	else
	{
		//std::cout << "Player connected, local port: " << tcpSocket.getLocalPort() << "\n";

		sf::Packet packet;
		if (tcpSocket.receive(packet) == sf::Socket::Done)
		{
			//std::cout << "received something\n";
			int type;
			packet >> type;
			if (type == 0) //connected to server
			{
				int id;
				packet >> id;
				player->setId(id);
				std::cout << "received id: " << id << "\n";
			}
		}
		else
		{
			std::cout << "Something went wrong\n ";
		}
	}
}

void Network::disconnectPlayer()
{
	tcpSocket.disconnect();
	//delete player;
}

void Network::init(sf::Vector2f _mapSize, sf::Vector2f _mapPosition, sf::Vector2f _windowSize)
{
	sf::Packet outPacket;
	outPacket << 3 << player->getId() << _mapSize.x << _mapSize.y << _mapPosition.x << _mapPosition.y << _windowSize.x << _windowSize.y;;
	//std::cout << udpSocket.send(outPacket, serverIp, serverUdpPortSend) << std::endl;
	tcpSocket.send(outPacket);
}

void Network::sendPosition(sf::Vector2f position)
{
	sf::Packet outPacket;
	outPacket << 1 << player->getId() << position.x << position.y;
	//std::cout << udpSocket.send(outPacket, serverIp, serverUdpPortSend) << std::endl;
	udpSocket.send(outPacket, serverIp, serverUdpPortSend);
}

void Network::getResponse()
{
	sf::Packet packet;
	sf::IpAddress sender;
	if (udpSocket.receive(packet, sender, serverUdpPortReceive) == sf::Socket::Done)
	{
		//std::cout << "getResponse: received something\n";
		int type, id;
		packet >> type;
		switch (type)
		{
			//received new pos
		case 2:
		{
			sf::Vector2f pos;
			packet >> id >> pos.x >> pos.y;
			std::cout << "oldPos: " << player->getPosition().x << "," << player->getPosition().y << " newPos: " << pos.x << "," << pos.y << "\n";
			player->setPosition(pos);
			std::cout << "received new location\n";
			break;
		}
			
		}
	}
	//outPacket << 1 << player->getId() << position.x << position.y;
	//std::cout << udpSocket.send(outPacket, serverIp, serverUdpPortSend) << std::endl;
}

void Network::run()
{

	tcpSocket.setBlocking(false);
	sf::Clock clock; //to set refresh rate
	while (true)
	{

		//receiving data from server: id when connected, 
		sf::Packet packet;
		if (tcpSocket.receive(packet) == sf::Socket::Done)
		{
			//std::cout << "received something\n";
			int type;

			packet >> type;
			switch (type)
			{
			case 1:
				break;
			}
		}

		//receiveng data from server: location, 
		sf::IpAddress sender;
		if (udpSocket.receive(packet, sender, serverUdpPortReceive) == sf::Socket::Done)
		{
			std::cout << "udp socket received something\n";
			int type, id;
			packet >> type >> id;
			if (type == 2) //received location
			{
				sf::Vector2f pos;
				packet >> pos.x >> pos.y;
				player->setPosition(pos);
				position = pos; //will be set by window
								//std::cout << pos.x << " " << pos.y << std::endl;
			}
		}

		//around 30 fps desired 
		if (clock.getElapsedTime().asMilliseconds() > 33)
		{
			clock.restart();
			//sending current location
			sf::Packet outPacket;
			outPacket << 1 << player->getId() << position.x << position.y;
			//std::cout << udpSocket.send(outPacket, serverIp, serverUdpPortSend) << std::endl;
			udpSocket.send(outPacket, serverIp, serverUdpPortSend);
		}


	}
}
