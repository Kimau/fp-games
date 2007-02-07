#include "vector.h"
#include <math.h>
#include <float.h>

//--------------------------------------------------------------------
//	Copy Constructor
//--------------------------------------------------------------------
FPVec2::FPVec2(const FPVec2& _vector)
{
	if(this != &_vector)
	{
		x = _vector.x;
		y = _vector.y;
	}
}

//--------------------------------------------------------------------
//	Addition Operator
//--------------------------------------------------------------------
FPVec2 FPVec2::operator+(FPVec2 _vector)
{
	return FPVec2((x+_vector.x),(y+_vector.y));
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
FPVec2 FPVec2::operator-(FPVec2 _vector)
{
	return FPVec2((x-_vector.x),(y-_vector.y));
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
FPVec2 FPVec2::operator*(float _num)
{
	return FPVec2((x*_num),(y*_num));
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
FPVec2 FPVec2::operator/(float _num)
{
	return FPVec2((x/_num),(y/_num));
}

//--------------------------------------------------------------------
//	Addition Operator
//--------------------------------------------------------------------
void FPVec2::operator+=(FPVec2 _vector)
{
	x += _vector.x;
	y += _vector.y;
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
void FPVec2::operator-=(FPVec2 _vector)
{
	x -= _vector.x;
	y -= _vector.y;
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
void FPVec2::operator*=(float _num)
{
	x *= _num;
	y *= _num;
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
void FPVec2::operator/=(float _num)
{
	x /= _num;
	y /= _num;
}

//--------------------------------------------------------------------
//	Normalizes the vector
//--------------------------------------------------------------------
void FPVec2::Normalize()
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
void FPVec2::Invert()
{
	x = -x;
	y = -y;
}

//--------------------------------------------------------------------
//	Returns the maginitude of the vector
//--------------------------------------------------------------------
float FPVec2::Magnitude() const
{
	return (sqrt(pow(x,2) + pow(y,2)));
}
//--------------------------------------------------------------------
//	Returns the maginitude of the vector
//--------------------------------------------------------------------
float FPVec2::SqrdMag() const
{
	return (pow(x,2) + pow(y,2));
}
//--------------------------------------------------------------------
//	Gets the Dot product of the Vectors
//--------------------------------------------------------------------
float FPVec2::Dot(FPVec2 _vector) const
{
	return ((x * _vector.x) + (y * _vector.y));
}