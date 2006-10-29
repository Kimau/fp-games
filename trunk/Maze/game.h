#ifndef __GAME_H__
#define __GAME_H__

class Level;
class Game
{
protected:
	static Level *tmpLevel;
public:
	static bool init();
	static void run();
	static bool reload();
	static void unload();
	static Level* getLevel();
  static void quit();
};

#include "level.h"

#endif

