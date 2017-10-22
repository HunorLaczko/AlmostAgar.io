// AlmostAgarIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Button.h"
#include "Windows.h"
#include <SFML/Graphics.hpp>

#include "iostream"

int main()
{
	Windows window_first(sf::VideoMode(1000, 600), "szerver");

	lambdaButton *g1 = new lambdaButton(100 + 300, 200, 200, 50, "Uj szerver",
		[&]//mindent akarok használni
	()
	{
		
	}
	);
	window_first.add(g1);

	window_first.event_loop();
	
	return 0;
}