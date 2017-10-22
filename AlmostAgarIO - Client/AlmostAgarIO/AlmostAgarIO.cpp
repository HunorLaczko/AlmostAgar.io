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

	TextEditor *textinput = new TextEditor(300, 100, 400, 65, "", "Írd be a neved!");
	window_first.add(textinput);

	lambdaButton *g1 = new lambdaButton(100 + 300, 200, 200, 50, "Játék",
		[&]//mindent akarok használni
	()
	{
		if (!textinput->isempty())
			std::cout << textinput->getValue() << std::endl;
	}
	);
	window_first.add(g1);

	window_first.event_loop();
	return 0;
}