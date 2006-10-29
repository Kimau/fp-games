#include "DrawCube.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Construction
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//===================================================================================
//	Blank Contructor
//===================================================================================
BGLDrawCube::BGLDrawCube()
: m_size(CVector3(0,0,0)), m_inside(false)
{
	makeList();
}
//===================================================================================
//	Initiated Constructor
//===================================================================================
BGLDrawCube::BGLDrawCube(CVector3 _size)
: m_size(_size), m_inside(false)
{
	makeList();
}
//===================================================================================
//	Deconstructor
//===================================================================================
BGLDrawCube::~BGLDrawCube()
{}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void BGLDrawCube::makeList()
{
	//-----------------------------------
	//	Draw Cube
	//-----------------------------------
	m_list = glGenLists(1);
	glNewList(m_list,GL_COMPILE);
		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
			glNormal3f(0,0, 1);			// Draw Top
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,1.0f); 
			glTexCoord2f(1.0f,0.0f);	glVertex3f(1.0f,0.0f,1.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(1.0f,1.0f,1.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,1.0f,1.0f);			

			glNormal3f(0,0,-1);			// Draw Bottom
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.0f); 
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,1.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(1.0f,1.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(1.0f,0.0f,0.0f);

			glNormal3f(0, 1,0);			// Draw North
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,1.0f,0.0f); 
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,1.0f,1.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(1.0f,1.0f,1.0f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(1.0f,1.0f,0.0f);

			glNormal3f(0,-1,0);			// Draw South
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.0f); 
			glTexCoord2f(1.0f,0.0f);	glVertex3f(1.0f,0.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(1.0f,0.0f,1.0f);			
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.0f,1.0f);

			glNormal3f( 1,0,0);			// Draw East
			glTexCoord2f(0.0f,0.0f);	glVertex3f(1.0f,0.0f,0.0f); 
			glTexCoord2f(1.0f,0.0f);	glVertex3f(1.0f,1.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(1.0f,1.0f,1.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(1.0f,0.0f,1.0f);			

			glNormal3f(-1,0,0);			// Draw West
			glTexCoord2f(0.0f,0.0f);	glVertex3f(0.0f,0.0f,0.0f); 
			glTexCoord2f(0.0f,1.0f);	glVertex3f(0.0f,0.0f,1.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(0.0f,1.0f,1.0f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(0.0f,1.0f,0.0f);

		glEnd();
	glEndList();
}
//===================================================================================
//	Render Cube
//===================================================================================
void BGLDrawCube::Render()
{
	if(m_inside)
		glCullFace(GL_FRONT);

	glPushMatrix();
		glScalef(m_size.x,m_size.y,m_size.z);
		glCallList(m_list);
	glPopMatrix();

	glCullFace(GL_BACK);
}

//===================================================================================
//	Set Size of Cube
//===================================================================================
void BGLDrawCube::setSize(CVector3 _size)
{
	m_size = _size;
}
//===================================================================================
//	Set Size of Cube
//===================================================================================
void BGLDrawCube::setInside(bool _inside)
{
	m_inside = _inside;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Accessors
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//===================================================================================
//	Return Size of Cube
//===================================================================================
CVector3 BGLDrawCube::getSize()
{
	return m_size;
}