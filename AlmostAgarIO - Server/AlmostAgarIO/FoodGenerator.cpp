#include "stdafx.h"
#include "FoodGenerator.h"
#include <random>
#include <iostream>

FoodGenerator::FoodGenerator() {
	radius = 7.0f;
}

FoodGenerator::FoodGenerator(const sf::Vector2f _map_pos, const sf::Vector2f _map_size)
{
	map_pos = _map_pos;
	map_size = _map_size;
	radius = 7.0f;
	srand(time(0));
}


FoodGenerator::~FoodGenerator()
{
}


void FoodGenerator::generateFood(const int number)
{
	srand(time(0));
	for (int i = 0; i <number; i++) {
		sf::Vector2f tmp(map_pos.x + rand() % (int) map_size.x, map_pos.y + rand() % (int) map_size.y);
		//sf::Vector2f tmp((background.x- map.x) / 2 + rand() % (int)map.x, (background.y - map.y) / 2 + rand() % (int)map.y);
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
	
	sf::Vector2f tmp(map_pos.x + rand() % (int)map_size.x, map_pos.y + rand() % (int)map_size.y);
	food[index] = tmp;
	return tmp;
}

float FoodGenerator::getFoodRadius() {
	return radius;
}