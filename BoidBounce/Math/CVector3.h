#ifndef MY_CVECTOR_3D
#define MY_CVECTOR_3D

typedef unsigned int UINT;

//#########################################################################
//	3D Vector Class
//	Float base, note floating point errors
//#########################################################################
class CVector3
{
public:
	//-------------------------------------------
	//	Construcotrs
	//-------------------------------------------
	CVector3();
	CVector3(float _X, float _Y, float _Z);
	CVector3(double _X, double _Y, double _Z);
	CVector3(UINT _X, UINT _Y, UINT _Z);
	CVector3(UINT _X, UINT _Y, float _Z);
	CVector3(int _X, int _Y, int _Z);
	CVector3(const CVector3& _vector);

	//-------------------------------------------
	//	Operators
	//-------------------------------------------
	CVector3 operator+(CVector3 _vector);
	CVector3 operator-(CVector3 _vector);	
	CVector3 operator*(CVector3 _vector);
	CVector3 operator*(float _num);
	CVector3 operator/(float _num);
	float operator/(CVector3 _vector);

	//-------------------------------------------
	//	Self Operators
	//-------------------------------------------
	void operator+=(CVector3 _vector);
	void operator-=(CVector3 _vector);	
	void operator*=(CVector3 _vector);
	void operator*=(float _num);
	void operator/=(float _num);
	void operator/=(CVector3 _vector);

	//-------------------------------------------
	//	Comparison Operators
	//-------------------------------------------
	bool operator==(float _mag) const;
	bool operator==(CVector3 _vector) const;
	bool operator!=(CVector3 _vector) const;

	//-------------------------------------------
	//	Functions
	//-------------------------------------------
	void Normalize();
	void Invert();

	//-------------------------------------------
	//	Accesors
	//-------------------------------------------
	CVector3 getNormalized() const;
	float Magnitude() const;
	float SqrdMag() const;
	float Dot(CVector3 _vector) const;
	float Angle(CVector3 _vector) const;
	float Distance(CVector3 _vector) const;
	void  getArray(float* _array) const;

	float x,y,z;
};

typedef CVector3 CVert;
typedef CVector3 CNorm;

#endif