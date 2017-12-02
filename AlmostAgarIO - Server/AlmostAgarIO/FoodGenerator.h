#pragma once
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
class FoodGenerator
{
private:
	std::vector<sf::Vector2f> food;
	float radius;
	sf::Vector2f map_pos;
	sf::Vector2f map_size;

public:
	FoodGenerator();
	FoodGenerator(const sf::Vector2f _map_pos, const sf::Vector2f _map_size);
	~FoodGenerator();

	void generateFood(const int number);
	std::vector<sf::Vector2f> getFood();
	void setFood(const std::vector<sf::Vector2f>& newFood);
	sf::Vector2f updateElement(int index);
	float getFoodRadius();
};

