#include "leftyfactory.h"
#include "lefty.h"

LeftyFactory::LeftyFactory()
{ }

Controller* LeftyFactory::createController() const
{
   return new Lefty;
}

