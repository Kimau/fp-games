#ifndef MY_QUATERNION_3_3434
#define MY_QUATERNION_3_3434

//-------------------------------
//	Defines
//-------------------------------
#define PI 3.141592654f                                 // Simpler PI
#define GET_RADIANS(deg) (float)((deg * PI) / 180.0f)	// Quick Convert Deg to Rad

//-------------------------------
//	Includes
//-------------------------------
#include<math.h>

//#########################################################################
//	Quaterion Class
//	Float based so beware floating Errors
//#########################################################################
class CQuaternion 
{
   public:
	   //---------------------------------
	   //	Constructors
	   //---------------------------------
	   CQuaternion();	   
	   CQuaternion(float _x, float _y, float _z, float _w);	  

	   //---------------------------------
	   //	Operators
	   //---------------------------------
	   CQuaternion operator*(const CQuaternion& _other);

	   //---------------------------------
	   //	Self Operators
	   //---------------------------------
	   void operator=(const CQuaternion& _other);
	   void operator*=(const CQuaternion& _other);

	   //---------------------------------
	   //	Functions
	   //---------------------------------
	   void Conjugate();
	   void Normalize();
	   void CreateQuat(float _deg, float _x, float _y, float _z);
	   void CreateMatrix(float* _matrix);
	   //-------------------------------------------
	   //	Accesors
	   //-------------------------------------------
	   CQuaternion getNormalized();
	   CQuaternion getConjugated();


	   float x, y, z, w;
};

#endif