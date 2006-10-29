#include "Helpers.h"

//=================================================================
//	Simple Billboarding function
//	Rember to Push and Pop (removes Scaling)
//=================================================================
void BGLHelper::Billboard()
{
		float _blank[16];
		glGetFloatv(GL_MODELVIEW_MATRIX , _blank);

		for(int x=0; x < 3; ++x) 
			for(int y=0; y < 3; ++y) 
				_blank[y*4 + x] = (x == y);

		glLoadMatrixf(_blank);
}

//=================================================================
//	UnProject
//=================================================================
CVector3 BGLHelper::UnProject(CVector2 _point)
{
	//----------------------------------
	//	Get Matrixs
	//----------------------------------
	GLdouble _proj[16];			glGetDoublev(GL_PROJECTION_MATRIX,_proj);
	GLdouble _model[16];		glGetDoublev(GL_MODELVIEW_MATRIX,_model);
	GLint _view[4];				glGetIntegerv(GL_VIEWPORT,_view);

	GLdouble _res[6] = {0,0,0,0,0,0};

	//-------------------------------
	//	Unproject
	//-------------------------------
	gluUnProject((double)_point.x,_view[3]-(double)_point.y,0,_model,_proj,_view,&(_res[0]),&(_res[1]),&(_res[2]));
	gluUnProject((double)_point.x,_view[3]-(double)_point.y,1,_model,_proj,_view,&(_res[3]),&(_res[4]),&(_res[5]));

	//-------------------------------
	//	Export Normalised
	//-------------------------------
	CVector3 _dir(_res[3] - _res[0], _res[4] - _res[1], _res[5] - _res[2]);
	_dir.Normalize();
	return _dir;
}
//=================================================================
//	Draw Cube
//=================================================================
void BGLHelper::DrawCube(float _size)
{
	float _halfSize = _size / 2.0f;
	glBegin(GL_QUADS);
		glNormal3f(0,0, 1);			// Draw Top
		glTexCoord2f(0.0f,0.0f);	glVertex3f(-_halfSize,-_halfSize, _halfSize); 
		glTexCoord2f(1.0f,0.0f);	glVertex3f( _halfSize,-_halfSize, _halfSize);
		glTexCoord2f(1.0f,1.0f);	glVertex3f( _halfSize, _halfSize, _halfSize);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(-_halfSize, _halfSize, _halfSize);			

		glNormal3f(0,0,-1);			// Draw Bottom
		glTexCoord2f(0.0f,0.0f);	glVertex3f(-_halfSize,-_halfSize,-_halfSize); 
		glTexCoord2f(0.0f,1.0f);	glVertex3f(-_halfSize, _halfSize,-_halfSize);
		glTexCoord2f(1.0f,1.0f);	glVertex3f( _halfSize, _halfSize,-_halfSize);
		glTexCoord2f(1.0f,0.0f);	glVertex3f( _halfSize,-_halfSize,-_halfSize);

		glNormal3f(0, 1,0);			// Draw North
		glTexCoord2f(0.0f,0.0f);	glVertex3f(-_halfSize, _halfSize,-_halfSize); 
		glTexCoord2f(0.0f,1.0f);	glVertex3f(-_halfSize, _halfSize, _halfSize);
		glTexCoord2f(1.0f,1.0f);	glVertex3f( _halfSize, _halfSize, _halfSize);
		glTexCoord2f(1.0f,0.0f);	glVertex3f( _halfSize, _halfSize,-_halfSize);

		glNormal3f(0,-1,0);			// Draw South
		glTexCoord2f(0.0f,0.0f);	glVertex3f(-_halfSize,-_halfSize,-_halfSize); 
		glTexCoord2f(1.0f,0.0f);	glVertex3f( _halfSize,-_halfSize,-_halfSize);
		glTexCoord2f(1.0f,1.0f);	glVertex3f( _halfSize,-_halfSize, _halfSize);			
		glTexCoord2f(0.0f,1.0f);	glVertex3f(-_halfSize,-_halfSize, _halfSize);

		glNormal3f( 1,0,0);			// Draw East
		glTexCoord2f(0.0f,0.0f);	glVertex3f( _halfSize,-_halfSize,-_halfSize); 
		glTexCoord2f(1.0f,0.0f);	glVertex3f( _halfSize, _halfSize,-_halfSize);
		glTexCoord2f(1.0f,1.0f);	glVertex3f( _halfSize, _halfSize, _halfSize);
		glTexCoord2f(0.0f,1.0f);	glVertex3f( _halfSize,-_halfSize, _halfSize);			

		glNormal3f(-1,0,0);			// Draw West
		glTexCoord2f(0.0f,0.0f);	glVertex3f(-_halfSize,-_halfSize,-_halfSize); 
		glTexCoord2f(0.0f,1.0f);	glVertex3f(-_halfSize,-_halfSize, _halfSize);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(-_halfSize, _halfSize, _halfSize);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(-_halfSize, _halfSize,-_halfSize);

	glEnd();
}