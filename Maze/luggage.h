#ifndef LUGGAGE_H
#define LUGGAGE_H

/* Luggage
 * 
 * This is a concrete class of monster
 */

class Luggage : public Monster
{
public:
  Luggage(Controller *controller, float posX, float posY, float posZ);  
  void draw();
};

#endif

