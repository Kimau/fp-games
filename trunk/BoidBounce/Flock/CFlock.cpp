#include "CFlock.h"
#include <math.h>		// For Float Functions
#include <stdlib.h>		// For Rand
#include <memory.h>

#define DEG_NUM 57.29577951308232f

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Construction
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//====================================================================================
//	Blank Flock
//====================================================================================
bFlock3::bFlock3() :
m_size(0)
{
	m_boids = 0;
	m_wrapped = false;
	m_mouse = true;
	m_avoid = 1.0f;
	m_align = 1.0f;
	m_attract = 1.0f;
	m_speed = 1.0f;
}
//====================================================================================
//	Basic Construction
//====================================================================================
bFlock3::bFlock3(UINT _size, CVector3 _maxSpace) :
m_size(_size),
m_maxSpace(_maxSpace)
{
	m_boids = new bBoid3[m_size];
	m_wrapped = false;
	m_mouse = true;
	m_avoid = 1.0f;
	m_align = 1.0f;
	m_attract = 1.0f;
	m_speed = 1.0f;
}
//====================================================================================
//	Copy Constructor
//====================================================================================
bFlock3::bFlock3(const bFlock3& _ref) :
m_size(_ref.m_size),
m_wrapped(_ref.m_wrapped),
m_mouse(_ref.m_mouse),
m_avoid(_ref.m_avoid),
m_align(_ref.m_align),
m_attract(_ref.m_attract),
m_speed(_ref.m_speed)
{
	if(this != &_ref)
	{
		m_boids = new bBoid3[m_size];
		memcpy(m_boids,_ref.m_boids,sizeof(bBoid3)*m_size);
	}
}
//====================================================================================
//	Equals Operator
//====================================================================================
bFlock3& bFlock3::operator=(const bFlock3& _ref)
{
	if(this != &_ref)
	{
		//-----------------------------
		//	Copy Over Size
		//-----------------------------
		UINT* _ptr = const_cast<UINT*>(&m_size);
		*(_ptr) = _ref.m_size;
		//-----------------------------
		//	Copy Over Boids
		//-----------------------------
		if(m_boids) delete[] m_boids;
		m_boids = new bBoid3[m_size];
		memcpy(m_boids,_ref.m_boids,sizeof(bBoid3)*m_size);

		//----------------------------------
		//	Shallow Copy Everything Else
		//----------------------------------
		m_wrapped	= _ref.m_wrapped;
		m_mouse		= _ref.m_mouse;
		m_avoid		= _ref.m_avoid,
		m_align		= _ref.m_align;
		m_attract	= _ref.m_attract;
		m_speed		= _ref.m_speed;
		m_maxSpace  = _ref.m_maxSpace;
		m_mouseDir	= _ref.m_mouseDir;
		m_mouseStart= _ref.m_mouseStart;
	}

	return *this;
}
	
