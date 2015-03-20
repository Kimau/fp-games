#include "gwVector.h"

//============================================================================
//	Angle Between two Vectors
//============================================================================
float gwVector::Angle(const gwVector& _other) const 
{
	float _angle = acosf((Dot(_other) / (Length() * _other.Length())));

	// if(_isnan(_angle))				return 0;

	// Return the angle in radians
	return(_angle);
}

//============================================================================
//	Intersection of two segments
//============================================================================
int gwSegment::Intersect(const gwSegment& _second, gwVector& _result)
{
	gwVector _alpha(ex - sx, ey - sy);		// First Direction
	gwVector _beta(_second.sx - _second.ex, _second.sy - _second.ey);	// Second Direction Flipped
	_result = gwVector();

	//----------------------------------------
	//	X Bounding Check
	//----------------------------------------
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

	//----------------------------------------
	//	Y Bounding Check
	//----------------------------------------
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

	gwVector _charlie(sx - _second.sx, sy - _second.sy);
	
	//-----------------------------------
	//	Parrell Test
	//-----------------------------------
	float _dem = (_alpha.y * _beta.x) - (_alpha.x * _beta.y);
	if(fabsf(_dem) < 0.00000001f) return GW_PARREL;

	//-----------------------------------
	//	Alpha Test
	//-----------------------------------
	float _an = (_beta.y * _charlie.x) - (_beta.x * _charlie.y);

	if(_dem > 0)
	{
		if(_an < 0 || _an > _dem) return GW_NO_CROSS;
	}
	else 
	{
		if(_an > 0 || _an < _dem) return GW_NO_CROSS;
	}

	//-----------------------------------
	//	Beta Test
	//-----------------------------------
	float _bn = (_alpha.x * _charlie.y) - (_alpha.y * _charlie.x);

	if(_dem > 0)
	{
		if(_bn < 0 || _bn > _dem) return GW_NO_CROSS;
	}
	else 
	{
		if(_bn > 0 || _bn < _dem) return GW_NO_CROSS;
	}

	//-----------------------------------
	//	They Do Intersect
	//	Calc Point
	//-----------------------------------
	gwVector _num(_an*_alpha.x);
	gwVector _offset;
	_offset.x = (((_num.x > 0)&&(_dem > 0)) || ((_num.x < 0)&&(_dem < 0))) ? (_dem / 2.0f) : -(_dem/2.0f);
	_offset.y = (((_num.y > 0)&&(_dem > 0)) || ((_num.y < 0)&&(_dem < 0))) ? (_dem / 2.0f) : -(_dem/2.0f);

	_result = gwVector(sx,sy) + (_num + _offset) / _dem;
	return GW_CROSS;
}

//============================================================================
//	Nearest point on this Segment
//============================================================================
gwVector gwSegment::NearestPoint(const gwVector& _orig) const
{
	// Get Vectors
	gwVector _offset(_orig.x - sx, _orig.y - sy);
	gwVector _dir = Dir();
 	float _oLen = _offset.Length();
	float _dLen = _dir.Length();

	/*
	// Find the cos of the angle between the vectors 
	float cosTheta = _offset.Dot(_dir) / (_offset.Length() * _dir.Length()); 
	// Use that to calculate the length of B' 
	float BPrimeLength = _offset.Length() * cosTheta; 
	// Find the ratio of the length of B' and B 
	float scale = BPrimeLength / B.Length(); 
	// Scale B by that ratio 
	B *= scale; */

	 /* SIMPLER VERSION */
	_dir = _dir * ( _oLen * (_offset.Dot(_dir) / (_oLen * _dLen))) / _dLen; 

	return gwVector(sx + _dir.x, sy + _dir.y);
}
