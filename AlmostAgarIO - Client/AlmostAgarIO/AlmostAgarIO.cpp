// AlmostAgarIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Button.h"
#include "Windows.h"
#include "Game.h"

#include "iostream"

#include <SFML/Graphics.hpp>


int main()
{
	bool start = false;
	bool fut = true;

	TextEditor *textinput1 = new TextEditor(300, 100, 400, 65, "", "Szerver IP");
	TextEditor *textinput2 = new TextEditor(300, 200, 400, 65, "", "Írd be a neved!");
	while (fut) {
		Windows window_first(sf::VideoMode(1000, 600), "almostagar.io");
		window_first.add(textinput1);

		window_first.add(textinput2);

		lambdaButton *g1 = new lambdaButton(300 + 100, 300, 200, 50, "Játék",
			[&]//mindent akarok használni
		()
		{
			if (!textinput1->isempty() && !textinput2->isempty()) {
				std::cout << "IP: " << textinput1->getValue() << std::endl;
				std::cout << "Nev: " << textinput2->getValue() << std::endl;
			}
			window_first.close();
			start = true;
		}
		);
		window_first.add(g1);

		lambdaButton *g2 = new lambdaButton(300 + 100, 400, 200, 50, "Kilépés",
			[&]//mindent akarok használni
		()
		{
			window_first.close();
			start = false;
		}
		);
		window_first.add(g2);

		window_first.event_loop();
		fut = false;

		if (start) {
			fut = true;
			static const float WIDTH = 1000.0f;
			static const float HEIGHT = 563.0f;
			float ZOOM = 1.1f; //10%-os zoom
			float radius = 30;
			float radius2 = 7;

			srand(time(0));
			sf::CircleShape circle(radius);

			sf::ContextSettings settings;
			settings.antialiasingLevel = 4;

			Game window(sf::VideoMode(WIDTH, HEIGHT), "Teszteles alatt...", sf::IpAddress("127.0.0.1"), sf::Style::Default, settings);

			window.event_loop();
		}
		start = false;
	}
	return 0;
}