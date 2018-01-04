#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <SFML/Network.hpp>


Player::Player(int _id, sf::Vector2f _position, sf::TcpSocket *_tcpSocket)
	: id(_id), position(_position), tcpSocket(_tcpSocket), radius(80),
	name("unknown"), udpSocket(nullptr), playerIp(sf::IpAddress()),
	velocity(sf::Vector2f(0, 0)), points(0),
	mapSize(sf::Vector2f(0, 0)), mapPosition(sf::Vector2f(0, 0)),
	windowSize(sf::Vector2f(0, 0)), mousePosition(sf::Vector2f(0, 0)), initReady(false),
	invisibleActive(false), speedActive(false), invisibleTime(30000), speedTime(40000),
	invisibleAvailable(false), speedAvailable(false), numberOfUpdate(0), canUpdateNumber(0),
	invisibleDuration(5000), speedDuration(4000), speedBoost(1.0f)
{
	defRadius = radius;
	invClockReload.restart();
	speedClockReload.restart();
	invClockUse.restart();
	speedClockUse.restart();
	invisibilityChanged = true;
	invisibleAvailableChanged = true;
	speedAvailableChanged = true;
	canUpdateNumberChanged = true;
	speed = (float)(3.2 - (0.005 * (radius + points)));
	if (speed <= 0.6f) speed = 0.6f;
}

Player::~Player()
{
}

sf::TcpSocket * Player::getTcpSocket() const
{
	return tcpSocket;
}

sf::UdpSocket * Player::getUdpSocket() const
{
	if (udpSocket == nullptr) std::cout << "udpSocket hasn't been initialized\n";
	return udpSocket;
}

unsigned int Player::getId() const
{
	return id;
}

std::string Player::getName() const
{
	return name;
}

void Player::setName(std::string _name)
{
	name = _name;
}

sf::Vector2f Player::getPosition() const
{
	return position;
}

void Player::setPosition(sf::Vector2f _position)
{
	position = _position;
}

sf::Vector2f Player::getMapSize() const
{
	return mapSize;
}

void Player::setMapSize(sf::Vector2f _mapSize)
{
	mapSize = _mapSize;
}

sf::Vector2f Player::getMapPosition() const
{
	return mapPosition;
}

void Player::setMapPosition(sf::Vector2f _mapPosition)
{
	mapPosition = _mapPosition;
}

sf::Vector2f Player::getWindowSize() const
{
	return windowSize;
}

void Player::setWindowSize(sf::Vector2f _windowSize)
{
	windowSize = _windowSize;
}

float Player::getPoints() const {
	return points;
}

//itt az utkozes miatt nem adhatja vissza a raduis+points reszt
float Player::getRadius() const
{
	return radius;
}

//itt mindig csak a 330 feletti elterest adja a pontokhoz
void Player::setRadius(float _radius)
{
	if (_radius > 330.0f ) {
		radius = 330.0f;
		points = _radius - radius;
		//std::cout << "Points above 330(virtual radius): " << points << std::endl;
	}
	else {
		points = 0;
		radius = _radius;
	}
	int oldCanUpdateNumber = canUpdateNumber;
	int point = 2 * (radius - defRadius + points);
	canUpdateNumber = floor(point / 150) - numberOfUpdate;
	if(oldCanUpdateNumber != canUpdateNumber)  canUpdateNumberChanged = true;
	speed = (float)(3.2 - (0.005 * (radius + points)));
	if (speed <= 0.6f) speed = 0.6f;
}

sf::IpAddress Player::getPlayerIp() const
{
	return playerIp;
}

void Player::setPlayerIp(sf::IpAddress _playerIp)
{
	playerIp = _playerIp;
}

sf::UdpSocket * Player::bindUdpSocket(unsigned short _udpPortReceive, unsigned short _udpPortSend)
{
	udpSocket = new sf::UdpSocket();
	udpSocket->bind(_udpPortReceive);
	udpPortReceive = _udpPortReceive;
	udpPortSend = _udpPortSend;
	return udpSocket;
}

unsigned short Player::getUdpPortSend() const
{
	return udpPortSend;
}

unsigned short Player::getUdpPortReceive() const
{
	return udpPortReceive;
}

sf::Vector2f Player::getMousePosition() const
{
	return mousePosition;
}

void Player::setMousePosition(sf::Vector2f _mousePosition)
{
	mousePosition = _mousePosition;
}
void Player::initPosition()
{
	//srand(time(0));
	position.x = rand() % (int)mapSize.x + mapPosition.x;
	position.y = rand() % (int)mapSize.y + mapPosition.y;
	//position = sf::Vector2f(2000, 2000);
}

sf::Color Player::getColor() const {
	return color;
}

void Player::setColor(sf::Color _color) {
	color = _color;
}

bool Player::getInitReady() const
{
	return initReady;
}

void Player::setInitReady(bool _initReady)
{
	initReady = _initReady;
}


bool operator==(const Player &a, const Player &b)
{
	if (a.getId() == b.getId())
		return true;
	return false;
}

bool operator<(const Player &a, const Player &b)
{
	if (a.getRadius() < b.getRadius())
		return true;
	return false;
}

