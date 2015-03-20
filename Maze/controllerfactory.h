#ifndef __CONTROLLERFACTORY_H__
#define __CONTROLLERFACTORY_H__
//#pragma once

//TODO: perhaps remove

#include "types.h"
#include "controller.h"
#include "commongl.h"

/* ControllerFactory
 *
 * ControllerFactoryis an abstract factory that creates objects derived from
 * Controller (which is the abstract product) 
 */
class ControllerFactory
{
public:

  /* createController
   *
   * This is the factory method of Controller Factory    
   */
  virtual Controller* createController() const = 0;
};


#endif

