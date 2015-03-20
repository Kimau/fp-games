/*					Checkers
	Uses PTK lib located at http://www.phelios.com/ptk/

			http://www.flammablepenguins.com/	- Claire "Kimau" Blackshaw
*/

//--------------------------------
//	Includes
//--------------------------------
#include "ptk.h"
#include "Checkers.h"

//--------------------------------
//	Define
//--------------------------------
#define		FP_SCREENW		800
#define		FP_SCREENH		600
#define		FP_TITLE		"Flammable Penguins: Checkers"

//--------------------------------
//	Predefine
//--------------------------------
void Render(Checkers& _game);
void RenderDone(Checkers& _game);
bool EventHandler(KEvent* _eventPtr);
void InputMove(int x, int y);

//--------------------------------
//	Globals
//--------------------------------
KWindow*	g_gameWin;			// PTK Window
KTrueText*	g_gameTxt;			// PTK Text Instance
KGraphic*	g_gameGFX[5];		// PTK GFX Background

int			g_State;			// Game State
Checkers*	g_Game;
int			g_Cursor[4];

//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	//-----------------------------------
	//	Init Window
	//-----------------------------------
	unsigned long _oldTime = KMiscTools::getMilliseconds();
	unsigned long _newTime = 0;
	srand(_oldTime);																// Seed Random
	g_Game = 0;

	//-----------------------------------
	//	Create Window
	//-----------------------------------
	g_gameWin = KPTK::createKWindow(K_OPENGL);													// Init Window
	g_gameWin->createGameWindow(FP_SCREENW,FP_SCREENH,32,true,FP_TITLE);						// Create Window
	g_gameWin->setPTKCallBack(&EventHandler);

	KTrueText _text("5by5.ttf");
	g_gameTxt = &_text;
	g_gameTxt->setHeightPix(60);
	g_gameTxt->setColor(0.0f,0.0f,0.0f,1.0);

	g_gameGFX[0] = KPTK::createKGraphic("background.png");
	g_gameGFX[1] = KPTK::createKGraphic("red.png");
	g_gameGFX[2] = KPTK::createKGraphic("black.png");
	g_gameGFX[3] = KPTK::createKGraphic("rking.png");
	g_gameGFX[4] = KPTK::createKGraphic("bking.png");

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_Game = &Checkers();
	g_Game->Setup();
	g_Cursor[0] = -1;
	g_Cursor[1] = -1;
	g_Cursor[2] = -1;
	g_Cursor[3] = -1;
	
	//-----------------------------------
	//	Game Loop
	//-----------------------------------
	while(g_gameWin->isQuit() == false)
	{
		g_State = g_Game->getState();
		switch(g_State)
		{
			//-------------------------
			//	Setup Game
			//-------------------------
		default:
			g_Game->Setup();
			g_Cursor[0] = -1;
			g_Cursor[1] = -1;
			g_Cursor[2] = -1;
			g_Cursor[3] = -1;
			break;
		case CK_PLAY:
			//--------------------------
			//	Play Game
			//--------------------------
			Render(*g_Game);
			g_gameWin->processEvents();
			break;
		case CK_COMPLETE:
			//--------------------------
			//	Game Finished
			//--------------------------
			RenderDone(*g_Game);
			g_gameWin->processEvents();
			break;
		};
	}

	return 1;
}

