#include "stdafx.h"
#include "Network.h"
#include<iostream>
#include <thread>

Network::Network(sf::IpAddress _serverIp, Player* _player)
{
	serverIp = _serverIp;
	player = _player;

	udpSocket.bind(serverUdpPortReceive, serverIp);
	udpSocket.setBlocking(false);
}


Network::~Network()
{

}

void Network::setPosition(sf::Vector2f _position)
{
	position = _position;
}

void Network::connectPlayer()
{
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
			std::cout << udpSocket.send(outPacket, serverIp, serverUdpPortSend) << std::endl;
		}


	}
}
