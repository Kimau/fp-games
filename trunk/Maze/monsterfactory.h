#ifndef __MONSTERFACTORY_H__
#define __MONSTERFACTORY_H__
//#pragma once

#include "types.h"
#include "commongl.h"
#include "monster.h"
#include "controllerfactory.h"


/* MonsterFactory 
 *
 * MonsterFactory is an abstract factory that creates objects derived from
 * Monster (which is the abstract product)
 *
 * Todo:
 *  Might implement functionality that allows the caller to specify some other ControllerFactory
 *  that overrides the default ControllerFactory in createMonster in future
 *
 * Also see OrcFactory, OgreFactory etc, for examples of concrete factory derived from this class
 */
class MonsterFactory
{
protected:
  ControllerFactory* controllerFactory;
public:

  /* constructor
   *
   * controllerFactory - The default controller factory to use when creating monsters. Note that
   *                     once a controller factory is assigned to a monster factory, it should not
   *                     be deleted or given to another factory since the monster factory manages
   *                     the controller factory itself
   */
  MonsterFactory(ControllerFactory* controllerFactory):controllerFactory(controllerFactory) {}
  
  /* destructor
   */
  ~MonsterFactory() { delete controllerFactory;}

  /* createMonster
   *
   * This is the factory method of Monster Factory 
   * posX, posY, posZ : Initial coordinates on map  - this must be on an open space on the map. 
   *                    Note that this point is located at the bottom-center point of the monster's sprite.
   */
  virtual Monster* createMonster(float posX, float posY, float posZ) const = 0;
  
  /* reload
   *
   * Reload factory (and monster) resources.
   */
  virtual bool reload() = 0;
  
  /* unload
   *
	 * Clean up all resources in use (i.e. delete textures etc.)
	 */
  virtual void unload() = 0;

  
};

#endif


