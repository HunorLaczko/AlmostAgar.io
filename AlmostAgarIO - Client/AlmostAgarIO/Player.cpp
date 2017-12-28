#include "stdafx.h"
#include "Player.h"
#include "iostream"


Player::Player()
{
	changed = false;
	name = "Anonymous";
}


Player::~Player()
{
}

float Player::default_radius; //ez a qva sor hogy rohandjon meg kellett neki mert idiota :D

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

void Player::setRadius(float _radius)
{
	if(_radius != radius) 	changed = true;

	radius = _radius;
}

float Player::getRadius()
{
	return radius;
}

sf::String Player::getName()
{
	return name;
}

void Player::setName(sf::String _name)
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
}

void Player::resetEnemies()
{
	enemies.clear();
}

sf::String Player::getEnemyName(unsigned int id)
{
	return enemies.find(id)->second.getName();
}

int Player::getEnemyRadius(unsigned int id)
{
	return enemies.find(id)->second.getRadius();
}

sf::Color Player::getEnemyColor(unsigned int id)
{
	return enemies.find(id)->second.getColor();
}

void Player::draw(sf::RenderWindow & window)
{
	sf::CircleShape circle;
	circle.setFillColor(sf::Color(0, 250, 0));
	circle.setOutlineThickness(-5);
	circle.setOutlineColor(sf::Color(0, 150, 0));
	circle.setRadius(radius);
	circle.scale(1, 1);
	circle.setOrigin(radius, radius);
	circle.setPosition(position);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) std::cout << "Couldn't load font file!" << std::endl;

	sf::Text text;
	text.setFont(font);
	text.setString(name);
	text.setColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	if(name.getSize()>2)
		text.setCharacterSize(2*radius/name.getSize());
	else
		text.setCharacterSize(2 * radius / 3);

	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(circle.getPosition());

	window.draw(circle);
	window.draw(text);

}
