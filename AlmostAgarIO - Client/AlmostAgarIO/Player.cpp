#include "stdafx.h"
#include "Player.h"
#include "iostream"
#include <sstream>

Player::Player()
{
	changed = false;
	name = "Anonymous";
	canUpgradeNumber = 0;
	invisibleAvailable = false;
	speedAvailable = false;
	invisible = false;
	initReady = false;
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

Player Player::getEnemyById(unsigned int e_id)
{
	return enemies.find(e_id)->second;
}

bool Player::hasThisEnemy(unsigned int e_id)
{
	if (enemies.find(e_id) != enemies.end() && e_id!=id)
		return true;
	else
		return false;
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

bool Player::getInitReady() const
{
	return initReady;
}

void Player::setInitReady(bool _initReady)
{
	initReady = _initReady;
}

void Player::setEnemyInitReady(bool _initReady) {
	enemies[id].initReady = _initReady;
}

void Player::draw(sf::RenderWindow & window,bool self_draw)
{
	if (!initReady && self_draw) return;
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
	//std::ostringstream ss;
	//ss << name << "\n" << position.x <<", " << position.y;
	//text.setString(ss.str());
	text.setString(name);
	text.setColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	if (name.size() > 2)
		text.setCharacterSize(2 * radius / name.size());
	else
		text.setCharacterSize(2 * radius / 3);
	text.setPosition(circle.getPosition().x - text.getLocalBounds().width / 2, circle.getPosition().y - text.getCharacterSize() / 2);

	if(self_draw){
		sf::Text score;
		score.setFont(font);
		std::ostringstream ss_score;
		ss_score << round(2 * (radius + points - default_radius));
		score.setCharacterSize(45);
		score.setString(ss_score.str());
		score.setColor(sf::Color::White);
		score.setStyle(sf::Text::Bold);
		score.setOutlineColor(sf::Color::Black);
		score.setOutlineThickness(2);
		score.setPosition(window.getView().getCenter().x - score.getLocalBounds().width / 2, window.getView().getCenter().y - window.getView().getSize().y / 2 + 10);

		window.draw(score);
	}
	window.draw(circle);
	window.draw(text);

}

