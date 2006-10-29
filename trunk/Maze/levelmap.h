#ifndef __LEVELMAP_H__
#define __LEVELMAP_H__

#include "maze.h"
#include "types.h"

/* LevelMap 
 *
 * Connects the level with it's maze
 */
class LevelMap
{
public:  
  LevelMap(uint8 sizeX, uint8 sizeY, uint8 sizeZ, int bigroom, int medroom, int smlroom);
  ~LevelMap();
  void draw();
  Maze* getMaze() const {return maze;}

protected:
  static Maze* maze;

  //Resources
  uint wallTextures[6];
  uint indestructableWallTexture;
  uint doorTexture;
  uint floorTextures[4];
};

#endif

