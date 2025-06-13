/*
* This part is to create a role in the game
* This character is the one I'm controlling to break in.
*/
#include "Role.h"
#include "Global.h"

Role::Role() :
	x(0),
    y(0),
	isEnd(false),
	role("resources/role.png"),
	roleSprite(role){
	sf::FloatRect bound = roleSprite.getGlobalBounds();
	height = bound.size.y;
	width = bound.size.x;
}
void Role::draw(sf::RenderWindow& window) const{
	window.draw(roleSprite);
}
//Get private members inside role
sf::Sprite& Role::getSprite() {
	return roleSprite;
}
sf::FloatRect Role::getBoundBox() const{
	return roleSprite.getGlobalBounds();
}
bool Role::getState() const{
	return isEnd;
}
void Role::setState(bool state) {
	isEnd = state;
}
void Role::restart() {
	x = 0;
	y = 0;
	roleSprite.setPosition({ 0,0 });
}
//this part is to update character movement based on events
void Role::update(const sf::Time& elapsedTime, unsigned char isGround, unsigned char isContact) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)&&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && !(isContact & 2)) {
		x -= Hspeed * elapsedTime.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !(isContact & 1)) {
		x += Hspeed*elapsedTime.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) &&( isGround == 1||isContact & 4)) {
		y -= Vspeed * elapsedTime.asSeconds();
	}
	//Simulate gravity; if it touches the ground, it will not continue to fall.
	else if (!isGround && !(isContact & 4)) {
		y += GRAVITY * elapsedTime.asSeconds();
	}
	//Detection boundary
	if (x < 0 || y < 0) {
		x = 0.f;
		y = 0.f;
	}
	if (roleSprite.getPosition().x +width> MAPWIDTH*CEILSIZE) {
		x = MAPWIDTH * CEILSIZE -width;
	}
	roleSprite.setPosition({ x,y });
	x = roleSprite.getPosition().x;
	y = roleSprite.getPosition().y;
}