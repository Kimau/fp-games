#ifndef __ACTOR_H__
#define __ACTOR_H__
//#pragma once

#include "types.h"

class Actor
{
protected:
  struct
  {
    float x, y, z;
  }position;
public:
  Actor(float posX, float posY, float posZ = 0.0f)
  {
    setPosition(posX,posY,posZ);
  }

  /*int getGrid() const;
  getGraphic() const;
  setGrid()
  setGraphic*/
  

  void getPosition(float& x, float& y)
  {
    x = position.x;
    y = position.y;
  }
  void getPosition(float& x, float& y, float& z)
  {
    x = position.x;
    y = position.y;
    z = position.z;
  }
  void setPosition(float x, float y)
  {
    position.x = x;
    position.y = y;
  }
  void setPosition(float x, float y, float z)
  {
    position.x = x;
    position.y = y;
    position.z = z;
  }
};

#endif
