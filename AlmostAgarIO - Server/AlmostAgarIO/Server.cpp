#include "stdafx.h"
#include "Server.h"
#include <string>
#include <iostream>

Server::Server()
{
	listener.listen(port);
	selector.add(listener);
	running = true;
	udpSocket.bind(udpPortReceive);
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

					if (players.back().getTcpSocket()->send(packet) != sf::Socket::Done) //Send client id
						std::cout << "Error sending id" << std::endl;
				}

			}
			else
			{
				//check udp socket
				//player locations should arrive here
				if (selector.isReady(udpSocket))
				{
					sf::Packet packet;
					sf::IpAddress sender;
					udpSocket.receive(packet, sender, udpPortReceive);

					int type;
					packet >> type;

					std::cout << type << std::endl;
				}

				// The listener socket is not ready, test all other sockets (the clients)
				for (size_t i = 0; i < players.size(); i++)
				{
					//sf::TcpSocket& client = it->getSocket;
					if (selector.isReady(*players[i].getTcpSocket()))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						if (players[i].getTcpSocket()->receive(packet) == sf::Socket::Done)
						{
							
						}
					}

					
					
				}
			}
		}
	}
}
