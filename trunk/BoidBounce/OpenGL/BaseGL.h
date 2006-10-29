#ifndef WINDOWS_OPENGL_BASE_3245
#define WINDOWS_OPENGL_BASE_3245

//----------------------------------
// HEader Includes
//----------------------------------
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

//----------------------------------
//	Libary Links
//----------------------------------
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

class WinGLengine
{
	//----------------------------------
	// Initialisation Functions
	//----------------------------------
public:
	WinGLengine(HINSTANCE _hInstance, HWND _hWnd, UINT _height, UINT _width);
private:
	bool InitializeOpenGL();								// Init OpenGL
	bool CreatePixelFormat(HDC _hdc);						// Creates Pixel Format
	bool SizeOpenGL();										// Resize Screen

	//----------------------------------
	//	Switch Display Modes
	//----------------------------------
public:
	void modeModel();
	void modeOrtho();
	//----------------------------------
	// Destruction Functions
	//----------------------------------
	~WinGLengine();
private:
	bool DestroyOpenGL();									// Destroy's OpenGL

	//-------------------------------
	// Variables and Accessors
	//-------------------------------
public:
	HDC getDeviceCon() const;
	UINT getWinHeight() const;
	UINT getWinWidth() const;

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
};

#endif WINDOWS_OPENGL_BASE_3245