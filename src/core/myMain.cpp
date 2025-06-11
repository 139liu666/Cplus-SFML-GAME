#include <iostream>
#include "myMain.h"
#include "Game.h"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

using namespace std;
int myMain()
{
    Game game;
    game.run();
    return 0;
}