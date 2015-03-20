#include "item.h"
#include "game.h"
#include <algorithm>

Item::Item(uint x, uint y, uint z) : Pawn(x+0.5f,y+0.5f,z+0.5f)
{
  isMovable	  =	false;
	isPickup	  =	true;
	baseSpeed	  =	0.0f;
  health = 1;
}

void Item::anihilateMe()
{  
}

bool Item::biteHim()
{
  //todo: add to character's inventory
  health = 0; //remove item from level
  return true;
}

