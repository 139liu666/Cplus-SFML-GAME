#pragma once
/*
* This code is to create and operate the element of the map
* Ground,step,platform;these are elements that can be interacted with,such as collision detection
* Cloud,tree;these re elements that just background of the scene
*/
#include<vector>
#include<random>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Role.h"

//Parent class:basic function and variable,this is to create the basic ground of the map
class Map {
public:
	Map() = default;
    explicit Map(const std::filesystem::path& tileset);
	virtual void drawMap(sf::RenderWindow& window) const;
	virtual void createMap();
	virtual unsigned char checkCollision(sf::FloatRect boundBox) const;
	void updateMap(int level);
	virtual void initMap();
	virtual ~Map()=default;
private:
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
	std::vector<std::vector<int>> mapData;
};

//Child class:this is to create the step that can be a obstacle
class Step:public::Map{
public:
	explicit Step(const std::filesystem::path& tileset);
	void drawMap(sf::RenderWindow& window) const override;
	void createMap() override;
	unsigned char checkCollision(sf::FloatRect boundBox) const override;
	void initMap() override;
private:
	int offset;
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
	std::vector<std::vector<int>> mapData;
};

//Child class:this is to create the suspended springboard
class PlatForm :public::Map {
public:
	explicit PlatForm(const std::filesystem::path& tileset);
	void drawMap(sf::RenderWindow& window) const override;
	void createMap() override;
	unsigned char checkCollision(sf::FloatRect boundBox) const override;
	void initMap() override;
private:
	int offset;
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
	std::vector<std::vector<int>> mapData;
};

/*
* Below is the background
*/
class Cloud :public::Map{
public:
	explicit Cloud(const std::filesystem::path& tileset);
	void drawMap(sf::RenderWindow& window) const override;
	void createMap() override;
	void initMap() override;
private:
	int offset;
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
};

class Tree :public::Map {
public:
	explicit Tree(const std::filesystem::path& tileset);
	void drawMap(sf::RenderWindow& window) const override;
	void createMap() override;
	void initMap() override;
private:
	int offset;
	sf::Texture ceilTexture;
	std::vector<sf::Sprite> ceilSprite;
};

