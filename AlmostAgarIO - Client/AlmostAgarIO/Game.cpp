#include "stdafx.h"

#include "Game.h"
#include "iostream"
#include <unordered_map>

float ZOOM = 1.01f; //1%-os zoom
float radius = 30;
const float radius2 = 7;
sf::Vector2f oldPos(0, 0);

void Game::func() {
	std::cout << "Betoltes elindult...\n";
	if (!texture.loadFromFile("palya.jpg") || !texture2.loadFromFile("background.png"))
	{
		close();
	}
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	view.zoom(ZOOM);

	//window.setMouseCursorVisible(false);
	player.setRadius(radius);
	player.setPosition(sf::Vector2f((float)texture2.getSize().x / 2, (float)texture2.getSize().y / 2));
	///view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
	map.setTexture(texture);
	background.setTexture(texture2);
	map.scale(1, 1);
	background.scale(1, 1);
	map.setPosition(1000, 750);

	finished = true;
	std::cout << "Betolto szal leall!" << std::endl;


}

Game::Game() : thread(&Game::func, this)
{
	finished = false;
	thread.launch();
	sf::Vector2f vec(0, 0);
	sf::Vector2f movement(0, 0);
	sf::Clock clock;
	bool first = true;
	network = new Network(this);
	zoom_count = 0;
	gameover = false;
}

Game::~Game()
{

}

void Game::threadWait() {
	std::cout << "A szal varakozik\n";
	thread.wait();
	std::cout << "A tolto szal befejezte a varakozast\n";
}

void Game::setFirst() {
	first = true;
	zoom_count = 0;
}

void Game::setFood(const std::vector<sf::Vector2f>& _food, float foodRadius)
{
	for (int i = 0; i < 1000; i++) {
		sf::CircleShape circ(foodRadius);
		circ.setPosition(_food[i]);
		circ.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		circ.setOrigin(foodRadius / 2, foodRadius / 2);
		food.push_back(circ);
	}
}

void Game::updateFood(int index, sf::Vector2f newFood)
{
	food[index].setPosition(newFood);
}

void Game::resize(sf::Event::SizeEvent event_size, sf::Vector2u window_size) {
	setView(sf::View(sf::FloatRect(0.f, 0.f, (float) event_size.width, (float) event_size.height)));
	view.reset((sf::FloatRect(player.getPosition().x - (float) window_size.x / 2, player.getPosition().y - (float) window_size.y / 2, (float) window_size.x, (float) window_size.y)));
	
	std::cout << "zoom: " << zoom_count << "x\n";
	for (int i = 0; i < zoom_count; i++) {
		view.zoom(ZOOM);
	}
}

sf::Vector2f Game::getCirclePos()
{
	return player.getPosition();
}

void Game::setLeaderboard(std::vector<unsigned int> _leaderboard)
{
	leaderboard = _leaderboard;
}

bool Game::isOver()
{
	return gameover;
}

void Game::gameOver()
{
	gameover = true;
}

void Game::init(sf::IpAddress _serverIp, sf::Vector2u window_size, sf::String _playerName)
{
	network->setIp(_serverIp);
	player.setWindowSize(window_size);
	if(_playerName !="")
		player.setName(_playerName.toAnsiString());
}


void Game::connect()
{	
	network->connectPlayer(&player);
	//player.resetEnemies();
	network->init(sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height), (sf::Vector2f)map.getPosition(), (sf::Vector2f)player.getWindowSize());
	std::cout << "player pos in init: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;
	gameover = false;
}

void Game::disconnect()
{
	network->disconnectPlayer();
	food.clear();
	player.reset();
	//delete network;
}

