#include "CVector2.h"
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
CVector2::CVector2() : x(0), y(0)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
CVector2::CVector2(float _X, float _Y) : x(_X), y(_Y)
{}
//--------------------------------------------------------------------
//	Copy Constructor
//--------------------------------------------------------------------
CVector2::CVector2(const CVector2& _vector)
{
	if(this != &_vector)
	{
		x = _vector.x;
		y = _vector.y;
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
CVector2 CVector2::operator+(CVector2 _vector)
{
	return CVector2((x+_vector.x),(y+_vector.y));
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
CVector2 CVector2::operator-(CVector2 _vector)
{
	return CVector2((x-_vector.x),(y-_vector.y));
}
//--------------------------------------------------------------------
//	Cross Product of Two Vectors
//--------------------------------------------------------------------
CVector2 CVector2::operator*(CVector2 _vector)
{
	CVector2 _vCross;									// The vector to hold the cross product
	_vCross.x = ((y) - (_vector.y));	// Get the X value
	_vCross.y = ((_vector.x) - (x));	// Get the Y value												

	return _vCross;										// Return the cross product
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
CVector2 CVector2::operator*(float _num)
{
	return CVector2((x*_num),(y*_num));
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
CVector2 CVector2::operator/(float _num)
{
	return CVector2((x/_num),(y/_num));
}

//--------------------------------------------------------------------
//	Returns the Maginitude of two vectors divided
//--------------------------------------------------------------------
float CVector2::operator/(CVector2 _vector)
{
	return ((Magnitude()) / (_vector.Magnitude()));
}
//--------------------------------------------------------------------
//	Check Magnitude
//	Due to the nature of floats it checks within a 5% tolerance
//--------------------------------------------------------------------
bool CVector2::operator==(float _mag) const
{
	float _result = Magnitude() / _mag;	
	return ((_result < 1.05f) || (_result > 0.95f));
}
//--------------------------------------------------------------------
//	Equal Comparison for Vectors
//	Due to the nature of floats it checks within a 5% tolerance
//--------------------------------------------------------------------
bool CVector2::operator==(CVector2 _vector) const
{
	float _result = ((x/_vector.x) + (y/_vector.y)) / 2.0f;
	return ((_result < 1.05f) || (_result > 0.95f));
}
//--------------------------------------------------------------------
//	Checks for inquality
//	Uses equality operator
//--------------------------------------------------------------------
bool CVector2::operator!=(CVector2 _vector) const
{
	return !((*this) == _vector);
}

//####################################################################################################
//####################################################################################################
//		Functions
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Normalizes the vector
//--------------------------------------------------------------------
void CVector2::Normalize()
{
	// Get the magnitude
	float _Magnitude = Magnitude();

	// Recalculate Lengths
	x /= _Magnitude;
	y /= _Magnitude;
}
//--------------------------------------------------------------------
//	Inverts all the values of the Vector
//--------------------------------------------------------------------
void CVector2::Invert()
{
	x = -x;
	y = -y;
}

//####################################################################################################
//####################################################################################################
//		Accessors
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Returns Normalized form of the Vector
//--------------------------------------------------------------------
CVector2 CVector2::getNormalized() const
{
	// Get the magnitude
	float _Magnitude = Magnitude();
	CVector2 _normalized(x,y);

	// Recalculate Lengths
	_normalized.x /= _Magnitude;
	_normalized.y /= _Magnitude;

	return _normalized;
}
//--------------------------------------------------------------------
//	Returns the maginitude of the vector
//--------------------------------------------------------------------
float CVector2::Magnitude() const
{
	return (sqrt(pow(x,2) + pow(y,2)));
}
//--------------------------------------------------------------------
//	Gets the Dot product of the Vectors
//--------------------------------------------------------------------
float CVector2::Dot(CVector2 _vector) const
{
	return ((x * _vector.x) + (y * _vector.y));
}
//--------------------------------------------------------------------
//	Gets the angle between
//--------------------------------------------------------------------
float CVector2::Angle(CVector2 _vector) const
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
