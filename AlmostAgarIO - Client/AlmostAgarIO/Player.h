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
	float points;
	std::string name;
	bool changed;
	std::unordered_map<int, Player> enemies;
	sf::Color color;
	static float default_radius;
	int canUpgradeNumber;
	bool invisibleAvailable;
	bool speedAvailable;
	bool invisible;		
	bool initReady;

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

	float getPoints();

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
	bool hasThisEnemy(unsigned int id);

	int getEnemyRadius(unsigned int id);

	sf::Color getEnemyColor(unsigned int id);
	void setEnemyColor(unsigned int id, sf::Color _color);

	void draw(sf::RenderWindow & window, bool self_draw);
	void drawscore(sf::RenderWindow & window);

	sf::Color getColor();
	void setColor(sf::Color _color);

	int getUpgradeAvailable();	//get the number of upgrades available
	void upgradedSkill();	//call this when we sent the upgrade to server
	void canUpgrade(int numberOfUpgrades);	//call this when we received upgrade available from server
	

	bool getInvisible();
	void setInvisible(bool _invisible);
	bool getEnemyInvisible(unsigned int id);
	void setEnemyInvisible(unsigned int id, bool _invisible);


	bool getInvisibleAvailable();
	void setInvisibleAvailable(bool _invisibleAvailable);

	bool getSpeedAvailable();
	void setSpeedAvailable(bool _speedAvailable);

	bool getInitReady() const;
	void setInitReady(bool _initReady);
	void setEnemyInitReady(bool _initReady);
};

