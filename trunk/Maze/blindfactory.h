#ifndef __BLINDFACTORY_H__
#define __BLINDFACTORY_H__
#include "controllerfactory.h"

/* BlindFactory 
 *
 * RandomWalkFactory is a concrete factory derived from ControllerFactory. 
 */
class BlindFactory : public ControllerFactory
{
public:
  /*
   *
   * defaultDeviationTime - See Blind::Blind
   * productionVariation - This is the variation in the default deviation time for each monster produced.
   */
  BlindFactory();
  ~BlindFactory() {}

  /* createController
   *
   * deviationTime - Use this to overide the default deviationTime for this specific controller
   *                 production variation is not used if this is specified
   * Also see ControllerFactory::createController
   */
  Controller* createController() const;
};


#endif

