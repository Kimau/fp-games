#include "gwMagnet.h"

//========================================================================================
//	Calculate Force generated by Magnet on Object
//========================================================================================
gwVector gwMagnet::Force(const gwVector& _pos) const
{
	gwVector _res = m_pos - _pos;
	_res *= MAG_CONST * m_size / (_res.SqrLength());
	_res.Cap(1.5f);

	return _res;
}

