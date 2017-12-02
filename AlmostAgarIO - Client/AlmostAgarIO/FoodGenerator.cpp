#include "stdafx.h"
#include "FoodGenerator.h"
#include <random>

FoodGenerator::FoodGenerator() {
	radius = 7.0f;
}

FoodGenerator::FoodGenerator(const sf::Vector2f _background, const sf::Vector2f _map)
{
	background = _background;
	map = _map;
	radius = 7.0f;
}


FoodGenerator::~FoodGenerator()
{
}


void FoodGenerator::generateFood(const int number)
{
	srand(time(0));
	for (int i = 0; i <number; i++) {
		//sf::CircleShape tmp(radius);
		sf::Vector2f tmp((background.x- map.x) / 2 + rand() % (int)map.x, (background.y - map.y) / 2 + rand() % (int)map.y);
		//tmp.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		//tmp.setOrigin(radius / 2, radius / 2);
		food.push_back(tmp);
	}
}


void FoodGenerator::setFood(const std::vector<sf::Vector2f>& newFood) {
	food = newFood;
}

std::vector<sf::Vector2f> FoodGenerator::getFood() 
{
	return food;
}

sf::Vector2f FoodGenerator::updateElement(int index){
	srand(time(0));
	sf::Vector2f tmp((background.x - map.x) / 2 + rand() % (int)map.x, (background.y - map.y) / 2 + rand() % (int)map.y);
	food[index] = tmp;
	return tmp;
}

float FoodGenerator::getFoodRadius() {
	return radius;
}