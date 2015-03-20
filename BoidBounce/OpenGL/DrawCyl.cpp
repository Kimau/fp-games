#include "DrawCyl.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Setup
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//======================================================================================
//	Contructor
//======================================================================================
BGLDrawTube::BGLDrawTube(CVector3 _start, CVector3 _end, float _width)
: m_start(_start), 
m_end(_end),
m_width(_width)
{
	m_quadric = gluNewQuadric();
	gluQuadricDrawStyle(m_quadric,GLU_FILL);
	gluQuadricNormals(m_quadric,GLU_SMOOTH);
	gluQuadricTexture(m_quadric,GLU_TRUE);
}
//======================================================================================
//	Deconstructor
//======================================================================================
BGLDrawTube::~BGLDrawTube()
{
	gluDeleteQuadric(m_quadric);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//======================================================================================
//	Render Tube
//======================================================================================
void BGLDrawTube::Render()
{
	glPushMatrix();
		glTranslatef(m_start.x,m_start.y,m_start.z);		// Move to Start Pos
		//-------------------------------------------
		//	Rotate around so the tube is pointing
		//	at end. TO BE IMPLEMENTED
		//-------------------------------------------

		float _height = 100.0f;

		gluCylinder(m_quadric,m_width,m_width,_height,8,2);
	glPopMatrix();
}
//======================================================================================
//	Render Tube SIMPLY VERSION
//======================================================================================
void BGLDrawTube::RenderSimple()
{
	glPushMatrix();
		glTranslatef(m_start.x,m_start.y,m_start.z);		// Move to Start Pos
		CVector3 _dir = (m_start - m_end).getNormalized();
		_dir *= 10.0f;

		glTranslatef(_dir.x,_dir.y,_dir.z);
		gluSphere(m_quadric,0.1f,4,4);

	glPopMatrix();
}
//======================================================================================
//	Set Starting Point
//======================================================================================
void BGLDrawTube::setStart(CVector3 _start)
{
	m_start = _start;
}
//======================================================================================
//	Set End Point
//======================================================================================
void BGLDrawTube::setEnd(CVector3 _end)
{
	m_end = _end;
}
