#include "CVector3.h"

//####################################################################################################
//####################################################################################################
//					CVector
//####################################################################################################
//####################################################################################################
// A default constructor
CVector3::CVector3()
{
	CVector3(0,0,0);
}

// This is our constructor that allows us to initialize our data upon creating an instance
CVector3::CVector3(double _X, double _Y, double _Z)
{
	x = _X; y = _Y; z = _Z;
}

// Here we overload the + operator so we can add vectors together 
CVector3 CVector3::operator+(CVector3 _vector)
{
	// Return the added vectors result.
	return CVector3(_vector.x + x, _vector.y + y, _vector.z + z);
}

// Here we overload the - operator so we can subtract vectors 
CVector3 CVector3::operator-(CVector3 _vector)
{
	// Return the subtracted vectors result
	return CVector3(x - _vector.x, y - _vector.y, z - _vector.z);
}

// Here we overload the * operator so we can multiply by scalars
CVector3 CVector3::operator*(double _num)
{
	// Return the scaled vector
	return CVector3(x * _num, y * _num, z * _num);
}

// Here we overload the / operator so we can divide by a scalar
CVector3 CVector3::operator/(double _num)
{
	// Return the scale vector
	return CVector3(x / _num, y / _num, z / _num);
}

// Divide vector by vector and return double
double CVector3::operator/(CVector3 _vector)
{
	return (Magnitude() / _vector.Magnitude());
}

// This returns the cross product between 2 vectors
CVector3 CVector3::operator*(CVector3 _vector)
{
	CVector3 vCross;							// The vector to hold the cross product
												// Get the X value
	vCross.x = ((y * _vector.z) - (z * _vector.y));
												// Get the Y value
	vCross.y = ((z * _vector.x) - (x * _vector.z));
												// Get the Z value
	vCross.z = ((x * _vector.y) - (y * _vector.x));

	return vCross;								// Return the cross product
}

// Carefull of this function as double comparison is not precise and sometimes flips out
bool CVector3::operator==(CVector3 _vector) const
{
	return((x == _vector.x) && (y == _vector.y) && (z == _vector.z));
}

// Carefull of this function as double comparison is not precise and sometimes flips out
bool CVector3::operator!=(CVector3 _vector) const
{
	return((x != _vector.x) || (y != _vector.y) || (z != _vector.z));
}

// This returns the normal of a vector
CVector3 CVector3::getNormalized() const
{
	// Get the magnitude
	double _Magnitude = Magnitude();

	// Return the normal
	CVector3 _normalized(x / (double)_Magnitude,y / (double)_Magnitude,z / (double)_Magnitude);
	return _normalized;
}

// This normalizes the vector
void CVector3::Normalize()
{
	// Get the magnitude
	double _Magnitude = Magnitude();

	// Return the normal
	x = x / _Magnitude;
	y = y / _Magnitude;
	z = z / _Magnitude;
}

double CVector3::Magnitude() const
{
	return (sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
}

void CVector3::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}

double CVector3::Dot(CVector3 _vector) const
{
	return ((x * _vector.x) + (y * _vector.y) + (z * _vector.z));
}

float CVector3::Angle(CVector3 _vector) const
{
	// Get the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
	// (IE.   PI/2 radians = 90 degrees      PI radians = 180 degrees    2*PI radians = 360 degrees)
	// To convert radians to degress use this equation:   radians * (PI / 180)
	// TO convert degrees to radians use this equation:   degrees * (180 / PI)
	float _angle = acos((float)(Dot(_vector) / (Magnitude() * _vector.Magnitude())));

	// Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate.
	// acos() thinks it's funny when it returns -1.#IND0000000.  If we don't do this check,
	// our collision results will sometimes say we are colliding when we aren't.  I found this
	// out the hard way after MANY hours and already wrong written tutorials :)  Usually
	// this value is found when the dot product and the maginitude are the same value.
	// We want to return 0 when this happens.
	if(_isnan(_angle))
		return 0;

	// Return the angle in radians
	return(_angle);
}
//####################################################################################################
//####################################################################################################
//					Plane
//####################################################################################################
//####################################################################################################
CPlane::CPlane(double _a, double _b, double _c, double _d) : a(_a), b(_b), c(_c), d(_d)
{}

CPlane::CPlane(CVector3 _normal, double _d) : a(_normal.x), b(_normal.y), c(_normal.z), d(_d)
{}

CPlane::CPlane(CVector3 _normal, CVector3 _pop) : a(_normal.x), b(_normal.y), c(_normal.z)
{
	d = 0 - ((a * _pop.x) + (b * _pop.y) + (c * _pop.z));
}

CVector3 CPlane::Normal() const
{
	return CVector3(a,b,c);
}

double CPlane::DistanceFrom(CVector3 _point) const
{
	return(
		(a * _point.x)+
		(b * _point.y)+
		(c * _point.z)+ d);
}

CVector3 CPlane::Intersect(CVector3 _start, CVector3 _dir) const
{
	//-------------------------------------
	// Get Direction and Normalize it
	//-------------------------------------
	_dir.Normalize();

	//-------------------------------------
	// Numerator is Distance from plane
	//-------------------------------------
	double _Top = 0 - DistanceFrom(_start);

	//-------------------------------------
	// Get the cosine of the angle between
	// the 2 vectors. Simple math because
	// length is 1.
	//-------------------------------------
	double _Bottom = Normal().Dot(_dir);

	//-------------------------------------
	// If _Denominator is 0
	// Then Line is on plane
	// Return ARB point
	//-------------------------------------
	if( _Bottom == 0.0)
		return _start;

	// We divide the (distance from the point to the plane) by (the dot product)
	// to get the distance (dist) that we need to move from our arbitrary point.  We need
	// to then times this distance (dist) by our line's vector (direction).  When you times
	// a scalar (single number) by a vector you move along that vector.  That is what we are
	// doing.  We are moving from our arbitrary point we chose from the line BACK to the plane
	// along the lines vector.  It seems logical to just get the numerator, which is the distance
	// from the point to the line, and then just move back that much along the line's vector.
	// Well, the distance from the plane means the SHORTEST distance.  What about in the case that
	// the line is almost parallel with the polygon, but doesn't actually intersect it until half
	// way down the line's length.  The distance from the plane is short, but the distance from
	// the actual intersection point is pretty long.  If we divide the distance by the dot product
	// of our line vector and the normal of the plane, we get the correct length.  Cool huh?
	double _dist = _Top / _Bottom;	// Divide to get the multiplying (percentage) factor

	return (_start + (_dir*_dist));								// Return the intersection point
}