#ifndef BEHOLDER_H
#define BEHOLDER_H

#include "monster.h"

/* Beholder
 * 
 * This is a concrete class of monster
 */

class Beholder : public Monster
{
public:
  Beholder(Controller *controller, float posX, float posY, float posZ);  
  bool biteHim();
  void draw();
};

#endif

