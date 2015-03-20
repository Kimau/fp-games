#ifndef BASIC_OPENGL_RENDERER_FOR_CSURFACE_CLASS
#define BASIC_OPENGL_RENDERER_FOR_CSURFACE_CLASS

//----------------------------------
// HEader Includes
//----------------------------------
#include "BaseGL.h"
#include "..\Math\CSurface.h"

//------------------------------------------------------------------------
//		Basic OpenGL Renderer for CSurface class
//------------------------------------------------------------------------
class BGLDrawSurface
{
	//----------------------------------
	//	Initialsation Functions
	//----------------------------------
public:
	BGLDrawSurface();
	BGLDrawSurface(const WinGLengine& _engine, const CSurface& _surface);
	BGLDrawSurface(const BGLDrawSurface& _ref);
	BGLDrawSurface& operator=(const BGLDrawSurface& _ref);
	~BGLDrawSurface();
	//----------------------------------
	//	Array Functions
	//----------------------------------
	void CreateArray();
	void UpdateArray();
	//----------------------------------
	//	Render Functions
	//----------------------------------
	void Render();
	//----------------------------------
	//	Varibles
	//----------------------------------
	const CSurface& m_surface;
	const WinGLengine& m_engine;
	float* m_vertArray;
};

#endif