//====================================================================================
//	Clean Up
//====================================================================================
bFlock3::~bFlock3()
{
	if(m_boids) delete[] m_boids;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//====================================================================================
//	Calculate Attraction Forces acting on Boid
//	The attraction force is a direct coralation to strength. Linear.
//====================================================================================
CVector3 bFlock3::Attract(UINT _index, float* _influence)
{
	//----------------------------------
	//	Zero Force and get Position
	//----------------------------------
	CVector3 _force(0,0,0);
	CVector3 _myPos = m_boids[_index].m_Pos;
	//----------------------------------
	//	Loop Through
	//----------------------------------
	for(UINT c = 0; c < _index; ++c)
	{
		CVector3 _dir = (m_boids[c].m_Pos - _myPos).getNormalized();
		_force += _dir * _influence[c] * m_attract;
	}
	for(UINT c = _index+1; c < m_size; ++c)
	{
		CVector3 _dir = (m_boids[c].m_Pos - _myPos).getNormalized();
		_force += _dir * _influence[c] * m_attract;
	}
	return _force;
}
//====================================================================================
//	Calculate Avoidence Forces acting on Boid
//	The avoidence force ignores everything not in its view
//	Take two vectors calculate angle.
//	Influence * ((90-angle) / 90)
//====================================================================================
CVector3 bFlock3::Avoid(UINT _index, float* _influence)
{
	//----------------------------------
	//	Zero Force and get Position
	//----------------------------------
	CVector3 _force(0,0,0);
	float _myMass = m_boids[_index].m_Mass;
	CVector3 _myPos = m_boids[_index].m_Pos;
	//----------------------------------
	//	Avoid Walls First
	//----------------------------------
	if(!m_wrapped)
	{
		if(_myPos.x < 5.0f)						_force.x = 5.0f - _myPos.x;
		if(_myPos.x > (m_maxSpace.x - 5.0f))	_force.x = (m_maxSpace.x - 5.0f) - _myPos.x;

		if(_myPos.y < 5.0f)						_force.y = 5.0f - _myPos.y;
		if(_myPos.y > (m_maxSpace.y - 5.0f))	_force.y = (m_maxSpace.y - 5.0f) - _myPos.y;

		if(_myPos.z < 5.0f)						_force.z = 5.0f - _myPos.z;
		if(_myPos.z > (m_maxSpace.z - 5.0f))	_force.z = (m_maxSpace.z - 5.0f) - _myPos.z;
	}
	//----------------------------------
	//	Avoid Mouse Pointer
	//----------------------------------
	if(m_mouse)
	{
		CVector3 _transPoint = _myPos - (m_mouseStart);
		CVector3 _pole(0,0,1);
		float _distance = _pole.Distance(_transPoint);

		if(_distance < 10.0f)
		{
			CVector3 _dir = (m_mouseStart - _myPos);
			_dir.z = 0;
			_dir.Normalize();
			_force -= _dir * 1000000.0f;
		}
	}
	//----------------------------------
	//	Loop Through
	//----------------------------------
	for(UINT c = 0; c < _index; ++c)
	{
		CVector3 _dir = (m_boids[c].m_Pos - _myPos).getNormalized();
		float _distance = (_myMass + m_boids[c].m_Mass) / _influence[c];
		if(_distance < _myMass*5)
			_force -= _dir * 10.0f;
		else
            _force -= _dir * pow(_influence[c],m_avoid);
	}
	for(UINT c = _index+1; c < m_size; ++c)
	{
		CVector3 _dir = (m_boids[c].m_Pos - _myPos).getNormalized();
		float _distance = (_myMass + m_boids[c].m_Mass) / _influence[c];
		if(_distance < _myMass*5)
			_force -= _dir * 10.0f;
		else
            _force -= _dir * pow(_influence[c],m_avoid);
	}
	return _force;
}
//====================================================================================
//	Calculate Alignment Forces acting on Boid
//====================================================================================
CVector3 bFlock3::Align(UINT _index, float* _influence)
{
	//----------------------------------
	//	Zero Force and get Position
	//----------------------------------
	CVector3 _force(0,0,0);
	//----------------------------------
	//	Loop Through
	//----------------------------------
	for(UINT c = 0; c < _index; ++c)
	{
		_force += m_boids[c].m_Dir * _influence[c] * m_align;
	}
	for(UINT c = _index+1; c < m_size; ++c)
	{
		_force += m_boids[c].m_Dir * _influence[c] * m_align;
	}
	return _force;
}
//====================================================================================
//	Calculate Influence between Boids
//	The equation is as follows
//	I = (mass[i] + mass[c]) / (distance squared)
//====================================================================================
void bFlock3::CalcInfluence(UINT _index, float* _influence)
{
	//----------------------------------
	//	Prefetch these to avoid endless
	//	array refrences
	//----------------------------------
	float _myMass = m_boids[_index].m_Mass;
	CVector3 _myPos = m_boids[_index].m_Pos;
	//-----------------------------------
	//	Loop through the points
	//	Measure Distance add to Total
	//-----------------------------------
	for(UINT c = 0; c < m_size; ++c)
	{
		float _distance = (_myPos - m_boids[c].m_Pos).SqrdMag();
		float _mass = (_myMass + m_boids[c].m_Mass);
		if(_distance < _mass)
			_distance = _mass + 0.1f;

		_influence[c] = _mass / _distance;
	}
	_influence[_index] = 0;
}
//====================================================================================
//	Update a single boid
//====================================================================================
void bFlock3::UpdateBoid(UINT _index)
{
	//-----------------------------------
	//	Allocate Space for Influence
	//-----------------------------------
	float* _influence = new float[m_size];
	memset(_influence,0,sizeof(float)*(m_size));
	CalcInfluence(_index, _influence);
	//-----------------------------------
	//	Get the forces in play
	//-----------------------------------
	CVector3 _attract	= Attract(_index, _influence);
	CVector3 _avoid		= Avoid(_index, _influence);
	CVector3 _align		= Align(_index, _influence);
	//-----------------------------------
	//	Clean Up Influence
	//-----------------------------------
	if(_influence) delete[] _influence;
	//----------------------------------------------------------------------------
	//	TODO :: Implement Gravity and Surface Collision will be in move function
	//----------------------------------------------------------------------------
	//-----------------------------------
	//	Update Boid
	//	A = F/M
	//-----------------------------------
	m_boids[_index].m_Dir = (_attract + _avoid + _align) / m_boids[_index].m_Mass;
	if(m_boids[_index].m_Dir.Magnitude() > 5.0f)
	{
		m_boids[_index].m_Dir.Normalize();
		m_boids[_index].m_Dir *= 5.0f;
	}
}
//====================================================================================
//	Update System
//	This Doesn't move boids, only updates their vectors
//====================================================================================
void bFlock3::Update()
{
	//----------------------------------
	// Calculate NEW boids
	//----------------------------------
	for(UINT _index = 0; _index < m_size; ++_index)
	{
		UpdateBoid(_index);
	}
}
//====================================================================================
//	Move Flock
//	The reason for this function is you might want to move the system say
//	at say 60 fps but only calculate once per second.
//====================================================================================
void bFlock3::Move(float _time)
{
	//----------------------------------
	//	Move boids
	//----------------------------------
	for(UINT _index = 0; _index < m_size; ++_index)
	{
		bBoid3& _boid = m_boids[_index];							// Avoid lots of [] calls

		_boid.m_Pos += (_boid.m_Dir * _time);
		float& _x = _boid.m_Pos.x;
		float& _y = _boid.m_Pos.y;
		float& _z = _boid.m_Pos.z;
		//-------------------------------------------------
		//	Check for out of Bounds
		//-------------------------------------------------
		if(m_wrapped)
		{
			//-----------------------------
			//	Wrapped Space
			//-----------------------------
			if(_x < 0)					_x = (m_maxSpace.x + _x);
			if(_x > m_maxSpace.x)		_x = (_x - m_maxSpace.x);

			if(_y < 0)					_y = (m_maxSpace.y + _y);
			if(_y > m_maxSpace.y)		_y = (_y - m_maxSpace.y);

			if(_z < 0)					_z = (m_maxSpace.z + _z);
			if(_z > m_maxSpace.z)		_z = (_z - m_maxSpace.z);
		}
		else
		{
			//-----------------------------
			//	Wall Collision
			//-----------------------------
			if(_x < 0)					_x = (0 - _x);
			if(_x > m_maxSpace.x)		_x = ((m_maxSpace.x*2) - _x);

			if(_y < 0)					_y = (0 - _y);
			if(_y > m_maxSpace.y)		_y = ((m_maxSpace.y*2) - _y);

			if(_z < 0)					_z = (0 - _z);
			if(_z > m_maxSpace.z)		_z = ((m_maxSpace.z*2) - _z);
		}
	}
}
//====================================================================================
//	Randomly Generates the Flock of Boids
//====================================================================================
void bFlock3::RandomBoid()
{
	//---------------------------------------------------
	//	Generate Boids
	//---------------------------------------------------
	for(UINT _index = 0; _index < m_size; ++_index)
	{
		float _max[3] = {0,0,0};
		m_maxSpace.getArray(_max);

		//------------------------------------------
		//	Seed and Generate
		//------------------------------------------
		srand(rand() + m_size + _index);
		UINT _temp[3] = 
		{
			rand() % UINT(_max[0] * 100),
			rand() % UINT(_max[1] * 100),
			rand() % UINT(_max[2] * 100)
		};

		//------------------------------------------
		//	Save Value
		//------------------------------------------
		m_boids[_index] = bBoid3(
			CVector3(float(_temp[0]) / 10.0f,
			float(_temp[1]) / 10.0f,
			float(_temp[2]) / 10.0f),
			CVector3(0,0,0),
			1.0f);
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Setup Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//====================================================================================
//	Set Wrapped Space
//====================================================================================
void bFlock3::setWrapped(bool _wrap)
{
	m_wrapped = _wrap;
}
//====================================================================================
//	Set Mouse Fear
//====================================================================================
void bFlock3::setMouse(bool _mouse)
{
	m_mouse = _mouse;
}
//====================================================================================
//	Set Avoidence Strength
//====================================================================================
void bFlock3::setAvoid(float _avoid)
{
	m_avoid = _avoid;
}
//====================================================================================
//	Set Alignment Strength
//====================================================================================
void bFlock3::setAlign(float _align)
{
	m_align = _align;
}
//====================================================================================
//	Set Attraction Strength
//====================================================================================
void bFlock3::setAttract(float _attract)
{
	m_attract = _attract;
}
//====================================================================================
//	Set Speed of Flock
//====================================================================================
void bFlock3::setSpeed(float _speed)
{
	m_speed = _speed;
}
//====================================================================================
//	Set Mouse Varibles
//====================================================================================
void bFlock3::setMouse(CVector3 _start, CVector3 _dir)
{
	m_mouseDir = _dir;
	m_mouseStart = _start;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Accessors
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//====================================================================================
//	Get Boid Position
//====================================================================================
CVector3 bFlock3::getBoidPos(UINT _index) const
{
	return m_boids[_index].m_Pos;
}
//====================================================================================
//	Get Boid Mass
//====================================================================================
float bFlock3::getBoidMass(UINT _index) const
{
	return m_boids[_index].m_Mass;
}
//====================================================================================
//	Get Boid Direction
//====================================================================================
CVector3 bFlock3::getBoidDir(UINT _index) const
{
	return m_boids[_index].m_Dir;
}
//====================================================================================
//	Is the space wrapped? In other words will you fly out and back in or just into
//	a brick wall and SPLAT!
//====================================================================================
bool bFlock3::isWrapped() const
{
	return m_wrapped;
}
//====================================================================================
//	Are they scared of the mouse
//====================================================================================
bool bFlock3::isMoused() const
{
	return m_mouse;
}
//====================================================================================
// Get avoidence strength
//====================================================================================
float bFlock3::getAvoidence() const
{
	return m_avoid;
}
//====================================================================================
//	Get alignment strength
//====================================================================================
float bFlock3::getAlignment() const
{
	return m_align;
}
//====================================================================================
//	Get attraction strength
//====================================================================================
float bFlock3::getAttraction() const
{
	return m_attract;
}
//====================================================================================
//	Get Speed of Flock
//====================================================================================
float bFlock3::getSpeed() const
{
	return m_speed;
}
//====================================================================================
//	Get Size of Flock
//====================================================================================
UINT bFlock3::getSize() const
{
	return m_size;
}