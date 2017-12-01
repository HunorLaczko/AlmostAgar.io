#include "stdafx.h"
#include "Windows.h"


/*(Game osztályban van)
float ZOOM = 1.01f; //1%-os zoom
float radius = 30;
const float radius2 = 7;
sf::Vector2f oldPos(0, 0);*/

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
	Game game();
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
	//Játék megnyitása/megjelenítése
		//Eseménykezelés
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
					//game.disconnect();//Lejjebb raktam
					//close();
				}
				case sf::Event::Resized: {
					game.resize(event.size, getSize());
					break;
				}
				case sf::Event::KeyPressed: {
					if (event.key.code == sf::Keyboard::Escape) {
						changeview(Views::game_over_menu_view);
						//close();
					}
					/*if (event.key.code == sf::Keyboard::Up) {
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

					}*/

					break;
				}
				}
			}

			//Számolás, küldés, stb (egyenlõre egyben)
			game.counting(*this);

			//Window beállítás, rajzolás
			game.draw(*this);

		}
		//Kilépés a játékból, kapcsolat bontás
		//first = true;
		game.disconnect();//Átraktam ide
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
	game.setIp(_serverIp, getSize());
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