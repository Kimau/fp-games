#ifndef __GHOSTFACTORY_H__
#define __GHOSTFACTORY_H__

#include "monsterfactory.h"
#include "ghost.h"

/* GhostFactory 
 *
 * GhostFactory is a concrete factory derived from MonsterFactory.
 * TODO: Perhaps make GhostFactory into a singleton? 
 */
class GhostFactory : public MonsterFactory
{
  friend class Ghost;
protected:
  static uint texture;
public:
  GhostFactory(ControllerFactory* controllerFactory);
  ~GhostFactory();

  /* createMonster
   *
   * See MonsterFactory::createMonster
   */
  Monster* createMonster(float posX, float posY, float posZ) const;
  bool reload();
  void unload();
};

#endif

