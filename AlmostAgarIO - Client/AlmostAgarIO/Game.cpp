#include "stdafx.h"

#include "Game.h"
#include "iostream"
#include <unordered_map>
#include <sstream>

void Game::func() {
	std::cout << "Betoltes elindult...\n";
	if (!texture.loadFromFile("palya.jpg") || !texture2.loadFromFile("background.png"))
	{
		close();
	}
	view.setViewport(sf::FloatRect(0, 0, 1, 1));

	texture.setSmooth(true);
	player.setPosition(sf::Vector2f((float)texture2.getSize().x / 2, (float)texture2.getSize().y / 2));
	map.setTexture(texture);
	background.setTexture(texture2);
	map.scale(1, 1);
	background.scale(1, 1);
	map.setPosition(1000, 750);
	

	finished = true;
	_loaded();
	std::cout << "Betolto szal leall!" << std::endl;
}

Game::Game() : thread(&Game::func, this)
{
	finished = false;
	thread.launch();
	sf::Vector2f vec(0, 0);
	sf::Vector2f movement(0, 0);
	sf::Vector2f oldPos(0, 0);
	ZOOM = 1.01f; //1%-os zoom
	sf::Clock clock;
	first = true;
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
	//Ranglistához:
	if (!_font.loadFromFile("arial.ttf")) std::cout << "Couldn't load font file!" << std::endl;
	_loaded = [&]() {};
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

bool Game::isLoeaded()
{
	return finished;
}

void Game::setLoeadedFunc(std::function<void()> loaded)
{
	_loaded = loaded;
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
	network->init(sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height), (sf::Vector2f)map.getPosition(), (sf::Vector2f)player.getWindowSize());
	std::cout << "player pos in init: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;
	gameover = false;
}

void Game::disconnect()
{
	network->disconnectPlayer();
	food.clear();
	player.reset();
}

void Game::counting(sf::RenderWindow & window)
{
	if (first) {
		view.reset((sf::FloatRect(player.getPosition().x - (float)window.getSize().x / 2, player.getPosition().y - (float)window.getSize().y / 2, (float)window.getSize().x, (float)window.getSize().y)));
		view.setCenter(player.getPosition().x, player.getPosition().y);
		first = false;
	}
	
	//Pozició küldés/fogadás
	if (clock.getElapsedTime() > sf::milliseconds(33)) {
		network->sendPosition(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));
		oldPos = player.getPosition();
		
		sf::Vector2f vecFromServer = player.getPosition() - oldPos;
		clock.restart();

	}
	network->getResponse();
	//meret valtoztatas
	int zoom_times = floor(player.getRadius() - Player::getDefRad()) / 5;

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
}


void Game::draw(sf::RenderWindow & window)
{

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
				player.draw(window);
			}
			if (player.hasThisEnemy(leaderboard[i])) {
				player.getEnemyById(leaderboard[i]).draw(window);
			}
		}
		if (larger) {
			player.draw(window);
		}
		player.drawscore(window);

		//Ranglista
		float top_margin = 10;

		sf::RectangleShape leaderboard_bg;
		leaderboard_bg.setFillColor(sf::Color(0,0,0,50));
		//-5 a padding, 185 a merete, -10 a right-margin
		leaderboard_bg.setPosition(-5 + window.getView().getCenter().x + window.getView().getSize().x / 2 - 185 - 10, -5 + window.getView().getCenter().y - window.getView().getSize().y / 2 + top_margin);

		sf::Text leaderboard_title;
		leaderboard_title.setFont(_font);
		leaderboard_title.setCharacterSize(16);
		leaderboard_title.setString("Ranglista:");
		leaderboard_title.setStyle(sf::Text::Bold);
		leaderboard_title.setOutlineColor(sf::Color(25, 25, 25));
		leaderboard_title.setOutlineThickness(1);
		leaderboard_title.setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 185 - 10, window.getView().getCenter().y - window.getView().getSize().y / 2 + top_margin);
		window.draw(leaderboard_title);
		top_margin += leaderboard_title.getLocalBounds().height + 5;

		bool player_drawed = false;
		for (unsigned int i = 0; i <  leaderboard.size(); i++) {
			if (player.getId() == leaderboard[i] || (i < 4) || (i<5 && player_drawed)) {
				sf::Text draw_leaderboard;
				draw_leaderboard.setFont(_font);
				std::ostringstream ss_leaderboard;
				std::string p_name = player.getEnemyName(leaderboard[i]);
				if (p_name.length() > 10)
					p_name = p_name.substr(0,9)+"...";
				ss_leaderboard << i + 1 << ". " << p_name << " (" << player.getEnemyScore(leaderboard[i]) << ")";
				draw_leaderboard.setCharacterSize(12);
				draw_leaderboard.setString(ss_leaderboard.str());
				if (draw_leaderboard.getLocalBounds().width > 185) {
					draw_leaderboard.setCharacterSize(185 / ss_leaderboard.str().length());
				}
				if (player.getId() == leaderboard[i]) {
					draw_leaderboard.setColor(sf::Color(255,80,80));
					player_drawed = true;
				}
				else {
					draw_leaderboard.setColor(sf::Color::White);
				}
				draw_leaderboard.setStyle(sf::Text::Bold);
				draw_leaderboard.setOutlineColor(sf::Color(25,25,25));
				draw_leaderboard.setOutlineThickness(1);
				draw_leaderboard.setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 185 - 10, window.getView().getCenter().y - window.getView().getSize().y / 2 + top_margin);
				window.draw(draw_leaderboard);
				top_margin += draw_leaderboard.getLocalBounds().height + 5;
			}
		}
		leaderboard_bg.setSize(sf::Vector2f(185 +10, top_margin-10+10));//185(meret), 10(5+5 a padding); -10(top-margin),10(5+5 a padding)
		window.draw(leaderboard_bg);

		bool a = player.getUpgradeAvailable() > 0;
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
