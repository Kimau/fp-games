#include "gParticle.h"

static GLuint s_TexZero = 0;
static GLuint s_TexOne = 0;

//==========================================================================
//	Create Particle
//==========================================================================
GOBParticle::GOBParticle()
{
	if(s_TexZero == 0)		s_TexZero = PH_TEX->Load("Data/Zero.png");
	if(s_TexOne  == 0)		s_TexOne  = PH_TEX->Load("Data/One.png");

	m_life = 10000;
	static unsigned int _id = 0;
	m_id = ++_id;
}
//==========================================================================
//	Destroy Particle
//==========================================================================
GOBParticle::~GOBParticle()
{
}

//==========================================================================
//	Render Particle
//==========================================================================
void GOBParticle::Render()
{
	glPushMatrix();

		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		if(m_id % 2)
			PH_TEX->BindTexture(s_TexZero);
		else
			PH_TEX->BindTexture(s_TexOne);
		glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);

		//----------------------------------
		//	Move and Billboard
		//----------------------------------
		glTranslatef(m_Pos.x,m_Pos.y,m_Pos.z);
		BGLHelper::Billboard();

		//----------------------------------
		//	Draw Quad
		//----------------------------------
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f,0.0f);	glVertex3f( 0.05f, 0.05f, 0.0f);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(-0.05f, 0.05f, 0.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(-0.05f,-0.05f, 0.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f( 0.05f,-0.05f, 0.0f);
		glEnd();

		glDepthMask(TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
//==========================================================================
//	Update Particle
//==========================================================================
void GOBParticle::Update()
{
	if(m_life > x_delta)
		m_life -= x_delta;
	else
		m_life = 0;

	m_Move += m_Acc * (x_delta / GOBPARTICLE_SPEED);
	m_Pos  += m_Move * (x_delta / GOBPARTICLE_SPEED);	
}
//==========================================================================
//	Return Type
//==========================================================================
int GOBParticle::Type()
{
	return GOBPARTICLE_ID;
}