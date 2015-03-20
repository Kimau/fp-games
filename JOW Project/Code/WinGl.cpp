#include "WinGl.h"

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       INITALISATION FUNCTIONS                                                   ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################

//////////////////////////////////////////////////////////////////////////////
//				START UP WINDOWS GL ENGINE
//////////////////////////////////////////////////////////////////////////////
WinGLengine::WinGLengine(HINSTANCE _hInstance, HWND _hWnd)
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
    m_MeshCount		= 0;											// How many Display Lists currently used
	m_ObjCount		= 0;											// How many Objects are currently Used
	m_UnitCount		= 0;
	m_LightCount	= 0;
	memset(m_Objects,0,sizeof(m_Objects));
	memset(m_Units,0,sizeof(m_Units));
	memset(m_Textures,0,sizeof(m_Textures));
	memset(m_Meshes,0,sizeof(m_Meshes));
	memset(m_string,0,sizeof(m_string));

	// Camera Varibles
	m_cPosition = CVector3(0,0,0);
	m_cView		= CVector3(0,0,0);
	m_cUpVector = CVector3(0,0,0);

	// Cursors
	m_mapCur	= CVector3(0,0,0);
	m_unitCur	= 0;

	// Map Varibles
	m_mapNorms	= 0;
	m_path		= 0;

	//-------------------------------
	// Start Windows and OpenGL
	//-------------------------------
	if (!CreateOpenGL())
	{
		DWORD _error = GetLastError();
		PostQuitMessage(NULL);
	}
}

