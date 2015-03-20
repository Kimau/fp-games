#ifndef __LOOPFACTORY_H__
#define __LOOPFACTORY_H__

#include "types.h"
#include "controllerfactory.h"
#include "commongl.h"



/* 
	Loop
	Goes until it hits something then turns right
	Ends up looping
 */
class LoopFactory: public ControllerFactory
{
public:
  /* constructor
   */
  LoopFactory();
  ~LoopFactory() {}

  /* createController
   *
   * Also see ControllerFactory::createController
   */
  Controller* createController() const;

};


#endif

