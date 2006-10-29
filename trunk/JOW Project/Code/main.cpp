#include <windows.h>
#include <stdio.h>
#include "main.h"

//=========================================================================================================
//        MAIN WINDOWS FUNCTION OF ANNOANCE
//=========================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//----------------------------------
	//	Create Window
	//----------------------------------
	HWND        hwnd = 0;									// HWND - a HWND is a handle to a window.
	CreateWin(hInstance, hwnd, iCmdShow);

	//----------------------------------
	//	Menu Creation
	//----------------------------------
	JWL_CHOICE hChoice = {0};

	hChoice.m_map = 1;
	hChoice.m_ShowObjs = true;
	hChoice.m_ShowUnits = true;
	strcpy(hChoice.m_scence,"BigGeyser.scn");

	GameLoop(hInstance, hwnd, hChoice);
	//----------------------------------
	// Game Creation
	//----------------------------------
	// GameLoop(hInstance, hwnd, hChoice);

	//----------------------------------
	// Destruction
	//----------------------------------
	UnregisterClass(_J_SHORTNAME,hInstance);
	return 0;
}

//=========================================================================================================
//		CREATES WINDOW
//=========================================================================================================
void CreateWin(HINSTANCE& hInstance, HWND& hwnd, int iCmdShow)
{
	//----------------------------------------
	// Window Creation
	//----------------------------------------
    WNDCLASSEX  wndclass;									// WNDCLASSEX - This variable will hold all the information about the window

    wndclass.cbSize        = sizeof(wndclass);				// Here we set the size of the wndclass. You will see this a lot in windows, it's kinda odd.  We use the "sizeof()" function to tell windows the size of our class.
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;		// The style we want is Verticle-Redraw and Horizontal-Redraw
    wndclass.lpfnWndProc   = WndProc;						// Here is where we assing our CALLBACK function.  Remember up above our function "WndProc"?  This just tells windows which function it needs to call to check for window messages.
    wndclass.cbClsExtra    = 0;								// We don't want to allocate any extra bytes for a class (useless for us)
    wndclass.cbWndExtra    = 0;								// Another useless thing for us.  I believe these last 2 are initialized to 0 anyway.
    wndclass.hInstance     = hInstance;						// We assign our hInstance to our window.  Once again, You can have several instances of a program, so this keeps track of the current one.
    wndclass.hIcon         = LoadIcon (NULL, IDI_WINLOGO);
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = _J_SHORTNAME;
    wndclass.hIconSm       = LoadIcon (NULL, IDI_WINLOGO);

	RegisterClassEx(&wndclass);

    hwnd = CreateWindow(_J_SHORTNAME, _J_LONGNAME,0,0,0,_J_DEFWIDTH,_J_DEFHEIGHT,NULL,NULL,hInstance,NULL);								// If we wanted to pass in a variable or structure to the "WndProc", we would do it here.  (I just prefer using global variables though).  I just pass in NULL.

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

	ShowCursor(1);
}

//=========================================================================================================
//		ACTUAL GAME RUNNING
//=========================================================================================================
void GameLoop(HINSTANCE& hInstance, HWND& hwnd, JWL_CHOICE& hChoice)
{
	//----------------------------------
	// Create Graphic Envrioment
	//----------------------------------
	WinGLengine _Gui(hInstance,hwnd);						
	
	//----------------------------------
	// Create Game Scenario
	//----------------------------------
	JWL_Scenario _Scene(hChoice.m_scence);
	g_Help = new JWL_Helpers(_Scene,_Gui);

	//----------------------------------
	// Build Map Mesh
	//----------------------------------
	switch(hChoice.m_map)
	{
		case 1:		g_Help->BuildLogicMesh();			break;
		case 2:		g_Help->BuildMapMesh();				break;
		case 3:		g_Help->BuildStripMesh();			break;
		case 4:		g_Help->BuildQuadMesh();			break;
	};

	//----------------------------------
	// Transfer Objects
	//----------------------------------
	if(hChoice.m_ShowObjs)
		g_Help->ObjectTransfer();

	//----------------------------------
	// Transfer Units
	//----------------------------------
	if(hChoice.m_ShowUnits)
		g_Help->AddUnitMeshes();

	// Set Camera
	_Gui.CamLook(CVector3(2,2,20),CVector3(20,20,8),_Gui.getCamUp());

	//----------------------------------
	// Game Loop
	//----------------------------------
	MSG _msg = {0};										// MSG  - A MeSsaGe variable to hold what messages are being sent to the window
	while(true)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(_msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&_msg);					// Find out what the message does
            DispatchMessage(&_msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			g_Help->ProcessCommands();
			_Gui.Render();			
        } 
	}
}
//=========================================================================================================
//		MENU SYSTEM
//=========================================================================================================
void MenuLoop(HINSTANCE& hInstance, HWND& hwnd, JWL_CHOICE& hChoice)
{
	//----------------------------------
	//	Create OpenGL
	//----------------------------------
	MenuGL _menu(hInstance,hwnd);

	//----------------------------------
	//	Menu Loop
	//----------------------------------
	while(true)
	{
		_menu.Render();
	}
}


