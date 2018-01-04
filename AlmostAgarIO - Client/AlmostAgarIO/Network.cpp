#include "stdafx.h"
#include "Network.h"
#include <iostream>
#include <thread>
#include <unordered_map>
#include <random>
#include <time.h>

Network::Network()
{
	srand(time(NULL));
	serverUdpPortReceive = rand() % 5000 + 50000;
	serverUdpPortSend = serverUdpPortReceive + 1;
}

Network::Network(Game* _game) : game(_game)
{
	srand(time(NULL));
	serverUdpPortReceive = rand() % 5000 + 50000;
	serverUdpPortSend = serverUdpPortReceive + 1;
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
	udpSocket.bind(serverUdpPortReceive); // .bind(serverUdpPortReceive, serverIP)
	udpSocket.setBlocking(false);

	sf::Socket::Status status = tcpSocket.connect(serverIp, serverTcpPort,sf::seconds(1));
	//tcpSocket.setBlocking(false);
	//tcpSocket.setBlocking(true);
	//udpSocket.bind(serverUdpPortReceive);
	if (status != sf::Socket::Done)
	{
		std::cout << "Player could not connect\n";
		return;
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
				float radius;
				packet >> id >> radius;
				player->setId(id);
				player->setRadius(radius);
				Player::setDefRad(radius);
				std::cout << "received id: " << id << "\n";

				sf::Packet udpInfo;
				udpInfo << serverUdpPortSend << serverUdpPortReceive;
				tcpSocket.send(udpInfo);
				std::cout << "sent udp ports to server\n";
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
	udpSocket.unbind();
	//delete player;
}

void Network::init(sf::Vector2f _mapSize, sf::Vector2f _mapPosition, sf::Vector2f _windowSize)
{
	//TODO Huni lefagy ha nincs szerver es megnyomod az uj jatekot!!!!

	//sending initializing parameters
	sf::Packet outPacket;
	//std::cout << "checking name before sending: " << player->getName() << "\n";
	outPacket << 3 << player->getId() << _mapSize.x << _mapSize.y << _mapPosition.x << _mapPosition.y << _windowSize.x << _windowSize.y << player->getName();// << serverUdpPortSend << serverUdpPortReceive;
	//std::cout << udpSocket.send(outPacket, serverIp, serverUdpPortSend) << std::endl;
	tcpSocket.send(outPacket);

	sf::Packet initPacket;
	tcpSocket.receive(initPacket);

	//receiving initial position
	sf::Packet initPositionPacket;
	//tcpSocket.receive(initPositionPacket);
	unsigned int id;
	unsigned int playerId = player->getId();
	initPacket >> id;
	if (id == playerId)//just making sure we received the right packet
	{
		sf::Vector2f pos;
		initPacket >> pos.x >> pos.y;
		player->setPosition(pos);
	}

	//receiving initially the whole food vector
	sf::Packet foodPacket;
	//tcpSocket.receive(foodPacket);
	std::vector<sf::Vector2f> food;
	size_t foodSize;
	float foodRadius;
	sf::Vector2f tmp;
	initPacket >> foodRadius;
	initPacket >> foodSize;
	for (size_t i = 0; i < foodSize; ++i)
	{
		initPacket >> tmp.x >> tmp.y;
		food.push_back(tmp);
	}
	game->setFood(food, foodRadius);

	//receiving other players' names and colors
	std::cout << "receiving other players' names and colors\n";
	sf::Packet nameAndColorPacket;
	//tcpSocket.receive(nameAndColorPacket);
	size_t packetSize;
	std::string name;
	sf::Color color;
	int r, g, b;
	initPacket >> packetSize;
	for (size_t i = 0; i < packetSize; i++)
	{
		initPacket >> id >> name >> r >> g >> b;
		if (id == playerId)
		{
			player->setName(name);
			player->setColor(sf::Color(r,g,b));
			std::cout << "set my own name: " << player->getName() << "\n";
			std::cout << "set my own color: " << (int)player->getColor().r << "," << (int)player->getColor().g << "," << (int)player->getColor().b << "\n";
		}
		else
		{
			std::cout << "set enemy's name: " << name << "\n";
			player->setEnemyName(id, name);
			player->setEnemyColor(id, sf::Color(r, g, b));
			std::cout << "set enemies color: " << (int)player->getEnemyColor(id).r << "," << (int)player->getEnemyColor(id).g << "," << (int)player->getEnemyColor(id).b << "\n";
		}
	}
	player->setInitReady(true);
	tcpSocket.setBlocking(false);
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
	unsigned short udpPortReceivedFrom;
	if (udpSocket.receive(packet, sender, udpPortReceivedFrom) == sf::Socket::Done)
	{
		//std::cout << "getResponse: received something\n";
		int type;
		unsigned int id;
		packet >> type;
		switch (type)
		{
			//received new pos
		case 2:
		{
			sf::Vector2f pos;
			size_t playersSize;
			float radius;
			int myId = player->getId();
			packet >> playersSize;
			//if (playersSize != player->getEnemies().size() + 1) player->resetEnemies();
			for (size_t i = 0; i < playersSize; i++)
			{
				packet >> id >> pos.x >> pos.y >> radius;
				if (id == myId)
				{
					player->setPosition(pos);
					player->setRadius(radius);
				}
				else
				{
					player->updateEnemy(id, pos, radius);
				}
			}

			break;
		}
		//receiveng food change
		case 4:
		{
			
			int index;
			sf::Vector2f newFood;
			size_t tmpSize;
			packet >> tmpSize;
			for (size_t i = 0; i < tmpSize; ++i)
			{
				packet >> index >> newFood.x >> newFood.y;
				game->updateFood(index, newFood);
			}
			//std::cout << "received food update\n";
			break;
		}
		
			
		}
	}
	tcpSocket.setBlocking(false);
	if (tcpSocket.receive(packet) == sf::Socket::Done)
	{
		int type;
		unsigned int id;
		packet >> type;
		switch (type)
		{
		//received ranking update
		case 6:
		{
			std::cout << "received ranking update\n";
			size_t rankingSize;
			std::vector<unsigned int> ranking;
			unsigned int id;
			packet >> rankingSize;
			for (size_t i = 0; i < rankingSize; i++)
			{
				packet >> id;
				ranking.push_back(id);
				std::cout << id << " ";
			}
			std::cout<<"\n";
			game->setLeaderboard(ranking);
			break;
		}
		//received new player info
		case 7:
		{
			std::cout << "received new player info";
			std::string name;
			sf::Color color;
			packet >> id >> name >> color.r >> color.g >> color.b;
			player->setEnemyName(id, name);
			player->setEnemyColor(id, color);
			break;
		}
		//receive game over message
		case 8:
		{
			packet >> id;
			if (id == player->getId())
			{
				game->gameOver();
			}
			else
			{
				player->deleteEnemy(id);
			}
			break;
		}
		//receive skill upgrade possible message
		case 9:
		{
			int numberOfUpgrades;
			packet >> numberOfUpgrades;
			player->canUpgrade(numberOfUpgrades);
			std::cout << "received " << numberOfUpgrades << " upgrades\n";
			break;
		}
		//receive invisibility info
		case 12:
		{
			bool isInvisible;
			packet >> id >> isInvisible;
			std::cout << "player " << id << " invisibility: " << isInvisible << "\n";
			if (id == player->getId())
			{
				player->setInvisible(isInvisible);
			}
			else
			{
				player->setEnemyInvisible(id, isInvisible);
			}
			break;
		}
		//receive skill availabity info
		case 13:
		{
			bool invisibleAvailable, speedAvailable;
			packet >> invisibleAvailable >> speedAvailable;
			player->setInvisibleAvailable(invisibleAvailable);
			player->setSpeedAvailable(speedAvailable);
			std::cout << "received skill availabity info\n";
			break;
		}
		}
	}
	tcpSocket.setBlocking(true);
}

void Network::sendKey(int key, bool active)
{
	sf::Packet keyPacket;
	//if upgrade key pressed
	if (key == 4 || key == 16 || key == 17 || key == 22)
	{
		keyPacket << 10 << player->getId() << key;
		tcpSocket.send(keyPacket);
		std::cout << "sent skill upgrade\n";
	}
	//if skill use key pressed 
	//d is speed, s is invisibility
	else if (key == 18 || key == 3)
	{
		keyPacket << 11 << player->getId() << key << active;
		tcpSocket.send(keyPacket);
		std::cout << "sent skill use\n";
	}
}

bool Network::test()
{
	sf::Socket::Status status = tcpSocket.connect(serverIp, serverTcpPort+1, sf::seconds(1));
	if (status != sf::Socket::Done)
	{
		std::cout << "Connection failed\n";
		tcpSocket.disconnect();
		return false;
	}
	if (status == sf::Socket::Done)
	{
		std::cout << "Connection can be established\n";
		tcpSocket.disconnect();
		return true;
	}
	
}
