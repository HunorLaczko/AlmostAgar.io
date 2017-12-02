#pragma once
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
class FoodGenerator
{
private:
	std::vector<sf::Vector2f> food;
	float radius;
	sf::Vector2f background;
	sf::Vector2f map;

public:
	FoodGenerator();
	FoodGenerator(const sf::Vector2f _background, const sf::Vector2f _map);
	~FoodGenerator();

	void generateFood(const int number);
	std::vector<sf::Vector2f> getFood();
	void setFood(const std::vector<sf::Vector2f>& newFood);
	sf::Vector2f updateElement(int index);
	float getFoodRadius();
};

