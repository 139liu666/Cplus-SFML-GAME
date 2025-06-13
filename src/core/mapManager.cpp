/*
* This code is to create and operate the element of the map
* Ground,step,platform;these are elements that can be interacted with,such as collision detection
* Cloud,tree;these re elements that just background of the scene
*/
#include<iostream>
#include "mapManager.h"
#include "Global.h"


Map::Map(const std::filesystem::path& tileset)
{
    //This is for loading textures.
	if (!ceilTexture.loadFromFile(tileset)) {
		exit(1);
	}
}
void Map::drawMap(sf::RenderWindow& window) const{
	for (auto &cs:ceilSprite) {
		window.draw(cs);
	}
}
/*
* This function creates map elements and determines the position of each texture.
* All textures form an vector.
*/
void Map:: createMap() {
    int tileId;
    for (size_t row = 0; row < mapData.size(); ++row)
    {
        for (size_t col = 0; col < mapData[row].size(); ++col)
        {
            size_t height = mapData.size();
            tileId = mapData[row][col];
            if (tileId == 0)
            {
                continue;
            }
            sf::Sprite sprite{ ceilTexture };

            sprite.setPosition({ static_cast<float>(col) * CEILSIZE, static_cast<float>(row) * CEILSIZE+HEIGHT- static_cast<float>(height)*CEILSIZE });
            ceilSprite.push_back(sprite);
        }
      
    }
}
//this is to update the sprite of the map accroding to the level
void Map:: updateMap(int level) {
    ceilSprite.clear();
    if (level == 1) {
        if (!ceilTexture.loadFromFile("resources/sade.jpg")) {
            exit(1);
        }
    }
    if (level == 2) {
        if (!ceilTexture.loadFromFile("resources/break.jpg")) {
            exit(1);
        }
    }
    if (level == 3) {
        if (!ceilTexture.loadFromFile("resources/ground.jpg")) {
            exit(1);
        }
    }
}
//this is to load the data of the map by myself
void Map:: initMap() {
    mapData.clear();
    for (int i = 0; i < 2; ++i) {
        std::vector<int> row;
        for (int j = 0; j < MAPWIDTH; ++j) {
            if (j % 10 == 0&&j!=0) {
                row.push_back(0);
            }
            else {
                row.push_back(1);
            }
        }
        mapData.push_back(row);
    }
}
/*
* This is the core code that determines whether each sprite intersects with the character's bounding box.
* Detection on the ground is relatively simple, but detection of subclasses is more complex and will be rewritten later.
*/
unsigned char Map::checkCollision(sf::FloatRect boundBox) const{
    sf::FloatRect boundRole = boundBox;
    for (const auto& ceil : ceilSprite) {
        sf::FloatRect bound = ceil.getGlobalBounds();
        if (const std::optional intersection = bound.findIntersection(boundRole))
        {
            return 1;
        }
    }
    return 0;
}

//*****Step class*****
Step::Step(const std::filesystem::path& tileset){
    //This part will randomly determine the position of obstacles.
    static std::random_device dc;
    static std::default_random_engine gen(dc());
    std::uniform_int_distribution<int> distri_pos(10, MAPWIDTH-10);
    if (!ceilTexture.loadFromFile(tileset)) {
        exit(1);
    }
    offset = distri_pos(gen);
}
void Step::drawMap(sf::RenderWindow& window) const {
    for (auto& cs : ceilSprite) {
        window.draw(cs);
    }
}
void Step::createMap() {
    ceilSprite.clear();
    int tileId;
    for (size_t row = 0; row < mapData.size(); ++row)
    {
        for (size_t col = 0; col < mapData[row].size(); ++col)
        {
            size_t height = mapData.size();
            tileId = mapData[row][col];
            if (tileId == 0)
            {
                continue;
            }
            sf::Sprite sprite{ ceilTexture };

            sprite.setPosition({ static_cast<float>(col+offset) * CEILSIZE, static_cast<float>(row) * CEILSIZE + HEIGHT - static_cast<float>(height) * CEILSIZE -2*CEILSIZE});
            ceilSprite.push_back(sprite);
        }

    }
}
void Step::initMap() {
    mapData = {
    { 0, 0, 1, 1},
    { 0, 1, 1, 1},
    { 1, 1, 1, 1}
    };
}
/*
* This part is more precise collision detection, using char type as the carrier of the bitmask
* The up, down, left, and right directions correspond to one bit each. A char has 8 bits, but we only use the first 4 bits. 
* Based on the detection results, we perform an OR operation, and during subsequent detections, we perform an AND operation.
*/
unsigned char Step::checkCollision(sf::FloatRect boundBox) const {
    //use bitmask to check which part is contacted,only use 4 bit to store the collision state
    unsigned char flag = 0;
    float roleLeft = boundBox.position.x;
    float roleRight = boundBox.position.x + boundBox.size.x;
    float roleTop = boundBox.position.y;
    float roleBottom = boundBox.position.y + boundBox.size.y;
    float ceilLeft;
    float ceilRight;
    float ceilTop;
    float ceilBottom;
    for (const auto& ceil : ceilSprite) {
        sf::FloatRect bound = ceil.getGlobalBounds();
        if (const std::optional intersection = bound.findIntersection(boundBox)) {

            ceilLeft = bound.position.x;
            ceilRight = bound.position.x + bound.size.x;
            ceilTop = bound.position.y;
            ceilBottom = bound.position.y + bound.size.y;
            if (ceilLeft < roleRight && roleLeft<ceilLeft && roleTop > ceilTop) {
                flag |= 1; //right contact: 0 0 0 1
            }
            else if (roleLeft < ceilRight && roleRight>ceilRight && roleTop > ceilTop) {
                flag |= 2; //left contact: 0 0 1 0
            }
            if (roleBottom > ceilTop && roleTop < ceilTop) {
                flag |= 4; //bottom contact: 0 1 0 0
            }
        }
    }
    return flag;
}
int Step::getOffset() {
    return offset;
}

