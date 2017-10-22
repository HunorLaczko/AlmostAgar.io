#pragma once
#include<SFML/Network.hpp>


class Network
{
	sf::IpAddress serverIp;
	unsigned short serverTcpPort = 50000;
	unsigned short serverUdpPortSend = 50005;
	unsigned short serverUdpPortReceive = 50006;
	sf::TcpSocket tcpSocket;
	sf::UdpSocket udpSocket;

public:
	Network(sf::IpAddress _serverIp);
	~Network();

	void connectPlayer();
	void sendPosition(sf::Vector2f position);
	void sendKey(char key);
};

