#ifndef BOOK_H
#define BOOK_H

#include "monster.h"

/* Book
 * 
 * This is a concrete class of monster
 */

class Book : public Monster
{
public:
  Book(Controller *controller, float posX, float posY, float posZ);  
  bool biteHim();
  void draw();
};

#endif

