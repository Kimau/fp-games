/*-----------------------------------------------------------------------------------------
		Basic OpenGL Class
		Its important to keep this file as clean as possible
-----------------------------------------------------------------------------------------*/
#include "BaseGL.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Initialisation Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================
//	Constructor
//=========================================================================================
WinGLengine::WinGLengine(HINSTANCE _hInstance, HWND _hWnd, UINT _height, UINT _width)
{
	//----------------------------------
	//	Check for Dud
	//----------------------------------
	if(!((_hInstance) && (_hWnd)))
	{
		m_hInstance = 0;
		m_hWnd = 0;
		//-------------------------------
		// Load Settings From File
		//-------------------------------
		m_DrawStyle		= 0;							// Holds Drawing Style of Window
		m_Height		= 0;							// Height of Window
		m_Width			= 0;							// Width of Window
		m_Depth			= 0;							// Depth of Window
	}
	else
	//----------------------------------
	//	Go Ahead with it
	//----------------------------------
	{
		m_hInstance = _hInstance;
		m_hWnd = _hWnd;
		//-------------------------------
		// Load Settings From File
		//-------------------------------
		m_DrawStyle		= WS_OVERLAPPEDWINDOW;							// Holds Drawing Style of Window
		m_Height		= _height;										// Height of Window
		m_Width			= _width;										// Width of Window
		m_Depth			= 32;											// Depth of Window

		//-------------------------------
		// Start Windows and OpenGL
		//-------------------------------
		if (!InitializeOpenGL())
		{
			DWORD _error = GetLastError();
			PostQuitMessage(NULL);
		}
	}
}

//=========================================================================================
// Init OpenGL
//=========================================================================================
bool WinGLengine::InitializeOpenGL()
{  
	//---------------------------------------------------
	//	Get a HANDLE on things
	//---------------------------------------------------
    m_hDevCon = GetDC(m_hWnd);							// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!CreatePixelFormat(m_hDevCon))					// This sets our pixel format/information
	{
        PostQuitMessage(0);								// If there's an error, quit
		return false;
	}

    m_hGLRemCon = wglCreateContext(m_hDevCon);			// This creates a rendering context from our hdc
    wglMakeCurrent(m_hDevCon, m_hGLRemCon);				// This makes the rendering context we just created the one we want to use

	//---------------------------------------------------
	//	Setup OpenGL
	//---------------------------------------------------
	glShadeModel(GL_SMOOTH);							// Set Shade Model
	glClearColor(0, 0, 0, 1);							// Black Background

	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Test To Do


	glEnable(GL_TEXTURE_2D);							// Enables Textures
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return SizeOpenGL();								// Setup the screen translations and viewport
}

//=========================================================================================
// Creates Pixel Format
//=========================================================================================
bool WinGLengine::CreatePixelFormat(HDC _hdc)
{
    PIXELFORMATDESCRIPTOR _pfd; 
    int _PixelFMT; 
 
    _pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    _pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    _pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    _pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    _pfd.iPixelType = PFD_TYPE_RGBA;					// We want RGB and Alpha pixel type
    _pfd.cColorBits = m_Depth;							// Here we use our #define for the color bits
    _pfd.cDepthBits = m_Depth;							// Depthbits is ignored for RGBA, but we do it anyway
    _pfd.cAccumBits = 0;								// No special bitplanes needed
    _pfd.cStencilBits = 0;								// We desire no stencil bits
 
	// This gets us a pixel format that best matches the one passed in from the device
    if ( (_PixelFMT = ChoosePixelFormat(_hdc, &_pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// This sets the pixel format that we extracted from above
    if (SetPixelFormat(_hdc, _PixelFMT, &_pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;										// Return a success!
}
//=========================================================================================
// Resize Screen
//=========================================================================================
bool WinGLengine::SizeOpenGL()
{
	glViewport(0,0,m_Width,m_Height);					// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what our our drawing boundries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  The closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(45.0f,(float)m_Width/(float)m_Height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	return true;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		Switches Display Modes
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================
//	Switches to Model Mode
//=========================================================================================
void WinGLengine::modeModel()
{
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f,(float)m_Width/(float)m_Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}
//=========================================================================================
//	Switches to Ortho Mode
//=========================================================================================
void WinGLengine::modeOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0f,m_Width,m_Height,0.0f,-1.0f,1.0f);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Destruction Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================
//	Destructor
//=========================================================================================
WinGLengine::~WinGLengine()
{
	DestroyOpenGL();
}
//=========================================================================================
//	The actual DeInit function
//=========================================================================================
bool WinGLengine::DestroyOpenGL()
{
	//---------------------------------------------------
	//	Free Mem and Delete OpenGL Render Context
	//---------------------------------------------------
	if (m_hGLRemCon)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(m_hGLRemCon);					// Delete our OpenGL Rendering Context	
	}
	
	//---------------------------------------------------
	//	Release Device Controller Handle
	//---------------------------------------------------
	if (m_hDevCon) 
		ReleaseDC(m_hWnd, m_hDevCon);					// Release our HDC from memory
		
	PostQuitMessage (0);								// Post a QUIT message to the window
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Accessors
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================
//	Get Device Controller handel
//=========================================================================================
HDC WinGLengine::getDeviceCon() const
{
	return m_hDevCon;
}
//=========================================================================================
//	Get Window Height
//=========================================================================================
UINT WinGLengine::getWinHeight() const
{
	return m_Height;
}
//=========================================================================================
//	Get Window Width
//=========================================================================================
UINT WinGLengine::getWinWidth() const
{
	return m_Width;
}