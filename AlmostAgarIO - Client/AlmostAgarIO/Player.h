#pragma once
#include "SFML\System.hpp"
#include <vector>
#include <map>

class Player
{
	unsigned int id;
	sf::Vector2f position;
	sf::Vector2u windowSize;
	float radius;
	bool changed;
	std::map<int, Player> enemies;
	//std::vector<sf::Vector2f> food;
public:
	Player();
	~Player();

	unsigned int getId() const;
	void setId(unsigned int _id);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f _position);

	void setWindowSize(sf::Vector2u _windowSize);
	sf::Vector2u getWindowSize();

	void setRadius(float _radius);
	float getRadius();

	bool isRadiusChanged();
	void setChange(bool _change);

	std::map<int, Player> getEnemies();
	void setEnemies(std::map<int, Player> _enemies);
	void updateEnemy(int id, sf::Vector2f position);

	/*std::vector<sf::Vector2f> getFood();
	void setFood(const std::vector<sf::Vector2f> _food);*/
};

