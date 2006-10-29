#ifndef BASIC_OPENGL_TEXTURE_LOADER_564
#define BASIC_OPENGL_TEXTURE_LOADER_564

//--------------------------------------
//	Includes
//--------------------------------------
#include "BaseGL.h"
#include <vector>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

//--------------------------------------
//	Libaries
//--------------------------------------
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

#pragma warning(disable : 4996) // Disables silly warning about win "security"

//--------------------------------------
//	Structs
//--------------------------------------


//----------------------------------------------------------
//	Basic OpenGL Texture Loader
//----------------------------------------------------------
class BGLTexture
{
	//-------------------------------------
	//	Constructors and Decon
	//-------------------------------------
public:
	BGLTexture();
	~BGLTexture();
	//-------------------------------------
	//	Bitmap Functions
	//-------------------------------------
	GLuint Load(char* _filename);
	void BindTexture(GLuint _id);
	//-------------------------------------
	//	Varibles
	//-------------------------------------
private:
	std::vector<GLuint> m_tex;
};

#endif