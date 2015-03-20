#include "gVector.h"

//============================================================================
//	Angle Between two Vectors
//============================================================================
float gVector::Angle(const gVector& _other) const 
{
	float _angle = acosf((Dot(_other) / (Length() * _other.Length())));

	// if(_isnan(_angle))				return 0;

	// Return the angle in radians
	return(_angle);
}

//============================================================================
//	Intersection of two segments
//============================================================================
int gSegment::Intersect(const gSegment& _second, gVector& _result)
{
	gVector _alpha(ex - sx, ey - sy);		// First Direction
	gVector _beta(_second.sx - _second.ex, _second.sy - _second.ey);	// Second Direction Flipped
	_result = gVector();

	/*---------------------------------------
		X Bounding Check
	 ---------------------------------------*/
	if(_alpha.x > 0)	// Ensure Work is Left to Right
	{
		if(_beta.x > 0)
		{
			if((ex < _second.ex) || (_second.sx < sx))		return GW_NO_CROSS;
		}
		else
		{
			if((ex < _second.sx) || (_second.ex < sx))		return GW_NO_CROSS;
		}
	}
	else
	{
		if(_beta.x > 0)
		{
			if((sx < _second.ex) || (_second.sx < ex))		return GW_NO_CROSS;
		}
		else
		{
			if((sx < _second.sx) || (_second.ex < ex))		return GW_NO_CROSS;
		}
	}

	/*---------------------------------------
		Y Bounding Check
	 ---------------------------------------*/
	if(_alpha.y > 0)	// Ensure Work is Left to Right
	{
		if(_beta.y > 0)
		{
			if((ey < _second.ey) || (_second.sy < sy))		return GW_NO_CROSS;
		}
		else
		{
			if((ey < _second.sy) || (_second.ey < sy))		return GW_NO_CROSS;
		}
	}
	else
	{
		if(_beta.y > 0)
		{
			if((sy < _second.ey) || (_second.sy < ey))		return GW_NO_CROSS;
		}
		else
		{
			if((sy < _second.sy) || (_second.ey < ey))		return GW_NO_CROSS;
		}
	}

	gVector _charlie(sx - _second.sx, sy - _second.sy);
	
	/*----------------------------------
		Parallel Test
	 ---------------------------------------*/
	float _dem = (_alpha.y * _beta.x) - (_alpha.x * _beta.y);
	if(fabsf(_dem) < 0.00000001f) return GW_PARREL;

	/*----------------------------------
		Alpha Test
	 ---------------------------------------*/
	float _an = (_beta.y * _charlie.x) - (_beta.x * _charlie.y);

	if(_dem > 0)
	{
		if(_an < 0 || _an > _dem) return GW_NO_CROSS;
	}
	else 
	{
		if(_an > 0 || _an < _dem) return GW_NO_CROSS;
	}

	/*----------------------------------
		Beta Test
	 ---------------------------------------*/
	float _bn = (_alpha.x * _charlie.y) - (_alpha.y * _charlie.x);

	if(_dem > 0)
	{
		if(_bn < 0 || _bn > _dem) return GW_NO_CROSS;
	}
	else 
	{
		if(_bn > 0 || _bn < _dem) return GW_NO_CROSS;
	}

	/*----------------------------------
		They Do Intersect
		Calc Point
	 ---------------------------------------*/
	gVector _num(_an*_alpha.x);
	gVector _offset;
	_offset.x = (((_num.x > 0)&&(_dem > 0)) || ((_num.x < 0)&&(_dem < 0))) ? (_dem / 2.0f) : -(_dem/2.0f);
	_offset.y = (((_num.y > 0)&&(_dem > 0)) || ((_num.y < 0)&&(_dem < 0))) ? (_dem / 2.0f) : -(_dem/2.0f);

	_result = gVector(sx,sy) + (_num + _offset) / _dem;
	return GW_CROSS;
}

//============================================================================
//	Nearest point on this Segment
//============================================================================
gVector gSegment::NearestPoint(const gVector& _orig) const
{
	// Get Vectors
	gVector _offset(_orig.x - sx, _orig.y - sy);
	gVector _dir = Dir();
 	float _oLen = _offset.Length();
	float _dLen = _dir.Length();

	 /* SIMPLER VERSION */
	_dir = _dir * ( _oLen * (_offset.Dot(_dir) / (_oLen * _dLen))) / _dLen; 

	return gVector(sx + _dir.x, sy + _dir.y);
}
