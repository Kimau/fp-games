/*		[K]eep [I]t [S]imple [S]tupid	

	Uses PTK lib located at http://www.phelios.com/ptk/
	http://www.flammablepenguins.com/	- Claire "Kimau" Blackshaw

*/

//--------------------------------
//	Includes
//--------------------------------
#include "ptkGameRender.h"
#include "gEvents.h"
#include "KISSproto.h"

//-------------------------------
//	Predefines
//-------------------------------
bool EventHandler(KEvent* _eventPtr);

//--------------------------------
//	Globals
//--------------------------------
ptkGameRender*	g_Renderer;
KISSproto*		g_Game;
gEventManager	g_Stack;

//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{{
	//-----------------------------------
	//	Startup
	//-----------------------------------
	unsigned long _oldTime = KMiscTools::getMilliseconds();
	unsigned long _newTime = 0;
	srand(_oldTime);																// Seed Random
	g_Game = &KISSproto();

	//-----------------------------------
	//	Setup Api
	//-----------------------------------
	g_Renderer = &ptkGameRender();
	KWindow* _gwin = g_Renderer->Setup(g_Game);
	_gwin->setPTKCallBack(&EventHandler);

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_Game->Setup(3);
	//-----------------------------------
	//	Sample Cast
	//-----------------------------------
							//	ID,	Energy,	Type, NULL, Comedy, Drama, Horror, Love
	g_Stack.post(INBOX,GEM_SET_ACTOR, g_Game->getActorID(0), 20, 0, -1, 1.0f, 1.0f, 1.0f, 1.0f);
	g_Stack.post(INBOX,GEM_SET_ACTOR, g_Game->getActorID(1), 20, 0, -1, 1.0f, 1.0f, 1.0f, 1.0f);
	g_Stack.post(INBOX,GEM_SET_ACTOR, g_Game->getActorID(2), 20, 0, -1, 1.0f, 1.0f, 1.0f, 1.0f);
	g_Stack.postif(INBOX,GEM_MOVE_ACTOR,g_Game->getActorID(0),-1.0f,0.0f);
	g_Stack.postif(INBOX,GEM_MOVE_ACTOR,g_Game->getActorID(1), 0.0f,0.0f);
	g_Stack.postif(INBOX,GEM_MOVE_ACTOR,g_Game->getActorID(2), 1.0f,1.0f);
	g_Game->Update(0);

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
			_gwin->terminate();
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
	} // Little Trick to catch Decon Errors
	g_Game = 0;
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
			if(_eventPtr->type == K_EVENT_MOUSEDOWN)
			{
				g_Renderer->MouseClick(_eventPtr->mouseX,_eventPtr->mouseY,!(_eventPtr->buttonIndex == K_LBUTTON));
			}
		}
		break;
	case GS_GAMEOVER:		break;
	};

	return true;
}