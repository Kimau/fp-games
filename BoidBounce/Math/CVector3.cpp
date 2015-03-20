#include "CVector3.h"
#include <math.h>
#include <float.h>


//####################################################################################################
//####################################################################################################
//		Constructors
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Default Constructor
//--------------------------------------------------------------------
CVector3::CVector3() : x(0), y(0), z(0)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
CVector3::CVector3(float _X, float _Y, float _Z) : x(_X), y(_Y), z(_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
CVector3::CVector3(double _X, double _Y, double _Z) : x((float)_X), y((float)_Y), z((float)_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
CVector3::CVector3(UINT _X, UINT _Y, UINT _Z) : x((float)_X), y((float)_Y), z((float)_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
CVector3::CVector3(UINT _X, UINT _Y, float _Z) : x((float)_X), y((float)_Y), z(_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
CVector3::CVector3(int _X, int _Y, int _Z) : x((float)_X), y((float)_Y), z((float)_Z)
{}
//--------------------------------------------------------------------
//	Copy Constructor
//--------------------------------------------------------------------
CVector3::CVector3(const CVector3& _vector)
{
	if(this != &_vector)
	{
		x = _vector.x;
		y = _vector.y;
		z = _vector.z;
	}
}

//####################################################################################################
//####################################################################################################
//		Operators
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Addition Operator
//--------------------------------------------------------------------
CVector3 CVector3::operator+(CVector3 _vector)
{
	return CVector3(
		(x+_vector.x),
		(y+_vector.y),
		(z+_vector.z));
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
CVector3 CVector3::operator-(CVector3 _vector)
{
	return CVector3(
		(x-_vector.x),
		(y-_vector.y),
		(z-_vector.z));
}
//--------------------------------------------------------------------
//	Cross Product of Two Vectors
//--------------------------------------------------------------------
CVector3 CVector3::operator*(CVector3 _vector)
{
	CVector3 _vCross;									// The vector to hold the cross product
	_vCross.x = ((y * _vector.z) - (z * _vector.y));	// Get the X value
	_vCross.y = ((z * _vector.x) - (x * _vector.z));	// Get the Y value												
	_vCross.z = ((x * _vector.y) - (y * _vector.x));	// Get the Z value

	return _vCross;										// Return the cross product
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
CVector3 CVector3::operator*(float _num)
{
	return CVector3(
		(x*_num),
		(y*_num),
		(z*_num));
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
CVector3 CVector3::operator/(float _num)
{
	return CVector3(
		(x/_num),
		(y/_num),
		(z/_num));
}

//--------------------------------------------------------------------
//	Returns the Maginitude of two vectors divided
//--------------------------------------------------------------------
float CVector3::operator/(CVector3 _vector)
{
	return ((Magnitude()) / (_vector.Magnitude()));
}
//--------------------------------------------------------------------
//	Check Magnitude
//	Due to the nature of floats it checks within a 5% tolerance
//--------------------------------------------------------------------
bool CVector3::operator==(float _mag) const
{
	float _result = Magnitude() / _mag;	
	return ((_result < 1.05f) || (_result > 0.95f));
}
//--------------------------------------------------------------------
//	Equal Comparison for Vectors
//	Due to the nature of floats it checks within a 5% tolerance
//--------------------------------------------------------------------
bool CVector3::operator==(CVector3 _vector) const
{
	float _result = ((x/_vector.x) + (y/_vector.y) + (z/_vector.z)) / 3.0f;
	return ((_result < 1.05f) || (_result > 0.95f));
}
//--------------------------------------------------------------------
//	Checks for inquality
//	Uses equality operator
//--------------------------------------------------------------------
bool CVector3::operator!=(CVector3 _vector) const
{
	return !((*this) == _vector);
}
//--------------------------------------------------------------------
//	Addition Operator
//--------------------------------------------------------------------
void CVector3::operator+=(CVector3 _vector)
{
	x += _vector.x;
	y += _vector.y;
	z += _vector.z;
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
void CVector3::operator-=(CVector3 _vector)
{
	x -= _vector.x;
	y -= _vector.y;
	z -= _vector.z;
}
//--------------------------------------------------------------------
//	Cross Product of Two Vectors
//--------------------------------------------------------------------
void CVector3::operator*=(CVector3 _vector)
{
	x = ((y * _vector.z) - (z * _vector.y));	// Get the X value
	y = ((z * _vector.x) - (x * _vector.z));	// Get the Y value												
	z = ((x * _vector.y) - (y * _vector.x));	// Get the Z value
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
void CVector3::operator*=(float _num)
{
	x *= _num;
	y *= _num;
	z *= _num;
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
void CVector3::operator/=(float _num)
{
	x /= _num;
	y /= _num;
	z /= _num;
}

//--------------------------------------------------------------------
//	Returns the Maginitude of two vectors divided
//--------------------------------------------------------------------
void CVector3::operator/=(CVector3 _vector)
{
	float _mag = _vector.Magnitude();
	x /= _mag;
	y /= _mag;
	z /= _mag;
}

//####################################################################################################
//####################################################################################################
//		Functions
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Normalizes the vector
//--------------------------------------------------------------------
void CVector3::Normalize()
{
	// Get the magnitude
	float _Magnitude = Magnitude();
	if((_Magnitude < 0.000000001f) || (_isnan(_Magnitude)))
	{
		x = 0;
		y = 0;
		z = 0;
	}
	else
	{
		x /= _Magnitude;
		y /= _Magnitude;
		z /= _Magnitude;
	}
}
//--------------------------------------------------------------------
//	Inverts all the values of the Vector
//--------------------------------------------------------------------
void CVector3::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}

//####################################################################################################
//####################################################################################################
//		Accessors
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Returns Normalized form of the Vector
//--------------------------------------------------------------------
CVector3 CVector3::getNormalized() const
{
	// Get the magnitude
	float _Magnitude = Magnitude();
	if((_Magnitude < 0.000000001f) || (_isnan(_Magnitude)))
	{
		return CVector3(0,0,0);
	}
	else
	{
		CVector3 _normalized(x,y,z);

		// Recalculate Lengths
		_normalized.x /= _Magnitude;
		_normalized.y /= _Magnitude;
		_normalized.z /= _Magnitude;
		return _normalized;
	}
}
//--------------------------------------------------------------------
//	Returns the maginitude of the vector
//--------------------------------------------------------------------
float CVector3::Magnitude() const
{
	return (sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
}
//--------------------------------------------------------------------
//	Returns the Squared Magnitude of a Vector
//--------------------------------------------------------------------
float CVector3::SqrdMag() const
{
	return (pow(x,2) + pow(y,2) + pow(z,2));
}
//--------------------------------------------------------------------
//	Gets the Dot product of the Vectors
//--------------------------------------------------------------------
float CVector3::Dot(CVector3 _vector) const
{
	return ((x * _vector.x) + (y * _vector.y) + (z * _vector.z));
}
//--------------------------------------------------------------------
//	Gets the angle between
//--------------------------------------------------------------------
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
//--------------------------------------------------------------------
//	Gets the distance to the closest point on the line
//	The supplied vector must be the point in relation to the vectors
//	start point. In other words if the vector is from the origin then
//	the point can be given as is. Otherwise it must be transformed
//	to compensate
//--------------------------------------------------------------------
float CVector3::Distance(CVector3 _vector) const
{
	CVector3 _top = _vector * (*this);
	_top /= Magnitude();
	return _top.Magnitude();
}

//--------------------------------------------------------------------
//	Returns an array for use
//	ASSUMES ARRAY IS OF LENGTH 3 OR GREATER
//--------------------------------------------------------------------
void CVector3::getArray(float* _array) const
{
	_array[0] = x;
	_array[1] = y;
	_array[2] = z;
}