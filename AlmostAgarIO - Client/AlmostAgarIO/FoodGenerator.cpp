#include "stdafx.h"
#include "FoodGenerator.h"
#include <random>


FoodGenerator::FoodGenerator()
{
	radius = 7;
}


FoodGenerator::~FoodGenerator()
{
}


void FoodGenerator::generateFood(const int number, const sf::Vector2f background, const sf::Vector2f map)
{
	srand(time(0));
	for (int i = 0; i <number; i++) {
		sf::CircleShape tmp(radius);
		tmp.setPosition(sf::Vector2f((background.x- map.x) / 2 + rand() % (int)map.x, (background.y - map.y) / 2 + rand() % (int)map.y));
		tmp.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		tmp.setOrigin(radius / 2, radius / 2);
		food.push_back(tmp);
	}
}

void FoodGenerator::setFood(std::vector<sf::CircleShape> newFood) {
	food = newFood;
}

std::vector<sf::CircleShape> FoodGenerator::getFood() 
{
	return food;
}


