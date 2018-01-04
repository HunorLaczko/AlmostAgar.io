// AlmostAgarIO.cpp : Defines the entry point for the console application.
//
//Server

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Server.h"
#include <iostream>
#include <time.h>
#include <random>

int main()
{
	srand(time(NULL));

	Server server;
	server.run();
	return 0;
}