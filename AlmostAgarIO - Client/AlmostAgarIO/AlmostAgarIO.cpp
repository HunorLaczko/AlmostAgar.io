// AlmostAgarIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Button.h"
#include "Windows.h"

#include "iostream"

#include <SFML/Graphics.hpp>


int main()
{
	Windows window_first(sf::VideoMode(1000, 600), "almostagar.io");

	TextEditor *textinput1 = new TextEditor(300, 100, 400, 65, "", "Szerver IP");
	window_first.add(textinput1);

	TextEditor *textinput2 = new TextEditor(300, 200, 400, 65, "", "Írd be a neved!");
	window_first.add(textinput2);

	lambdaButton *g1 = new lambdaButton(300+100, 300, 200, 50, "Játék",
		[&]//mindent akarok használni
	()
	{
		if (!textinput1->isempty() && !textinput2->isempty())
			std::cout << "IP: " <<  textinput1->getValue() << std::endl;
			std::cout << "Nev: " << textinput2->getValue() << std::endl;
	}
	);
	window_first.add(g1);

	window_first.event_loop();
	return 0;
}