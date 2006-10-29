/*----------------------------------------------------------------------------------------------------
				Game Framework - Claire Blackshaw
	I'm not going to try track versions lets just call this version 1. If a magor overhaul happens
	I'll rename version. The truth is this is from several years of writing I finnally think I'm
	ready to write a universal framework which I can use. This is for personal use but I'll be
	commenting it the whole way and following strict documentation procedure just to make life easier.

	Okay this file should remain UNCHANGED bar two or three lines of code. Basically this file deals
	with windows.
----------------------------------------------------------------------------------------------------*/
//=======================================================
//	DEFINES
//=======================================================
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable : 4996) // Disables silly warning about win "security"

//=======================================================
//	Headers
//	Include only your base header
//=======================================================
#include <windows.h>
#include "GLHeader.h"
#include "Map\PHMap.h"
#include "ICE\comBase.h"

//=======================================================
//	Predeclaration
//=======================================================
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _iMsg, WPARAM wParam, LPARAM lParam);
void CreateWin(HINSTANCE& hInstance, HWND& hwnd, int iCmdShow);

//=======================================================
//	Global Varible
//=======================================================
int g_State = -1;
std::vector<gameBlock*> g_BlockPTR;
unsigned long x_delta = 1;

//=========================================================================================================
//        MAIN WINDOWS FUNCTION OF ANNOANCE
//=========================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//----------------------------------
	//	Seed Random
	//----------------------------------
	unsigned long _lastTick = GetTickCount();
	srand(_lastTick);
	//----------------------------------
	//	Create Window
	//----------------------------------
	HWND      _hwnd = 0;									// HWND - a HWND is a handle to a window.
	CreateWin(hInstance, _hwnd, iCmdShow);
	//--------------------------------------------
	//	Start OpenGL
	//--------------------------------------------
	PH_ENGINE->Load(hInstance, _hwnd, WINDOW_HEIGHT, WINDOW_WIDTH);
	PH_LOG->Load(PH_ENGINE,"Arial",8,10,10,0);
	PH_LOG->PrintS(LG_SYSTEM_REPORT,"Log Started : %d", GetTickCount());
	//--------------------------------------------
	//	Initialize Blocks
	//--------------------------------------------
	g_BlockPTR.push_back(new iceBlock());
	g_BlockPTR[0]->Open();

	g_State = 0;					// Set State as ZERO

	//----------------------------------
	//	Main Loop
	//----------------------------------
	MSG _msg = {0};										// MSG  - A Msg variable to hold what messages are being sent to the window
	while(true)											// Do our infinate loop
	{
		//--------------------------------
		// Loop through all the messages
		//--------------------------------
		if(PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
        { 
			if(_msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&_msg);					// Find out what the message does
            DispatchMessage(&_msg);						// Execute the message
        }
		else
		{
			//-------------------------------
			//	Call your Game class
			//-------------------------------
			if(g_State >= 0)
			{
				int _retValue = g_BlockPTR[g_State]->Step();

				switch(_retValue)
				{
				case  0: // Nothing Special
					break;
				}
			}
			else
			{
				PostQuitMessage(0);
			}
			//-------------------------
			//	Get elapsed Time
			//-------------------------
			unsigned long _temp = GetTickCount();
			x_delta = _temp - _lastTick;
			_lastTick = _temp;
		}
	}

	//----------------------------------
	// Destruction
	//----------------------------------
	while(g_BlockPTR.empty() == 0)
	{
		delete g_BlockPTR.front();
		g_BlockPTR.erase(g_BlockPTR.begin());
	}
	UnregisterClass(WINDOW_NAME,hInstance);
	ChangeDisplaySettings(0,0);
	ShowCursor(1);
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
    WNDCLASS  wndclass;								

    memset(&wndclass, 0, sizeof(WNDCLASS));		
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;	
    wndclass.lpfnWndProc   = WndProc;
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclass.lpszClassName = WINDOW_NAME;

	RegisterClass(&wndclass);

	//----------------------------------------
	//	Device Mode
	//----------------------------------------
	DEVMODE _ScreenSets;						
	memset(&_ScreenSets,0,sizeof(_ScreenSets));

	EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&_ScreenSets);
	_ScreenSets.dmPelsWidth		= WINDOW_WIDTH;
	_ScreenSets.dmPelsHeight	= WINDOW_HEIGHT;
	_ScreenSets.dmFields		= DM_PELSWIDTH|DM_PELSHEIGHT;
	
	ChangeDisplaySettings(&_ScreenSets,CDS_FULLSCREEN);

	RECT _winSize = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

	hwnd = CreateWindow(	WINDOW_NAME,WINDOW_TITLE, 
							WINDOW_STYLE,
							0,0,WINDOW_WIDTH,WINDOW_HEIGHT,
							0,0, hInstance,0);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
	SetFocus(hwnd);

	//----------------------------------------
	//	Trap Cursor
	//----------------------------------------
	ShowCursor(0);
	ClipCursor(&_winSize);
}

//=========================================================================================================
//        WINDOWS MESSAGE HANDLER
//=========================================================================================================
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam)
{
	PAINTSTRUCT    ps;

	//----------------------------------
	// Switch Message Type
	//----------------------------------
    switch (_iMsg)
    {
		case WM_SIZE:
			switch(_wParam)
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
			BeginPaint(_hWnd, &ps);
			EndPaint(_hWnd, &ps);
		    break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		//---------------------------------------------
		//	Mouse and Key Action
		//---------------------------------------------
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_KEYDOWN:
		case WM_KEYUP:
			if(g_State >= 0)
			{
				int _retvalue = g_BlockPTR[g_State]->Input(_iMsg, _wParam, _lParam);
				switch(_retvalue)
				{
					//-----------------------------
					//	Exit State
					//	Note can only be activated
					//	While State is Running
					//-----------------------------
				case -1:
					g_BlockPTR[g_State]->Exit();
					g_State = -1;
					break;
				};
			}
			break;
	}													
	return DefWindowProc(_hWnd, _iMsg, _wParam, _lParam);
}