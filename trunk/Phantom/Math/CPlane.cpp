#include "CPlane.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	CONSTRUCTORS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==================================================================================
// Create "Blank" plane
//==================================================================================
CPlane::CPlane() : m_normal(CVector3(0,0,1)), m_point(CVector3(0,0,0))
{}
//==================================================================================
//	Create Plane from 4 floats : Ax + By + Cz + D = 0
//==================================================================================
CPlane::CPlane(float x, float y, float z, float d) : m_normal(CVector3(x,y,z))
{
	m_normal.Normalize();
	m_point = m_normal * -d;
}
//==================================================================================
//	Create Plane from Normal and orig
//==================================================================================
CPlane::CPlane(CVector3 _normal) : m_normal(_normal), m_point(CVector3(0,0,0))
{
}
//==================================================================================
//	Create Plane from Normal and point on Plane
//==================================================================================
CPlane::CPlane(CVector3 _normal, CVector3 _point) : m_normal(_normal), m_point(_point)
{
}
//==================================================================================
//	Create Plane from 3 Points
//==================================================================================
CPlane::CPlane(CVector3 _point1, CVector3 _point2, CVector3 _point3) : m_point(_point1)
{
	m_normal = (_point1 - _point2) * (_point2 - _point3);
	m_normal.Normalize();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	ACCESSORS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==================================================================================
//	Get Plane Distance from Orig
//==================================================================================
float CPlane::PlaneDistance() const
{
	return(- m_normal.Dot(m_point));
}
//==================================================================================
//	Distant between point and plane
//==================================================================================
float CPlane::DistanceFrom(CVector3 _point) const
{
	return(- m_normal.Dot(_point) + PlaneDistance());
}
//==================================================================================
// Returns if line intersects plane
//==================================================================================
bool CPlane::isIntersect(CVector3 _start, CVector3 _end) const
{
	return((DistanceFrom(_start) > 0) != (DistanceFrom(_end) > 0));
}
//==================================================================================
// Get point of Intersection
//==================================================================================
CVector3 CPlane::getIntersectPoint(CVector3 _start, CVector3 _dir) const
{
	_dir.Normalize();

	//----------------------------
	// Place both in same frame
	//----------------------------
	float _numer = m_point.Magnitude() +
		m_normal.x + _start.x +
		m_normal.y + _start.y +
		m_normal.z + _start.z;

	//----------------------------
	//	Get Angle
	//----------------------------
	float _denom = m_normal.Dot(_dir);

	//------------------------
	//	Line is axis of plane
	//------------------------
	if(_denom == 0.0f)
		return m_point;

	//---------------------------
	//	Get Distance to intersect
	//---------------------------
	float _dist = _numer / _denom;

	return (_start + (_dir * _dist));
}
