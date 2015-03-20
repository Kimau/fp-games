#ifndef __ITEM_H__
#define __ITEM_H__

#include "pawn.h"

class Item : public Pawn
{ 
public:
  Item(uint x, uint y, uint z);
  virtual void anihilateMe();
  bool biteHim();
};

#endif 


