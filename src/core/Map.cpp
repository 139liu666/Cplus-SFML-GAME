/*
* This section includes interactive elements in the game
* including items that can be picked up and monsters that can be attacked.
*/
#include "Map.h"
#include "Global.h"

/*
* The end point of the current level in the game.
*/
Fin::Fin() :
finTexture("resources/fin.png"),
finSprite{finTexture} {
	//Place the Fin in the end of map
	finSprite.setPosition({MAPWIDTH*CEILSIZE-CEILSIZE,HEIGHT-3*CEILSIZE});
}
void Fin::draw(sf::RenderWindow& window) const {
	window.draw(finSprite);
}
void Fin::Collision(Role &role) const{
	sf::FloatRect boundBox = role.getBoundBox();
	sf::FloatRect finBound = finSprite.getGlobalBounds();
	if (const std::optional intersection = finBound.findIntersection(boundBox)) {
		//if collision:reset the position of the role and set the next level
		role.setState(true);
		role.restart();
	}
}

//*****Tool class*****
Tool::Tool() :offset(8),
ceilTexture("resources/mais.png") {
  
}
void Tool::draw(sf::RenderWindow& window) const {
    for (auto& cs : ceilSprite) {
        window.draw(cs);
    }
}
void Tool::create() {
    ceilSprite.clear();
    for (int i = 0; i < NUMTOOL; i++) {
        sf::Sprite sprite{ ceilTexture };
        sprite.setPosition({ static_cast<float>((i * 5 + offset+2)) * CEILSIZE,HEIGHT - 8 * CEILSIZE });
        ceilSprite.push_back(sprite);
    }
	for (int i = 0; i < NUMTOOL; i++) {
		sf::Sprite sprite{ ceilTexture };
		sprite.setPosition({ static_cast<float>((i * 5 + offset)) * CEILSIZE,HEIGHT - 3 * CEILSIZE });
		ceilSprite.push_back(sprite);
	}
}
bool Tool::Collision(const Role& role) {
	sf::FloatRect boundBox = role.getBoundBox();
	for (auto it = ceilSprite.begin(); it != ceilSprite.end(); it++) {
		sf::FloatRect bound = it->getGlobalBounds();
		if (const std::optional intersection = bound.findIntersection(boundBox)) {
			//if collision ,it can be pick up and be erase
			ceilSprite.erase(it);
			return true;
		}
	}
	return false;
}

//*****Monster class*****
Monster::Monster(const std::filesystem::path& tileset):offset(15) {
	if (!ceilTexture.loadFromFile(tileset)) {
		exit(1);
	}
}
void Monster::draw(sf::RenderWindow& window) const {
	for (auto& cs : ceilSprite) {
		window.draw(cs);
	}
}
void Monster::create() {
	ceilSprite.clear();
	for (int i = 0; i < NUMMONSTER; i++) {
		sf::Sprite sprite{ ceilTexture };
		sprite.setPosition({ static_cast<float>((i * 5 + offset)) * CEILSIZE,HEIGHT - 3 * CEILSIZE });
		ceilSprite.push_back(sprite);
	}
}
bool Monster::Collision(Role& role) {
	sf::FloatRect boundBox = role.getBoundBox();
	for (auto it = ceilSprite.begin(); it != ceilSprite.end(); it++) {
		sf::FloatRect bound = it->getGlobalBounds();
		if (const std::optional intersection = bound.findIntersection(boundBox)) {
			return true;
		}
	}
	return false;
}
//this is to let the monster can move automaticlly
void Monster::move(const sf::Time& elapsedTime) {
	for (auto& cs : ceilSprite) {
		cs.move(speed*elapsedTime.asSeconds());
	}
}