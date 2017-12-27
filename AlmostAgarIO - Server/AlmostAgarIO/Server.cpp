#include "stdafx.h"
#include "Server.h"
#include <string>
#include <iostream>
#include <algorithm>


Server::Server() : foodGenerator(sf::Vector2f(1000,750), sf::Vector2f(4000,3000))
{
	id = 0;
	listener.listen(port);
	selector.add(listener);
	running = true;
	udpSocket.bind(udpPortReceive);
	udpSocket.setBlocking(false);
	selector.add(udpSocket);
	clock.restart();
	foodGenerator.generateFood(1000);
	food = foodGenerator.getFood();
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
					players.insert(std::make_pair(id,(Player(id, sf::Vector2f(3000, 2000), client))));
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
					packet << players.at(id - 1).getRadius();
					if (client->send(packet) != sf::Socket::Done) //Send client id
						std::cout << "Error sending id" << std::endl;
					else
						std::cout << "\nId sent to " << client->getRemoteAddress() << " on port " << client->getRemotePort() << "\n";
					//players.at(id - 1).init(foodGenerator.getFood());
					//setFood(id - 1);
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
						updatePlayerPosition(id, pos);
						updateFood(id);
						break; 
					}
					
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
				for (std::map<int, Player>::iterator it = players.begin(), next_it = players.begin(); it != players.end(); it = next_it)
				{
					next_it = it; ++next_it;
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
								//initializing parameters from client
								int id;
								sf::Vector2f _mapSize;
								sf::Vector2f _mapPosition;
								sf::Vector2f _windowSize;
								packet >> id >> _mapSize.x >> _mapSize.y >> _mapPosition.x >> _mapPosition.y >> _windowSize.x >> _windowSize.y;
								Player *player = &players.find(id)->second;
								player->setMapSize(_mapSize);
								player->setMapPosition(_mapPosition);
								player->setWindowSize(_windowSize);

								//generating random start position and sending it to player
								player->initPosition();
								sf::Packet initPositionPacket;
								initPositionPacket << player->getId() << player->getPosition().x << player->getPosition().y;
								(*it).second.getTcpSocket()->send(initPositionPacket);

								//sending whole food vector to client
								sf::Packet foodPacket;
								foodPacket << foodGenerator.getFoodRadius();
								foodPacket << food.size();
								for (sf::Vector2f f : food)
								{
									foodPacket << f.x << f.y;
								}
								(*it).second.getTcpSocket()->send(foodPacket);

								//player->setPosition(sf::Vector2f((_mapPosition.x+_mapSize.x/2),(_mapPosition.y+_mapSize.y/2)));
								std::cout << "\n\n\nreceived init params: "<<player->getPosition().x<<", "<<player->getPosition().y<<"\n\n\n\n";

							}
							else if (type == 4)
							{
								//
							}
						}
						else
						{
							//not sure when this occours
							std::cout << status;
							if (sf::Socket::Disconnected == status)
							{
								//(*it).second.getTcpSocket()->disconnect();
								selector.remove(*(*it).second.getTcpSocket());
								
								std::cout << "disconnected player: " << it->second.getId() << std::endl;
								players.erase(it++);
								//it = players.erase(it);
								//if (players.size() == 0) break;
								//it = players.begin();
							}
						}
					}	
				}
			}
		}


		if (clock.getElapsedTime() > sf::milliseconds(33) )
		{
			//std::cout << "nr of players: " << players.size() << "\n";
			//generating packet with all players' location
			sf::Packet positionPacket;
			positionPacket << 2 << players.size();
			for (std::map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				positionPacket << it->first << it->second.getPosition().x << it->second.getPosition().y << it->second.getRadius();
			}

			//generating food packet
			sf::Packet foodPacket;
			foodPacket << 4 << foodToUpdate.size();
			for (std::unordered_map<int, sf::Vector2f>::iterator it = foodToUpdate.begin(); it != foodToUpdate.end(); it++)
			{
				foodPacket << it->first << it->second.x << it->second.y;
			}

			for (std::map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				//sending players' positions
				udpSocket.send(positionPacket, it->second.getPlayerIp(), udpPortSend);

				///itt nem csak a elso jatekos kapja meg a foodto updatet?? mert a clear utan ez tuti ures lesz de az nem 
				//sending changed food
				if (foodToUpdate.size() != 0)
				{
					udpSocket.send(foodPacket, it->second.getPlayerIp(), udpPortSend);
				}
			  	
			}
			clock.restart();
			foodToUpdate.clear();

			//std::cout << "sent new positions\n";
		}

	
	}
}

void Server::updatePlayerPosition(int id, sf::Vector2f pos)
{
	Player* player = &players.at(id);

	sf::Vector2f vec;
	sf::Vector2f distance(pos.x - player->getPosition().x, pos.y - player->getPosition().y);
	float speed = (float)(2.2 - (0.005 * player->getRadius()));
	if (speed <= 0.06f) speed = 0.06f;

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
	//std::cout << "playerPos: " << player->getPosition().x << "," << player->getPosition().y << "\n";
	pos = player->getPosition() + vec;
	//std::cout << "vec: " << vec.x << "," << vec.y << " dist: " << distance.x << "," << distance.y << " length: " << length << "\n";
	//std::cout << "vec: " << vec.x << "," << vec.y << " pos: " << pos.x << "," << pos.y << "\n";
	player->setPosition(pos);
	//std::cout << "playerPos: " << player->getPosition().x << "," << player->getPosition().y << "\n";
}

void Server::setFood(unsigned int id)
{
}

void Server::updateFood(unsigned int id)
{
	Player* player = &players.at(id);
	float foodRadius = foodGenerator.getFoodRadius();
	for (int i = 0; i < food.size(); i++) {
		sf::Vector2f distance(player->getPosition().x - food[i].x, player->getPosition().y - food[i].y);
		float lenght = sqrt(distance.x*distance.x + distance.y*distance.y);

		//check if player ate a food
		if (lenght < (player->getRadius() )) {
			food[i] = foodGenerator.updateElement(i);
			player->setRadius(player->getRadius() + 0.5f);
			foodToUpdate.emplace(i, food[i]);

			//sending food change to every player
			/*for (std::map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				sf::Packet outPacket;
				outPacket << 4 << i << food[i].x << food[i].y;
				udpSocket.send(outPacket, it->second.getPlayerIp(), udpPortSend);
			}	*/		
		}
	}
}
