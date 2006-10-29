#ifndef __SEEKERFACTORY_H__
#define __SEEKERFACTORY_H__

#include "types.h"
#include "controllerfactory.h"
#include "commongl.h"


/* SeekerFactory
 *
 * RandomWalkFactory is a concrete factory derived from ControllerFactory. 
 */
class SeekerFactory: public ControllerFactory
{
public:
  /* constructor
   */
  SeekerFactory();
  ~SeekerFactory() {}

  /* createController
   *
   * Also see ControllerFactory::createController
   */
  Controller* createController() const;

};


#endif

