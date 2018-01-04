#include "stdafx.h"
#include "Server.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>


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
	rankingChanged = true;
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
					//std::cout << client->getRemotePort();
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
					rankingChanged = true;
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
								std::vector<sf::Color> colors = 
								{
									sf::Color(128,0,0), sf::Color(139,0,0), sf::Color(165,42,42), sf::Color(178,34,34),
									sf::Color(220,20,60), sf::Color(255,0,0), sf::Color(255,99,71), sf::Color(255,127,80),
									sf::Color(205,92,92), sf::Color(240,128,128), sf::Color(233,150,122), sf::Color(250,128,114),
									sf::Color(255,160,122), sf::Color(255,69,0), sf::Color(255,140,0), sf::Color(255,165,0),
									sf::Color(255,215,0), sf::Color(184,134,11), sf::Color(218,165,32), sf::Color(245,222,179),
									sf::Color(189,183,107), sf::Color(240,230,140), sf::Color(128,128,0), sf::Color(255,255,0),
									sf::Color(154,205,50), sf::Color(85,107,47), sf::Color(107,142,35), sf::Color(124,252,0),
									sf::Color(127,255,0), sf::Color(173,255,47), sf::Color(0,100,0), sf::Color(0,128,0),
									sf::Color(34,139,34), sf::Color(0,255,0), sf::Color(50,205,50), sf::Color(144,238,144),
									sf::Color(152,251,152), sf::Color(143,188,143), sf::Color(0,250,154), sf::Color(0,255,127),
									sf::Color(46,139,87), sf::Color(102,205,170), sf::Color(60,179,113), sf::Color(32,178,170),
									sf::Color(47,79,79), sf::Color(0,128,128), sf::Color(0,139,139), sf::Color(0,255,255),
									sf::Color(0,255,255), sf::Color(0,206,209), sf::Color(64,224,208), sf::Color(255,228,225),
									sf::Color(72,209,204), sf::Color(127,255,212), sf::Color(176,224,230), sf::Color(255,235,205),
									sf::Color(95,158,160), sf::Color(70,130,180), sf::Color(100,149,237), sf::Color(0,191,255),
									sf::Color(30,144,255), sf::Color(173,216,230), sf::Color(135,206,235), sf::Color(135,206,250),
									sf::Color(25,25,112), sf::Color(0,0,128), sf::Color(0,0,139), sf::Color(0,0,205),
									sf::Color(0,0,255), sf::Color(65,105,225), sf::Color(138,43,226), sf::Color(75,0,130),
									sf::Color(72,61,139), sf::Color(106,90,205), sf::Color(123,104,238), sf::Color(147,112,219),
									sf::Color(139,0,139), sf::Color(148,0,211), sf::Color(153,50,204), sf::Color(186,85,211),
									sf::Color(128,0,128), sf::Color(216,191,216), sf::Color(221,160,221), sf::Color(238,130,238),
									sf::Color(255,0,255), sf::Color(218,112,214), sf::Color(199,21,133), sf::Color(219,112,147),
									sf::Color(255,20,147), sf::Color(255,105,180), sf::Color(255,182,193), sf::Color(255,192,203),
									sf::Color(255,228,196), sf::Color(139,69,19), sf::Color(160,82,45), sf::Color(210,105,30),
									sf::Color(205,133,63),  sf::Color(244,164,96), sf::Color(222,184,135), sf::Color(210,180,140),
									sf::Color(188,143,143), sf::Color(255,228,181), sf::Color(255,222,173), sf::Color(255,218,185),
									sf::Color(255,239,213), sf::Color(112,128,144), sf::Color(220,220,220),	sf::Color(211,211,211),
									sf::Color(119,136,153), sf::Color(176,196,222), sf::Color(230,230,250), sf::Color(240,255,240), 
								    sf::Color(105,105,105), sf::Color(128,128,128), sf::Color(169,169,169),	sf::Color(192,192,192)
								};
								player->setColor(colors[rand() % colors.size()]);

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
								it->second.setInitReady(true);

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
							//receive skill upgrade
							else if (type == 10)
							{
								unsigned int id;
								int keyCode;
								packet >> id >> keyCode;
								it->second.updateSkill(keyCode);
							}
							//receive skill use
							else if (type == 11)
							{
								unsigned int id;
								int keyCode;
								bool active;
								packet >> id >> keyCode >> active;
								if (keyCode == 3)
								{
									if (active)
									{
										it->second.speedActivate();
									}
									else
									{
										it->second.speedDeActivate();
									}
								}
								else if (keyCode == 18)
								{
									if (active)
									{
										it->second.invisibleActivate();
									}
									else
									{
										it->second.invisibleDeActivate();
									}
								}
							}
						}
						else
						{
							std::cout << status;
							if (sf::Socket::Disconnected == status)
							{
								std::cout << "disconnected player: " << it->second.getId() << std::endl;
								unsigned int id = it->second.getId();
								it++;
								deletePlayerFromRanking(id);
								playerDied(id);
								continue;
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
														
								//received location
							if(type == 1)
							{
								packet >> id >> pos.x >> pos.y;
								//might cause problem with iterator if somebody dies in this update that's why I check this
								if (updatePlayerPosition(id, pos))
								{
									break;
								}
								updateFood(id);
							}

							
						}
					}
				}
			}
		}

		if (clock.getElapsedTime() > sf::milliseconds(33))
		{
			//std::cout << "nr of players: " << players.size() << "\n";
			//generating packet with all players' location
			sf::Packet positionPacket;
			positionPacket << 2 << players.size();
			for (std::unordered_map<int, Player>::iterator it = players.begin(); it != players.end(); it++)
			{
				//a vegen a getpoinst azert kell h a meret novekedes ellenere is elmentse a kliens az ellenseg 330feluli pontjait
				positionPacket << it->first << it->second.getPosition().x << it->second.getPosition().y << (it->second.getRadius() + it->second.getPoints());
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
				if (!it->second.getInitReady()) continue;
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
					std::cout << "sent ranking update\n";
					it->second.getTcpSocket()->send(rankingPacket);
					rankingChanged = false;
				}

				//sending skill upgrade message
				if (it->second.getUpdateAvailable() >= 0 && it->second.getCanUpdateNumberChanged())
				{
					sf::Packet upgradePacket;
					upgradePacket << 9 << it->second.getUpdateAvailable();
					it->second.setCanUpdateNumberChanged(false);
					std::cout << "sent " << it->second.getUpdateAvailable() << " upgrades to player " << it->second.getId() << '\n';
					it->second.getTcpSocket()->send(upgradePacket);
				}

				//sending my invisibility for everyone if it changed
				if (it->second.getInvisibilityChanged())
				{
					sf::Packet invisiblePacket;
					invisiblePacket << 12 << it->second.getId() << it->second.isInvisible();

					for (std::unordered_map<int, Player>::iterator it2 = players.begin(); it2 != players.end(); it2++)
					{
						it2->second.getTcpSocket()->send(invisiblePacket);
					}
					it->second.setInvisibilityChanged(false);
				}	

				//sending skill availabity's if changed
				if (it->second.getInvisibleAvailableChanged() || it->second.getSpeedAvailableChanged())
				{
					sf::Packet skillAvailablePacket;
					skillAvailablePacket << 13 << it->second.getInvisibleAvailable() << it->second.getSpeedAvailable();
					it->second.getTcpSocket()->send(skillAvailablePacket);
					it->second.setInvisibleAvailableChanged(false);
					it->second.setSpeedAvailableChanged(false);
				}
			}
			clock.restart();
			foodToUpdate.clear();

			//std::cout << "sent new positions\n";
		}

	
	}
}

