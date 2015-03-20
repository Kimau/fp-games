#pragma once

#include <math.h>
#define PI_SMALL 3.14159265f

/*------------------------------------------
	Simple Vector Class
------------------------------------------*/
class gVector
{
public:
	gVector() : x(0), y(0) {}
	gVector(const gVector& _other) : x(_other.x), y(_other.y) {}
	gVector(float _c)  : x(_c), y(_c) {}
	gVector(float _x, float _y) : x(_x), y(_y) {}
	~gVector() {}

	gVector operator+(const gVector& _other) const {return gVector(x+_other.x,y+ _other.y);}
	gVector operator-(const gVector& _other) const {return gVector(x-_other.x,y- _other.y);}
	gVector operator*(const float _c) const {return gVector(x*_c,y*_c);}
	gVector operator/(const float _c) const {return gVector(x/_c,y/_c);}

	gVector& operator+=(const gVector& _other) { x += _other.x; y += _other.y; return *this; }
	gVector& operator-=(const gVector& _other) { x -= _other.x; y -= _other.y; return *this; }
	gVector& operator*=(const gVector& _other) { x *= _other.x; y *= _other.y; return *this; }
	gVector& operator/=(const gVector& _other) { x /= _other.x; y /= _other.y; return *this; }

	float Angle(const gVector& _other) const;
	float Length() const	{return sqrtf(x*x + y*y);}
	float SqrLength() const {return (x*x + y*y);}
	gVector Normal() const	{return (*this) / Length(); }
	float Dot(const gVector& _other) const { return ((x * _other.x) + (y * _other.y)); }

	void Normalise() { operator/=(Length()); }
	void Cap(float _c) { if(Length() > _c) { Normalise(); operator*=(_c); } }


    float x,y;
};

/*------------------------------------------
		Simple Segment Class
-------------------------------------------*/
class gSegment
{
public:
	gSegment() : sx(0), sy(0), ex(0), ey(0) {}
	gSegment(float _x, float _y) : sx(_x), sy(_y), ex(0), ey(0) {}
	gSegment(float _sx, float _sy, float _ex, float _ey) : sx(_sx), sy(_sy), ex(_ex), ey(_ey) {}
	gSegment(const gVector& _start) : sx(_start.x), sy(_start.y), ex(0), ey(0) {}
	gSegment(const gVector& _start, const gVector& _end) : sx(_start.x), sy(_start.y), ex(_end.x), ey(_end.y) {}
	~gSegment() {}

	gVector Dir() const { return gVector(ex-sx,ey-sy); }
	gVector Normal() const { gVector _res(sx-ex, sy-ey); _res.Normalise(); return _res; }
	int Intersect(const gSegment& _second, gVector& _result);

	gVector NearestPoint(const gVector& _orig) const;

	float sx,sy,ex,ey;
};

/*--------------------------------------
	Intersection
--------------------------------------*/
#define GW_NO_CROSS	0
#define GW_CROSS	1
#define	GW_PARREL	2
