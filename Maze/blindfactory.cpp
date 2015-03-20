#include "blindfactory.h"
#include "blind.h"
#include "random.h"

BlindFactory::BlindFactory()
{
}

Controller* BlindFactory::createController() const
{
  return new Blind();
}

