#include "gObject.h"

//################################
//################################
//			CONSTRUCTORS
//################################
//################################
//=====================================================================
//	Constructor
//=====================================================================
GObject::GObject()
{
	m_type = GOBJ_BASIC;
	m_id = getNewID();

	m_pos[0] = 0;	m_pos[1] = 0;	m_pos[2] = 0;
	m_vel[0] = 0;	m_vel[1] = 0;	m_vel[2] = 0;
	m_acl[0] = 0;	m_acl[1] = 0;	m_acl[2] = 0;
	m_size[0] = 0;	m_size[1] = 0;
}

//=====================================================================
//	Copy Constructor
//=====================================================================
GObject::GObject(const GObject& _other)
{
	m_type	 = _other.m_type;
	m_id	 = _other.m_id;

	m_pos[0] = _other.m_pos[0]; 	m_pos[1] = _other.m_pos[1];		m_pos[2] = _other.m_pos[2];
	m_vel[0] = _other.m_vel[0];		m_vel[1] = _other.m_vel[1];		m_vel[2] = _other.m_vel[2];
	m_acl[0] = _other.m_acl[0];		m_acl[1] = _other.m_acl[1];		m_acl[2] = _other.m_acl[2];

	m_size[0] = _other.m_size[0];	m_size[1] = _other.m_size[1];
}

//=====================================================================
//	Deconstructor
//=====================================================================
GObject::~GObject()
{
}

//=====================================================================
//	Constructor
//=====================================================================
unsigned int GObject::getNewID()
{
	static int s_id = 0;
    ++s_id;
	return s_id;
}
//################################
//################################
//			OPERATORS
//################################
//################################
//=====================================================================
//	Assignment Operator		:: Virtual
//=====================================================================
GObject& GObject::operator= (const GObject& _other)
{
	//-------------------------------------
	//	Check Object Type
	//	NOTE :: For inheritence you might
	//	want to plug in virtual clone function
	//-------------------------------------
	m_type	 = _other.m_type;
	m_id	 = _other.m_id;

	m_pos[0] = _other.m_pos[0];
	m_pos[1] = _other.m_pos[1];
	m_pos[2] = _other.m_pos[2];
	m_vel[0] = _other.m_vel[0];
	m_vel[1] = _other.m_vel[1];
	m_vel[2] = _other.m_vel[2];
	m_acl[0] = _other.m_acl[0];
	m_acl[1] = _other.m_acl[1];
	m_acl[2] = _other.m_acl[2];

	m_size[0] = _other.m_size[0];
	m_size[1] = _other.m_size[1];

	return (*this);
}

//=====================================================================
//	Less Than Operator		:: Virtual
//=====================================================================
bool GObject::operator< (const GObject& _other) const
{
	//------------------------------
	//	Sort by Type First then ID
	//------------------------------
	if(m_type == _other.m_type)
	{
		return (m_id < _other.m_id);
	}
	else return (m_type < _other.m_type);
}

//=====================================================================
//	Larger Than Operator	:: Virtual
//=====================================================================
bool GObject::operator> (const GObject& _other) const
{
	//------------------------------
	//	Sort by Type First then ID
	//------------------------------
	if(m_type == _other.m_type)
	{
		return (m_id > _other.m_id);
	}
	else return (m_type > _other.m_type);
}

//=====================================================================
//	Equality Operator		:: Virtual
//=====================================================================
bool GObject::operator== (const GObject& _other) const
{
	//------------------------------
	//	Match ID first
	//------------------------------
	if(m_id == _other.m_id)
	{
		return true;
	}
	return false;
}

//################################
//################################
//			FUNCTIONS
//################################
//################################
//=====================================================================
//	Get Type	:: Virtual
//=====================================================================
unsigned int GObject::getType() const
{
	return GOBJ_BASIC;
}

//=====================================================================
//	Update		:: Virtual
//=====================================================================
 void GObject::Update(unsigned long _delta)
{
	for(int i = 0; i < 3; ++i)
	{
		m_pos[i] = m_pos[i] + m_vel[i] * _delta;
		m_vel[i] = m_vel[i] + m_acl[i] * _delta;		
	}
}

//=====================================================================
//	Set Moment
//=====================================================================
void GObject::setMoment(float* _moment)
{
	if(_moment == 0)
		return;

	m_pos[0] = _moment[0];
	m_pos[1] = _moment[1];
	m_pos[2] = _moment[2];
	m_vel[0] = _moment[3];	
	m_vel[1] = _moment[4];	
	m_vel[2] = _moment[5];
	m_acl[0] = _moment[6];	
	m_acl[1] = _moment[7];	
	m_acl[2] = _moment[8];
}

//=====================================================================
//	Set Moment
//=====================================================================
void GObject::setMoment(float _posX, float _posY, float _posR,
						float _velX, float _velY, float _velR,
						float _aclX, float _aclY, float _aclR)
{
	m_pos[0] = _posX;
	m_pos[1] = _posY;
	m_pos[2] = _posR;
	m_vel[0] = _velX;	
	m_vel[1] = _velY;	
	m_vel[2] = _velR;
	m_acl[0] = _aclX;	
	m_acl[1] = _aclY;	
	m_acl[2] = _aclR;
}
//################################
//################################
//			ACCESSORS
//################################
//################################
//=====================================================================
//	Get ID
//=====================================================================
const unsigned int GObject::Id()
{
	return m_id;
}