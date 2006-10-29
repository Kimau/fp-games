#ifndef MENU_WINDOWS_OPENGL_SYSTEM_HEADER
#define MENU_WINDOWS_OPENGL_SYSTEM_HEADER

//----------------------------------
// HEader Includes
//----------------------------------
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\glu.h>
#include <gl\gl.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "Structs.h"
#include "Loaders.h"


class MenuGL
{
	//----------------------------------
	// Initialisation Functions
	//----------------------------------
public:
	MenuGL(HINSTANCE _hInstance, HWND _hWnd);
private:
	MenuGL();
	bool CreateOpenGL();									// Create OpenGL
	bool InitializeOpenGL();								// Init OpenGL
	bool CreatePixelFormat(HDC _hdc);						// Creates Pixel Format
	bool SizeOpenGL();										// Resize Screen	
	//----------------------------------
	// Destruction Functions
	//----------------------------------
public:
	~MenuGL();
private:
	bool DestroyOpenGL();									// Destroy's OpenGL
	//----------------------------------
	//	Render Functions
	//----------------------------------
public:
	void Render();

	//-------------------------------
	// Variables and Accessors
	//-------------------------------
private:
	// Windows Varibles
	HWND m_hWnd;							// Pointer to Window Handle Windows
	HDC m_hDevCon;							// Pointer to a Struct of Device Controller Handle
	HGLRC m_hGLRemCon;						// Pointer to a Struct of OpenGL Remote Controller Handle
	HINSTANCE m_hInstance;					// Pointer to a Struct of Instance Handle
	
	DEVMODE m_dmSettings;					// FIGURE OUT ? -> ? to Device Mode Struct
	DWORD m_DrawStyle;						// Holds Drawing Style of Window
    UINT m_Height;							// Height of Window
	UINT m_Width;							// Width of Window
	UINT m_Depth;							// Depth of Window

	// OpenGL Varibles
	bool  m_Fullscreen;						// Fullscreen or not
	UINT  m_TexCount;						// How many textures currently used
    UINT  m_ListCount;						// How many Display Lists currently used

	// OpenGL Data
	OGL_FILE 	m_Textures[_MAX_TEX_COUNT];	// Holds OpenGL Textures
	OGL_FILE 	m_Display[_MAX_MESH_COUNT];	// Holds OpenGL DrawList

	// Camera Varibles
	CVector3 m_cPosition;					// The camera's position
	CVector3 m_cView;						// The camera's view
	CVector3 m_cUpVector;					// The camera's up vector
};

#endif