void Player::skillChecking() {
	//skillek ujratoltesenek ellenorzese
	if (invClockReload.getElapsedTime() > sf::milliseconds(invisibleTime) && invisibleAvailable == false) {
		invisibleAvailable = true;
		invisibleAvailableChanged = true;
		std::cout << "Lathatatlansag elerheto " << id << ". jatekos szamara\n";
	}
	if (speedClockReload.getElapsedTime() > sf::milliseconds(speedTime) && speedAvailable == false) {
		speedAvailable = true;
		speedAvailableChanged = true;
		std::cout << "Speed elerheto " << id << ". jatekos szamara\n";
	}

	//skillek idejenek lejarata
	if (invClockUse.getElapsedTime() > sf::milliseconds(invisibleDuration) && invisibleActive) {
		invisibleActive = false;
		std::cout << "Lathatatlansag ideje lejart\n";
		invisibilityChanged = true;
	}

	if (speedClockUse.getElapsedTime() > sf::milliseconds(speedDuration) && speedActive) {
		speedActive = false;
		std::cout << "Speed ideje lejart\n";
	}
}

//aktivak e a skillek
bool Player::isInvisible() {
	return invisibleActive;
}

bool Player::isSpeeding() {
	return speedActive;
}

//ezeket kell meghivni hogy aktivaljuk a skilleket
void Player::invisibleActivate() {
	if (invisibleAvailable && invisibleActive == false) {
		invisibleActive = true;
		invClockUse.restart();
		std::cout << "Lathatatlansag aktivalva " << id << ". jatekos szamara\n";

		invisibleAvailable = false;
		invisibleAvailableChanged = true;
		invClockReload.restart();

		invisibilityChanged = true;
	}
}

void Player::speedActivate() {
	if (speedAvailable && speedActive == false) {
		speedActive = true;
		speedClockUse.restart();
		std::cout << "Speed aktivalva " << id << ". jatekos szamara\n";

		speedAvailable = false;
		speedAvailableChanged = true;
		speedClockReload.restart();
	}
}

//TODO ellenorizni hogy tenyleg fejleszthet-e
void Player::updateSkill(int key) {
	int oldCanUpdateNumber = canUpdateNumber;
	int point = 2 * (radius - defRadius + points);
	canUpdateNumber = floor(point / 150) - numberOfUpdate; 
	if (oldCanUpdateNumber != canUpdateNumber)  canUpdateNumberChanged = true;

	//debughoz
	std::cout << canUpdateNumber << "db fejlesztes elerheto " << id << ". jatekos szamara\n";
	
	//4-e, 16-q, 17-r, 22-w
	if ( canUpdateNumber > 0) {
		switch (key)
		{
		case 16:{
			//update invisibility reload time
			invisibleTime -= 500; //ennyivel kevesebb ideig tolt
			numberOfUpdate++;
			std::cout << "Lathatatlansag ujratoltest fejlesztett a(z) " << id << ". jatekos\n";
			break;
		}
		case 4: {
			//update speed reload time
			speedTime -= 500; //ennyivel kevesebb ideig tolt
			numberOfUpdate++;
			std::cout << "Sebesseg ujratoltest fejlesztett a(z) " << id << ". jatekos\n";
			break;
		}
		case 22: {
			//update invisibility duration time
			invisibleDuration += 500;
			numberOfUpdate++;
			std::cout << "Lathatatlansag idotartamot fejlesztett a(z) " << id << ". jatekos\n";
			break;
		}
		case 17: {
			//update speed boost
			speedBoost += 0.25f;
			numberOfUpdate++;
			std::cout << "Sebesseg merteket fejlesztett a(z) " << id << ". jatekos\n";
			break;
		}
		}
	}
}

int Player::getUpdateAvailable() {
	return canUpdateNumber;
}

bool Player::getInvisibilityChanged()
{
	return invisibilityChanged;
}

void Player::setInvisibilityChanged(bool _invisibilityChanged)
{
	invisibilityChanged = _invisibilityChanged;
}

bool Player::getInvisibleAvailableChanged()
{
	return invisibleAvailableChanged;
}

void Player::setInvisibleAvailableChanged(bool _invisibleAvailableChanged)
{
	invisibleAvailableChanged = _invisibleAvailableChanged;
}

bool Player::getSpeedAvailableChanged()
{
	return speedAvailableChanged;
}

void Player::setSpeedAvailableChanged(bool _speedAvailableChanged)
{
	speedAvailableChanged = _speedAvailableChanged;
}

bool Player::getInvisibleAvailable()
{
	return invisibleAvailable;
}

bool Player::getSpeedAvailable()
{
	return speedAvailable;
}

float Player::getSpeed() {
	if (speedActive) {
		return speed + speedBoost;
	}
	return speed;
}

void Player::invisibleDeActivate() {
	if (invisibleActive) {
		invisibleActive = false;
		std::cout << "Lathatatlansag deaktivalva " << id << ". jatekos szamara\n";

		invisibleAvailable = false;
		invisibleAvailableChanged = true;
		invClockReload.restart();

		invisibilityChanged = true;
	}
}

void Player::speedDeActivate() {
	if (speedActive) {
		speedActive = false;
		std::cout << "Speed deaktivalva " << id << ". jatekos szamara\n";

		speedAvailable = false;
		speedAvailableChanged = true;
		speedClockReload.restart();
	}
}

bool Player::getCanUpdateNumberChanged()
{
	return canUpdateNumberChanged;
}

void Player::setCanUpdateNumberChanged(bool _canUpdateNumberChanged)
{
	canUpdateNumberChanged = _canUpdateNumberChanged;
}