//////////////////////////////////////////////////////////////////////////////
//				CHANGE TO FULLSCREEN
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::toFullScreen()
{
	memset(&m_dmSettings,0,sizeof(DEVMODE));			// Makes Sure Memory's Cleared

	//-------------------------------
	// Get current settings 
	// This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	//-------------------------------
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&m_dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return false;
	}

	m_dmSettings.dmPelsWidth	= m_Width;			// Selected Screen Width
	m_dmSettings.dmPelsHeight	= m_Height;			// Selected Screen Height
	m_dmSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	//-------------------------------
	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	//-------------------------------
	int _retvalue = ChangeDisplaySettings(&m_dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't recieved a good return message From the function
	if(_retvalue != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//				MAKES THE OPEN GL BITS
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::CreateOpenGL()
{
	bool _flag = true;

	_flag &= InitializeOpenGL();
	_flag &= CreateRasterFont();
	//_flag &= CreateLights();
	_flag &= CreateCamera();

	return _flag;
}

//////////////////////////////////////////////////////////////////////////////
//				INITIALIZE OPENGL
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::InitializeOpenGL() 
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

//////////////////////////////////////////////////////////////////////////////
//				CREATE PIXEL FORMAT
//////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////
//				MAKES PRETTTY LIGHTS GLOW
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::CreateLights()
{
	//-------------------------------
	// Setup Lighting
	//-------------------------------
	glClearColor(0, 0, 0, 1);

	//-------------------------------
	// Map Cursor Light
	// LIGHT_ZERO
	//-------------------------------
	INT_LIGHT _light =
	{
		0,
		{0,0,0,1},						//  m_ambient[4];
		{1,1,1,1},						//  m_diffuse[4];
		{1,1,1,1},						//  m_specular[4];
		{5.0f,5.0f,9.0f,1},				//  m_position[4];

		0,								//  m_constant_attenuation;
		0,								//  m_linear_attenuation;
		0.02f,							//  m_quadratic_attenuation;

		{0,0,-1},						//  m_spot_direction[3];
		0,								//  m_spot_exponent;
		180,							//  m_spot_cutoff;
	};

	AddLight(_light);

	glEnable(GL_LIGHTING);

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//				MAKES CAMERA ROLL DOWN THE HILL
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::CreateCamera()
{
	//-------------------------------
	// Setup Camera
	//-------------------------------
	m_cPosition	= CVector3(0.0,	0.0, 5.0);
	m_cView		= CVector3(0.0,	1.5, 0.0);
	m_cUpVector = CVector3(0.0,	0.0, 1.0);

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//				CREATES THE RASTER FONT
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::CreateRasterFont()
{
	HFONT	hFont;										// This will store the handle to our font

	m_FontID = glGenLists(_MAX_CHAR);					// Generate the list for the font

	hFont = CreateFont(	_J_FONT_HEIGHT,					// Our desired HEIGHT of the font
						0,								// The WIDTH (If we leave this zero it will pick the best width depending on the height)
						0,								// The angle of escapement
						0,								// The angle of orientation
						FW_BOLD,						// The font's weight (We want it bold)
						FALSE,							// Italic - We don't want italic
						FALSE,							// Underline - We don't want it underlined
						FALSE,							// Strikeout - We don't want it strikethrough
						ANSI_CHARSET,					// This is the type of character set
						OUT_TT_PRECIS,					// The Output Precision
						CLIP_DEFAULT_PRECIS,			// The Clipping Precision
						ANTIALIASED_QUALITY,			// The quality of the font - We want anitaliased fonts
						FF_DONTCARE|DEFAULT_PITCH,		// The family and pitch of the font.  We don't care.
						_J_FONT);						// The font name (Like "Arial", "Courier", etc...)

	wglUseFontBitmaps(m_hDevCon, 0, _MAX_CHAR - 1, m_FontID);	// Builds 255 bitmap characters
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//				RESIZE OPENGL SCREEN
//////////////////////////////////////////////////////////////////////////////
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
	gluPerspective(_J_FOV,(m_Width)/(m_Height), _J_CLIPNEAR, _J_CLIPFAR);
	glOrtho(0.0f,m_Width,m_Height,0.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	return true;
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##              MANIPULATION FUNCTIONS                                             ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				Move Objects around
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::MoveObject(const UINT _objID, const CVector3 _pos, const CVector3 _up, const float _face)
{
	if(_objID < _MAX_OBJ_COUNT)
	{
		m_Objects[_objID].m_Pos = _pos;
		m_Objects[_objID].m_Up = _up;
		m_Objects[_objID].m_Face = _face;
	}
}
//////////////////////////////////////////////////////////////////////////////
//				Move Unit
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::MoveUnit(const UINT _objID, const CVector3 _pos, const CVector3 _up, const float _face)
{
	if(_objID < _MAX_OBJ_COUNT)
	{
		m_Units[_objID].m_Pos = _pos;
		m_Units[_objID].m_Up = _up;
		m_Units[_objID].m_Face = _face;
	}
}
//////////////////////////////////////////////////////////////////////////////
//				Move Map Cursor
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::MoveCursor(const CVector3 _newPos)
{
	m_mapCur = _newPos;

	float _pos[4] = {
		((float)_newPos.x + 0.5f),
		((float)_newPos.y + 0.5f),
		((float)_newPos.z + 0.8f),
		1};

	glLightfv(GL_LIGHT0, GL_POSITION, _pos);
}

//////////////////////////////////////////////////////////////////////////////
//				Select a Unit with Map Cursor
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::selUnit(const UINT _index)
{
	m_unitCur = _index;
}
//////////////////////////////////////////////////////////////////////////////
//				Set Mouse Position
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::setMouse(const UINT_POINT _pos)
{
	m_mouse = _pos;
}
//////////////////////////////////////////////////////////////////////////////
//				Set Halo
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::setHalo(CVector3 _newPos)
{
	//-------------------------------------
	//	Move Halo
	//-------------------------------------
	//m_halo.m_Face = _face;						// Face towards selected unit
	m_halo.m_Pos  = _newPos;					// Position as calculated from normals
	//m_halo.m_Up	  = _up;						// Fetched from normals
	MoveCursor(_newPos);

	//-------------------------------------
	//	Project point
	//	Center cursor
	//-------------------------------------

	//----------------------------------------
	// Move Camera to Half Way
	//----------------------------------------
	CVector3 _move = m_halo.m_Pos - m_cView;
	// Minium Speed Check
	if(_move.Magnitude() > 2)
	{
		// Maxium Speed
		if(_move.Magnitude() > 5)
		{
			_move.Normalize();
			_move = _move * 5;
		}

		CamLookAt(m_cView + _move);
	}

	/*=========================================
	*	Get Matrixs
	*=========================================*/
	GLdouble _proj[16];
	GLdouble _model[16];
	GLint _view[4];
	CVector3 _win(0,0,0);

	glGetDoublev(GL_PROJECTION_MATRIX,_proj);
	glGetDoublev(GL_MODELVIEW_MATRIX,_model);
	glGetIntegerv(GL_VIEWPORT,_view);
	

	/*=========================================
	*	Unproject
	*=========================================*/
	gluProject(m_halo.m_Pos.x,m_halo.m_Pos.y,m_halo.m_Pos.z,_model,_proj,_view,&(_win.x),&(_win.y),&(_win.z));
}
//////////////////////////////////////////////////////////////////////////////
//				Set Map Face Normals
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::setMapNorms(CNorm* _norm)
{
	//---------------------------
	//	Delete Old Data
	//---------------------------
	if(m_mapNorms)
	{
		delete[] m_mapNorms;
	}

	m_mapNorms = _norm;
}
//////////////////////////////////////////////////////////////////////////////
//				Set Map Path
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::setPath(MAP_PATH* _path)
{
	m_path = _path;
}
//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##              DESTRUCTION FUNCTIONS                                              ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				DESTROY THE EVIL WINDOWS ENGINE OF SPLOOOOSHIE
//////////////////////////////////////////////////////////////////////////////
WinGLengine::~WinGLengine()
{
	if(m_mapNorms)
		delete[] m_mapNorms;

	DestroyOpenGL();
}

//////////////////////////////////////////////////////////////////////////////
//				ATTEMPT TO KILL THE INVINCIBLE OPEN GL ENGINE
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::DestroyOpenGL()
{
	//---------------------------------------------------
	//	Kill Display Lists
	//---------------------------------------------------
	ResetObjects();
	ResetUnits();
	ResetMeshes();
	ResetTextures();

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
//////////////////////////////////////////////////////////////////////////////
//	Reset Object List
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::ResetObjects()
{
	memset(m_Objects,0,sizeof(m_Objects));
	m_ObjCount = 0;
	return true;
}
//////////////////////////////////////////////////////////////////////////////
//	Reset Texture List
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::ResetTextures()
{
	for(UINT c = 0; c < m_MeshCount; ++c)
	{
		glDeleteTextures(1,&(m_Textures[c].m_id));
	}

	m_TexCount = 0;
	memset(m_Textures,0,sizeof(m_Textures));
	return true;
}
//////////////////////////////////////////////////////////////////////////////
//	Reset Mesh List
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::ResetMeshes()
{
	for(UINT c = 0; c < m_MeshCount; ++c)
	{
		glDeleteLists(m_Meshes[c].m_id,1);
	}

	m_MeshCount = 0;
	memset(m_Meshes,0,sizeof(m_Meshes));
	return true;
}
//////////////////////////////////////////////////////////////////////////////
//	Reset Units List
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::ResetUnits()
{
	memset(m_Objects,0,sizeof(m_Units));
	m_UnitCount = 0;
	return true;
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##              CAMERA FUNCTIONS                                                   ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//		      ROTATE THE CAMERA VIEW
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamRotateView(float _angle, double x, double y, double z)
{
	CVector3 _nView;

	// Get the view vector (The direction we are facing)
	CVector3 _View = m_cView - m_cPosition;		

	// Calculate the sine and cosine of the angle once
	double _cosTheta = cos(_angle);
	double _sinTheta = sin(_angle);

	// Find the new x position for the new rotated point
	_nView.x  = (_cosTheta + (1 - _cosTheta) * x * x)		* _View.x;
	_nView.x += ((1 - _cosTheta) * x * y - z * _sinTheta)	* _View.y;
	_nView.x += ((1 - _cosTheta) * x * z + y * _sinTheta)	* _View.z;

	// Find the new y position for the new rotated point
	_nView.y  = ((1 - _cosTheta) * x * y + z * _sinTheta)	* _View.x;
	_nView.y += (_cosTheta + (1 - _cosTheta) * y * y)		* _View.y;
	_nView.y += ((1 - _cosTheta) * y * z - x * _sinTheta)	* _View.z;

	// Find the new z position for the new rotated point
	_nView.z  = ((1 - _cosTheta) * x * z - y * _sinTheta)	* _View.x;
	_nView.z += ((1 - _cosTheta) * y * z + x * _sinTheta)	* _View.y;
	_nView.z += (_cosTheta + (1 - _cosTheta) * z * z)		* _View.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_cView = m_cPosition + _nView;
}

//////////////////////////////////////////////////////////////////////////////
//		ROTATE THE CAMERA AROUND SOME POINT
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamRotateAroundPoint(CVector3 _point, double _height, double _distance, float _rot)
{
	m_cView = _point;
	m_cPosition.z = _point.z + _height;
	double _distSqr = pow(_distance,2);

	//---------------------------------------
	// Rotation must be between 0 and 1
	// If it overflows we reset it
	// This number represents the percentage
	// of a complete circle around the point
	// We can esaily calculate the position
	// by using ratios and some case functions
	//---------------------------------------
	if(_rot < 0)
		_rot = _rot - floor(_rot);
	if(_rot > 1)
		_rot = _rot - floor(_rot);

	double _ratX = 0.0;
	double _ratY = 0.0;

	// Calculate Ratio
	switch((int)_rot * 4)
	{
	case 0:		
		_ratX = (_rot     ) / 0.25;	
		_ratY = 1.0 - _ratX;
		break;
	case 1:		
		_ratX = (_rot-0.25) / 0.25;	
		_ratY = 1.0 - _ratX;
		_ratY = 0.0 - _ratY;
		break;
	case 2:		
		_ratX = (_rot-0.75) / 0.25;	
		_ratY = 1.0 - _ratX;
		_ratX = 0.0 - _ratX;
		_ratY = 0.0 - _ratY;
		break;
	case 3:		
		_ratX = (_rot-1.00) / 0.25;	
		_ratY = 1.0 - _ratX;
		_ratX = 0.0 - _ratX;
		_ratY = 0.0 - _ratY;
		break;
	}	

	// Add to Final
	m_cPosition.x = m_cPosition.x + sqrt(_ratX * _distSqr);
	m_cPosition.y = m_cPosition.y + sqrt(_ratY * _distSqr);
}
//////////////////////////////////////////////////////////////////////////////
//				CAMERA LOOK!
// This uses gluLookAt() to tell OpenGL where to look
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamLookCur()
{
	m_cView = m_mapCur;
	m_cView.x += 0.5;
	m_cView.y += 0.5;
	m_cView.z += 0.5;
	CamLook();

	/*=========================================
	*	Reset MousePos
	*=========================================*/
	SetCursorPos(403,297);
}
//////////////////////////////////////////////////////////////////////////////
//				CAMERA LOOK!
// This uses gluLookAt() to tell OpenGL where to look
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamLook()
{
	gluLookAt(m_cPosition.x, m_cPosition.y, m_cPosition.z,	
			  m_cView.x,	 m_cView.y,     m_cView.z,	
			  m_cUpVector.x, m_cUpVector.y, m_cUpVector.z);

	MoveCursor(m_mapCur);
}
//////////////////////////////////////////////////////////////////////////////
//				CAMERA LOOK!
// Sets varibles
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamLook(CVector3 _pos)
{
	m_cPosition = _pos;
	CamLook();
}
//////////////////////////////////////////////////////////////////////////////
//				CAMERA LOOK!
// Sets varibles
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamLookAt(CVector3 _view)
{
	m_cView = _view;
	CamLook();
}
//////////////////////////////////////////////////////////////////////////////
//				CAMERA LOOK!
// Sets varibles
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::CamLook(CVector3 _pos, CVector3 _view, CVector3 _up)
{
	m_cPosition = _pos;
	m_cView = _view;
	m_cUpVector = _up;
	CamLook();
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       LOADER FUNCTIONS                                                          ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				ADDS MAP MESH TO LIST
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::AddMap(UINT _MapListID, const char* _tex)
{
	//----------------------------------
	//	Delete Old Map
	//----------------------------------
	if(m_MeshCount > 0)
	{
		strcpy(m_Meshes[0].m_file,"");
		glDeleteLists(m_Meshes[0].m_id,1);
		glDeleteTextures(1,&(m_Textures[0].m_id));

		UINT _temp = m_TexCount;
		m_TexCount = 0;
		AddTexture(_tex);
		m_TexCount = _temp;
	}
	else
	{
		m_MeshCount = 1;
		m_ObjCount = 1;
		AddTexture(_tex);
	}
	//----------------------------------
	//	Update Map List ID
	//----------------------------------
	strcpy(m_Meshes[0].m_file,"MapMesh");
	m_Meshes[0].m_id = _MapListID;
}

//////////////////////////////////////////////////////////////////////////////
//				CREATE TEXTURE
//////////////////////////////////////////////////////////////////////////////
UINT WinGLengine::AddTexture(const char *_filename)
{
	if((m_TexCount == _MAX_TEX_COUNT) || (!_filename))									// Return from the function if no file name was passed in
		return INVALID_PARAM;

	// Add Path to Name
	char* _finalFN = new char[strlen(_filename) + strlen(JWL_FN_TEXTURE)];
	strcpy(_finalFN,JWL_FN_TEXTURE);
	strcat(_finalFN,_filename);

	//------------------------------------------
	// Search for Image Match
	//  - So no skin is added twice
	//------------------------------------------
	for(UINT c = 0; c < m_TexCount; ++c)
	{
		if(_strnicmp(_finalFN,m_Textures[c].m_file,90) == 0)
			return c;
	}
	//------------------------------------------
	// Search for Image Type
	//------------------------------------------
	INT_IMAGE* _Image = NULL;

	//------------------------------------------
	// JPEG (Joint Photographic E Graphics)
	//------------------------------------------
	if(
		(strstr(_finalFN,".jpg")) ||
		(strstr(_finalFN,".JPG")) ||
		(strstr(_finalFN,".jpeg")) ||
		(strstr(_finalFN,".JPEG"))
	   )
	{
		_Image = MyLoaders::LoadJPG(_finalFN);
	}
	//------------------------------------------
	// TGA
	//------------------------------------------
	if(
		(strstr(_finalFN,".tga")) ||
		(strstr(_finalFN,".TGA"))
	   )
	{
		_Image = MyLoaders::LoadTGA(_finalFN);
	}

	if(_Image == NULL)									// If we can't load the file, quit!
	{
		return INVALID_PARAM;
	}

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &(m_Textures[m_TexCount].m_id));
	strcpy(m_Textures[m_TexCount].m_file,_finalFN);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, m_Textures[m_TexCount].m_id);

	// Increment the texture count
	++m_TexCount;


	//------------------------------------
	// This is too fix errors that creep
	// in over distance on map tex
	//------------------------------------
	if(m_TexCount == 1)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _Image->x, _Image->y, GL_RGB, GL_UNSIGNED_BYTE, _Image->m_data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_LINEAR);	
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, _Image->x, _Image->y, GL_RGB, GL_UNSIGNED_BYTE, _Image->m_data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
	}

	// Now we need to free the image data that we loaded since OpenGL stored it as a texture
	if (_Image)										// If we loaded the image
	{
		if (_Image->m_data)							// If there is texture data
		{
			free(_Image->m_data);					// Free the texture data, we don't need it anymore
		}

		free(_Image);								// Free the image structure
		return (m_TexCount-1);
	}
	else
		return INVALID_PARAM;
}
//////////////////////////////////////////////////////////////////////////////
//				CREATE DISPLAY LIST
//////////////////////////////////////////////////////////////////////////////
UINT WinGLengine::AddMesh(const char* _filename)
{	
	if((m_MeshCount == 0) || (m_MeshCount == _MAX_MESH_COUNT) || (strlen(_filename) < 3))
		return INVALID_PARAM;

	//--------------------------------------------
	//	Check File Format
	//--------------------------------------------
	{
		JWL_Mesh* _mesh = 0;

		// Add Path to Name
		char* _finalFN = new char[strlen(_filename) + strlen(JWL_FN_MODEL)];
		strcpy(_finalFN,JWL_FN_MODEL);
		strcat(_finalFN,_filename);

		//------------------------------------------
		// Search for Model Match
		//  - So no mesh is added twice
		//------------------------------------------
		for(UINT c = 0; c < m_MeshCount; ++c)
		{
			if(_strnicmp(_finalFN,m_Meshes[c].m_file,90) == 0)
				return c;
		}

		//-----------------------------
		// WAVEFRONT OBJECT
		//-----------------------------
		if((strstr(_finalFN,".obj")) || (strstr(_finalFN,".OBJ")))
			_mesh = MyLoaders::LoadWaveObj(_finalFN);

		//-----------------------------
		// 3D STUDIO OBJECT
		//-----------------------------
		if((strstr(_finalFN,".3ds")) || (strstr(_finalFN,".3DS")))
			_mesh = MyLoaders::Load3DS(_finalFN);

		if(_mesh)
		{
			// ADD MESH
			UINT _ret = AddMesh(*(_mesh));
			strcpy(m_Meshes[(m_MeshCount - 1)].m_file,_finalFN);
			delete _mesh;
			return _ret;
		}
	}

	return INVALID_PARAM;
}
//////////////////////////////////////////////////////////////////////////////
//				CREATE DISPLAY LIST FROM MESH
//////////////////////////////////////////////////////////////////////////////
UINT WinGLengine::AddMesh(const JWL_Mesh& _Mesh)
{
	// Generate list and place it in array
	if((m_MeshCount == 0) || (m_MeshCount == _MAX_MESH_COUNT) || (_Mesh.m_size < 3))
        return INVALID_PARAM;

	m_Meshes[m_MeshCount].m_id = glGenLists(1);

	//-----------------------------------------------------------
	//             Make the List
	//-----------------------------------------------------------
	glNewList(m_Meshes[m_MeshCount].m_id,GL_COMPILE);
	glPushMatrix();
		glEnable(GL_CULL_FACE);

		// Enable Clients to Recieve Data	
		glInterleavedArrays(GL_T2F_N3F_V3F,0,_Mesh.m_array);

		glDrawArrays(_Mesh.m_mode, 0, _Mesh.m_size);				// Draw All Of The Quads At Once

		glCullFace(GL_BACK);
	glPopMatrix();
	glEndList();

	// Increment the mesh count
	++m_MeshCount;

	return (m_MeshCount-1);
}
//////////////////////////////////////////////////////////////////////////////
//				CREATE LIGHT
//////////////////////////////////////////////////////////////////////////////
UINT WinGLengine::AddLight(INT_LIGHT& _Light)
{
	//-----------------------------
	// Fetch and Set Varibles
	//-----------------------------
	glLightfv(GL_LIGHT0 + m_LightCount, GL_AMBIENT, _Light.m_ambient);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_DIFFUSE, _Light.m_diffuse);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_SPECULAR, _Light.m_specular);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_POSITION, _Light.m_position);

	glLightfv(GL_LIGHT0 + m_LightCount, GL_CONSTANT_ATTENUATION, &_Light.m_constant_attenuation);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_LINEAR_ATTENUATION, &_Light.m_linear_attenuation);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_QUADRATIC_ATTENUATION, &_Light.m_quadratic_attenuation);

	glLightfv(GL_LIGHT0 + m_LightCount, GL_SPOT_DIRECTION, _Light.m_spot_direction);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_SPOT_EXPONENT, &_Light.m_spot_exponent);
	glLightfv(GL_LIGHT0 + m_LightCount, GL_SPOT_CUTOFF, &_Light.m_spot_cutoff);

	//-----------------------------
	// Enable Light
	//-----------------------------
	glEnable(GL_LIGHT0 + m_LightCount);

	++m_LightCount;
	return (m_LightCount-1);
}

