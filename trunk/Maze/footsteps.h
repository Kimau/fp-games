#ifndef FOOTSTEPS
#define FOOTSTEPS
#include "pointXYZ.h"
#include "vector.h"

class Footsteps  
{
public:
	Footsteps(pointXYZ);
	void takeStep(pointXYZ);
	virtual ~Footsteps();
	pointXYZ getStep(int element) const;
	int getMark() const;
	Vector<float,2> getMidpoint();
	Vector<float,2> getTangent();
private:
	Footsteps();
	pointXYZ prints[10];
	int marker;
};

#endif

