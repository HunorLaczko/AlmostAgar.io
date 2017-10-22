#include "stdafx.h"
#include "Windows.h"


Windows::Windows()
	:RenderWindow()
{

}

Windows::Windows(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings)
	: sf::RenderWindow(mode, title, style, settings)
{
}

Windows::Windows(sf::WindowHandle handle, const sf::ContextSettings & settings)
	: RenderWindow(handle, settings)
{
}


Windows::~Windows()
{

}



void Windows::event_loop(){

	while (isOpen())
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
			}

			if (event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Escape) {
					close();
				}
			}

			/*if (event.type == sf::Event::Resized)
			{
				std::cout << "new width: " << event.size.width << std::endl;
				std::cout << "new height: " << event.size.height << std::endl;
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128)
					std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					std::cout << "the escape key was pressed" << std::endl;
					std::cout << "control:" << event.key.control << std::endl;
					std::cout << "alt:" << event.key.alt << std::endl;
					std::cout << "shift:" << event.key.shift << std::endl;
					std::cout << "system:" << event.key.system << std::endl;
				}
			}*/
		}

		clear();
		//widgetek kirajzolása
		for (unsigned i = 0; i < widgets.size(); i++) {
			widgets[i]->draw(*this);
		}
		display();
	}
}

void Windows::add(Widget *w)
{
	widgets.push_back(w);
}
