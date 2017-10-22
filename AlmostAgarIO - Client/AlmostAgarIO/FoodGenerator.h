#pragma once
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
class FoodGenerator
{
private:
	std::vector<sf::CircleShape> food;
	float radius;

public:
	FoodGenerator();
	~FoodGenerator();

	void generateFood(const int number, const sf::Vector2f background, const sf::Vector2f map);
	std::vector<sf::CircleShape> getFood();
	void setFood(std::vector<sf::CircleShape> newFood);
};

