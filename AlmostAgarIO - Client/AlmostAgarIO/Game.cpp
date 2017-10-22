#include "stdafx.h"
#include "Game.h"
#include "iostream"

Game::Game(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings)
	: sf::RenderWindow(mode, title, style, settings)
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
	std::cout << "Kész a kost" << std::endl;
}

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
	std::cout << "Kész a kost" << std::endl;
}
*/

Game::~Game()
{

}



void Game::event_loop() {
	float ZOOM = 1.01f; //1%-os zoom
	float radius = 30;
	const float radius2 = 7;


	sf::Vector2f vec(0, 0);
	sf::Vector2f movement(0, 0);

	sf::Sprite map(texture);
	sf::Sprite background(texture2);
	map.scale(1, 1);
	background.scale(1, 1);
	map.setPosition(1000, 750);

	while (isOpen())
	{
		sf::Event event;
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
		float speed = 2;
		float lenght = sqrt(distance.x*distance.x + distance.y*distance.y);
		vec.x = speed * distance.x / lenght;
		vec.y = speed * distance.y / lenght;

		if (abs(distance.x) < 2 || ((circle.getPosition().x - vec.x) <= map.getPosition().x && vec.x <= 0) || ((circle.getPosition().x + vec.x) >= (map.getPosition().x + texture.getSize().x) && vec.x >= 0) /*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*/) {
			vec.x = 0;
		}
		if (abs(distance.y) < 2 || ((circle.getPosition().y - vec.y) <= map.getPosition().y && vec.y <= 0) || ((circle.getPosition().y - vec.y) >= (map.getPosition().y + texture.getSize().y) && vec.y >= 0)/*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*/) {
			vec.y = 0;
		}
		circle.move(vec);

		for (int i = 0; i < kaja.size(); i++) {
			sf::Vector2f distance2(circle.getPosition().x - kaja[i].getPosition().x, circle.getPosition().y - kaja[i].getPosition().y);
			float lenght2 = sqrt(distance2.x*distance2.x + distance2.y*distance2.y);

			if (lenght2 < (circle.getRadius() + kaja[i].getRadius())) {
				kaja.erase(kaja.begin() + i);
				circle.setRadius(circle.getRadius() + 1);
				circle.setOrigin(circle.getRadius(), circle.getRadius());
				if (circle.getRadius() - 30 == 50) {
					view.zoom(ZOOM);
				}
				else if (circle.getRadius() - 30 == 100) {
					view.zoom(ZOOM);
				}
				else if (circle.getRadius() - 30 == 150) {
					view.zoom(ZOOM);
				}

			}
		}
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


		circle.setFillColor(sf::Color(0, 250, 0, 150));
		circle.setOutlineThickness(-5);
		circle.setOutlineColor(sf::Color(0, 150, 0));

		clear(sf::Color::Black);


		setView(view);
		draw(background);
		draw(map);
		for (int i = 0; i < kaja.size(); i++) {
			draw(kaja[i]);
		}
		draw(circle);
		display();

	}
}

void Game::add()
{

}
