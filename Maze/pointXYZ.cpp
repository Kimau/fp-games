#include "pointXYZ.h"

pointXYZ::pointXYZ()
{
	x = 0;
	y = 0;
	z = 0;
}

pointXYZ::pointXYZ(int x_, int y_, int z_)
{
	x = x_;
	y = y_;
	z = z_;
}

int pointXYZ::getX() const
	{return x;}

int pointXYZ::getY() const
	{return y;}

int pointXYZ::getZ() const
	{return z;}

bool pointXYZ::operator==(const pointXYZ& other) const
{
  if((x == other.getX()) && (y == other.getY()) && (z == other.getZ()))
   return true;
  else
   return false;
}

