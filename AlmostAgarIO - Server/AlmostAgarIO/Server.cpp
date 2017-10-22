#include "stdafx.h"
#include "Server.h"
#include <string>
#include <iostream>

Server::Server()
{
	id = 0;
	listener.listen(port);
	selector.add(listener);
	running = true;
	udpSocket.bind(udpPortReceive);
	udpSocket.setBlocking(false);
	selector.add(udpSocket);
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
					//players.emplace_back(id++, sf::Vector2f(300, 300), &client);
					players.push_back((Player(id++, sf::Vector2f(300, 300), client)));
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*players.back().getTcpSocket());

					sf::Packet packet;
					packet << 0;
					packet << players.back().getId();

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
					udpSocket.receive(packet, sender, udpPortReceive);

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
						outPacket << 2 << id << pos.x + 1 << pos.y +1;
						std::cout<<"responding: "<<udpSocket.send(outPacket, sender, udpPortSend)<<std::endl;
						break;
					}

					std::cout << type << std::endl;
				}
			
			else
			{



				


				// The listener socket is not ready, test all other sockets (the clients)
				for (size_t i = 0; i < players.size(); i++)
				{
					//sf::TcpSocket& client = it->getSocket;
					if (selector.isReady(*players[i].getTcpSocket()))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						sf::Socket::Status status = players[i].getTcpSocket()->receive(packet);
						if ( status == sf::Socket::Done)
						{
							std::cout << "received something\n";
						}
						else
						{
							
							std::cout << status;
							if (sf::Socket::Disconnected == status)
							{
								players[i].getTcpSocket()->disconnect();
								players.erase(players.begin() + i);
							}
							
						}
					}

					
					
				}
			}
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
