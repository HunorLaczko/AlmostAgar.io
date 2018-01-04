#include "stdafx.h"
#include "Player.h"
#include "iostream"
#include "sstream"

Player::Player()
{
	changed = false;
	name = "Anonymous";
	canUpgradeNumber = 0;
	invisibleAvailable = false;
	speedAvailable = false;
	invisible = false;
}


Player::~Player()
{
}

float Player::default_radius;

float Player::getDefRad() {
	return default_radius;
}

void Player::setDefRad(float radius) {
	default_radius = radius;
}

unsigned int Player::getId()
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

float Player::getPoints() {
	return points;
}

void Player::setRadius(float _radius)
{
	if(_radius != (radius + points)) 	changed = true;
	if (_radius > 330.0f) {
		radius = 330.0f;
		points = _radius - radius;
	}
	else {
		points = 0.0f;
		radius = _radius;
	}
}

float Player::getRadius()
{
	return radius;
}

std::string Player::getName()
{
	return name;
}

void Player::setName(std::string _name)
{
	name = _name;
}

sf::Color Player::getColor() {
	return color;
}

void Player::setColor(sf::Color _color) {
	color = _color;
}

bool Player::isRadiusChanged() {
	return changed;
}

void Player::setChange(bool _change) {
	changed = _change;
}

std::unordered_map<int, Player> &Player::getEnemies()
{
	return enemies;
}

void Player::setEnemies(std::unordered_map<int, Player> _enemies)
{
	enemies = _enemies;
}

void Player::updateEnemy(int id, sf::Vector2f position, float radius)
{
	enemies[id].setPosition(position);
	enemies[id].setRadius(radius);
}

void Player::reset()
{
	id = -1;
	position = sf::Vector2f(0, 0);
	enemies.clear();
	invisibleAvailable = false;
	speedAvailable = false;
	invisible = false;
}

void Player::resetEnemies()
{
	enemies.clear();
}

void Player::deleteEnemy(unsigned int id)
{
	enemies.erase(id);
}

std::string Player::getEnemyName(unsigned int id)
{
	return enemies[id].getName();
}

void Player::setEnemyName(unsigned int id, std::string _name)
{
	enemies[id].setName(_name);
}

Player Player::getEnemyById(unsigned int id)
{
	return enemies[id];
}

int Player::getEnemyRadius(unsigned int id)
{
	return enemies[id].getRadius();
}

sf::Color Player::getEnemyColor(unsigned int id)
{
	return enemies[id].getColor();
}

void Player::setEnemyColor(unsigned int id, sf::Color _color)
{
	enemies[id].setColor(_color);
}

int Player::getUpgradeAvailable() {
	return canUpgradeNumber;
}

void Player::upgradedSkill()
{
	canUpgradeNumber--;
}

void Player::canUpgrade(int numberOfUpgrades)
{
	canUpgradeNumber = numberOfUpgrades;
}

bool Player::getInvisibleAvailable()
{
	return invisibleAvailable;
}

void Player::setInvisibleAvailable(bool _invisibleAvailable)
{
	invisibleAvailable = _invisibleAvailable;
}

bool Player::getSpeedAvailable()
{
	return speedAvailable;
}

void Player::setSpeedAvailable(bool _speedAvailable)
{
	speedAvailable = _speedAvailable;
}

bool Player::getInvisible()
{
	return invisible;
}

void Player::setInvisible(bool _invisible)
{
	invisible = _invisible;
}

bool Player::getEnemyInvisible(unsigned int id)
{
	return enemies[id].getInvisible();
}

void Player::setEnemyInvisible(unsigned int id, bool _invisible)
{
	enemies[id].setInvisible(_invisible);
}

void Player::draw(sf::RenderWindow & window)
{
	sf::CircleShape circle;
	circle.setOutlineThickness(-5);
	if (invisible) {
		circle.setFillColor(sf::Color(color.r, color.g, color.b, 50));
		circle.setOutlineColor(sf::Color(0, 0, 0, 50));
	}
	else {
		circle.setFillColor(color);
		circle.setOutlineColor(sf::Color(0, 0, 0));
	}
	circle.setRadius(radius);
	circle.scale(1, 1);
	circle.setOrigin(radius, radius);
	circle.setPosition(position);
	circle.setPointCount(100);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) std::cout << "Couldn't load font file!" << std::endl;

	sf::Text text;
	text.setFont(font);
	///Debug:koordináták is
	std::ostringstream ss;
	ss << name << "\n" << position.x <<", " << position.y;
	text.setString(ss.str());
	//text.setString(name);
	text.setColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	if(name.size()>2)
		text.setCharacterSize(2*radius/name.size());
	else
		text.setCharacterSize(2 * radius / 3);

	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(circle.getPosition());

	window.draw(circle);
	window.draw(text);

}
