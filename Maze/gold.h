#ifndef _GOLD_H__
#define _GOLD_H__

#include "item.h"

class Gold : public Item 
{  
protected:
  uint value;
public:
	Gold(uint x, uint y, uint z);
	virtual ~Gold();

  /* biteHim
   * 
   * In this case biteHim doesn't dammage the player but adds gold to the player's inventory
   */
  bool biteHim();
  
  /* draw
   */
  void draw();
};

#endif

