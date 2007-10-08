#pragma once

//-----------------------------
//	Includes
//-----------------------------
#include "gwVector.h"

#define	MAG_CONST	1.0f

/*----------------------------------
		Magnet
------------------------------------*/
class gwMagnet
{
public:
	gwMagnet() : m_size(0), m_alive(true) {}
	gwMagnet(float _size) : m_size(_size), m_alive(true) {}
	gwMagnet(float _size, float _x, float _y) : m_size(_size), m_alive(true), m_pos(_x,_y) {}
	gwMagnet(float _size, gwVector _pos) : m_size(_size), m_alive(true), m_pos(_pos) {}
	~gwMagnet() {}

	gwVector Force(const float& _x, const float& _y) const { Force(gwVector(_x,_y)); }
	gwVector Force(const gwVector& _pos) const;

	gwVector m_pos;
	float m_size;
	bool m_alive;
};

//========================================================================================
//	Little Hacked Gravity function to avoid objects
//========================================================================================
inline gwVector Avoid(const gwVector& _obj, const gwVector& _avoid, float _size)
{
	gwVector _res = _avoid - _obj;
	if(_res.SqrLength() < (_size*_size*2))
	{
		_res *= MAG_CONST * (-_size) / (_res.SqrLength());
		_res.Cap(_size*_size);
		return _res;
	}
	return gwVector();
}
