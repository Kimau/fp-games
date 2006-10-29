#ifndef BUNNY_H
#define BUNNY_H
#include "monster.h"


/* Bunny
 * 
 * This is a concrete class of monster
 */

class Bunny: public Monster
{
public:
  Bunny(Controller *controller, float posX, float posY, float posZ);  
  bool biteHim();
  void draw();
};

#endif

