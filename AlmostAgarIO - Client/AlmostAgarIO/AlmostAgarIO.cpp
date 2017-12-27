// AlmostAgarIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Button.h"
#include "Windows.h"
//#include "Game.h"

#include "iostream"

#include <SFML/Graphics.hpp>


int main()
{
	bool start = false;
	bool fut = true;

	std::vector<Widget*>  main_menu, game_over_menu;

	TextEditor *serverIp = new TextEditor(300, 100, 400, 65, "127.0.0.1", "Szerver IP");
	TextEditor *playerName = new TextEditor(300, 200, 400, 65, "", "Írd be a neved!");
	main_menu.push_back(serverIp);
	main_menu.push_back(playerName);



	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	Windows window(sf::VideoMode(1000, 600), "almostagar.io", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);
	window.setMouseCursorGrabbed(false);

	lambdaButton *g1 = new lambdaButton(300 + 100, 300, 200, 50, "Játék",
		[&]//mindent akarok használni
	()
	{
		/*if (!serverIp->isempty() && !playerName->isempty()) {
		std::cout << "IP: " << serverIp->getValue() << std::endl;
		std::cout << "Nev: " << playerName->getValue() << std::endl;
		}*/
		//window.close();
		window.threadWait();
		window.setIp(sf::IpAddress(serverIp->getValue()));
		window.setName(playerName->getValue());
		window.changeview(Views::game_view);
		start = true;
	}
	);
	main_menu.push_back(g1);

	lambdaButton *g2 = new lambdaButton(300 + 100, 400, 200, 50, "Kilépés",
		[&]//mindent akarok használni
	()
	{
		window.close();
		start = false;
		fut = false;
	}
	);
	main_menu.push_back(g2);

	lambdaButton *newgame = new lambdaButton(300 + 100, 100, 200, 50, "Új játék",
		[&]//mindent akarok használni
	()
	{
		//window.close();
		window.changeview(Views::game_view);
		start = false;
	}
	);
	game_over_menu.push_back(newgame);

	lambdaButton *exit = new lambdaButton(300 + 100, 200, 200, 50, "Kilépés",
		[&]//mindent akarok használni
	()
	{
		window.close();
		start = false;
		fut = false;
	}
	);
	game_over_menu.push_back(exit);

	while (fut && window.isOpen()) {

		window.set(main_menu, game_over_menu);

		//window.changeview(1);
		window.event_loop();
		/*
		fut = false;
		if (start) {
			fut = true;
			window.changeview(1);
			//Windows window(sf::VideoMode(WIDTH, HEIGHT), "Teszteles alatt...", sf::IpAddress(serverIp->getValue()), sf::Style::Default, settings);
			window.setIp(sf::IpAddress(serverIp->getValue()));
			window.event_loop();
		}
		start = false;*/
		//window.changeview(0);
	}
	window.threadWait();
	return 0;
}