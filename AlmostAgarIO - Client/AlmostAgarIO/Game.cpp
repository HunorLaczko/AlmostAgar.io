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
	//view.zoom(ZOOM);
	texture.setSmooth(true);
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
	//Speciális képességekhez a bilentyűk(számozás abc szerint): a-0, s-18, d-3
	specalkeys = { { 0,false },{ 18,false } ,{ 3,false }, {4, false},{ 16, false },{ 17, false },{ 22, false } };
	specalkeys_value = {
		new Letter(0,0,40,40,"S",sf::Color(255,0,0), 0),
		new Letter(0,0,40,40,"D",sf::Color(255,0,0), 1),
		new Letter(0,0,40,40,"Q",sf::Color(255,0,0), 2),
		new Letter(0,0,40,40,"W",sf::Color(255,0,0), 3),
		new Letter(0,0,40,40,"E",sf::Color(255,0,0), 4),
		new Letter(0,0,40,40,"R",sf::Color(255,0,0), 5)
	};
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
	for (int i = 0; i < _food.size(); i++) {
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
//TODO csak akkor hivd meg a sendkey-t ha a megfelelo skill elerheto
void Game::keyPressed(int key)
{
	if (key == 18 && !player.getInvisibleAvailable()) return;
	if (key == 3 && !player.getSpeedAvailable()) return;
	if ((key == 4 || key == 16 || key == 17 || key == 22) && player.getUpgradeAvailable() <= 0) return;
	std::map<int, bool>::iterator it;
	it = specalkeys.find(key);
	if (it != specalkeys.end()) {
		if (!it->second) {
			it->second = true;
			network->sendKey(key, true);
		}
	}
}

void Game::keyReleased(int key)
{
	
	
	std::map<int, bool>::iterator it;
	it = specalkeys.find(key);
	if (it != specalkeys.end()) {
		if (it->second == false && key == 18 && !player.getInvisibleAvailable()) return;
		if (it->second == false && key == 3 && !player.getSpeedAvailable()) return;
		if (key == 4 || key == 16 || key == 17 || key == 22) { it->second = false; return; }
		it->second = false;
		network->sendKey(key,false);
	}
}

bool Game::testServer()
{
	return network->test();
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
	int zoom_times = floor(player.getRadius() - Player::getDefRad()) / 5;

	//if (player.isRadiusChanged() && (int)(player.getRadius() - Player::getDefRad()) % 5 == 0 && (player.getRadius() - Player::getDefRad()) == (int)(player.getRadius() - Player::getDefRad())) {
	if (player.isRadiusChanged() && zoom_times != zoom_count) {
		int m = zoom_times - zoom_count;
		if (m > 0) {
			for (int i = 0; i < m; i++) {
				view.zoom(ZOOM);
				std::cout << "ZOOOOOOOOMMMMMMM\n";
				zoom_count++;

			}

		}
		else if (m < 0) {
			for (int i = 0; i < -m; i++) {
				view.zoom(1/ZOOM);
				std::cout << "UNZOOMING\n";
				zoom_count--;
			}
		}
		
		player.setChange(false);
		std::cout << player.getRadius() - Player::getDefRad() << std::endl;
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
	//std::cout << leaderboard.size() << std::endl;
	//view.move(movement);
	view.setCenter(player.getPosition());
	
	window.clear(sf::Color::Black);

	if (finished) {
		//Háttér
		window.setView(view);
		window.draw(background);
		window.draw(map);

		//Kaja
		for (int i = 0; i < food.size(); i++) {
			if ((window.mapPixelToCoords(sf::Vector2i(0, 0)).x + view.getSize().x) > food[i].getPosition().x && window.mapPixelToCoords(sf::Vector2i(0, 0)).x < food[i].getPosition().x &&
				(window.mapPixelToCoords(sf::Vector2i(0, 0)).y + view.getSize().y) > food[i].getPosition().y && window.mapPixelToCoords(sf::Vector2i(0, 0)).y < food[i].getPosition().y) {
				window.draw(food[i]);
			}
		}

		//Játékosok
		bool larger = true;
		for (int i = leaderboard.size()-1; i >= 0; i--) {
			if (larger && player.getEnemyRadius(leaderboard[i]) > player.getRadius()) {
				larger = false;
				player.draw(window, true);
			}
			if (player.hasThisEnemy(leaderboard[i])) {
				player.getEnemyById(leaderboard[i]).draw(window,false);
			}
		}
		if (larger) {
			player.draw(window,true);
		}

		//ronda megoldas tudom :(
		bool a = player.getUpgradeAvailable() > 0; //TODO Huni nezd meg hogy ha mukodik a fuggveny ez jo lesz-e
		bool tmp[] = {player.getInvisibleAvailable(), player.getSpeedAvailable(), a, a, a, a};

		for (unsigned i = 0; i < specalkeys_value.size(); i++) {
			if (tmp[i] == true) {
				specalkeys_value[i]->changeStatus(Letter::Status::Active);
			}
			else if (tmp[i] == false) {
				specalkeys_value[i]->changeStatus(Letter::Status::Loading);
			}
			specalkeys_value[i]->draw(window);
		}
	}
	window.display();
}
