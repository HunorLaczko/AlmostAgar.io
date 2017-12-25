#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	changed = false;
}


Player::~Player()
{
}

unsigned int Player::getId() const
{
	return id;
}

void Player::setId(unsigned int _id)
{
	id = _id;
}

sf::Vector2f Player::getPosition()
{
	return position;
}

void Player::setPosition(sf::Vector2f _position)
{
	position = _position;
}

void Player::setWindowSize(sf::Vector2u _windowSize)
{
	windowSize = _windowSize;
}

sf::Vector2u Player::getWindowSize()
{
	return windowSize;
}

void Player::setRadius(float _radius)
{
	if(_radius != radius) 	changed = true;

	radius = _radius;
}

float Player::getRadius()
{
	return radius;
}

bool Player::isRadiusChanged() {
	return changed;
}

void Player::setChange(bool _change) {
	changed = _change;
}

std::map<int, Player> Player::getEnemies()
{
	return enemies;
}

void Player::setEnemies(std::map<int, Player> _enemies)
{
	enemies = _enemies;
}

void Player::updateEnemy(int id, sf::Vector2f position)
{
	enemies[id].setPosition(position);
}

/*
std::vector<sf::Vector2f> Player::getFood()
{
	std::vector<sf::Vector2f> tmp = food;
	food.clear();
	return tmp;
}

void Player::setFood(const std::vector<sf::Vector2f> _food)
{
	food = _food;
}
*/

bool operator==(const Player &a, const Player &b)
{
	if (a.getId() == b.getId())
		return true;
	return false;
}
