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
	//udpSocket.bind(udpPortReceive);
	//udpSocket.setBlocking(false);
	//selector.add(udpSocket);
	clock.restart();
	foodGenerator.generateFood(1000);
	food = foodGenerator.getFood();
	rankingChanged = false;
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
					//add player to ranking and refresh order
					ranking.push_back(players.at(id - 1));
					checkRanking();
					if (client->send(packet) != sf::Socket::Done) //Send client id
						std::cout << "Error sending id" << std::endl;
					else
						std::cout << "\nId sent to " << client->getRemoteAddress() << " on port " << client->getRemotePort() << "\n";
					
					sf::Packet udpInfo;
					unsigned short udpPortReceive;
					unsigned short udpPortSend;
					client->receive(udpInfo);
					udpInfo >> udpPortReceive >> udpPortSend;
					selector.add(*players.at(id - 1).bindUdpSocket(udpPortReceive, udpPortSend));
					std::cout << "received udp ports from client: " << udpPortReceive << " , " << udpPortSend << "\n";
				}

			}			
			//check udp socket
			//player locations should arrive here
			/*else if (selector.isReady(udpSocket))
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

			
			}*/
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::unordered_map<int, Player>::iterator it = players.begin(), next_it = players.begin(); it != players.end(); it = next_it)
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
								unsigned int id;
								sf::Vector2f _mapSize;
								sf::Vector2f _mapPosition;
								sf::Vector2f _windowSize;
								std::string name;
								
								packet >> id >> _mapSize.x >> _mapSize.y >> _mapPosition.x >> _mapPosition.y >> _windowSize.x >> _windowSize.y >> name;// >> udpPortReceive >> udpPortSend;
								//Player *player = &players.find(id)->second;
								Player* player = &players.at(id);
								player->setMapSize(_mapSize);
								player->setMapPosition(_mapPosition);
								player->setWindowSize(_windowSize);
								player->setName(name);
								//selector.add(*player->bindUdpSocket(udpPortReceive, udpPortSend));

								sf::Packet initPacket;


								//generating random start position and sending it to player
								player->initPosition();
								sf::Packet initPositionPacket;
								initPositionPacket << player->getId() << player->getPosition().x << player->getPosition().y;
								initPacket << player->getId() << player->getPosition().x << player->getPosition().y;
								//(*it).second.getTcpSocket()->send(initPositionPacket);

								//sending whole food vector to client
								sf::Packet foodPacket;
								initPacket << foodGenerator.getFoodRadius();
								initPacket << food.size();
								for (sf::Vector2f f : food)
								{
									initPacket << f.x << f.y;
								}
								//(*it).second.getTcpSocket()->send(foodPacket);

								//generating random color
								player->setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

								//sending every player's name and color for new player
								std::cout << "sending every player's name and color for new player\n";
								sf::Packet nameAndColorPacket;
								initPacket << players.size();
								for (std::unordered_map<int, Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
								{
									initPacket << it2->second.getId() << it2->second.getName() << (int)it2->second.getColor().r << (int)it2->second.getColor().g << (int)it2->second.getColor().b;
									std::cout << "id: " << it2->second.getId() << " name: " << it2->second.getName() << " color: (" <<(int)it2->second.getColor().r << "," << (int)it2->second.getColor().g << "," << (int)it2->second.getColor().b << ")\n";
								}
								//(*it).second.getTcpSocket()->send(nameAndColorPacket);
								(*it).second.getTcpSocket()->send(initPacket);

								//sending new player's name and color for others
								
								std::cout << "sending new player's name and color for others\n";
								sf::Packet newPlayer;
								newPlayer << 7 << it->second.getId() << it->second.getName() << it->second.getColor().r << it->second.getColor().g << it->second.getColor().b;
								for (std::unordered_map<int, Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
								{
									if(it2->second.getId() != it->second.getId())
										(*it2).second.getTcpSocket()->send(newPlayer);
								}
								

								//player->setPosition(sf::Vector2f((_mapPosition.x+_mapSize.x/2),(_mapPosition.y+_mapSize.y/2)));
								std::cout << "\nreceived init params: "<<player->getPosition().x<<", "<<player->getPosition().y<<"\n";

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
								unsigned int id = it->second.getId();
								it++;
								deletePlayerFromRanking(id);
								//needs testing
								playerDied(id);
								continue;
									//players.erase(it++);
									
								//it = players.erase(it);
								//if (players.size() == 0) break;
								//it = players.begin();
							}
						}
					}
					if (selector.isReady(*(*it).second.getUdpSocket()))
					{
						sf::Packet packet;
						sf::IpAddress sender;
						unsigned short udpPortReceive;
						if (it->second.getUdpSocket()->receive(packet, sender, udpPortReceive) == sf::Socket::Done)
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
			for (std::unordered_map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
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

			//generating ranking packet
			sf::Packet rankingPacket;
			rankingPacket << 6 << ranking.size();
			for (Player player : ranking)
			{
				rankingPacket << player.getId();
			}

			for (std::unordered_map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				//sending players' positions
				it->second.getUdpSocket()->send(positionPacket, it->second.getPlayerIp(), it->second.getUdpPortSend());

				//sending changed food
				if (foodToUpdate.size() != 0)
				{
					it->second.getUdpSocket()->send(foodPacket, it->second.getPlayerIp(), it->second.getUdpPortSend());
				}
				
				//sending ranking if changed
				if (rankingChanged)
				{	
					it->second.getTcpSocket()->send(rankingPacket);
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
	float speed = (float)(3.2 - (0.005 * player->getRadius()));
	if (speed <= 0.06f) speed = 0.06f;

	//std::cout << "Size: " << circle.getRadius() << " Speed: " << speed << std::endl;
	float length = sqrt(distance.x*distance.x + distance.y*distance.y);
	vec.x = speed * distance.x / length;
	vec.y = speed * distance.y / length;

	if (abs(distance.x) < 10 || ((player->getPosition().x - vec.x) <= player->getMapPosition().x && vec.x <= 0) || ((player->getPosition().x + vec.x) >= (player->getMapPosition().x + player->getMapSize().x) && vec.x >= 0) /*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*/) {
		vec.x = 0;
	}
	if (abs(distance.y) < 10 || ((player->getPosition().y - vec.y) <= player->getMapPosition().y && vec.y <= 0) || ((player->getPosition().y - vec.y) >= (player->getMapPosition().y + player->getMapSize().y) && vec.y >= 0)/*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*/) {
		vec.y = 0;
	}
	//std::cout << "playerPos: " << player->getPosition().x << "," << player->getPosition().y << "\n";
	pos = player->getPosition() + vec;
	//std::cout << "vec: " << vec.x << "," << vec.y << " dist: " << distance.x << "," << distance.y << " length: " << length << "\n";
	//std::cout << "vec: " << vec.x << "," << vec.y << " pos: " << pos.x << "," << pos.y << "\n";
	player->setPosition(pos);
	//std::cout << "playerPos: " << player->getPosition().x << "," << player->getPosition().y << "\n";



	bool eatSomeOne = false;
	int eatenID = -1;
	float sizeDif = 5; //meret kulonbseg ahhoz h megeegyem a kisebbet

	for (std::unordered_map<int, Player>::iterator it = players.begin(); it != players.end() && !eatSomeOne; it++)
	{
		if (it->first != player->getId()) {
			sf::Vector2f distancePlayer(player->getPosition().x - it->second.getPosition().x, player->getPosition().y - it->second.getPosition().y);
			float lenghtPlayer = sqrt(distancePlayer.x*distancePlayer.x + distancePlayer.y*distancePlayer.y);
			if (lenghtPlayer < player->getRadius() && player->getRadius() > (it->second.getRadius() + sizeDif)) {
				eatenID = it->first;
				eatSomeOne = true;
				std::cout << "A " << player->getId() << " jatekos megette a kovektkezo jatekost: " << eatenID << std::endl;
				//TODO barkinek megnovelni a player meretet valamennyivel , akar szintetlepve is majd
				//itt ne az osszes radiust adjuk at annak aki megevett valakit
			}
		}
	}
	//if somebody ate someone
	if (eatSomeOne == true)
	{
		//eatenID  ez a megett jatekos :)
		deletePlayerFromRanking(eatenID);
		//players.erase(eatenID);
		playerDied(eatenID);

		//TODO: Huni: send gameover packet
		//TODO: Huni: checkranking
		//TODO: Huni: delete eaten player
	}
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
		if (lenght < player->getRadius()) {
			food[i] = foodGenerator.updateElement(i);
			player->setRadius(player->getRadius() + 0.5f);
			foodToUpdate.emplace(i, food[i]);	
			checkRanking();
		}
	}
}

struct less
{
	bool operator()(const Player& lhs, const Player& rhs) const {
		return lhs.getRadius() < rhs.getRadius();
	}
};

void Server::checkRanking()
{
	if (std::is_sorted(std::begin(ranking), std::end(ranking), less()))
		return;

	std::sort(std::begin(ranking), std::end(ranking), less());
	rankingChanged = true;
}

void Server::deletePlayerFromRanking(int id)
{
	for (size_t i = 0; i < ranking.size(); i++)
	{
		if (ranking[i].getId() == id)
		{
			ranking.erase(ranking.begin() + i);
			return;
		}
	}
}

void Server::playerDied(unsigned int id)
{
	for (std::unordered_map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
	{
			sf::Packet gameOverPacket;
			gameOverPacket << 8 << id;
			it->second.getTcpSocket()->send(gameOverPacket);
	}
	selector.remove(*players.at(id).getTcpSocket());
	players.at(id).getTcpSocket()->disconnect();
	selector.remove(*players.at(id).getUdpSocket());
	players.at(id).getUdpSocket()->unbind();
	players.erase(id);
}
