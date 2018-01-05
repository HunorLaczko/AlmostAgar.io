// AlmostAgarIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Button.h"
#include "TextEditor.h"
#include "SimpleText.h"
#include "Windows.h"
//#include "Game.h"

#include "iostream"

#include <SFML/Graphics.hpp>


int main()
{
	bool start = false;
	bool fut = true;

	std::vector<Widget*>  main_menu, game_over_menu;
	std::map<int, char> ip_filter{ {46,'.'} };
	TextEditor *serverIp = new TextEditor(300, 100, 400, 65, "127.0.0.1", "Szerver IP",48,57, ip_filter, [&](){}, [&](){});
	//int filter[] = {193,201,205,211,214,218,220,225,233,237,243,246,250,252,336,337,368,369};
	std::map<int, char> name_filter{ { 193,'Á' },{ 201,'É' },{ 205,'Í' },{ 211,'Ó' },{ 214,'Ö' },{ 218,'Ú' } ,{ 220,'Ü' } ,{ 225,'á' } ,{ 233,'é' } ,{ 237,'í' } ,{ 243,'ó' } ,{ 246,'ö' } ,{ 250,'ú' } ,{ 252,'ü' } ,{ 336,'Õ' } ,{ 337,'õ' } ,{ 368,'Û' } ,{ 369 ,'Û' } };
	TextEditor *playerName = new TextEditor(300, 200, 400, 65, "", "Írd be a neved!", 32, 126, name_filter, [&]() {}, [&]() {});

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	Windows window(sf::VideoMode(1000, 600), "almostagar.io", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);
	window.setMouseCursorGrabbed(false);


	SimpleText *errors = new SimpleText(300 + 100, 25, 200, 50, "");

	Button *game = new Button(300 + 100, 300, 200, 50, "Játék",
		[&]//mindent akarok használni
	()
	{
		/*if (!serverIp->isempty() && !playerName->isempty()) {
		std::cout << "IP: " << serverIp->getValue() << std::endl;
		std::cout << "Nev: " << playerName->getValue() << std::endl;
		}*/
		//window.close();
		window.initGame(sf::IpAddress(serverIp->getValue()), playerName->getValue());
		std::string answer = window.validate();
		//Ha minden renben, akkor kezdõdjön a játék, ha nem, akkor hibaüzenet
		if (answer == "OK") {
			window.threadWait();
			errors->setValue("");
			window.changeview(Views::game_view);
			start = true;
		}
		else if(answer == "WRONGIP") {
			errors->setValue("Ezen az IP címen nincs szerver!");
			serverIp->setSelected(true);
		}
		else if (answer == "LOADING") {
			errors->setValue("Próbálkozz késõbb, a játék, még töltödik!");//Csúnya, de Windows.validate-be is át kell írni, ha itt átírod
		}
	}
	, [&]() {});

	Button *close = new Button(300 + 100, 400, 200, 50, "Kilépés",
		[&]//mindent akarok használni
	()
	{
		window.close();
		start = false;
		fut = false;
	}
	, [&]() {});

	serverIp->setTabFunc([&]()
	{
		serverIp->setSelected(false);
		playerName->setSelected(true);
	}
	);
	playerName->setTabFunc([&]()
	{
		playerName->setSelected(false);
		game->setSelected(true);
	}
	);
	game->setTabFunc([&]()
	{
		game->setSelected(false);
		close->setSelected(true);
	}
	);
	close->setTabFunc([&]()
	{
		close->setSelected(false);
		serverIp->setSelected(true);
	}
	);
	serverIp->setEnterFunc([&](){
		game->action();
	}
	);
	playerName->setEnterFunc([&]()
	{
		game->action();
	}
	);
	main_menu.push_back(serverIp);
	main_menu.push_back(playerName);
	main_menu.push_back(errors);
	main_menu.push_back(game);
	main_menu.push_back(close);

	Button *newgame = new Button(300 + 100, 175, 200, 50, "Új játék",
		[&]//mindent akarok használni
	()
	{
		//window.close();
		window.changeview(Views::game_view);
		start = false;
	}
	, [&]() {});

	Button *exit = new Button(300 + 100, 250, 200, 50, "Fõmenü",
		[&]//mindent akarok használni
	()
	{
		//window.close();
		window.changeview(Views::main_menu_view);
		start = false;
		fut = true;

	}
	, [&]() {});

	SimpleText *gameover = new SimpleText(300 + 100, 100, 200, 50, "Game Over");

	newgame->setTabFunc([&]()
	{
		newgame->setSelected(false);
		exit->setSelected(true);
	}
	);
	exit->setTabFunc([&]()
	{
		exit->setSelected(false);
		newgame->setSelected(true);
	}
	);
	game_over_menu.push_back(newgame);
	game_over_menu.push_back(exit);
	game_over_menu.push_back(gameover);


	//Menü elemek átadása az ablaknak, majd IP és név betöltés
	window.set(main_menu, game_over_menu);
	window.load();

	//Fut a program
	while (fut && window.isOpen()) {
		//Fut valamelyik menü, vagy a játék
		window.event_loop();
	}

	//Biztonsági mentés(akkora kell, amikor a fõmenübõl lép ki)
	window.save();
	window.threadWait();
	return 0;
}