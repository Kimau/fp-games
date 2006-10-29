#ifndef MY_PLANE_FOR_DIM_MATH_324
#define MY_PLANE_FOR_DIM_MATH_324

#include "CVector3.h"

// Ax + By + Cz + D = 0
class CPlane
{
public:
	//------------------------------------
	//	Constructors
	//------------------------------------
	CPlane();
	CPlane(float x, float y, float z, float d);
	CPlane(CVector3 _normal);
	CPlane(CVector3 _normal, CVector3 _point);
	CPlane(CVector3 _point1, CVector3 _point2, CVector3 _point3);
	//------------------------------------
	//	Manipulators
	//------------------------------------

	//------------------------------------
	//	Accessors
	//------------------------------------
	float PlaneDistance() const;							// Plane Distance from Origon
	float DistanceFrom(CVector3 _point) const;				// Distant between point and plane
	bool isIntersect(CVector3 _start, CVector3 _end) const;	// Returns if line intersects plane
	CVector3 getIntersectPoint(CVector3 _start, CVector3 _dir) const;		// Get point of Intersection

	//------------------------------------
	//	Varibles
	//------------------------------------
	CVector3 m_normal;
	CVector3 m_point;
};

#endif