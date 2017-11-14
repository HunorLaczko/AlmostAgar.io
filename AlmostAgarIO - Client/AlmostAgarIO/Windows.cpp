#include "stdafx.h"
#include "Windows.h"


float ZOOM = 1.01f; //1%-os zoom
float radius = 30;
const float radius2 = 7;
sf::Vector2f oldPos(0, 0);

/*
static const float WIDTH = 1000.0f;
static const float HEIGHT = 563.0f;
float ZOOM = 1.1f; //10%-os zoom
float radius = 30;
float radius2 = 7;
srand(time(0));
sf::CircleShape circle(radius);
*/
Windows::Windows()
	:RenderWindow()
{

}

Windows::Windows(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings)
	: sf::RenderWindow(mode, title, style, settings)
{
//Menü rész
	viewid = Views::main_menu_view;
	viewChanged = false;
	size_x=mode.width;
	size_y=mode.height;

//Játék rész
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
}

Windows::Windows(sf::WindowHandle handle, const sf::ContextSettings & settings)
	: RenderWindow(handle, settings)
{
}


Windows::~Windows()
{

}



void Windows::event_loop(){
	if (viewid == Views::game_view) {
		sf::Vector2f vec(0, 0);
		sf::Vector2f movement(0, 0);
		sf::Clock clock;
		while (isOpen() && !viewChanged)
		{
			sf::Event event;
			//std::vector<sf::CircleShape> food = gen.getFood();
			while (pollEvent(event))
			{

				switch (event.type)
				{
				case sf::Event::Closed: {
					changeview(Views::game_over_menu_view);
					//close();
				}
				case sf::Event::Resized: {
					setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
					view.reset((sf::FloatRect(circle.getPosition().x - getSize().x / 2, circle.getPosition().y - getSize().y / 2, getSize().x, getSize().y)));
					break;
				}
				case sf::Event::KeyPressed: {
					if (event.key.code == sf::Keyboard::Escape) {
						changeview(Views::game_over_menu_view);
						//close();
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

			if (abs(distance.x) < 2 || ((circle.getPosition().x - vec.x) <= map.getPosition().x && vec.x <= 0) || ((circle.getPosition().x + vec.x) >= (map.getPosition().x + map.getLocalBounds().width) && vec.x >= 0) /*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > background.getPosition().x && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x < (background.getPosition().x + texture.getSize().x))*/) {
				vec.x = 0;
			}
			if (abs(distance.y) < 2 || ((circle.getPosition().y - vec.y) <= map.getPosition().y && vec.y <= 0) || ((circle.getPosition().y - vec.y) >= (map.getPosition().y + map.getLocalBounds().height) && vec.y >= 0)/*|| !(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y > background.getPosition().y && window.mapPixelToCoords(sf::Mouse::getPosition(window)).y < (background.getPosition().y + texture.getSize().y))*/) {
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
	else {
		setView(sf::View(sf::FloatRect(0, 0, size_x, size_y)));
		//view.reset((sf::FloatRect(0, 0, getSize().x, getSize().y)));
		std::vector<Widget*> widgets;
		if (viewid == Views::main_menu_view) {
			widgets = menu;
		}
		else if(viewid == Views::game_over_menu_view) {
			widgets = game_over;
		}
		while (isOpen() && !viewChanged)
		{
			sf::Event event;
			while (pollEvent(event))
			{
				//widgetek eseményeit kezelem
				for (unsigned i = 0; i < widgets.size(); i++) {
					widgets[i]->handle(event);
				}
				//Amblak zárárs
				if (event.type == sf::Event::Closed) {
					///destruktor hívás???
					close();
				}

				if (event.type == sf::Event::Resized) {
					setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
					float r = 1;
					float r_x = (float)event.size.width / (float)size_x;
					float r_y = (float)event.size.height / (float)size_y;
					if (r_x == 1)
						r = r_y;
					else if (r_y = 1)
						r = r_x;
					else
						r = std::min(r_x, r_y);
					for (unsigned i = 0; i < widgets.size(); i++) {
						widgets[i]->resize(r);
					}
					size_x = event.size.width;
					size_y = event.size.height;
				}

				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Escape) {
						close();
					}
				}
			}

			clear();
			//widgetek kirajzolása
			for(unsigned i = 0; i < widgets.size(); i++) {
				widgets[i]->draw(*this);
			}
			display();
		}
	}
	viewChanged = false;
}
void Windows::changeview(Views view)
{
	//if (viewid >= 0 && viewid <=2) {
		viewid = view;
		viewChanged = true;
	//}
}
void Windows::setIp(sf::IpAddress _serverIp)
{
	network = new Network(_serverIp, &player);
	network->connectPlayer();
	network->init(sf::Vector2f(map.getLocalBounds().width, map.getLocalBounds().height), (sf::Vector2f)map.getPosition(), (sf::Vector2f)this->getSize());
}
/*
void Windows::add(Widget *w)
{
	widgets.push_back(w);
}*/

void Windows::set(std::vector<Widget*>  _menu, std::vector<Widget*> _game_over)
{
	menu = _menu;
	game_over = _game_over;
}