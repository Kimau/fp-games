#ifndef CVECTOR_34
#define CVECTOR_34

#include <math.h>
#include <float.h>

class CVector3;
class CPlane;
class CTexCoord;
class CCell;
class CFace;

typedef CVector3 CVert;
typedef CVector3 CNorm;

// This is our basic 3D point/vector class
class CVector3
{
public:
	CVector3();
	CVector3(double _X, double _Y, double _Z);
	CVector3 operator+(CVector3 _vector);
	CVector3 operator-(CVector3 _vector);
	CVector3 operator*(double _num);
	CVector3 operator*(CVector3 _vector);
	CVector3 operator/(double _num);
	double operator/(CVector3 _vector);
	bool operator==(CVector3 _vector) const;
	bool operator!=(CVector3 _vector) const;

	void Normalize();
	void Invert();
	CVector3 getNormalized() const;
	double Magnitude() const;
	double Dot(CVector3 _vector) const;
	float Angle(CVector3 _vector) const;

	double x, y, z;						
};

class CPlane
{
public:
	CPlane(double _a, double _b, double _c, double _d);
	CPlane(CVector3 _normal, double _d);
	CPlane(CVector3 _normal, CVector3 _pop);

	CVector3 Normal() const;
	double DistanceFrom(CVector3 _point) const;
	CVector3 Intersect(CVector3 _start, CVector3 _dir) const;

	double a,b,c,d;
};
class CTexCoord								// Texture Coordinate Class
{
public:
	CTexCoord() : u(0), v(0)
	{}
	CTexCoord(double _u, double _v) : u(_u), v(_v)
	{}
	double u;								// U Component
	double v;								// V Component
};

class CCell									// Cell Class
{
public:
	CCell() : x(0), y(0), z(0), logic(0)
	{}
	CCell(int _x, int _y, int _z, int _logic) : x(_x), y(_y), z(_z), logic(_logic)
	{} 
	int x;									// X Component
	int y;									// Y Component
	int z;									// Z Component
	int logic;
};

class CFace
{
public:
	CFace()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		c[0] = 0;
		c[1] = 0;
		c[2] = 0;
	}
	CFace(int _v1, int _v2, int _v3, int _c1, int _c2, int _c3)
	{
		v[0] = _v1;
		v[1] = _v2;
		v[2] = _v3;
		c[0] = _c1;
		c[1] = _c2;
		c[2] = _c3;
	}
	int v[3];
	int c[3];
};

#endif