// This part is to create a role in the game
// This character is the one I'm controlling to break in.
#pragma once
#include<vector>
#include <SFML/Graphics.hpp>

class Role {
public:
	Role();
	void draw(sf::RenderWindow& window) const;
	void update(const sf::Time& elapsedTime, unsigned char isGround, unsigned char isContact);
	sf::Sprite getSprite() const;
	sf::FloatRect getBoundBox() const;
	bool getState() const;
	void setState(bool state);
	void restart();

private:
	float x;
	float y;
	float height;
	float width;
	float Hspeed=100.f;
	float Vspeed=3500.f;
	bool isEnd;
	sf::Texture role;
	sf::Sprite roleSprite;
};