//========================================================================
//	Renders Checkers Game
//========================================================================
void Render(Checkers& _game)
{
	//-------------------------
	//	Clean
	//-------------------------
	g_gameWin->setDefaultWorldView();
	g_gameWin->setClearColor(0,0,1,0);

	//-------------------------
	//	Draw Board
	//-------------------------
	const int* _board = g_Game->getBoard();
	g_gameGFX[0]->blitRect(0,0,800,600, 0, 0);
	for(int c = 0; c < 64; ++c)
	{
		int x = (c % CK_WIDTH)*64 + 140;
		int y = (CK_WIDTH-(c / CK_WIDTH))*64 - 20;

		//-----------------------------
		//	Draw Pieces
		//-----------------------------
		if(_board[c] > 0)
		{
			g_gameGFX[_board[c]]->blitAlphaRect(0,0,64,64,x,y);
		}
	}

	//-------------------------
	//	Draw Cursor
	//-------------------------
	if((g_Cursor[0] >= 0) && (g_Cursor[1] >= 0))
	{
		int x = (g_Cursor[0])*64 + 140;
		int y = (CK_WIDTH-(g_Cursor[1]))*64 - 20;

		g_gameGFX[0]->drawRect(x+0.0f,y+0.0f,x+64.0f,y+64.0f,1.0f,1.0f,0.0f,0.5f);
	}
	//-------------------------
	// Buffer Flip
	//-------------------------
	g_gameWin->flipBackBuffer();
}
//========================================================================
//	Renders END GAME
//========================================================================
void RenderDone(Checkers& _game)
{
	//-------------------------
	//	Clean
	//-------------------------
	g_gameWin->setDefaultWorldView();
	g_gameWin->setClearColor(0,0,1,0);

	//-------------------------
	//	Draw Board
	//-------------------------
	const int* _board = g_Game->getBoard();
	g_gameGFX[0]->blitRect(0,0,800,600, 0, 0);
	for(int c = 0; c < 64; ++c)
	{
		int x = (c % CK_WIDTH)*64 + 140;
		int y = (CK_WIDTH-(c / CK_WIDTH))*64 - 20;

		//-----------------------------
		//	Draw Pieces
		//-----------------------------
		if(_board[c] > 0)
		{
			g_gameGFX[_board[c]]->blitAlphaRect(0,0,64,64,x,y);
		}
	}

	//-------------------------
	//	Draw Overlay
	//-------------------------
	g_gameGFX[0]->drawRect(0,0,800.0f,600.0f,0,0,0,0.2f);
	g_gameTxt->setColor(1.0f,1.0f,1.0f,1.0);
	g_gameTxt->drawStringCentered("GAME OVER",0,800,200);
	if(g_Game->getTurn())
	{
		g_gameTxt->setColor(1.0f,0.0f,0.0f,1.0);
		g_gameTxt->drawStringCentered("RED WINS",0,800,400);
	}
	else
	{
		g_gameTxt->setColor(0.0f,1.0f,0.0f,1.0);
		g_gameTxt->drawStringCentered("BLACK WINS",0,800,400);
	}
	//-------------------------
	// Buffer Flip
	//-------------------------
	g_gameWin->flipBackBuffer();
}

//========================================================================
//	Handles Input
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	//-----------------------------
	//	Not in Play
	//-----------------------------
	if(g_State != CK_PLAY)
	{
		if(_eventPtr->type == K_EVENT_KEYDOWN)
		{
			switch(_eventPtr->ptkKey)
			{
				//----------------------
				//	Start New Game
				//----------------------
			case K_VK_RETURN:
				g_Game->Setup();
				break;
				//----------------------
				//	Quit Game
				//----------------------
			case K_VK_ESCAPE:
				PostQuitMessage(1);
				break;
			};
		}
	}

	//---------------------------------------------
	//	Process Input
	//---------------------------------------------
	switch(_eventPtr->type)
	{
		//-------------------------------------
		//	MOUSE EVENTS
		//-------------------------------------
	case K_EVENT_MOUSEDOWN:
		{
			if(_eventPtr->buttonIndex == K_LBUTTON)
			{
				//----------------------------------
				//	Retrieve Coord
				//----------------------------------
				int x = (_eventPtr->mouseX - 140) / 64;
				int y = (FP_SCREENH - 42 - _eventPtr->mouseY) / 64;

				g_Cursor[0] = -1;
				g_Cursor[1] = -1;
				//----------------------------------
				//	Check for Valid Input
				//----------------------------------
				if(((x < 0) || (x > 7) || (y < 0) || (y > 7)) == false)
				{
					g_Cursor[0] = x;
					g_Cursor[1] = y;
					InputMove(x,y);
				}
			}
			break;
		}
		//-------------------------------------
		//	Key Presses
		//-------------------------------------
	case K_EVENT_KEYDOWN:
		{
			//------------------------------
			//	Quit Game
			//------------------------------
			if(_eventPtr->ptkKey == K_VK_ESCAPE)
			{
				PostQuitMessage(1);
			}
			break;
		}
	}
	return true;
}

//========================================================================
//	Handles Input
//========================================================================
void InputMove(int x, int y)
{
	//--------------------------------
	//	Select Piece if Valid
	//--------------------------------
	if(g_Game->CheckPiece(x,y))
	{
		g_Cursor[2] = x;
		g_Cursor[3] = y;
	}
	//--------------------------------
	//	Move Piece
	//--------------------------------
	else
	{
		if((g_Cursor[2] >= 0) && (g_Cursor[3] >= 0))
		{
			if(g_Game->CheckMove(g_Cursor[2],g_Cursor[3],g_Cursor[0],g_Cursor[1]))
			{
				g_Game->Move(g_Cursor[2],g_Cursor[3],g_Cursor[0],g_Cursor[1]);
			}
		}
		g_Cursor[0] = -1;
		g_Cursor[1] = -1;
		g_Cursor[2] = -1;
		g_Cursor[3] = -1;
	}
}