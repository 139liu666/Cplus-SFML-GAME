#include "Map.h"
#include "Global.h"

Fin::Fin() :
finTexture("resources/fin.png"),
finSprite{finTexture} {
	finSprite.setPosition({0,HEIGHT-3*CEILSIZE});
}
void Fin::draw(sf::RenderWindow& window) const {
	window.draw(finSprite);
}
void Fin::Collision(Role &role) const{
	sf::FloatRect boundBox = role.getBoundBox();
	sf::FloatRect finBound = finSprite.getGlobalBounds();
	if (const std::optional intersection = finBound.findIntersection(boundBox)) {
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
		sprite.setPosition({ static_cast<float>((i * 5 + offset)) * CEILSIZE,HEIGHT - 2 * CEILSIZE });
		ceilSprite.push_back(sprite);
	}
}
void Monster::Collision(Role& role) {
	sf::FloatRect boundBox = role.getBoundBox();
	for (auto it = ceilSprite.begin(); it != ceilSprite.end(); it++) {
		sf::FloatRect bound = it->getGlobalBounds();
		if (const std::optional intersection = bound.findIntersection(boundBox)) {
			role.restart();
		}
	}
}
void Monster::move(const sf::Time& elapsedTime) {
	for (auto& cs : ceilSprite) {
		cs.move(speed*elapsedTime.asSeconds());
	}
}