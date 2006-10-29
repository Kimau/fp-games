#include "MenuGL.h"

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       INITALISATION FUNCTIONS                                                   ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//==================================================================================================
//	Initialize Everything
//==================================================================================================
MenuGL::MenuGL(HINSTANCE _hInstance, HWND _hWnd)
{
	m_hInstance = _hInstance;
	m_hWnd = _hWnd;
	//-------------------------------
	// Load Settings From File
	//-------------------------------
	// TODO :: LOAD SETTINGS FROM FILE
	m_DrawStyle		= WS_OVERLAPPEDWINDOW;							// Holds Drawing Style of Window
	m_Height		= _J_DEFHEIGHT;									// Height of Window
	m_Width			= _J_DEFWIDTH;									// Width of Window
	m_Depth			= _J_DEFDEPTH;									// Depth of Window

	// OpenGL Varibles
	m_Fullscreen	= _J_DEFFULLSCREEN;								// Fullscreen or not
	m_TexCount		= 0;											// How many textures currently used
    m_ListCount		= 0;											// How many Display Lists currently used

	memset(m_Textures,0,sizeof(m_Textures));
	memset(m_Display,0,sizeof(m_Display));

	// Camera Varibles
	m_cPosition = CVector3(0,0,0);
	m_cView		= CVector3(0,0,0);
	m_cUpVector = CVector3(0,0,0);

	//-------------------------------
	// Start Windows and OpenGL
	//-------------------------------
	if (!CreateOpenGL())
	{
		DWORD _error = GetLastError();
		PostQuitMessage(NULL);
	}
}
//==================================================================================================
//	Create OpenGL Context (Start the Engine)
//==================================================================================================
bool MenuGL::CreateOpenGL()
{
	return InitializeOpenGL();
}
//==================================================================================================
//	Init OpenGL
//==================================================================================================
bool MenuGL::InitializeOpenGL() 
{  
    m_hDevCon = GetDC(m_hWnd);							// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!CreatePixelFormat(m_hDevCon))					// This sets our pixel format/information
	{
        PostQuitMessage(0);								// If there's an error, quit
		return false;
	}

    m_hGLRemCon = wglCreateContext(m_hDevCon);			// This creates a rendering context from our hdc
    wglMakeCurrent(m_hDevCon, m_hGLRemCon);				// This makes the rendering context we just created the one we want to use

	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_TEXTURE_2D);							// Enables Textures
	
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

	return SizeOpenGL();									// Setup the screen translations and viewport
}

//==================================================================================================
//	Create Pixel Format
//==================================================================================================
bool MenuGL::CreatePixelFormat(HDC _hdc)
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
//==================================================================================================
//	Size OpenGL Viewport
//==================================================================================================
bool MenuGL::SizeOpenGL()
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
	gluPerspective(_J_FOV,(m_Width)/(m_Height), _J_CLIPNEAR, _J_CLIPFAR);
	glOrtho(0.0f,m_Width,m_Height,0.0f,-1.0f,1.0f);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	return true;
}
//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       DESTRUCTION FUNCTIONS			                                         ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//==================================================================================================
//		Destroy Everything
//==================================================================================================
MenuGL::~MenuGL()
{
	DestroyOpenGL();
}
//==================================================================================================
//	Turn Everything Off
//==================================================================================================
bool MenuGL::DestroyOpenGL()
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
		
	//---------------------------------------------------
	//	Exit Fullscreen
	//---------------------------------------------------
	if(m_Fullscreen)									// If we were in full screen
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	PostQuitMessage (0);								// Post a QUIT message to the window
	return true;
}
//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##            RENDER FUNCTIONS			                                         ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//==================================================================================================
//		Render Everything
//==================================================================================================
void MenuGL::Render()
{
	//-------------------------------
	// Reset Scene
	//-------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

	SwapBuffers(m_hDevCon);								// Swap Buffers
}