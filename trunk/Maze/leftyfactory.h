#ifndef __LEFTYFACTORY_H__
#define __LEFTYFACTORY_H__

#include "types.h"
#include "controllerfactory.h"
#include "commongl.h"


/* LeftyFactory
 *
 * Follows the left hand Rule
 */
class LeftyFactory: public ControllerFactory
{
public:
  /* constructor
   */
  LeftyFactory();
  ~LeftyFactory() {}

  /* createController
   *
   * Also see ControllerFactory::createController
   */
  Controller* createController() const;

};


#endif

