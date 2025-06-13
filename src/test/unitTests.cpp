/*
* This unit test covers testing of character movement, map generation, collision detection, and main game logic.
*/
#include <gtest/gtest.h>
#include "Game.h"
#include "Role.h"
#include "mapManager.h"
#include "Map.h"
#include "Global.h"

//Role Test
TEST(RoleTest, Initialization) {
    Role role;
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().x, 0.f);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().y, 0.f);
    EXPECT_FALSE(role.getState());
    EXPECT_GT(role.getBoundBox().size.x, 0.f);
    EXPECT_GT(role.getBoundBox().size.y, 0.f);
}

TEST(RoleTest, MovementControls) {
    Role role;
    sf::Time elapsedTime = sf::seconds(0.1f);

    role.update(elapsedTime, 0, 0); 
    float initialX = role.getSprite().getPosition().x;
    EXPECT_FLOAT_EQ(initialX, 0.f);
}

TEST(RoleTest, GravityEffect) {
    Role role;
    role.getSprite().setPosition({ 100, 100 });
    float initialY = role.getSprite().getPosition().y;

    role.update(sf::seconds(100.0f), 0, 0);
    EXPECT_GT(role.getSprite().getPosition().y, initialY);
}


TEST(RoleTest, BoundaryConstraints) {
    Role role;
    role.getSprite().setPosition({ - 10, -10});
    role.update(sf::seconds(0.1f), 1, 0);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().x, 0.f);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().y, 0.f);

    role.getSprite().setPosition({ MAPWIDTH * CEILSIZE + 100, 0 });
    role.update(sf::seconds(0.1f), 0, 0);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().x, MAPWIDTH * CEILSIZE - role.getBoundBox().size.x);
}

TEST(RoleTest, StateManagement) {
    Role role;
    role.setState(true);
    EXPECT_TRUE(role.getState());
    role.setState(false);
    EXPECT_FALSE(role.getState());
}

TEST(RoleTest, RestartFunction) {
    Role role;
    role.getSprite().setPosition({ 100, 200 });
    role.restart();
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().x, 0.f);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().y, 0.f);
}

//Map Test
TEST(MapTest, CreationAndCollision) {
    Map map("resources/break.jpg");
    map.initMap();
    map.createMap();

    sf::FloatRect playerRectAbove({ 10, HEIGHT - 3 * CEILSIZE }, { 16, 16 });
    unsigned char collision = map.checkCollision(playerRectAbove);
    EXPECT_FALSE(collision);

    sf::FloatRect playerRectOnGround({ 10, HEIGHT - 2 * CEILSIZE }, { 16, 16 });
    collision = map.checkCollision(playerRectOnGround);
    EXPECT_TRUE(collision);
}

TEST(StepTest, CreationAndCollision) {
    Step step("resources/break.jpg");
    step.initMap();
    step.createMap();

    sf::FloatRect playerRect({ step.getOffset() * CEILSIZE + 30, HEIGHT - 5 * CEILSIZE }, { 20, 40 });
    unsigned char collision = step.checkCollision(playerRect);
    EXPECT_TRUE(collision & 4);
}

TEST(PlatFormTest, CreationAndCollision) {
    PlatForm platform("resources/break.jpg");
    platform.initMap();
    platform.createMap();

    sf::FloatRect playerRect({ platform.getOffset() * CEILSIZE , HEIGHT - 8 * CEILSIZE }, { 20, 40 });
    unsigned char collision = platform.checkCollision(playerRect);
    EXPECT_TRUE(collision & 4);
}

TEST(CloudTest, Creation) {
    Cloud cloud("resources/Cloud.png");
    cloud.createMap();

    sf::FloatRect playerRect({ 10, 2 * CEILSIZE }, { 16, 16 });
    unsigned char collision = cloud.checkCollision(playerRect);
    EXPECT_FALSE(collision);
}

TEST(TreeTest, Creation) {
    Tree tree("resources/tree.png");
    tree.createMap();

    sf::FloatRect playerRect({ 10, HEIGHT - 3 * CEILSIZE }, { 16, 16 });
    unsigned char collision = tree.checkCollision(playerRect);
    EXPECT_FALSE(collision);
}

TEST(FinTest, CollisionDetection) {
    Fin fin;
    Role role;

    role.getSprite().setPosition({ 100, 100 });
    fin.Collision(role);
    EXPECT_FALSE(role.getState());

    role.getSprite().setPosition({ MAPWIDTH * CEILSIZE - CEILSIZE, HEIGHT - 3 * CEILSIZE });
    fin.Collision(role);
    EXPECT_TRUE(role.getState());
}

TEST(ToolTest, Collection) {
    Tool tool;
    Role role;
    tool.create();

    role.getSprite().setPosition({ 0, 0 });
    bool collected = tool.Collision(role);
    EXPECT_FALSE(collected);

    role.getSprite().setPosition({ 10 * CEILSIZE, HEIGHT - 8 * CEILSIZE });
    collected = tool.Collision(role);
    EXPECT_TRUE(collected);

    role.getSprite().setPosition({ 8 * CEILSIZE, HEIGHT - 3 * CEILSIZE });
    collected = tool.Collision(role);
    EXPECT_TRUE(collected);
}

TEST(MonsterTest, PlayerReset) {
    Monster monster("resources/bee.png");
    monster.create();
    Role role;

    role.getSprite().setPosition({ 0, 0 });
    bool collided = monster.Collision(role);
    EXPECT_FALSE(collided);

    role.getSprite().setPosition({ 15 * CEILSIZE, HEIGHT - 3 * CEILSIZE });
    collided = monster.Collision(role);
    EXPECT_TRUE(collided);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().x, 0);
    EXPECT_FLOAT_EQ(role.getSprite().getPosition().y, 0);
}