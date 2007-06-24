/*		PTK Intro	

	Uses PTK lib located at http://www.phelios.com/ptk/
	http://www.flammablepenguins.com/	- Claire "Kimau" Blackshaw

*/

//--------------------------------
//	Includes
//--------------------------------
#include "MyGame.h"
#include "ptkGameRender.h"
#include "gEvents.h"

//-------------------------------
//	Predefines
//-------------------------------
bool EventHandler(KEvent* _eventPtr);

//--------------------------------
//	Globals
//--------------------------------
ptkGameRender* g_Renderer;
MyGame*		   g_Game;
gEventManager  g_Stack;

//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	//-----------------------------------
	//	Startup
	//-----------------------------------
	unsigned long _oldTime = KMiscTools::getMilliseconds();
	unsigned long _newTime = 0;
	srand(_oldTime);																// Seed Random
	g_Game = 0;

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_Game = &MyGame();
	g_Game->Setup();

	//-----------------------------------
	//	Setup Api
	//-----------------------------------
	g_Renderer = &ptkGameRender();
	KWindow* _gwin = g_Renderer->Setup(g_Game);
	_gwin->setPTKCallBack(&EventHandler);

	//-----------------------------------
	//	Game Loop
	//-----------------------------------
	while(_gwin->isQuit() == false)
	{
		//-------------------------
		//	Update Timer
		//-------------------------
		_newTime = KMiscTools::getMilliseconds();
		unsigned long _delta = _newTime - _oldTime;
		_oldTime = _newTime;
		srand(_oldTime * _delta);

		//-------------------------
		//	Action Game State
		//-------------------------
		switch(g_Game->getState())
		{
		//-----------------------------
		//	No Game Object Found
		//-----------------------------
		case GS_NULL:
			_gwin->terminate();
			break;
		//-----------------------------
		// Game not setup
		//-----------------------------
		case GS_SETUP:
			g_Game->Setup();
			break;
		//-----------------------------
		//	State of Play
		//-----------------------------
		case GS_PLAY:
			g_Game->Update(_delta);
			g_Renderer->Update(_delta);
			g_Renderer->Render();
			break;
		//-----------------------------
		//	Game Over
		//-----------------------------
		case GS_GAMEOVER:
			_gwin->terminate();
			break;
		};
		//-----------------------------
		//	Update State and Events
		//-----------------------------
		_gwin->processEvents();
	}

	//-----------------------------------
	//	Shutdown
	//-----------------------------------
	return 1;
}

//========================================================================
//	Handles Input
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	switch(g_Game->getState())
	{
	case GS_NULL:			break;
	case GS_SETUP:			break;
	case GS_PLAY:
		{
			//----------------------------------
			//	Mouse Click
			//----------------------------------
			if((_eventPtr->type == K_EVENT_MOUSEDOWN) && (_eventPtr->buttonIndex == K_LBUTTON))
			{
				g_Stack.post(GEM_SHOOT,(_eventPtr->mouseX),(PTK_SCREENH - _eventPtr->mouseY));
			}
		}
		break;
	case GS_GAMEOVER:		break;
	};

	return true;
}