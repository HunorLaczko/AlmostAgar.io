#pragma once
#include "SFML\System.hpp"
class Player
{
	unsigned int id;
	sf::Vector2f position;
public:
	Player();
	~Player();

	unsigned int getId();
	void setId(unsigned int _id);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f _position);
};

