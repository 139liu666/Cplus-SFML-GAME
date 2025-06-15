/*
* This section is the main code that controls the gameplay and
* is responsible for updating the state in real time
*/
/*
* --This game uses two design patterns: 
    Factory pattern and Composite pattern.
  --In this design, Map serves as an abstract base class or interface that defines the general characteristics of map terrain.
    Different types of terrain inherit from Map and implement their own specific behaviors.
  --Each large map object (Map) is treated as a composite node (Composite), which contains multiple terrain objects (Terrain), 
    and each terrain object contains its corresponding texture object (Texture), forming a hierarchical composite system. 
	These elements (map, terrain, texture) all implement the same unified interface.
* --Since the basic map data is simple, vector format was used directly instead of XML format.
* --This code additionally uses the ImGui library for graphical user interface.
*/
#include <iostream>
#include "Game.h"
#include <string>
#include "Global.h"
#include "mapManager.h"

using namespace std;
constexpr sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); //Frame limite


Game::Game():
	view(sf::FloatRect({ 0.f, 0.f }, { WIDTH, HEIGHT })) ,
	//set the menu
	menuTexture("resources/menu.png"),
	menu{menuTexture},
	mFont("resources/font.ttf"),
	mMu("resources/font.ttf"),
	Level(0),
	numTool(0){
	//set the text in the game
	sf::Vector2f viewPos = window.mapPixelToCoords(sf::Vector2i(100, 10));
	mText.setPosition(viewPos);//Level information
	mText.setCharacterSize(20);
	mText.setFillColor(sf::Color::Black);

	tMu.setPosition({ 100.f,40.f });//menu text
	tMu.setString("Evolution");
	tMu.setCharacterSize(80);
	tMu.setFillColor(sf::Color::Yellow);
	tMu.setOutlineThickness(1.f);
	tMu.setOutlineColor(sf::Color::Black);

	view.setCenter({ WIDTH / 2.f, HEIGHT / 2.f });
	window.setView(view);
}
//This part updates map information based on the current level.
void Game::initScene() {
	step.clear();
	//main map create
	map.updateMap(Level);
	map.initMap();
	map.createMap();

	tool.create();

	//this is to create the relevant map according to the level
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
		
		/*
		monster = Monster("resources/bee.png");
		monster.create();
		*/
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

		monster = Monster("resources/bee.png");
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
//Main game logic
void Game::run() {
	//fixed frame step
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	window.setVerticalSyncEnabled(true);
	//Init the GUI
	if (!ImGui::SFML::Init(window)) exit(1);
	//Check the window
	while (window.isOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		//update
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			//Handling map updates
			fin.Collision(role);
			if (role.getState()&&Level!=0&&Level<3) {
				Level++;
				initScene();
				numTool=0;
				view.setCenter({ WIDTH / 2.f, view.getCenter().y });
				window.setView(view);
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
		ImGui::SFML::ProcessEvent(window,*event);
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
	if (Level) {
		//Check the collision with the tool
		if (tool.Collision(role) && numTool < NUMTOOL) {
			numTool++;
		}
		monster.move(elapsedTime);
		//Check the collision with the monster
		if (monster.Collision(role)) {// role is death
			isDead = true;
		}
		//Check the collision with the map
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
	if (Level) {//game level render
		map.drawMap(window);
		for (const auto& s : step) {
			s->drawMap(window);
		}
		tool.draw(window);
		monster.draw(window);
		fin.draw(window);
		role.draw(window);
		window.draw(mText);
		//GUI render
		if (isDead) {
			ImGui::SFML::Update(window, deltaClock.restart());
			ImGui::SetNextWindowPos(ImVec2(WIDTH / 2, HEIGHT / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiCond_Always);
			ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::Text("You Died!");
			if (ImGui::Button("Restart", ImVec2(100, 50))) {
				initScene();
				role.restart();
				isDead = false;
				view.setCenter({ WIDTH / 2.f, view.getCenter().y });
				window.setView(view);
			}
			ImGui::End();
			ImGui::SFML::Render(window);
		}
		else {
			ImGui::SFML::Update(window, deltaClock.restart());
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(70, 130), ImGuiCond_Always);
			ImGui::Begin("Set");
			//Level choice
			if (ImGui::Button("Exit")) {
				Level = 0;
			}
			if (ImGui::Button("L1")) {
				Level = 1;
				initScene();
				role.restart();
			}
			if (ImGui::Button("L2")) {
				Level = 2;
				initScene();
				role.restart();
			}
			if (ImGui::Button("L3")) {
				Level = 3;
				initScene();
				role.restart();
			}
			ImGui::End();
			ImGui::SFML::Render(window);
		}
	}
	else{//menu render
		window.draw(menu);
		window.draw(tMu);
		//GUI render
		ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::SetNextWindowPos(ImVec2(WIDTH/2, HEIGHT/2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiCond_Always);
		ImGui::Begin("Menu");
		if (ImGui::Button("Start", ImVec2(100, 50))) {
			Level++;
			initScene();
		}
		if (ImGui::Button("End", ImVec2(100, 50))) {
			exit(0);
		}
		ImGui::End();
		ImGui::SFML::Render(window);
	}
	window.display();
}
//the info about the level
void Game::updateStatistics() {
	sf::Vector2f pos = view.getCenter();
	mText.setPosition({pos.x-50.f,10.f});
	mText.setString(format("Level: {}/3 \t Tool Number: {}/{}", Level, numTool,NUMTOOL));
}
