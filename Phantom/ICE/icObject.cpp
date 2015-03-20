#include "icObject.h"

//===========================================================================
//		Collision Check
//===========================================================================
bool icObject::Collide(icProject& _proj)
{
	CVector3 _dist = m_Pos - _proj.m_Pos;
	_dist *= 1.75f;
	if( (abs(_dist.x) <= m_Size.x) &&
		(abs(_dist.y) <= m_Size.y) &&
		(abs(_dist.z) <= m_Size.z))
	{
		Boom(_proj);
		return true;
	}
	return false;
}
//===========================================================================
//		Update Object
//===========================================================================
void icObject::Boom(icProject& _proj)
{

}
//===========================================================================
//		Update Object
//===========================================================================
void icObject::Update()
{
}
//===========================================================================
//		Render Object
//===========================================================================
void icObject::Render()
{
	glPushMatrix();
		glColor3f(0.0f,1.0f,0.0f);
		glTranslatef(m_Pos.x,m_Pos.y,m_Pos.z);
		BGLHelper::DrawCube(m_Size.x);
	glPopMatrix();
}