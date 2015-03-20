#ifndef __LEVEL_H__
#define __LEVEL_H__
//#pragma once

#include <stdlib.h>
#include <memory.h>
#include <vector>
#include "commongl.h"
#include "types.h"
#include "monsterfactory.h"
#include "itemfactory.h"
#include "camera.h"
#include "levelmap.h"


using namespace std;

class CreatureFactory;
class Creature;
class Level
{
  /*union Cell
  {
    struct
    {
      unsigned passable  : 1; //bit 0
      unsigned wall      : 1; //bit 1
      unsigned door      : 1; //bit 2.. etc
      unsigned breakable : 1;
      unsigned dammage   : 1;
      unsigned type1     : 1;
      unsigned type2     : 1;
    };
    uint8 flags;
  };*/

protected:
  LevelMap map;
  vector<MonsterFactory*> monsterFactoryList;
  vector<ItemFactory*> itemFactoryList;
  vector<Pawn*> pawnList;

  //vector<WeaponFactory*> weaponFactoryList;
  //vector<weapon*> weaponList;

  //todo: camera
  Camera camera;
  Camera minimapCamera;
public:
  Level(uint8 sizeX, uint8 sizeY, uint8 sizeZ, int bigroom, int medroom, int smlroom);
  ~Level();

  void addMonsterFactory(MonsterFactory* monsterFactory);
  void addItemFactory(ItemFactory* itemFactory);
  void generateRandomMonsters(int num);
  void generateRandomItems(int num);
  vector<Pawn*>& getPawnList();
  void draw();
  void performLogic();

  /* reload
	*
	* Reload factory (and monster) resources. 
	* Called when level is first initialized or if the level's resources have been lost (e.g like when
	* the window gets iconified)
	*/
	bool reload();

	/* unload
   *
   * Unload all level (i.e. factory and monster's) resources
   */
	void unload();
};

#endif

