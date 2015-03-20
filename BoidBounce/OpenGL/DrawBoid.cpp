#include "DrawBoid.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Initialsation Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//========================================================================================
//	Blank Constructor
//	Shouldn't e used except by scope and array initialisation
//========================================================================================
BGLDrawBoid::BGLDrawBoid()
: m_flock(0)
{
	m_quadric = gluNewQuadric();
	gluQuadricDrawStyle(m_quadric,GLU_FILL);
	gluQuadricNormals(m_quadric,GLU_SMOOTH);
	gluQuadricTexture(m_quadric,GLU_TRUE);
}
//========================================================================================
//	Basic Contructor
//========================================================================================
BGLDrawBoid::BGLDrawBoid(const WinGLengine& _engine, const bFlock3& _flock)
: m_flock(&_flock)
{
	m_quadric = gluNewQuadric();
	gluQuadricDrawStyle(m_quadric,GLU_FILL);
	gluQuadricNormals(m_quadric,GLU_SMOOTH);
	gluQuadricTexture(m_quadric,GLU_TRUE);
}
//========================================================================================
//	Deconstrucotr and Cleanup
//========================================================================================
BGLDrawBoid::~BGLDrawBoid()
{
	gluDeleteQuadric(m_quadric);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Render Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//========================================================================================
//	Basic Render Function
//========================================================================================
void BGLDrawBoid::Render()
{
	//----------------------------------------
	//	Frustrum Culling - NOT IMPLEMENTED
	//-----------------------------------------
	UINT _size = m_flock->getSize();
	bool* _visible = new bool[_size];
	memset(_visible ,1,sizeof(bool)*_size);
	//-----------------------------------------
	//	Loop Through the Boids and Draw
	//-----------------------------------------
	for(UINT c = 0; c < _size; ++c)
	{
		float _mass = m_flock->getBoidMass(c);
		CVector3 _dir = m_flock->getBoidDir(c);
		float _pos[3] ={0,0,0};
		m_flock->getBoidPos(c).getArray(_pos);

		glPushMatrix();
			glTranslatef(_pos[0],_pos[1],_pos[2]);

			gluSphere(m_quadric,_mass,6,6);
			glBegin(GL_LINES);
				glColor3f(0,0,0);			glVertex3f(-_dir.x,-_dir.y,-_dir.z);
				glColor3f(1,1,1);			glVertex3f(0,0,0);
			glEnd();

		glPopMatrix();
	}
}