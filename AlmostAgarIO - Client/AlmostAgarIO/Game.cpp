#include "stdafx.h"

#include "Game.h"
#include "iostream"

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
	setVerticalSyncEnabled(false);
	setFramerateLimit(60);
	setMouseCursorGrabbed(false);
	//window.setMouseCursorVisible(false);
	circle.setRadius(radius);
	circle.scale(1, 1);
	circle.setOrigin(radius, radius);
	circle.setPosition((float)texture2.getSize().x / 2, (float)texture2.getSize().y / 2);
	///view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
	map.setTexture(texture);
	background.setTexture(texture2);
	map.scale(1, 1);
	background.scale(1, 1);
	map.setPosition(1000, 750);

	//kajagenerálás
	gen = FoodGenerator(sf::Vector2f(background.getLocalBounds().width, background.getLocalBounds().height), sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height));
	gen.generateFood(1000);

	std::vector<sf::Vector2f> tmpPos = gen.getFood();
	for (int i = 0; i < 1000; i++) {
		sf::CircleShape circ(gen.getFoodRadius());
		circ.setPosition(tmpPos[i]);
		circ.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		circ.setOrigin(gen.getFoodRadius() / 2, gen.getFoodRadius() / 2);
		food.push_back(circ);
	}

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
	network = new Network();
	/*************************
	if (!texture.loadFromFile("palya.jpg") || !texture2.loadFromFile("background.png"))
	{
		close();
	}
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	view.zoom(ZOOM);
	setVerticalSyncEnabled(false);
	setFramerateLimit(60);
	setMouseCursorGrabbed(false);
	//window.setMouseCursorVisible(false);
	circle.setRadius(radius);
	circle.scale(1, 1);
	circle.setOrigin(radius, radius);
	circle.setPosition(texture2.getSize().x / 2, texture2.getSize().y / 2);
	///view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
	map.setTexture(texture);
	background.setTexture(texture2);
	map.scale(1, 1);
	background.scale(1, 1);
	map.setPosition(1000, 750);

	//lassu a generator
	//gen.generateFood(1000, sf::Vector2f(background.getLocalBounds().width, background.getLocalBounds().height), sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height));

	for (int i = 0; i < 1000; i++) {
		sf::CircleShape tmp(radius2);
		tmp.setPosition(sf::Vector2f((texture2.getSize().x - texture.getSize().x) / 2 + rand() % texture.getSize().x, (texture2.getSize().y - texture.getSize().y) / 2 + rand() % texture.getSize().y));
		tmp.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		tmp.setOrigin(radius2 / 2, radius2 / 2);
		food.push_back(tmp);
	}
	**************************/
}
/*
Game::Game(sf::WindowHandle handle, const sf::ContextSettings & settings)
{

}*/


Game::~Game()
{

}

void Game::threadWait() {
	std::cout << "A szal varakozik\n";
	thread.wait();
	std::cout << "A tolto szal befejezte a varakozast\n";
}

void Game::resize(sf::Event::SizeEvent event_size, sf::Vector2u window_size) {
	setView(sf::View(sf::FloatRect(0.f, 0.f, (float) event_size.width, (float) event_size.height)));
	view.reset((sf::FloatRect(circle.getPosition().x - (float) window_size.x / 2, circle.getPosition().y - (float) window_size.y / 2, (float) window_size.x, (float) window_size.y)));
}

sf::Vector2f Game::getCirclePos()
{
	return circle.getPosition();
}

void Game::setIpAndWindowSize(sf::IpAddress _serverIp, sf::Vector2u window_size)
{
	network->setIp(_serverIp);
	player.setWindowSize(window_size);
}

void Game::connect()
{	
	network->connectPlayer(&player);
	network->init(sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height), (sf::Vector2f)map.getPosition(), (sf::Vector2f)player.getWindowSize());
	std::cout << "player pos in init: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;
}

void Game::disconnect()
{
	network->disconnectPlayer();
	//delete network;
}

