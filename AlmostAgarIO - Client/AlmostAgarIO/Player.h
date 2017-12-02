#pragma once
#include "SFML\System.hpp"
class Player
{
	unsigned int id;
	sf::Vector2f position;
	sf::Vector2u windowSize;
public:
	Player();
	~Player();

	unsigned int getId();
	void setId(unsigned int _id);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f _position);

	void setWindowSize(sf::Vector2u _windowSize);
	sf::Vector2u getWindowSize();
};

