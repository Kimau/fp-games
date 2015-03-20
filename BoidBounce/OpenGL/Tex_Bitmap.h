#ifndef BASIC_OPENGL_BITMAP_LOADER_34654
#define BASIC_OPENGL_BITMAP_LOADER_34654

#include "BaseGL.h"

//----------------------------------------------------------
//	Basic OpenGL Bitmap Loaded
//----------------------------------------------------------
class BGL_Bitmap
{
	//-------------------------------------
	//	Constructors and Decon
	//-------------------------------------
public:
	BGL_Bitmap();
	~BGL_Bitmap();
	//-------------------------------------
	//	Bitmap Functions
	//-------------------------------------
public:
	void Load(const char* _filename);
	void BindTexture();
	//-------------------------------------
	//	Varibles
	//-------------------------------------
private:
	GLuint m_id;

};

#endif