void Game::counting(sf::RenderWindow & window)
{
	//Számolás
	sf::Vector2f distance(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x - circle.getPosition().x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y - circle.getPosition().y);
	float speed = 2.2f - (0.005f*circle.getRadius());
	if (speed <= 0.6) speed = 0.6f;

	//std::cout << "Size: " << circle.getRadius() << " Speed: " << speed << std::endl;
	float length = sqrt(distance.x*distance.x + distance.y*distance.y);
	vec.x = speed * distance.x / length;
	vec.y = speed * distance.y / length;

	if (abs(distance.x) < 2 || ((circle.getPosition().x - vec.x) <= map.getPosition().x && vec.x <= 0) || ((circle.getPosition().x + vec.x) >= (map.getPosition().x + map.getLocalBounds().width) && vec.x >= 0) /*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*/) {
		vec.x = 0;
	}
	if (abs(distance.y) < 2 || ((circle.getPosition().y - vec.y) <= map.getPosition().y && vec.y <= 0) || ((circle.getPosition().y - vec.y) >= (map.getPosition().y + map.getLocalBounds().height) && vec.y >= 0)/*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*/) {
		vec.y = 0;
	}

	//Pozició küldés/fogadás
	//circle.move(vec);
	//player.setPosition(circle.getPosition());
	if (clock.getElapsedTime() > sf::milliseconds(33)) {
		network->sendPosition(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y));
		oldPos = player.getPosition();
		network->getResponse();
		if (first) {
			view.reset((sf::FloatRect(player.getPosition().x - (float) window.getSize().x / 2, player.getPosition().y - (float) window.getSize().y / 2, (float) window.getSize().x, (float) window.getSize().y)));
			first = false;
		}
		sf::Vector2f vecFromServer = player.getPosition() - oldPos;
		//std::cout << "vec\fromServer: " << vecFromServer.x << "," << vecFromServer.y << " vecFromClient: " << vec.x << "," << vec.y << " dist: " << distance.x << "," << distance.y << " length: " << length << "\n";
		circle.setPosition(player.getPosition());
		clock.restart();
	}


	//circle.setPosition(circle.getPosition() + vec);

	//Egyéb számolás, ütközés a kajával, majd méret növelés
	//bool changed = false;
	for (int i = 0; i < food.size(); i++) {
		sf::Vector2f distance2(circle.getPosition().x - food[i].getPosition().x, circle.getPosition().y - food[i].getPosition().y);
		float lenght2 = sqrt(distance2.x*distance2.x + distance2.y*distance2.y);

		if (lenght2 < (circle.getRadius() + gen.getFoodRadius())) {
			//food.erase(food.begin() + i);
			food[i].setPosition(gen.updateElement(i));
			//food[i].setPosition();
			//changed = true;
			circle.setRadius(circle.getRadius() + 0.5f);
			circle.setOrigin(circle.getRadius(), circle.getRadius());
			//gen.delElement(i);
			if ((int)(circle.getRadius() - 30) % 5 == 0 && (circle.getRadius() - 30) == (int)(circle.getRadius() - 30)) {
				view.zoom(ZOOM);
			}

		}
	}
	//food = gen.getFood();
	//if(changed) gen.setFood(food);
	
	//kamera mozgas szamolasa
	sf::Vector2f distFromCenter(circle.getPosition().x - view.getCenter().x, circle.getPosition().y - view.getCenter().y);
	float lenght2 = sqrt(distFromCenter.x * distFromCenter.x + distFromCenter.y*distFromCenter.y);
	movement.x = speed * distFromCenter.x / lenght2;
	movement.y = speed * distFromCenter.y / lenght2;

	if (abs(distFromCenter.x) < 20 || (vec.x == 0 && vec.y == 0)) {
		movement.x = 0;
	}
	if (abs(distFromCenter.y) < 20 || (vec.x == 0 && vec.y == 0)) {
		movement.y = 0;
	}
}


void Game::draw(sf::RenderWindow & window)
{
	view.move(movement);

	circle.setFillColor(sf::Color(0, 250, 0));
	circle.setOutlineThickness(-5);
	circle.setOutlineColor(sf::Color(0, 150, 0));

	circle.getPosition().x;
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
		window.draw(circle);
	}
	window.display();
}

