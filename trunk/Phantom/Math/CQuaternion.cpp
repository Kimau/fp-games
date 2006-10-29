#include "CQuaternion.h"

//############################
//############################
//		CONSTRUCTORS
//############################
//############################
//==========================================================================
//	Blank Constructor
//==========================================================================
CQuaternion::CQuaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}
//==========================================================================
//	Initated Constructor
//==========================================================================
CQuaternion::CQuaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{}

//############################
//############################
//		OPERATORS
//############################
//############################
//==========================================================================
//	Multiplication 
//	Q1 (dot) Q2 + Q1 (cross) Q2
//==========================================================================
CQuaternion CQuaternion::operator*(const CQuaternion& _other)
{
    CQuaternion _retVaule;

    _retVaule.x = w * _other.x + x * _other.w + y * _other.z - z * _other.y;
    _retVaule.y = w * _other.y - x * _other.z + y * _other.w + z * _other.x;
    _retVaule.z = w * _other.z + x * _other.y - y * _other.x + z * _other.w;
    _retVaule.w = w * _other.w - x * _other.x - y * _other.y - z * _other.z;

    return _retVaule;
}
//############################
//############################
//		SELF OPERATORS
//############################
//############################
//==========================================================================
//	Assignment Operator
//==========================================================================
void CQuaternion::operator=(const CQuaternion& _other)
{
	x = _other.x;
	y = _other.y;
	z = _other.z;
	w = _other.w;
}
//==========================================================================
//	Multiplication with Self
//==========================================================================
void CQuaternion::operator*=(const CQuaternion& _other)
{
    CQuaternion _retVaule;

    _retVaule.x = w * _other.x + x * _other.w + y * _other.z - z * _other.y;
    _retVaule.y = w * _other.y - x * _other.z + y * _other.w + z * _other.x;
    _retVaule.z = w * _other.z + x * _other.y - y * _other.x + z * _other.w;
    _retVaule.w = w * _other.w - x * _other.x - y * _other.y - z * _other.z;

    x = _retVaule.x;
	y = _retVaule.y;
	z = _retVaule.z;
	w = _retVaule.w;
}

//############################
//############################
//		FUNCTIONS
//############################
//############################
//==========================================================================
//	Negate everything except w
//==========================================================================
void CQuaternion::Conjugate()
{ 
	x = -x;
	y = -y;
	z = -z;
}
//==========================================================================
//	Normalize Q
//==========================================================================
void CQuaternion::Normalize()
{
	float _length = 1 / (float)sqrt(x * x + y * y + z * z + w * w);
    x *= _length;
	y *= _length;
	z *= _length;
}
//==========================================================================
//	Creates a Quat for
//	Rotate Around an Arbitary Axis
//	Note old values ignored
//==========================================================================
void CQuaternion::CreateQuat(float _deg, float _x, float _y, float _z)
{
	//------------------------
	//	Normalize Params
	//	If needed
	//------------------------
	if( ((_x != 0) && (_x != 1)) ||
		((_y != 0) && (_y != 1)) ||
		((_z != 0) && (_z != 1)) )
	{
		float _length = 1 / (float)sqrt(_x * _x + _y * _y + _z * _z);
		_x *= _length;
		_y *= _length;
		_z *= _length;
	}

	//--------------------------
	//	Get Radians and Sine
	//--------------------------
	float _rad  = GET_RADIANS(_deg);
	float _sine = (float)sin(_rad / 2.0f);

	//--------------------------
	//	Rotate and Normalize
	//--------------------------
	x = _x * _sine;
	y = _y * _sine;
	z = _z * _sine;
	w = (float)cos(_rad / 2.0f);

	Normalize();
}
//==========================================================================
//	Creates a 4x4 Transform Matrix
//==========================================================================
void CQuaternion::CreateMatrix(float* _matrix = 0)
{
	//------------------------
	//	Allocate Memory
	//------------------------
	if(!_matrix)
		_matrix = new float[16];

	//------------------------
	//	Calculate
	//------------------------
	_matrix[0]  = 1.0f - 2.0f * (y * y + z * z); 
	_matrix[1]  =		 2.0f * (x * y + z * w);
	_matrix[2]  =		 2.0f * (x * z - y * w);
	_matrix[3]  = 0.0f;  

	_matrix[4]  =		 2.0f * (x * y - z * w);  
	_matrix[5]  = 1.0f - 2.0f * (x * x + z * z); 
	_matrix[6]  =	     2.0f * (z * y + x * w);  
	_matrix[7]  = 0.0f;  

	_matrix[8]  =		 2.0f * (x * z + y * w);
	_matrix[9]  =		 2.0f * (y * z - x * w);
	_matrix[10] = 1.0f - 2.0f * (x * x + y * y);  
	_matrix[11] = 0.0f;  

	_matrix[12] = 0;  
	_matrix[13] = 0;  
	_matrix[14] = 0;  
	_matrix[15] = 1.0f;
}
//############################
//############################
//		ACCESORS
//############################
//############################
//==========================================================================
//	Get Normalise version of Q
//==========================================================================
CQuaternion CQuaternion::getNormalized()
{
	float _length = 1 / (float)sqrt(x * x + y * y + z * z + w * w);
	return CQuaternion(x * _length,y * _length,z * _length, w);
}
//==========================================================================
//	Get Conjugated version of Q
//==========================================================================
CQuaternion CQuaternion::getConjugated()
{
	return CQuaternion(-x,-y,-z, w);
}