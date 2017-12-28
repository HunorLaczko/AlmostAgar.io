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
	TextEditor *playerName = new TextEditor(300, 200, 400, 65, "", "�rd be a neved!");
	main_menu.push_back(serverIp);
	main_menu.push_back(playerName);



	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	Windows window(sf::VideoMode(1000, 600), "almostagar.io", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);
	window.setMouseCursorGrabbed(false);

	lambdaButton *g1 = new lambdaButton(300 + 100, 300, 200, 50, "J�t�k",
		[&]//mindent akarok haszn�lni
	()
	{
		/*if (!serverIp->isempty() && !playerName->isempty()) {
		std::cout << "IP: " << serverIp->getValue() << std::endl;
		std::cout << "Nev: " << playerName->getValue() << std::endl;
		}*/
		//window.close();
		std::string answer = window.validate(serverIp->getValue(), playerName->getValue());
		//Ha minden renben, akkor kezd�dj�n a j�t�k, ha nem, akkor hiba�zenet
		if (answer == "OK") {
			window.threadWait();
			window.initGame(sf::IpAddress(serverIp->getValue()), playerName->getValue());
			window.changeview(Views::game_view);
			start = true;
		}
		else {
			//TODO B�lint: Hiba�zenet ki�r�sa
			std::cout << answer << std::endl;
		}
	}
	);
	main_menu.push_back(g1);

	lambdaButton *g2 = new lambdaButton(300 + 100, 400, 200, 50, "Kil�p�s",
		[&]//mindent akarok haszn�lni
	()
	{
		window.close();
		start = false;
		fut = false;
	}
	);
	main_menu.push_back(g2);

	lambdaButton *newgame = new lambdaButton(300 + 100, 100, 200, 50, "�j j�t�k",
		[&]//mindent akarok haszn�lni
	()
	{
		//window.close();
		window.changeview(Views::game_view);
		start = false;
	}
	);
	game_over_menu.push_back(newgame);

	lambdaButton *exit = new lambdaButton(300 + 100, 200, 200, 50, "Kil�p�s",
		[&]//mindent akarok haszn�lni
	()
	{
		window.close();
		start = false;
		fut = false;
	}
	);
	game_over_menu.push_back(exit);


	//Men� elemek �tad�sa az ablaknak, majd IP �s n�v bet�lt�s
	window.set(main_menu, game_over_menu);
	window.load();

	//Fut a program
	while (fut && window.isOpen()) {
		//Fut valamelyik men�, vagy a j�t�k
		window.event_loop();
	}

	//IP �s n�v ment�s
	window.save();
	window.threadWait();
	return 0;
}