void Game::counting(sf::RenderWindow & window)
{
	if (first) {
		view.setCenter(player.getPosition().x, player.getPosition().y);
		view.reset((sf::FloatRect(player.getPosition().x - (float)window.getSize().x / 2, player.getPosition().y - (float)window.getSize().y / 2, (float)window.getSize().x, (float)window.getSize().y)));
		//view.setCenter(3000, 2000);
		//view.reset((sf::FloatRect(3000- (float)window.getSize().x / 2, 2000 - (float)window.getSize().y / 2, (float)window.getSize().x, (float)window.getSize().y)));
		first = false;
	}
	
	/*
	//Számolás
	sf::Vector2f distance(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x - player.getPosition().x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y - player.getPosition().y);
	float speed = 2.2f - (0.005f*player.getRadius());
	if (speed <= 0.6f) speed = 0.6f;

	float length = sqrt(distance.x*distance.x + distance.y*distance.y);
	vec.x = speed * distance.x / length;
	vec.y = speed * distance.y / length;
	
	if (abs(distance.x) < 2 || ((player.getPosition().x - vec.x) <= map.getPosition().x && vec.x <= 0) || ((player.getPosition().x + vec.x) >= (map.getPosition().x + map.getLocalBounds().width) && vec.x >= 0)) {
		vec.x = 0;
	}
	if (abs(distance.y) < 2 || ((player.getPosition().y - vec.y) <= map.getPosition().y && vec.y <= 0) || ((player.getPosition().y - vec.y) >= (map.getPosition().y + map.getLocalBounds().height) && vec.y >= 0)) {
		vec.y = 0;
	}


	*/
	//Pozició küldés/fogadás
	//circle.move(vec);
	//player.setPosition(circle.getPosition());
	if (clock.getElapsedTime() > sf::milliseconds(33)) {
		network->sendPosition(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));
		oldPos = player.getPosition();
		
		sf::Vector2f vecFromServer = player.getPosition() - oldPos;
		clock.restart();

	}
	network->getResponse();
	//meret valtoztatas
	if (player.isRadiusChanged() && (int)(player.getRadius() - Player::getDefRad()) % 5 == 0 && (player.getRadius() - Player::getDefRad()) == (int)(player.getRadius() - Player::getDefRad())) {
		view.zoom(ZOOM);
		player.setChange(false);
		std::cout << "ZOOOOOOOOMMMMMMM\n";
		zoom_count++;
	}
	
	/*
	//kamera mozgas szamolasa
	sf::Vector2f distFromCenter(player.getPosition().x - view.getCenter().x, player.getPosition().y - view.getCenter().y);
	float lenght2 = sqrt(distFromCenter.x * distFromCenter.x + distFromCenter.y*distFromCenter.y);
	movement.x = speed * distFromCenter.x / lenght2;
	movement.y = speed * distFromCenter.y / lenght2;

	if (abs(distFromCenter.x) < 20 || (vec.x == 0 && vec.y == 0)) {
		movement.x = 0;
	}
	if (abs(distFromCenter.y) < 20 || (vec.x == 0 && vec.y == 0)) {
		movement.y = 0;
	}
	*/
}


void Game::draw(sf::RenderWindow & window)
{
	//view.move(movement);
	view.setCenter(player.getPosition());
	
	//TODO Bálint: ha szervertől megvan a leaderboard, akkor szerint átírni:

	//smaller enemy drawing
	std::vector<Player> smallerEnemy;
	
	window.clear(sf::Color::Black);

	if (finished) {
		window.setView(view);
		window.draw(background);
		window.draw(map);
		for (int i = 0; i < food.size(); i++) {
			if ((window.mapPixelToCoords(sf::Vector2i(0, 0)).x + view.getSize().x) > food[i].getPosition().x && window.mapPixelToCoords(sf::Vector2i(0, 0)).x < food[i].getPosition().x &&
				(window.mapPixelToCoords(sf::Vector2i(0, 0)).y + view.getSize().y) > food[i].getPosition().y && window.mapPixelToCoords(sf::Vector2i(0, 0)).y < food[i].getPosition().y) {
				window.draw(food[i]);
			}
		}
		//nem tudom miért nem tudok iterálni ...
		//talan igy?
		std::unordered_map<int, Player> enemies = player.getEnemies();
		for (std::unordered_map<int, Player>::iterator it = enemies.begin(); it != enemies.end(); it++)
		{
			//if (it->second.getRadius() > player.getRadius() || (it->second.getRadius() == player.getRadius() && (rand() % 2) == 0))
			if (it->second.getRadius() > player.getRadius())
				smallerEnemy.push_back(it->second);
			else
				it->second.draw(window);
		}

		player.draw(window);

		for (int i = 0; i < smallerEnemy.size(); i++) {
			smallerEnemy[i].draw(window);
		}
	}
	window.display();
}
