#pragma once

#include <math.h>
#define PI_SMALL 3.14159265f

/*------------------------------------------
	Simple Vector Class
------------------------------------------*/
class gwVector
{
public:
	gwVector() : x(0), y(0) {}
	gwVector(const gwVector& _other) : x(_other.x), y(_other.y) {}
	gwVector(float _c)  : x(_c), y(_c) {}
	gwVector(float _x, float _y) : x(_x), y(_y) {}
	~gwVector() {}

	gwVector operator+(const gwVector& _other) const {return gwVector(x+_other.x,y+ _other.y);}
	gwVector operator-(const gwVector& _other) const {return gwVector(x-_other.x,y- _other.y);}
	gwVector operator*(const float _c) const {return gwVector(x*_c,y*_c);}
	gwVector operator/(const float _c) const {return gwVector(x/_c,y/_c);}

	gwVector& operator+=(const gwVector& _other) { x += _other.x; y += _other.y; return *this; }
	gwVector& operator-=(const gwVector& _other) { x -= _other.x; y -= _other.y; return *this; }
	gwVector& operator*=(const gwVector& _other) { x *= _other.x; y *= _other.y; return *this; }
	gwVector& operator/=(const gwVector& _other) { x /= _other.x; y /= _other.y; return *this; }

	float Angle(const gwVector& _other) const;
	float Length() const	{return sqrtf(x*x + y*y);}
	float SqrLength() const {return (x*x + y*y);}
	gwVector Normal() const	{return (*this) / Length(); }
	float Dot(const gwVector& _other) const { return ((x * _other.x) + (y * _other.y)); }

	void Normalise() { operator/=(Length()); }
	void Cap(float _c) { if(Length() > _c) { Normalise(); operator*=(_c); } }


    float x,y;
};

/*------------------------------------------
		Simple Segment Class
-------------------------------------------*/
class gwSegment
{
public:
	gwSegment() : sx(0), sy(0), ex(0), ey(0) {}
	gwSegment(float _x, float _y) : sx(_x), sy(_y), ex(0), ey(0) {}
	gwSegment(float _sx, float _sy, float _ex, float _ey) : sx(_sx), sy(_sy), ex(_ex), ey(_ey) {}
	gwSegment(const gwVector& _start) : sx(_start.x), sy(_start.y), ex(0), ey(0) {}
	gwSegment(const gwVector& _start, const gwVector& _end) : sx(_start.x), sy(_start.y), ex(_end.x), ey(_end.y) {}
	~gwSegment() {}

	gwVector Dir() const { return gwVector(ex-sx,ey-sy); }
	gwVector Normal() const { gwVector _res(sx-ex, sy-ey); _res.Normalise(); return _res; }
	int Intersect(const gwSegment& _second, gwVector& _result);

	gwVector NearestPoint(const gwVector& _orig) const;

	float sx,sy,ex,ey;
};

/*--------------------------------------
	Intersection
--------------------------------------*/
#define GW_NO_CROSS	0
#define GW_CROSS	1
#define	GW_PARREL	2