//////////////////////////////////////////////////////////////////////////////
//				CREATE OBJECT
//////////////////////////////////////////////////////////////////////////////
UINT WinGLengine::AddObject(UINT _meshID, UINT _texID, CVector3 _pos, CVector3 _up, float _face)
{
	//-------------------------------
	// Check Values and Overflow
	//-------------------------------
	if((_meshID < m_MeshCount) && (_texID < m_TexCount) && (m_ObjCount < _MAX_OBJ_COUNT))
	{
		//-------------------------------
		//	Check for DUFFS
		//-------------------------------
		if((_meshID == INVALID_PARAM) || (_texID == INVALID_PARAM))
			return 0;
		//-------------------------------
		// Create Object
		//-------------------------------
		m_Objects[m_ObjCount].m_MeshID	= _meshID;
		m_Objects[m_ObjCount].m_TexID	= _texID;
		m_Objects[m_ObjCount].m_Pos		= _pos;
		m_Objects[m_ObjCount].m_Up		= _up;
		m_Objects[m_ObjCount].m_Face	= _face;

		++m_ObjCount;
		return (m_ObjCount-1);
	}
	else
		return 0;
}
//////////////////////////////////////////////////////////////////////////////
//				CREATE UNIT
//////////////////////////////////////////////////////////////////////////////
UINT WinGLengine::AddUnit(UINT _meshID, UINT _texID, UINT _team, CVector3 _pos, CVector3 _up, float _face)
{
	//-------------------------------
	// Check Values and Overflow
	//-------------------------------
	if((_meshID < m_MeshCount) && (_texID < m_TexCount) && (m_UnitCount < _MAX_UNIT_COUNT))
	{
		//-------------------------------
		//	Check for DUFFS
		//-------------------------------
		if((_meshID == INVALID_PARAM) || (_texID == INVALID_PARAM))
			return INVALID_PARAM;
		//-------------------------------
		// Create Object
		//-------------------------------
		memset(&(m_Units[m_UnitCount]),0,sizeof(OGL_UNIT));
		OGL_UNIT& _unit = m_Units[m_UnitCount];
		_unit.m_MeshID	= _meshID;
		_unit.m_TexID	= _texID;
		_unit.m_Pos		= _pos;
		_unit.m_Up		= _up;
		_unit.m_Face	= _face;
		_unit.m_Team	= _team;

		++m_UnitCount;
		return (m_UnitCount-1);
	}
	else
		return INVALID_PARAM;
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       THE BIG SUPER IMPORTANT RENDER FUNCTION                                   ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				MAIN RENDER FUNCTION
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::Render()
{
	//-------------------------------
	// Reset Scene
	//-------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix
	CamLook();

	//-------------------------------
	// Draw Lists
	//-------------------------------
	DrawMap();
	if(m_path) DrawPath();
	DrawObjectList();
	DrawUnitList();
	DrawMapCursor();
	DrawUnitCursor();

	//-------------------------------
	// Draw Font
	//-------------------------------
	MAP_POS _curr = {(UINT)m_mapCur.x,(UINT)m_mapCur.y,(UINT)m_mapCur.z};
	DrawRasterFont(5,_J_FONT_HEIGHT*1,"Cur Pos [%u:%u:%u]",_curr.x,_curr.y,_curr.z);
	DrawRasterFont(5,_J_FONT_HEIGHT*2,"Unit [%u]",m_unitCur);
	DrawRasterFont(5,_J_FONT_HEIGHT*3,m_string);

	return (SwapBuffers(m_hDevCon) > 0);								// Swap Buffers
}

//////////////////////////////////////////////////////////////////////////////
//				DRAW MAP
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawMap()
{
	glPushMatrix();
		//----------------------------
		//	Draw Textured Map
		//----------------------------
		glBindTexture(GL_TEXTURE_2D,m_Textures[0].m_id);								// Binds Texture
		glCallList(m_Meshes[0].m_id);													// Call Display List
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////
//				DRAW OBJECT LIST
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawObjectList()
{
	for(UINT c = 1; c < m_ObjCount; ++c)
	{
		glPushMatrix();
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
		{
			OGL_OBJ& _obj = m_Objects[c];
			//----------------------------------
			//	Texture
			//----------------------------------
			if(m_Textures[_obj.m_TexID].m_id > 0)
			{
				glBindTexture(GL_TEXTURE_2D,m_Textures[_obj.m_TexID].m_id);				// Binds Texture
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
				glColor3f(1.0f,1.0f,1.0f);
			}
			//-----------------------------------
			//	Position
			//-----------------------------------
			glTranslated(_obj.m_Pos.x,_obj.m_Pos.y,_obj.m_Pos.z);					// Translate Object to Point
			glRotated((_obj.m_Face)*360.0,_obj.m_Up.x,_obj.m_Up.y,_obj.m_Up.z);

			//-----------------------------------
			//	Draw Mesh
			//-----------------------------------
			if(m_Meshes[_obj.m_MeshID].m_id > 0)
				glCallList(m_Meshes[_obj.m_MeshID].m_id);								// Call Display List
		}
		glPopAttrib();
		glPopMatrix();
	}
}
//////////////////////////////////////////////////////////////////////////////
//				DRAW UNIT LIST
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawUnitList()
{
	for(UINT c = 0; c < m_UnitCount; ++c)
	{
		glPushMatrix();
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
		{
			OGL_UNIT& _unit = m_Units[c];
			//----------------------------------
			//	Texture
			//----------------------------------
			if(m_Textures[_unit.m_TexID].m_id > 0)
			{
				glBindTexture(GL_TEXTURE_2D,m_Textures[_unit.m_TexID].m_id);			// Binds Texture
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
				glColor3f(1.0f,1.0f,1.0f);
			}
			//-----------------------------------
			//	Position
			//-----------------------------------
			glTranslated(_unit.m_Pos.x,_unit.m_Pos.y,_unit.m_Pos.z);					// Translate Object to Point
			glRotatef((_unit.m_Face)*180.0f,0.0f,0.0f,1.0f);

			//-----------------------------------
			//	Draw Mesh
			//-----------------------------------
			if(m_Meshes[_unit.m_MeshID].m_id > 0)
				glCallList(m_Meshes[_unit.m_MeshID].m_id);								// Call Display List
		}
		glPopAttrib();
		glPopMatrix();
	}
}
//////////////////////////////////////////////////////////////////////////////
//				DRAW PATH
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawPath()
{
	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
	{
		glDisable(GL_DITHER);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		const MAP_PATH* _curr = m_path;
		while(_curr->m_next)
		{
			float _p[3] = 
			{
				 (float)(_curr->m_pos.x) + 0.5f,
				 (float)(_curr->m_pos.y) + 0.5f,
				((float)(_curr->m_pos.z) * 0.2f) + 0.5f
			};
			float _d[3] =
			{
				 (float)(_curr->m_next->m_pos.x) + 0.5f,
				 (float)(_curr->m_next->m_pos.y) + 0.5f,
				((float)(_curr->m_next->m_pos.z) * 0.2f) + 0.5f
			};
			
			glBegin(GL_LINES);
				glVertex3fv(_p);
				glVertex3fv(_d);
			glEnd();

			_curr = _curr->m_next;
		}
	}
	glPopAttrib();
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////
//		DRAW MAP CURSOR
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawMapCursor()
{
	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
	{
		glDisable(GL_DITHER);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTranslated(m_mapCur.x,m_mapCur.y,m_mapCur.z);
		glLineWidth(4.0f);
		glColor3d(1.0,1.0,1.0);
		glBegin(GL_LINE_LOOP);
		{
			glVertex3d(0,0,0.002);
			glVertex3d(1,0,0.002);
			glVertex3d(1,1,0.002);
			glVertex3d(0,1,0.002);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		{
			glVertex3d(0,0,1.002);
			glVertex3d(1,0,1.002);
			glVertex3d(1,1,1.002);
			glVertex3d(0,1,1.002);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex3d(0,0,0.002);			glVertex3d(0,0,1.002);
			glVertex3d(0,1,0.002);			glVertex3d(0,1,1.002);
			glVertex3d(1,0,0.002);			glVertex3d(1,0,1.002);
			glVertex3d(1,1,0.002);			glVertex3d(1,1,1.002);
		}
		glEnd();
	}
	glPopAttrib();
	glPopMatrix();
}
//////////////////////////////////////////////////////////////////////////////
//		DRAW UNIT CURSOR
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawUnitCursor()
{
	if(m_Units[m_unitCur].m_MeshID == 0)
		return;

	OGL_UNIT& _unit = m_Units[m_unitCur];

	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
	{
		glDisable(GL_DITHER);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glTranslated(_unit.m_Pos.x,_unit.m_Pos.y,_unit.m_Pos.z  + 0.1);
		glLineWidth(4.0f);
		switch(_unit.m_Team)
		{
			case 0:	glColor4d(TC_1,0.3); break;
			case 1:	glColor4d(TC_2,0.3); break;
			case 2:	glColor4d(TC_3,0.3); break;
			case 3:	glColor4d(TC_4,0.3); break;
			case 4:	glColor4d(TC_5,0.3); break;
			case 5:	glColor4d(TC_6,0.3); break;
			case INVALID_PARAM:	glColor4d(1.0,1.0,1.0,0.2); break;
		};
		
		glBegin(GL_POLYGON);
		{
			glVertex3d( 0.0, 0.6, 0.002);
			glVertex3d( 0.4, 0.4, 0.002);
			glVertex3d( 0.6, 0.0, 0.002);
			glVertex3d( 0.4,-0.4, 0.002);
			glVertex3d( 0.0,-0.6, 0.002);
			glVertex3d(-0.4,-0.4, 0.002);
			glVertex3d(-0.6, 0.0, 0.002);
			glVertex3d(-0.4, 0.4, 0.002);
		}
		glEnd();
	}
	glPopAttrib();
	glPopMatrix();
}
//////////////////////////////////////////////////////////////////////////////
//		DRAW MAP CURSOR
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawMouse()
{
	glPushMatrix();
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);

		glDisable(GL_DITHER);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glTranslated(m_mouse.x,m_Height - m_mouse.y,0);
		glBegin(GL_POLYGON);
			glColor3d(1,1,1); glVertex3d(00.0,-00.0,0);
			glColor3d(0,0,1); glVertex3d(50.0,-25.0,0);
			glColor3d(0,1,0); glVertex3d(25.0,-25.0,0);
			glColor3d(1,0,0); glVertex3d(25.0,-50.0,0);
		glEnd();

	glPopAttrib();
	glPopMatrix();
}
//////////////////////////////////////////////////////////////////////////////
//				Draw Raster Font
//////////////////////////////////////////////////////////////////////////////
void WinGLengine::DrawRasterFont(UINT x, UINT y, const char* _format, ...)
{
	//---------------------------------
	//  Generate Text
	//---------------------------------
	if(_format == NULL)		return;

	char _input[_MAX_CHAR] = {0};
	va_list _argPtr;

	va_start(_argPtr, _format);		
	vsprintf(_input, _format, _argPtr);	
	va_end(_argPtr);

	//---------------------------------
	//  Disable Wanted Flags
	//---------------------------------
	glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DITHER);

		//---------------------------------
		//  Position Text Cursor
		//---------------------------------
		glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
			// Here we use a new projection and modelview matrix to work with.
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
				glLoadIdentity();
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();
					glLoadIdentity();

					//----------------------------------------------
					// Calculate the weird screen position
					//----------------------------------------------
					y = m_Height - 35 - _J_FONT_HEIGHT - y;				

					glViewport( x - 1, y - 1, 0, 0 );					// Create a new viewport to draw into
					glRasterPos4f( 0, 0, 0, 1 );						// Set the drawing position

					glPopMatrix();
				glMatrixMode( GL_PROJECTION );
			glPopMatrix();
		glPopAttrib();

		//---------------------------------
		// Call Lists
		//---------------------------------
		glPushMatrix();
			glColor3f(1.0f,1.0f,1.0f);
			glListBase(m_FontID);
			glCallLists((int)strlen(_input), GL_UNSIGNED_BYTE, _input);
		glPopMatrix();	

	//---------------------------------
	//  Renable Flags
	//---------------------------------
	glPopAttrib();
}
//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       ACCESSOR FUNCTIONS TO GET STUFF THATS PRIVATE                             ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				Is Fullscreen ?
//////////////////////////////////////////////////////////////////////////////
bool WinGLengine::isFullscreen() const
{
	return m_Fullscreen;
}
//////////////////////////////////////////////////////////////////////////////
//				Object Accesors
//////////////////////////////////////////////////////////////////////////////
CVector3 WinGLengine::getObjPos(UINT _ObjID) const
{
	if(_ObjID < m_ObjCount)
		return m_Objects[_ObjID].m_Pos;
	return CVector3(0,0,0);
}
//////////////////////////////////////////////////////////////////////////////
//				Other Accesors
//////////////////////////////////////////////////////////////////////////////
const CVector3& WinGLengine::getMapPos() const
{
	return m_mapCur;
}
//////////////////////////////////////////////////////////////////////////////
//				Camera Accesors
//////////////////////////////////////////////////////////////////////////////
const CVector3& WinGLengine::getCamPos() const
{
	return m_cPosition;
}
//----------------------------------------------------------------------------
const CVector3& WinGLengine::getCamView() const
{
	return m_cView;
}
//----------------------------------------------------------------------------
const CVector3& WinGLengine::getCamUp() const
{
	return m_cUpVector;
}