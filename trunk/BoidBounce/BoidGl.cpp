#include "BoidGL.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		The Creation and Destruction of Boid Bounce
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================================
//	Boid Bounce Constructor
//	The reason for the extensively long constructor init list is simply this
//	You need to use the basic constructors before the scope or the constructor will attempt to do the
//	job for you create wasted memory space and processor time.
//
//	A not very well known bug however occurs. The local varible namespace is not into scope until the start
//	of the scope so for those reasons. Things like surface which refrence engine have to be constructed in
//	scope
//
//	You will also notice the repeated passing of varibles. While this is slightly inefficient it avoid the
//	Spaggetti code and mantains OO interface.
//=========================================================================================================
BoidBounce::BoidBounce(HINSTANCE _Instance, HWND _Wnd, UINT _height, UINT _width) :
m_WinHeight(_height),
m_WinWidth(_width), 
m_Instance(_Instance),
m_Window(_Wnd),
m_mouseStart(CVector3(0,0,0)),
m_mouseDir(CVector3(0,0,0))
{
	//---------------------------------
	//	Set up Rotional Axis
	//---------------------------------
	m_xAxis = 0.0f;
	m_yAxis = 0.0f;
	m_zAxis = 0.0f;

	m_xSize = 100;
	m_ySize = 100;
	m_zSize = 100;

	//---------------------------------
	//	Setup Sub Systems
	//---------------------------------
	m_enginePTR = new WinGLengine(_Instance,_Wnd,_height, _width);
	m_drawCubePTR = new BGLDrawCube(CVector3(m_xSize,m_ySize,m_zSize));
	m_drawTubePTR = new BGLDrawTube(CVector3(0,0,0),CVector3(0,0,0),1.0f);
	m_rasFontPTR = new BGLRasterFont(*m_enginePTR);

	m_flockPTR = new bFlock3(75,CVector3(m_xSize,m_ySize,m_zSize));
	m_drawFlockPTR = new BGLDrawBoid(*m_enginePTR, *m_flockPTR);

	//---------------------------------
	//	Load Textures
	//---------------------------------
	m_textures[0].Load("colour.bmp");
	m_textures[1].Load("point.bmp");
	m_textures[2].Load("hazard.bmp");

	//---------------------------------
	//	Setup Drawing Systems
	//---------------------------------
	m_drawCubePTR->setInside(true);
	m_flockPTR->setAlign(1.0f);
	m_flockPTR->setAvoid(5.0f);

	//---------------------------------
	//	Randomly Init Boids
	//---------------------------------
	m_flockPTR->setWrapped(false);
	m_flockPTR->RandomBoid();
}
//=========================================================================================================
//	Deconstructor
//	Thankfully there is very little clean up as the objects clean up after themselves
//=========================================================================================================
BoidBounce::~BoidBounce()
{
	if(m_flockPTR)  	delete m_flockPTR;
	if(m_drawTubePTR)	delete m_drawTubePTR;
	if(m_drawCubePTR)	delete m_drawCubePTR;
	if(m_rasFontPTR)	delete m_rasFontPTR;	
	if(m_enginePTR)		delete m_enginePTR;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Boid Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================================
//	Basic Step in Game Including Render
//=========================================================================================================
void BoidBounce::GameStep()
{
	m_flockPTR->Update();
	m_flockPTR->Move(0.1f);
	//-------------------------------
	// Reset Scene
	//-------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix


	//-------------------------------
	//	Draw Cube and Flock
	//-------------------------------
	glPushMatrix();
		glTranslatef(0.0,0.0, -20.0);
		//----------------------------------------
		// Rotate
		//----------------------------------------
		glRotatef(m_xAxis, 1.0, 0.0, 0.0);
		glRotatef(m_yAxis, 0.0, 1.0, 0.0);
		glRotatef(m_zAxis, 0.0, 0.0, 1.0);

		//----------------------------------------
		//	Scale and Translate
		//----------------------------------------
		glScalef(0.1f,0.1f,0.1f);
		glTranslatef(-50.0f,-50.0f,0);		

		//----------------------------------------
		//	Draw Cube
		//----------------------------------------
		m_textures[0].BindTexture();
		m_drawCubePTR->Render();

		//----------------------------------------
		//	Draw Flock
		//----------------------------------------
		m_textures[1].BindTexture();
		m_drawFlockPTR->Render();

		//----------------------------------------
		//	Draw Tube
		//----------------------------------------
		m_textures[2].BindTexture();
		m_drawTubePTR->Render();

	glPopMatrix();

	(SwapBuffers(m_enginePTR->getDeviceCon()) > 0);				// Swap Buffers
}
//=========================================================================================================
//	Basic Step in Game Including Render
//=========================================================================================================
void BoidBounce::KeyPressed(WPARAM _key)
{
	switch(_key)
	{
	//---------------------------------------------
	//	Escape Key - EXIT
	//---------------------------------------------
	case VK_ESCAPE:			PostQuitMessage(0);				break;
	//---------------------------------------------
	//	Axis Rotation Keys for Debugging
	//---------------------------------------------
	case 'W':	m_xAxis += 10.0f;		break;
	case 'S':	m_xAxis -= 10.0f;		break;
	case 'A':	m_zAxis += 10.0f;		break;
	case 'D':	m_zAxis -= 10.0f;		break;
	};
}
//=========================================================================================================
//	Mouse Move in Game Window
//=========================================================================================================
void BoidBounce::MouseMove(WPARAM _wPar, LPARAM _lPar)
{
	//-----------------------------
	//	Fetch Mouse Coords
	//	Correct for Title Bar
	//-----------------------------
	UINT _pos[2] = {(UINT)LOWORD(_lPar),(UINT)HIWORD(_lPar)};
	_pos[1] += 35;
	//---------------------------------------
	//	Measure Change
	//---------------------------------------
	CVector3 _dir = m_mouseDir - CVector3(_pos[0],_pos[1],0.0f);
	_dir.x = 0 - _dir.x;
	m_mouseDir = CVector3(_pos[0],_pos[1],0.0f);
	m_mouseStart += _dir;

	//---------------------------------------
	//	Check in Bounds
	//---------------------------------------
	if(m_mouseStart.x < 0)			m_mouseStart.x = 0;
	if(m_mouseStart.y < 0)			m_mouseStart.y = 0;
	if(m_mouseStart.x > m_xSize)	m_mouseStart.x = float(m_xSize);
	if(m_mouseStart.y > m_ySize)	m_mouseStart.y = float(m_ySize);

	//---------------------------------------
	//	Update
	//---------------------------------------
	m_flockPTR->setMouse(m_mouseStart,m_mouseDir);
	m_drawTubePTR->setStart(m_mouseStart);

	/*
	THIS IS OLD RAY TRACING METHOD
	//---------------------------------------------
	//	Set Up storage Locations
	//---------------------------------------------
	//	The reason we are using doubles is
	//	that the UnProject has to work with
	//	doubles and this function has to be
	//	fast so we want to cut down conversion
	//	time.
	//---------------------------------------------
	GLdouble _proj[16];
	GLdouble _model[16];
	GLint _view[4];

	GLdouble _near[3] = {0,0,0};
	GLdouble _farr[3] = {0,0,0};

	//---------------------------------------------
	//	Get Matrices
	//---------------------------------------------
	glGetDoublev(GL_PROJECTION_MATRIX,_proj);
	glGetDoublev(GL_MODELVIEW_MATRIX,_model);
	glGetIntegerv(GL_VIEWPORT,_view);

	//---------------------------------------------
	//	Unproject
	//	Dont forget to flip Y
	//---------------------------------------------
	gluUnProject(_pos[0],_view[3]-_pos[1],  0.0,_model,_proj,_view,&(_near[0]),&(_near[1]),&(_near[2]));
	gluUnProject(_pos[0],_view[3]-_pos[1],100.0,_model,_proj,_view,&(_farr[0]),&(_farr[1]),&(_farr[2]));

	//---------------------------------------------
	//	Update Tube and Varibles
	//---------------------------------------------
	m_mouseStart = CVector3(_near[0],_near[1],_near[2]);
	CVector3 _end = CVector3(_farr[0],_farr[1],_farr[2]);
	m_mouseDir = m_mouseStart - _end;

	m_drawTubePTR->setStart(m_mouseStart);
	m_drawTubePTR->setEnd(_end);

	//---------------------------------------------
	//	Transform to BOX space
	//	NO changes to projetion matrix
	//---------------------------------------------
	glPushMatrix();
		glLoadIdentity();
		//----------------------------------
		//	Match to BOX Space
		//	Inverting the operations
		//	by doing them in reverse order
		//----------------------------------
		glTranslatef( 50.0f, 50.0f,0);
		glScalef(10.0f,10.0f,10.0f);
		glRotatef(m_zAxis, 0.0, 0.0, 1.0);
		glRotatef(m_yAxis, 0.0, 1.0, 0.0);
		glRotatef(m_xAxis, 1.0, 0.0, 0.0);

		glTranslatef(0.0,0.0, 20.0);

		//---------------------------------------
		//	Get Inverted Matrix
		//---------------------------------------
		glGetDoublev(GL_MODELVIEW_MATRIX,_model);
        
		//--------------------------------------
		//	Get Transformed Mouse Start
		//--------------------------------------
		CVector3 _newStart = m_mouseStart;
		_newStart.x = (_newStart.x * _model[0]) + (_newStart.y * _model[1]) +  (_newStart.z * _model[2]) +  (_model[3]);
		_newStart.y = (_newStart.x * _model[4]) + (_newStart.y * _model[5]) +  (_newStart.z * _model[6]) +  (_model[7]);
		_newStart.z = (_newStart.x * _model[8]) + (_newStart.y * _model[9]) + (_newStart.z * _model[10]) + (_model[11]);

		//--------------------------------------
		//	Get Transformed Mouse Direction
		//--------------------------------------
		CVector3 _newDir = m_mouseDir;
		_newDir.x = (_newDir.x * _model[0]) + (_newDir.y * _model[1]) +  (_newDir.z * _model[2]) +  (_model[3]);
		_newDir.y = (_newDir.x * _model[4]) + (_newDir.y * _model[5]) +  (_newDir.z * _model[6]) +  (_model[7]);
		_newDir.z = (_newDir.x * _model[8]) + (_newDir.y * _model[9]) + (_newDir.z * _model[10]) + (_model[11]);

	glPopMatrix();

	m_flockPTR->setMouse(_newStart,_newDir);
	*/
}