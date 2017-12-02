#pragma once
#include "SFML\System.hpp"
#include <vector>
class Player
{
	unsigned int id;
	sf::Vector2f position;
	sf::Vector2u windowSize;
	float radius;
	bool changed;
	//std::vector<sf::Vector2f> food;
public:
	Player();
	~Player();

	unsigned int getId();
	void setId(unsigned int _id);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f _position);

	void setWindowSize(sf::Vector2u _windowSize);
	sf::Vector2u getWindowSize();

	void setRadius(float _radius);
	float getRadius();

	bool isRadiusChanged();
	void setChange(bool _change);
	/*std::vector<sf::Vector2f> getFood();
	void setFood(const std::vector<sf::Vector2f> _food);*/
};

