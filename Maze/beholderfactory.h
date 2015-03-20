#ifndef __BEHOLDERFACTORY_H__
#define __BEHOLDERFACTORY_H__

#include "monsterfactory.h"
#include "beholder.h"

/* BeholderFactory 
 *
 * BeholderFactory is a concrete factory derived from MonsterFactory.
 * TODO: Perhaps make BeholderFactory into a singleton? 
 */
class BeholderFactory : public MonsterFactory
{
  friend class Beholder;
protected:
  static uint texture;
public:
  BeholderFactory(ControllerFactory* controllerFactory);
  ~BeholderFactory();

  /* createMonster
   *
   * See MonsterFactory::createMonster
   */
  Monster* createMonster(float posX, float posY, float posZ) const;
  bool reload();
  void unload();
};

#endif

