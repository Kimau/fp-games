#ifndef CPP_VECTOR_2D
#define CPP_VECTOR_2D

//#########################################################################
//	2D Vector Class
//	Float base, note floating point errors
//#########################################################################
class CVector2
{
public:
	CVector2();
	CVector2(float _X, float _Y);
	CVector2(long  _X, long  _Y);
	CVector2(unsigned short  _X, unsigned short  _Y);
	CVector2(const CVector2& _vector);

	//-------------------------------------------
	//	Operators
	//-------------------------------------------
	CVector2 operator+(CVector2 _vector);
	CVector2 operator-(CVector2 _vector);	
	CVector2 operator*(float _num);
	CVector2 operator/(float _num);
	float operator/(CVector2 _vector);

	//-------------------------------------------
	//	Self Operators
	//-------------------------------------------
	void operator+=(CVector2 _vector);
	void operator-=(CVector2 _vector);	
	void operator*=(float _num);
	void operator/=(float _num);
	void operator/=(CVector2 _vector);

	//-------------------------------------------
	//	Comparison Operators
	//-------------------------------------------
	bool operator==(float _mag) const;
	bool operator==(CVector2 _vector) const;
	bool operator!=(CVector2 _vector) const;

	void Normalize();
	void Invert();
	CVector2 getNormalized() const;
	float Magnitude() const;
	float Dot(CVector2 _vector) const;
	float Angle(CVector2 _vector) const;

	float x,y;
};

typedef CVector2 CTexCoord;

#endif