bool Server::updatePlayerPosition(int id, sf::Vector2f pos)
{
	Player* player = &players.at(id);

	sf::Vector2f vec;
	sf::Vector2f distance(pos.x - player->getPosition().x, pos.y - player->getPosition().y);

	player->skillChecking(); // modify speed if need

	
	float length = sqrt(distance.x*distance.x + distance.y*distance.y);

	vec.x = player->getSpeed() * distance.x / length;
	vec.y = player->getSpeed() * distance.y / length;

	if (abs(distance.x) < 10 || ((player->getPosition().x - vec.x) <= player->getMapPosition().x && vec.x <= 0) || ((player->getPosition().x + vec.x) >= (player->getMapPosition().x + player->getMapSize().x) && vec.x >= 0) /*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*/) {
		vec.x = 0;
	}
	if (abs(distance.y) < 10 || ((player->getPosition().y - vec.y) <= player->getMapPosition().y && vec.y <= 0) || ((player->getPosition().y - vec.y) >= (player->getMapPosition().y + player->getMapSize().y) && vec.y >= 0)/*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*/) {
		vec.y = 0;
	}
	pos = player->getPosition() + vec;
	player->setPosition(pos);
	
	bool eatSomeOne = false;
	int eatenID = -1;
	float sizeDif = 5; //meret kulonbseg ahhoz h megeegyem a kisebbet

	//TODO skillekhez ha lathatatlan ne nezze az utkozest - elvileg kesz az iffeknel
	for (std::unordered_map<int, Player>::iterator it = players.begin(); it != players.end() && !eatSomeOne && player->isInvisible() == false ; it++)
	{
		if (it->first != player->getId() && it->second.isInvisible() == false) {
			sf::Vector2f distancePlayer(player->getPosition().x - it->second.getPosition().x, player->getPosition().y - it->second.getPosition().y);
			float lenghtPlayer = sqrt(distancePlayer.x*distancePlayer.x + distancePlayer.y*distancePlayer.y);
			if (lenghtPlayer < player->getRadius() && (player->getRadius() + player->getPoints()) > (it->second.getRadius() + it->second.getPoints() + sizeDif)) {
				eatenID = it->first;
				eatSomeOne = true;
				std::cout << "A " << player->getId() << " jatekos megette a kovektkezo jatekost: " << eatenID << std::endl;
				player->setRadius(sqrt(pow(player->getRadius() + player->getPoints(), 2) + pow(it->second.getRadius() + it->second.getPoints(), 2)));
			}
		}
	}
	//if somebody ate someone
	if (eatSomeOne == true)
	{
		deletePlayerFromRanking(eatenID);
		playerDied(eatenID);
		return true;
	}
	return false;
}

void Server::setFood(unsigned int id)
{
}

void Server::updateFood(unsigned int id)
{
	Player* player = &players.at(id);
	//ha lathatatlan akkor ne is nezze a kaja reszet
	if (player->isInvisible()) return;

	float foodRadius = foodGenerator.getFoodRadius();
	for (int i = 0; i < food.size(); i++) {
		sf::Vector2f distance(player->getPosition().x - food[i].x, player->getPosition().y - food[i].y);
		float lenght = sqrt(distance.x*distance.x + distance.y*distance.y);

		//TODO skillekhez hogy kajat ne egyen ha lathatatlan
		//check if player ate a food
		if (lenght < player->getRadius()) {
			food[i] = foodGenerator.updateElement(i);
			player->setRadius(player->getRadius() + player->getPoints() + 0.5f);
			foodToUpdate.emplace(i, food[i]);	
			checkRanking();
		}
	}
}

struct less
{
	bool operator()(const Player& lhs, const Player& rhs) const {
		return (lhs.getRadius() + lhs.getPoints()) < (rhs.getRadius() + rhs.getPoints());
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
			rankingChanged = true;
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