//Régi
/*
#include "Game.h"
#include "iostream"

float ZOOM = 1.01f; //1%-os zoom
float radius = 30;
const float radius2 = 7;
sf::Vector2f oldPos(0, 0);

Game::Game(sf::VideoMode mode, const sf::String &title, sf::IpAddress _serverIp, sf::Uint32 style, const sf::ContextSettings &settings)
	: sf::RenderWindow(mode, title, style, settings)
{
	if (!texture.loadFromFile("palya.jpg") || !texture2.loadFromFile("background.png"))
	{
		close();
	}
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	view.zoom(ZOOM);
	setVerticalSyncEnabled(false);
	setFramerateLimit(60);
	setMouseCursorGrabbed(false);
	//window.setMouseCursorVisible(false);
	circle.setRadius(radius);
	circle.scale(1, 1);
	circle.setOrigin(radius, radius);
	circle.setPosition(texture2.getSize().x / 2, texture2.getSize().y / 2);
	view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
	map.setTexture(texture);
	background.setTexture(texture2);
	map.scale(1, 1);
	background.scale(1, 1);
	map.setPosition(1000, 750);

	//lassu a generator
	//gen.generateFood(1000, sf::Vector2f(background.getLocalBounds().width, background.getLocalBounds().height), sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height));

	for (int i = 0; i <1000; i++) {
		sf::CircleShape tmp(radius2);
		tmp.setPosition(sf::Vector2f((texture2.getSize().x - texture.getSize().x) / 2 + rand() % texture.getSize().x, (texture2.getSize().y - texture.getSize().y) / 2 + rand() % texture.getSize().y));
		tmp.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		tmp.setOrigin(radius2 / 2, radius2 / 2);
		food.push_back(tmp);
	}


	network = new Network(_serverIp, &player);
	network->connectPlayer();
	network->init(sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height), (sf::Vector2f)map.getPosition(), (sf::Vector2f)this->getSize());

}
*/
/*
Game::Game(sf::WindowHandle handle, const sf::ContextSettings & settings)
: RenderWindow(handle, settings)
{
float ZOOM = 1.01f; //1%-os zoom
float radius = 30;
const float radius2 = 7;
if (!texture.loadFromFile("palya.jpg") || !texture2.loadFromFile("background.png"))
{
close();
}
view.setViewport(sf::FloatRect(0, 0, 1, 1));
view.zoom(ZOOM);
setVerticalSyncEnabled(false);
setFramerateLimit(60);
setMouseCursorGrabbed(false);
//window.setMouseCursorVisible(false);
circle.setRadius(radius);
circle.scale(1, 1);
circle.setOrigin(radius, radius);
circle.setPosition(texture2.getSize().x / 2, texture2.getSize().y / 2);
view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
for (int i = 0; i <700; i++) {
sf::CircleShape tmp(radius2);
tmp.setPosition(sf::Vector2f((texture2.getSize().x - texture.getSize().x) / 2 + rand() % texture.getSize().x, (texture2.getSize().y - texture.getSize().y) / 2 + rand() % texture.getSize().y));
tmp.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
tmp.setOrigin(radius2 / 2, radius2 / 2);
kaja.push_back(tmp);
}
std::cout << "K�sz a kost" << std::endl;
}
*/
/*
Game::~Game()
{

}



void Game::event_loop() {
	sf::Vector2f vec(0, 0);
	sf::Vector2f movement(0, 0);
	sf::Clock clock;
	while (isOpen())
	{
		sf::Event event;
		//std::vector<sf::CircleShape> food = gen.getFood();
		while (pollEvent(event))
		{

			switch (event.type)
			{
			case sf::Event::Closed: {
				close();
			}
			case sf::Event::Resized: {
				setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
				break;
			}
			case sf::Event::KeyPressed: {
				if (event.key.code == sf::Keyboard::Escape) {
					close();
				}
				if (event.key.code == sf::Keyboard::Up) {
					radius += 2;
					circle.setRadius(radius);
					circle.setOrigin(circle.getRadius(), circle.getRadius());
				}
				if (event.key.code == sf::Keyboard::Down) {
					radius -= 2;
					circle.setRadius(radius);
					circle.setOrigin(circle.getRadius(), circle.getRadius());

				}
				if (event.key.code == sf::Keyboard::Right) {
					view.zoom(1 / ZOOM);
				}

				if (event.key.code == sf::Keyboard::Left) {
					view.zoom(ZOOM);

				}

				break;
			}
			}
		}
		sf::Vector2f distance(mapPixelToCoords(sf::Mouse::getPosition(*this)).x - circle.getPosition().x, mapPixelToCoords(sf::Mouse::getPosition(*this)).y - circle.getPosition().y);
		float speed = 2.2 - (0.005*circle.getRadius());
		if (speed <= 0.6) speed = 0.6;

		//std::cout << "Size: " << circle.getRadius() << " Speed: " << speed << std::endl;
		float length = sqrt(distance.x*distance.x + distance.y*distance.y);
		vec.x = speed * distance.x / length;
		vec.y = speed * distance.y / length;

		if (abs(distance.x) < 2 || ((circle.getPosition().x - vec.x) <= map.getPosition().x && vec.x <= 0) || ((circle.getPosition().x + vec.x) >= (map.getPosition().x + map.getLocalBounds().width) && vec.x >= 0) *//*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*//*) {
			vec.x = 0;
		}
		if (abs(distance.y) < 2 || ((circle.getPosition().y - vec.y) <= map.getPosition().y && vec.y <= 0) || ((circle.getPosition().y - vec.y) >= (map.getPosition().y + map.getLocalBounds().height) && vec.y >= 0)*//*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*//*) {
			vec.y = 0;
		}

		//circle.move(vec);
		//player.setPosition(circle.getPosition());
		if (clock.getElapsedTime() > sf::milliseconds(33)) {
			network->sendPosition(sf::Vector2f(mapPixelToCoords(sf::Mouse::getPosition(*this)).x, mapPixelToCoords(sf::Mouse::getPosition(*this)).y));
			oldPos = player.getPosition();
			network->getResponse();
			sf::Vector2f vecFromServer = player.getPosition() - oldPos;
			std::cout << "vec\fromServer: " << vecFromServer.x << "," << vecFromServer.y << " vecFromClient: " << vec.x << "," << vec.y << " dist: " << distance.x << "," << distance.y << " length: " << length << "\n";
			circle.setPosition(player.getPosition());
			clock.restart();
		}


		//circle.setPosition(circle.getPosition() + vec);


		bool changed = false;
		for (int i = 0; i < food.size(); i++) {
			sf::Vector2f distance2(circle.getPosition().x - food[i].getPosition().x, circle.getPosition().y - food[i].getPosition().y);
			float lenght2 = sqrt(distance2.x*distance2.x + distance2.y*distance2.y);

			if (lenght2 < (circle.getRadius() + food[i].getRadius())) {
				food.erase(food.begin() + i);
				//changed = true;
				circle.setRadius(circle.getRadius() + 0.5);
				circle.setOrigin(circle.getRadius(), circle.getRadius());
				//gen.delElement(i);
				if ((int)(circle.getRadius() - 30) % 5 == 0 && (circle.getRadius() - 30) == (int)(circle.getRadius() - 30)) {
					view.zoom(ZOOM);
				}

			}
		}
		//food = gen.getFood();
		//if(changed) gen.setFood(food);

		sf::Vector2f distFromCenter(circle.getPosition().x - view.getCenter().x, circle.getPosition().y - view.getCenter().y);
		float lenght2 = sqrt(distFromCenter.x * distFromCenter.x + distFromCenter.y*distFromCenter.y);
		movement.x = speed * distFromCenter.x / lenght2;
		movement.y = speed * distFromCenter.y / lenght2;

		if (abs(distFromCenter.x) < 20 || (vec.x == 0 && vec.y == 0)) {
			movement.x = 0;
		}
		if (abs(distFromCenter.y) < 20 || (vec.x == 0 && vec.y == 0)) {
			movement.y = 0;
		}

		view.move(movement);


		circle.setFillColor(sf::Color(0, 250, 0));
		circle.setOutlineThickness(-5);
		circle.setOutlineColor(sf::Color(0, 150, 0));

		clear(sf::Color::Black);


		setView(view);
		draw(background);
		draw(map);
		for (int i = 0; i < food.size(); i++) {
			if ((mapPixelToCoords(sf::Vector2i(0, 0)).x + view.getSize().x) > food[i].getPosition().x && mapPixelToCoords(sf::Vector2i(0, 0)).x < food[i].getPosition().x &&
				(mapPixelToCoords(sf::Vector2i(0, 0)).y + view.getSize().y) > food[i].getPosition().y && mapPixelToCoords(sf::Vector2i(0, 0)).y < food[i].getPosition().y) {
				draw(food[i]);

			}
		}
		draw(circle);
		display();

	}
}

void Game::add()
{

}*/