#pragma once
#include <iostream>
#include "Game.h"
#include <string>
#include "Global.h"
#include "mapManager.h"

using namespace std;
constexpr sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); 


Game::Game():
	view(sf::FloatRect({ 0.f, 0.f }, { WIDTH, HEIGHT })) ,
	menuTexture("resources/menu.png"),
	menu{menuTexture},
	mFont("resources/font.ttf"),
	Level(1),
	numTool(0){
	mText.setPosition({100.f,10.f});
	mText.setCharacterSize(20);
	mText.setFillColor(sf::Color::Black);
	view.setCenter({ WIDTH / 2.f, HEIGHT / 2.f });
	window.setView(view);
}
void Game::initScene() {
	step.clear();
	map.updateMap(Level);
	map.initMap();
	map.createMap();

	tool.create();

	if (Level == 1) {
		for (int i = 0; i < 4; i++) {
			auto mapSet = make_unique<Step>("resources/wood.png");
			step.push_back(std::move(mapSet));
		}
		for (int i = 0; i < 4; i++) {
			auto mapSet = make_unique<PlatForm>("resources/wood.png");
			step.push_back(std::move(mapSet));
		}
		auto cloud = make_unique<Cloud>("resources/Cloud.png");
		step.push_back(std::move(cloud));
		auto tree = make_unique<Tree>("resources/xian.png");
		step.push_back(std::move(tree));
		
		monster = Monster("resources/bee.png");
		monster.create();
	}
	if (Level == 2) {
		for (int i = 0; i < 4; i++) {
			auto mapSet = make_unique<Step>("resources/stone.jpg");
			step.push_back(std::move(mapSet));
		}
		for (int i = 0; i < 4; i++) {
			auto mapSet = make_unique<PlatForm>("resources/stone.jpg");
			step.push_back(std::move(mapSet));
		}
		auto cloud = make_unique<Cloud>("resources/Cloud.png");
		step.push_back(std::move(cloud));
		auto tree = make_unique<Tree>("resources/tree.png");
		step.push_back(std::move(tree));

		monster = Monster("resources/huang.png");
		monster.create();
	}
	if (Level == 3) {
		for (int i = 0; i < 4; i++) {
			auto mapSet = make_unique<Step>("resources/fe.jpg");
			step.push_back(std::move(mapSet));
		}
		for (int i = 0; i < 4; i++) {
			auto mapSet = make_unique<PlatForm>("resources/fe.jpg");
			step.push_back(std::move(mapSet));
		}
		auto cloud = make_unique<Cloud>("resources/Cloud.png");
		step.push_back(std::move(cloud));
		auto tree = make_unique<Tree>("resources/light.png");
		step.push_back(std::move(tree));

		monster = Monster("resources/mouse.png");
		monster.create();
	}
	for (const auto& s : step) {
		s->initMap();
		s->createMap();
	}
}
void Game::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	window.setVerticalSyncEnabled(true);
	while (window.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			fin.Collision(role);
			if (role.getState()&&Level!=0&&Level<3) {
				Level++;
				initScene();
				role.setState(false);
			}
			update(TimePerFrame);
		}
		updateStatistics();
		render();
	}
}
void Game::processEvents() {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		if (const auto* resize=event->getIf<sf::Event::Resized>()) {
			sf::FloatRect area({ 0.f,0.f }, sf::Vector2f(resize->size));
			window.setView(sf::View(area));
		}
	}
}
void Game::update(sf::Time elapsedTime) {
	if (tool.Collision(role)&&numTool<NUMTOOL) {
		numTool++;
	}
	monster.move(elapsedTime);
	monster.Collision(role);
	if (Level) {
		unsigned char isContact = 0;
		for (const auto& s : step) {
			isContact = s->checkCollision(role.getBoundBox());
			if (isContact) {
				break;
			}
		}
		role.update(elapsedTime, map.checkCollision(role.getBoundBox()), isContact);

		//update the view according to the role's position
		sf::Vector2f rolePos = role.getSprite().getPosition();
		sf::Vector2f viewCenter = view.getCenter();
		float viewLeft = viewCenter.x - WIDTH / 2.f;
		float viewRight = viewCenter.x + WIDTH / 2.f;

		float threshold = 100.f;
		if (rolePos.x > viewRight - threshold && viewRight < MAPWIDTH * CEILSIZE) {
			view.move({ rolePos.x - (viewRight - threshold) ,0.f });
		}
		if (rolePos.x < viewLeft + threshold && viewLeft > 0) {
			view.move({ rolePos.x - (viewLeft + threshold) ,0.f });
		}
		window.setView(view);
	}
}
void Game::render() {
	window.clear(sf::Color::Cyan);
	if (Level) {
		map.drawMap(window);
		for (const auto& s : step) {
			s->drawMap(window);
		}
		tool.draw(window);
		monster.draw(window);
		fin.draw(window);
		role.draw(window);
		window.draw(mText);
	}
	else{
		window.draw(menu);
	}
	window.display();
}
void Game::updateStatistics() {
	mText.setString(format("Level: {}/3 \t Tool Number: {}/{}", Level, numTool,NUMTOOL));
}
