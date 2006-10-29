#ifndef __MONSTER_H__
#define __MONSTER_H__
//#pragma once

#include "pawn.h"


using namespace std;

/* 
 * Monster is an abstract product for the abstract factory design pattern 
 *
 * Todo:
 *  Move controller to Pawn
 */
class Monster : public Pawn
{
public:  
  /* constructor
   *
   * Also see setController
   */
  Monster(Controller* controller, float posX, float posY, float posZ);

  /* destructor
   */
  virtual ~Monster();

  /* anihilateMe
   *
   * Kill the monster and remove it from the pawn list in the level
   */
  virtual void anihilateMe();
    
};


#endif