//*****PlatForm class*****
//This part is very similar to the previous one.
PlatForm::PlatForm(const std::filesystem::path& tileset){
    static std::random_device dc;
    static std::default_random_engine gen(dc());
    std::uniform_int_distribution<int> distri_pos(10, MAPWIDTH - 15);
    if (!ceilTexture.loadFromFile(tileset)) {
        exit(1);
    }
    offset = distri_pos(gen);
}
void PlatForm::drawMap(sf::RenderWindow& window) const {
    for (auto& cs : ceilSprite) {
        window.draw(cs);
    }
}
void PlatForm::createMap() {
    ceilSprite.clear();
    int tileId;
    for (size_t row = 0; row < mapData.size(); ++row)
    {
        for (size_t col = 0; col < mapData[row].size(); ++col)
        {
            size_t height = mapData.size();
            tileId = mapData[row][col];
            if (tileId == 0)
            {
                continue;
            }
            sf::Sprite sprite{ ceilTexture };

            sprite.setPosition({ static_cast<float>(col + offset) * CEILSIZE, static_cast<float>(row) * CEILSIZE + HEIGHT - static_cast<float>(height) * CEILSIZE - 6 * CEILSIZE });
            ceilSprite.push_back(sprite);
        }

    }
}
void PlatForm::initMap() {
    mapData = {
    { 1, 1, 1, 1,1}
    };
}
unsigned char PlatForm::checkCollision(sf::FloatRect boundBox) const {
    //use bitmask to check which part is contacted,only use 4 bit to store the collision state
    unsigned char flag = 0;
    float roleLeft = boundBox.position.x;
    float roleRight = boundBox.position.x + boundBox.size.x;
    float roleTop = boundBox.position.y;
    float roleBottom = boundBox.position.y + boundBox.size.y;
    float ceilLeft;
    float ceilRight;
    float ceilTop;
    float ceilBottom;
    for (const auto& ceil : ceilSprite) {
        sf::FloatRect bound = ceil.getGlobalBounds();
        if (const std::optional intersection = bound.findIntersection(boundBox)) {

            ceilLeft = bound.position.x;
            ceilRight = bound.position.x + bound.size.x;
            ceilTop = bound.position.y;
            ceilBottom = bound.position.y + bound.size.y;
            if (ceilLeft < roleRight && roleLeft<ceilLeft && roleTop > ceilTop) {
                flag |= 1; //right contact: 0 0 0 1
            }
            else if (roleLeft < ceilRight && roleRight>ceilRight && roleTop > ceilTop) {
                flag |= 2; //left contact: 0 0 1 0
            }
            if (roleBottom > ceilTop && roleTop < ceilTop) {
                flag |= 4; //bottom contact: 0 1 0 0
            }
        }
    }
    return flag;
}
int PlatForm::getOffset() {
    return offset;
}

//*****Cloud class*****
Cloud::Cloud(const std::filesystem::path& tileset):offset(3){
    if (!ceilTexture.loadFromFile(tileset)) {
        exit(1);
    }
}
void Cloud::drawMap(sf::RenderWindow& window) const {
    for (auto& cs : ceilSprite) {
        window.draw(cs);
    }
}
void Cloud::createMap() {
    ceilSprite.clear();
    for (int i = 0; i < 5; i++) {
        sf::Sprite sprite{ ceilTexture };
        sprite.setPosition({ static_cast<float>((i * 10 + offset)) * CEILSIZE,3 * CEILSIZE });
        ceilSprite.push_back(sprite);
    }
}
void Cloud::initMap() {
    //nothing to do
}

//*****Tree class*****
Tree::Tree(const std::filesystem::path& tileset) :offset(5) {
    if (!ceilTexture.loadFromFile(tileset)) {
        exit(1);
    }
}
void Tree::drawMap(sf::RenderWindow& window) const {
    for (auto& cs : ceilSprite) {
        window.draw(cs);
    }
}
void Tree::createMap() {
    ceilSprite.clear();
    for (int i = 0; i < 5; i++) {
        sf::Sprite sprite{ ceilTexture };
        sprite.setPosition({ static_cast<float>((i * 10 + offset)) * CEILSIZE,HEIGHT - 2 * CEILSIZE-sprite.getGlobalBounds().size.y});
        ceilSprite.push_back(sprite);
    }
}
void Tree::initMap() {
    //nothing to do
}
