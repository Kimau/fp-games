#ifndef __BUNNYFACTORY_H__
#define __BUNNYFACTORY_H__

#include "monsterfactory.h"
#include "bunny.h"

/* BunnyFactory 
 *
 * BunnyFactory is a concrete factory derived from MonsterFactory.
 * TODO: Perhaps make BunnyFactory into a singleton? 
 */
class BunnyFactory : public MonsterFactory
{
  friend class Bunny;
protected:
  static uint texture;
public:
  BunnyFactory(ControllerFactory* controllerFactory);
  ~BunnyFactory();

  /* createMonster
   *
   * See MonsterFactory::createMonster
   */
  Monster* createMonster(float posX, float posY, float posZ) const;
  bool reload();
  void unload();
};

#endif