//=========================================================================================================
//        WINDOWS MESSAGE HANDLER
//=========================================================================================================
LRESULT CALLBACK WndProc (HWND _hWnd, UINT _iMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_Help == 0)
		return DefWindowProc(_hWnd, _iMsg, wParam, lParam);
	//----------------------------------
	// Get Mouse Pos
	// If NOT full screen then adjust
	// to compensate for window border
	//----------------------------------
	UINT_POINT _pos = {GET_X_LPARAM,GET_Y_LPARAM};
	if(!(g_Help->m_Gui.isFullscreen()))
        _pos.y += 35;

	//----------------------------------
	// Switch Message Type
	//----------------------------------
    switch (_iMsg)
    {
		case WM_SIZE:
			switch(wParam)
			{
			// Message is sent to all pop-up windows when some other window is maximized.
			case SIZE_MAXHIDE:
				// Pause Game
				// Stop Rendering
				return NULL;

			// The window has been maximized.
			case SIZE_MAXIMIZED:
				// Switch to Fullscreen Mode
				return NULL;

			// Message is sent to all pop-up windows when some other window has been restored to its former size.
			case SIZE_MAXSHOW:
				// Resume Game and Rendering
				return NULL;

			// The window has been minimized.
			case SIZE_MINIMIZED:
				// Pause Game
				// Stop Rendering
				return NULL;

			// The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
			case SIZE_RESTORED:
				// Do Not Allow
				return NULL;
			};
			return NULL;	// IGNORE RESIZING
			break;

		case WM_PAINT:
			// NOT USED
		    break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		//-------------------------------------------------
		//	Check for 'special Keys'
		//-------------------------------------------------
		case WM_KEYUP:
			switch(wParam)
			{
			case VK_ESCAPE:
				{
					PostQuitMessage(0);
					break;
				}
			};
			return g_Help->Parse(_hWnd, _iMsg, wParam, lParam);

		//----------------------------------
		// If Mouse Middle Button Then
		// Then Cam by Mouse
		//----------------------------------
		case WM_MBUTTONDOWN:
		//case WM_MBUTTONUP:
			g_Help->m_Gui.CamLookCur();
			return DefWindowProc(_hWnd, _iMsg, wParam, lParam);		
		case WM_MOUSEWHEEL:
			{
				int _delta = GET_WHEEL_DELTA_WPARAM;
				CVector3 _newPos = g_Help->m_Gui.getCamPos();
				CVector3 _view = g_Help->m_Gui.getCamView();
				CVector3 _dir = (_view - _newPos);
				//-----------------------------------
				//	Check for overrun
				//-----------------------------------
				if((_dir.Magnitude() < 5.0f) && (_delta > 0))
				{
					return DefWindowProc(_hWnd, _iMsg, wParam, lParam);
				}
				else
				{
					_dir = _dir * _delta;
					_dir.Normalize();
					_newPos = _newPos + _dir;
					g_Help->m_Gui.CamLook(_newPos);
					return DefWindowProc(_hWnd, _iMsg, wParam, lParam);
				}
			}
		//----------------------------------
		// If Mouse Moved then Update Cursor
		//----------------------------------
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			break;
		case WM_MOUSEMOVE:
			{
				g_Help->ConvertMousePos(_pos);
				return g_Help->Parse(_hWnd, _iMsg, wParam, lParam);
			}
	}													

	//----------------------------------
	// If a Scence is online then
	// Pass Keys on to it else let
	// windows take them
	//----------------------------------
	return g_Help->Parse(_hWnd, _iMsg, wParam, lParam);
}