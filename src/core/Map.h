#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
#include "Role.h"

/*
* The end point of the current level in the game. 
* Touching it will refresh the next level.
*/
class Fin {
public:
	Fin();
	void draw(sf::RenderWindow& window) const;
	void Collision(Role &role) const;
private:
	sf::Texture finTexture;
	sf::Sprite finSprite;
};

/*
* Below is the item that can be obtained in the game
*/
class Tool {
public:
	Tool() ;
	void draw(sf::RenderWindow& window) const ;
	void create() ;
	bool Collision(const Role& role) ;
private:
	int offset;
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
};

/*
* Below are the monsters in the game. 
* If you touch them, you will fail the current level.
*/
class Monster {
public:
	Monster() =default;
	explicit Monster(const std::filesystem::path& tileset);
	void draw(sf::RenderWindow& window) const;
	void create();
	void Collision(Role& role);
	void move(const sf::Time& elapsedTime);
private:
	sf::Vector2f speed{ -3.f,0.f };
	int offset;
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
};