/*
* This section is the main code that controls the gameplay and 
* is responsible for updating the state in real time
*/
#pragma once
#include<vector>
#include <SFML/Graphics.hpp>
#include "imgui-SFML.h"  
#include "imgui.h"
#include "Role.h"
#include "Global.h"
#include "mapManager.h"
#include "Map.h"

class Game {
public:
	Game();
	void run();
private:
	Role role;
	Map map;
	Fin fin;
	Tool tool;
	Monster monster;
	std::vector<std::unique_ptr<Map>> step;//Use smart pointers to manage memory better
	sf::View view;
	sf::Texture menuTexture;
	sf::Sprite menu;
	sf::Font mFont;
	sf::Text mText{ mFont };
	sf::Font mMu;
	sf::Text tMu{ mMu };
	sf::Clock deltaClock;
	bool isDead{false};

	void processEvents();
	void render();
	void update(sf::Time elapsedTime);

	void updateStatistics();
	void initScene();

	int Level;
	int numTool;
	static const sf::Time TimePerFrame;
	sf::RenderWindow window{ sf::VideoMode({WIDTH,HEIGHT}), "Evolution" };
};