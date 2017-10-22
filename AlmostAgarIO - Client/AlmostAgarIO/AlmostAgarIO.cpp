// AlmostAgarIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Button.h"
#include "Windows.h"

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
			Windows window_game(sf::VideoMode(1000, 600), textinput1->getValue());

			TextEditor *textinput3 = new TextEditor(300, 100, 400, 65, textinput2->getValue(), "Neved");
			window_game.add(textinput3);
			window_game.event_loop();
		}
	}
	return 0;
}