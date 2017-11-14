#include "stdafx.h"
#include "Server.h"
#include <string>
#include <iostream>
#include <algorithm>


Server::Server()
{
	id = 0;
	listener.listen(port);
	selector.add(listener);
	running = true;
	udpSocket.bind(udpPortReceive);
	udpSocket.setBlocking(false);
	selector.add(udpSocket);
	clock.restart();
}


Server::~Server()
{
}


void Server::run()
{
	// Endless loop that waits for new connections
	while (running)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				//std::unique_ptr<sf::TcpSocket> client = std::make_unique<sf::TcpSocket>();
				sf::TcpSocket* client = new sf::TcpSocket();
				if (listener.accept(*client) == sf::Socket::Done)
				{
					std::cout << client->getRemotePort();
					// Add the new client to the clients list
					///TODO: should randomize start position
					players.insert(std::make_pair(id,(Player(id, sf::Vector2f(3000, 3000), client))));
					id++;
					// Add the new client to the selector so that we will
					// be notified when he sends something
					//selector.add(*players.back().getTcpSocket());
					selector.add(*client);
					players.at(id - 1).setPlayerIp(client->getRemoteAddress());
					//should check if player name aleady exists?
					sf::Packet packet;
					packet << 0;
					//packet << players.back().getId();
					packet << id - 1;
					if (client->send(packet) != sf::Socket::Done) //Send client id
						std::cout << "Error sending id" << std::endl;
					else
						std::cout << "\nId sent to " << client->getRemoteAddress() << " on port " << client->getRemotePort() << "\n";
				}

			}			
			//check udp socket
			//player locations should arrive here
			else if (selector.isReady(udpSocket))
			{
				//std::cout << "udp socket received something\n";
				sf::Packet packet;
				sf::IpAddress sender;
				if (udpSocket.receive(packet, sender, udpPortReceive) == sf::Socket::Done)
				{
					int type;
					packet >> type;
					int id;
					sf::Vector2f pos;
					
					//Player player;

					switch (type)
					{
						//received location
					case 1:						
					{
						packet >> id >> pos.x >> pos.y;
						Player* player = &players.at(id);

						sf::Vector2f vec;
						sf::Vector2f distance(pos.x - player->getPosition().x, pos.y - player->getPosition().y);
						float speed = 0.02 - (0.0005 * player->getRadius());
						if (speed <= 0.001) speed = 0.001;

						//std::cout << "Size: " << circle.getRadius() << " Speed: " << speed << std::endl;
						float length = sqrt(distance.x*distance.x + distance.y*distance.y);
						vec.x = speed * distance.x / length;
						vec.y = speed * distance.y / length;

						if (abs(distance.x) < 2 || ((player->getPosition().x - vec.x) <= player->getMapPosition().x && vec.x <= 0) || ((player->getPosition().x + vec.x) >= (player->getMapPosition().x + player->getMapSize().x) && vec.x >= 0) /*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*/) {
							vec.x = 0;
						}
						if (abs(distance.y) < 2 || ((player->getPosition().y - vec.y) <= player->getMapPosition().y && vec.y <= 0) || ((player->getPosition().y - vec.y) >= (player->getMapPosition().y + player->getMapSize().y) && vec.y >= 0)/*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*/) {
							vec.y = 0;
						}

						

						
						pos += vec;
						std::cout << "vec: " << vec.x << "," << vec.y << " dist: " << distance.x << "," << distance.y << " length: " << length << "\n";
						//std::cout << "vec: " << vec.x << "," << vec.y << " pos: " << pos.x << "," << pos.y << "\n";
						player->setPosition(pos);
						//std::cout << "playerPos: " << player->getPosition().x << "," << player->getPosition().y << "\n";

						//std::cout << "received player location: (" << pos.x << "," << pos.y << ")\n";

						//sf::Packet outPacket;
						//outPacket << 2 << id << pos.x + 1 << pos.y + 1;
						//std::cout << "responding: " << udpSocket.send(outPacket, sender, udpPortSend) << std::endl;
						break; 
					}
					case 3:
						
						
						break;
					}
				}
				else
				{

				}

			
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				//for (size_t i = 0; i < players.size(); i++)
				for (auto it = players.begin(); it != players.end(); it++)
				{
					//sf::TcpSocket& client = it->getSocket;
					if (selector.isReady(*(*it).second.getTcpSocket()))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						sf::Socket::Status status = (*it).second.getTcpSocket()->receive(packet);
						if ( status == sf::Socket::Done)
						{
							//std::cout << "received something\n";
							int type;
							packet >> type;
							if (type == 3)
							{
								int id;
								sf::Vector2f _mapSize;
								sf::Vector2f _mapPosition;
								sf::Vector2f _windowSize;
								packet >> id >> _mapSize.x >> _mapSize.y >> _mapPosition.x >> _mapPosition.y >> _windowSize.x >> _windowSize.y;
								Player player = players.find(id)->second;
								player.setMapSize(_mapSize);
								player.setMapPosition(_mapPosition);
								player.setWindowSize(_windowSize);
								std::cout << "received init params\n";
							}
						}
						else
						{
							//not sure when this occours
							//std::cout << status;
							if (sf::Socket::Disconnected == status)
							{
								//(*it).second.getTcpSocket()->disconnect();
								//selector.remove(*(*it).second.getTcpSocket());
								//players.erase(it);
								
							}
							
						}
					}

					
					
				}
			}
		}


		if (clock.getElapsedTime() > sf::milliseconds(33))
		{
			std::cout << "nr of players: " << players.size() << "\n";
			for (auto it = players.begin(); it != players.end(); it++)
			{
				sf::Packet outPacket;
				outPacket << 2 << it->first << it->second.getPosition().x << it->second.getPosition().y;
				std::cout << "sent position: " << it->second.getPosition().x << "," << it->second.getPosition().y << "\n";
				udpSocket.send(outPacket, it->second.getPlayerIp(), udpPortSend);
			}
			clock.restart();
			std::cout << "sent new positions\n";
		}

		/*
		
		sf::Packet packet;
		sf::IpAddress sender;
		if (udpSocket.receive(packet, sender, udpPortReceive) == sf::Socket::Done)
		{
			std::cout << "udp socket received something\n";
			int type;
			packet >> type;

			switch (type)
			{
				//received location
			case 1:
				int id;
				sf::Vector2f pos;
				packet >> id >> pos.x >> pos.y;

				sf::Packet outPacket;
				outPacket << 2 << id << pos.x + 1 << pos.y << +1;
				udpSocket.send(packet, sender, udpPortSend);
				break;
			}

			std::cout << type << std::endl;
		
		}
		*/


	}
}
