#ifndef GHOST_H
#define GHOST_H
#include "monster.h"

/* Ghost
 * 
 * This is a concrete class of monster
 */

class Ghost : public Monster
{
public:
  Ghost(Controller *controller, float posX, float posY, float posZ);  
  bool biteHim();
  void draw();
};

#endif

