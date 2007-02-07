#ifndef FP_QUICK_2D_VECTOR
#define FP_QUICK_2D_VECTOR

class FPVec2
{
public:
	FPVec2() : x(0), y(0) {}
	FPVec2(float _X, float _Y) : x(_X), y(_Y) {}
	FPVec2(const FPVec2& _vector);

	//-------------------------------------------
	//	Operators
	//-------------------------------------------
	FPVec2 operator+(FPVec2 _vector);
	FPVec2 operator-(FPVec2 _vector);	
	FPVec2 operator*(float _num);
	FPVec2 operator/(float _num);
	float operator/(FPVec2 _vector);

	//-------------------------------------------
	//	Self Operators
	//-------------------------------------------
	void operator+=(FPVec2 _vector);
	void operator-=(FPVec2 _vector);	
	void operator*=(float _num);
	void operator/=(float _num);

	void Normalize();
	void Invert();
	float Magnitude() const;
	float SqrdMag() const;
	float Dot(FPVec2 _vector) const;

	float x,y;
};

#endif