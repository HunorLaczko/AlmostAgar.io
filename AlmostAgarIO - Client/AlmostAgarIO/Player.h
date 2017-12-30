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
	std::string name;
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

	std::string getName();
	void setName(std::string _name);

	bool isRadiusChanged();
	void setChange(bool _change);

	std::unordered_map<int, Player> &getEnemies();
	void setEnemies(std::unordered_map<int, Player> _enemies);
	void updateEnemy(int id, sf::Vector2f position, float radius);

	void reset();
	void resetEnemies();
	void deleteEnemy(unsigned int id);

	std::string getEnemyName(unsigned int id);
	void setEnemyName(unsigned int id, std::string _name);

	Player getEnemyById(unsigned int id);

	int getEnemyRadius(unsigned int id);

	sf::Color getEnemyColor(unsigned int id);
	void setEnemyColor(unsigned int id, sf::Color _color);

	void draw(sf::RenderWindow & window);

	sf::Color getColor();
	void setColor(sf::Color _color);
	/*std::vector<sf::Vector2f> getFood();
	void setFood(const std::vector<sf::Vector2f> _food);*/
};

