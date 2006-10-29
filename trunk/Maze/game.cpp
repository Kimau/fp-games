#include <time.h>
#include "game.h"
#include "main.h"
#include "types.h"
#include "commongl.h"	

#include "gold.h"
#include "player.h"

//monster factories
#include "bunnyfactory.h"
#include "ghostfactory.h"
#include "beholderfactory.h"

//item factories
#include "goldfactory.h"

//controller factories
#include "seekerfactory.h"
#include "blindfactory.h"
#include "leftyfactory.h"

Level* Game::tmpLevel;

bool Game::init()
{
  tmpLevel = new Level(20,20,1,5,5,5);
  tmpLevel->addMonsterFactory(new BunnyFactory(new LeftyFactory));
  tmpLevel->addMonsterFactory(new BeholderFactory(new SeekerFactory));
  tmpLevel->addMonsterFactory(new GhostFactory(new BlindFactory));
  tmpLevel->addItemFactory(new GoldFactory());
  tmpLevel->reload();
  tmpLevel->generateRandomMonsters(15);
  tmpLevel->generateRandomItems(5);
  
  //start the player's controller
  Player::getInstance()->update(getTime());

  return true;
}

void Game::run()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  tmpLevel->performLogic(); //game logic
  tmpLevel->draw();  
}
bool Game::reload()
{
  bool r = tmpLevel->reload();
  r &= Player::getInstance()->reload();  
	return r;
}
void Game::unload()
{
  Player::getInstance()->unload();  
  tmpLevel->unload();
}


Level* Game::getLevel()
{
	return tmpLevel;
}

void Game::quit()
{
  ::quit(0);
}


