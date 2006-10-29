
#include "seekerfactory.h"
#include "seeker.h"

SeekerFactory::SeekerFactory()
{ }

Controller* SeekerFactory::createController() const
{
   return new Seeker;
}

