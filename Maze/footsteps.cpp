#include "footsteps.h"

Footsteps::Footsteps(pointXYZ start_)
{
	for(int c = 0; c < 10; c++)
		prints[c] = start_;
	marker = 0;
}

Footsteps::Footsteps()
{

}

Footsteps::~Footsteps()
{
	
}

void Footsteps::takeStep(pointXYZ new_)
{
	// move marker
	marker = (marker++)%10;
	// replace oldest
	prints[marker] = new_;
}

pointXYZ Footsteps::getStep(int element) const
{
	if (element < 10)
	{
		return prints[element];
	}
	return prints[((marker+1)%10)];
}

int Footsteps::getMark() const
{
	return marker;
}

/*
	Used for camera calculation
	Get midpoint and tangent of a curve constructed from ten footsteps.
*/
Vector<float,2> Footsteps::getMidpoint()
{
  Vector<float,2> midPoint;  

  for(int c = 0; c < 10; c++)
	{
    midPoint[0] += prints[c].getX();
    midPoint[1] += prints[c].getY();
  }
  midPoint /= 10.0f;
  return midPoint;
}

Vector<float,2> Footsteps::getTangent()
{
	Vector<float,2> tang_(0,0);
	// Loop through footsteps and add the step
	for(int c = 0; c < 10; c++)
	{
		float percent_ = (float)(100 - (10 + marker - c)*10);
		int x_ = prints[c].getX()-prints[((c+1)%10)].getX();
		int y_ = prints[c].getY()-prints[((c+1)%10)].getY();
		Vector<float,2> v(x_*percent_,y_*percent_);
		tang_ += v;
	}
	tang_.setLength(1.0f);
	return tang_;
}


