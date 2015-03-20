/*----------------------------------------------------------------------------------------------------
	Boid Maze

	Object		Create a boid behaviours system that uses an extensible data structure
				that is reusable and efficient.
	Display		Use an OpenGL system that is totally independent of the Boid logic system
----------------------------------------------------------------------------------------------------*/
//=======================================================
//	Headers
//=======================================================
#include <windows.h>
#include "BoidGl.h"

//=======================================================
//	Predeclaration
//=======================================================
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _iMsg, WPARAM wParam, LPARAM lParam);
void CreateWin(HINSTANCE& hInstance, HWND& hwnd, int iCmdShow);

//=======================================================
//	Global Varible
//=======================================================
BoidBounce* g_BBref = 0;

//=========================================================================================================
//        MAIN WINDOWS FUNCTION OF ANNOANCE
//=========================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	srand(GetTickCount());
	//----------------------------------
	//	Create Window
	//----------------------------------
	HWND      _hwnd = 0;									// HWND - a HWND is a handle to a window.
	CreateWin(hInstance, _hwnd, iCmdShow);
	//--------------------------------------------
	//	Create Local Instances of Engines
	//--------------------------------------------
	BoidBounce _BBlocal(hInstance, _hwnd, 600, 800);
	g_BBref = &_BBlocal;
	//----------------------------------
	//	Main Loop
	//----------------------------------
	MSG _msg = {0};										// MSG  - A Msg variable to hold what messages are being sent to the window
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
			_BBlocal.GameStep();
        } 
	}

	//----------------------------------
	// Destruction
	//----------------------------------
	UnregisterClass(WIN_NAME,hInstance);
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
    wndclass.lpszClassName = WIN_NAME;
    wndclass.hIconSm       = LoadIcon (NULL, IDI_WINLOGO);

	RegisterClassEx(&wndclass);

    hwnd = CreateWindow(WIN_NAME, WIN_TITLE,0,0,0,WIN_WIDTH,WIN_HEIGHT,NULL,NULL,hInstance,NULL);								// If we wanted to pass in a variable or structure to the "WndProc", we would do it here.  (I just prefer using global variables though).  I just pass in NULL.

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

	ShowCursor(0);
}

//=========================================================================================================
//        WINDOWS MESSAGE HANDLER
//=========================================================================================================
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _iMsg, WPARAM wParam, LPARAM lParam)
{
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

		//---------------------------------------------
		//	Mouse Move
		//---------------------------------------------
		case WM_MOUSEMOVE:
			g_BBref->MouseMove(wParam,lParam);
			break;

		//---------------------------------------------
		//	Key Press
		//---------------------------------------------
		case WM_KEYDOWN:
			g_BBref->KeyPressed(wParam);
			break;
	}													
	return DefWindowProc(_hWnd, _iMsg, wParam, lParam);
}