#include "stdafx.h"
#include "Network.h"
#include<iostream>

Network::Network(sf::IpAddress _serverIp)
{
	serverIp = _serverIp;

}


Network::~Network()
{

}

void Network::connectPlayer()
{
	sf::Socket::Status status = tcpSocket.connect(serverIp, serverTcpPort);
	udpSocket.bind(serverUdpPortReceive);
	if (status != sf::Socket::Done)
	{
		std::cout << "Player could not connect\n";
	}
	else
	{
		std::cout << "Player connected\n";

		sf::Packet packet;
		if (tcpSocket.receive(packet) != sf::Socket::Done)
		{
			int type;
			if (type == 0)
			{
				int id;
				packet >> id;
				std::cout << id;
			}
		}
	}
}
