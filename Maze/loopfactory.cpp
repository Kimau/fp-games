#include "loopfactory.h"
#include "loop.h"

LoopFactory::LoopFactory()
{ }

Controller* LoopFactory::createController() const
{
   return new Loop;
}

