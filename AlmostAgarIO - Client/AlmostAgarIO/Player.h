#pragma once
#include "SFML\System.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

class Player
{
	unsigned int id;
	sf::Vector2f position;
	sf::Vector2u windowSize;
	float radius;
	sf::String name;
	bool changed;
	std::unordered_map<int, Player> enemies;
	sf::Color color;
	static float default_radius;

public:
	Player();
	~Player();

	static float getDefRad();
	static void setDefRad(float radius);

	unsigned int getId();
	void setId(unsigned int _id);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f _position);

	void setWindowSize(sf::Vector2u _windowSize);
	sf::Vector2u getWindowSize();

	void setRadius(float _radius);
	float getRadius();

	sf::String getName();
	void setName(sf::String _name);

	bool isRadiusChanged();
	void setChange(bool _change);

	std::unordered_map<int, Player> &getEnemies();
	void setEnemies(std::unordered_map<int, Player> _enemies);
	void updateEnemy(int id, sf::Vector2f position, float radius);

	void reset();
	void resetEnemies();

	void draw(sf::RenderWindow & window);

	sf::Color getColor();
	void setColor(sf::Color _color);
	/*std::vector<sf::Vector2f> getFood();
	void setFood(const std::vector<sf::Vector2f> _food);*/
};

