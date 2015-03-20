#include "monster.h"
#include "monsterfactory.h"
#include "level.h"
#include "game.h"
#include <algorithm>
using namespace std;

void Monster::anihilateMe()
{  
  
}

Monster::Monster(Controller* controller, float posX, float posY, float posZ) : Pawn(posX, posY, posZ)
{
  setController(controller);
}

Monster::~Monster() 
{  
}

