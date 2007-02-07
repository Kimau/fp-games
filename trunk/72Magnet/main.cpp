/*					72 Hour Magnets
	Uses PTK lib located at http://www.phelios.com/ptk/

												- Edge
			http://www.flammablepenguins.com/	- Claire "Kimau" Blackshaw
*/

//--------------------------------
//	Includes
//--------------------------------
#include <math.h>
#include <vector>
#include "vector.h"
#include "ptk.h"

//--------------------------------
//	Define
//--------------------------------
#define		FP_SCREENW		800
#define		FP_SCREENH		600
#define		FP_TITLE		"Flammable Penguins: 72Hour Magnets"
#define		FP_BOARDW		50				// Entire Play Board
#define		FP_BOARDH		50
#define		FP_BSIZE		(FP_BOARDW * FP_BOARDH)

#define		FP_LEVEL_COUNT	17

#define		FP_SM_MENU		2
#define		FP_SL_START		4
#define		FP_SL_PLAY		5
#define		FP_SL_PAUSE		6
#define		FP_SL_LOSE		7
#define		FP_SL_WIN		8

#define		FP_X_START		128
#define		FP_X_END		256
#define		FP_X_FLAG		512
#define		FP_X_DROP		1024

#define		FP_MAGCONST		10.0f

//--------------------------------
//	Struct
//--------------------------------
struct intXY
{
	int x;
	int y;
};
//--------------------------------
//	Predeclare
//--------------------------------
void RenderMINI();
void RenderGAME();
void RenderFLAT2();
void RenderISO2();
void RenderMENU();
FPVec2 CalcForce(FPVec2 _pos, int _mode);
void MoveBall();
void CheckPickups();
bool CollideBlock(int x, int y);
void saveTFile(const char* _filename);
void loadTFile(const char* _filename);
void saveBFile(const char* _filename);
void loadBFile(const char* _filename);
char* getTime(unsigned int _milli);
void saveHighScore(const char* _filename);
void loadHighScore(const char* _filename);
void CalcField();

//--------------------------------
//	Globals
//--------------------------------
KWindow*	g_gameWin;			// PTK Window
KTrueText*	g_gameTxt;			// PTK Text Instance
KGraphic*	g_gfxBack;			// Graphic Back
KGraphic*	g_gfxBall[6];		// Ball Graphics
KGraphic*	g_gfxTile[15];		// Tile Graphics
KGraphic*	g_gfxFlat;			// Flat Graphics

unsigned long g_Delta;
unsigned long g_Timer;

unsigned int g_Bronze;
unsigned int g_Silver;
unsigned int g_Gold;

int			g_State;			// Game State
int			g_bCursor[2];		// Game Board Cursor 
intXY		g_bOff;				// Board Offset
intXY		g_bStart;			// Start Point
intXY		g_bEnd;				// End Point
int			g_Tiles[FP_BSIZE];	// Game Board
FPVec2		g_Force[FP_BSIZE*3];// Precalculated Forces

int			g_ballMode;			// Ball Mode
FPVec2		g_ballPos;			// Ball Position
FPVec2		g_ballDir;			// Ball Direction

bool		g_showLines;		// Show Field Lines
bool		g_showISO;			// Show ISO
int			g_menu;				// Menu Selection
int			g_level;			// Level Selection
int			g_flags;			// Flags Left

unsigned int g_Highscore[FP_LEVEL_COUNT];	// Highscore list

using namespace std;

//========================================================================
//	Handels Events
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	//----------------------------
	//	Check State
	//----------------------------
	switch(g_State)
	{
		//##################################
		//			MENU
		//##################################
	case FP_SM_MENU:
		{
			if(_eventPtr->type == K_EVENT_KEYUP)
			{
				switch(_eventPtr->ptkKey)
				{
					//--------------------------------
					//	Navigate Menu
					//--------------------------------
				case K_VK_LEFT:		
					{
						int c = g_level;
						g_level = max(0, g_level-1);	
						if(c != g_level)
						{
							char _buffer[15] = {0};
							sprintf(_buffer,"map%02i.map",g_level);
							loadBFile(_buffer);
						}
					}
					break;
				case K_VK_RIGHT:
					{
						int c = g_level;
						g_level = min(FP_LEVEL_COUNT-1, g_level+1);	
						if(c != g_level)
						{
							char _buffer[15] = {0};
							sprintf(_buffer,"map%02i.map",g_level);
							loadBFile(_buffer);
						}
					}
					break;
				case K_VK_UP:		g_menu = max(0, g_menu-1);					break;
				case K_VK_DOWN:		g_menu = min(3, g_menu+1);					break;
					//--------------------------------
					//	Select Option
					//--------------------------------
				case K_VK_RETURN:
					{
						//--------------------------------
						//	Quit Game
						//--------------------------------
						if(g_menu == 3)		
							g_State = -1;
						//--------------------------------
						//	Highscores
						//--------------------------------
						if(g_menu == 2)	
							g_menu = 100;
						//--------------------------------
						//	Start Game
						//--------------------------------
						else if(g_menu == 0)	
						{
							char _buffer[15] = {0};
							sprintf(_buffer,"map%02i.map",g_level);
							loadBFile(_buffer);
							g_State = FP_SL_START;
						}
						//--------------------------------
						//	In all other cases
						//--------------------------------
						else
						{
							g_menu = 0;
						}
					}
					break;
					/*
					//-------------------------------
					//	Quit Game
					//-------------------------------
				case K_VK_ESCAPE:
					g_State = -1;
					break;*/
				};
				return true;
			}
		}
		break;
		//##################################
		//			LEVEL START
		//##################################
	case FP_SL_START:
		if(_eventPtr->type == K_EVENT_KEYDOWN)
		{
			g_State = FP_SL_PLAY;
			return true;
		}
		break;

		/*
	case FP_SL_PAUSE:
		if(_eventPtr->type == K_EVENT_KEYDOWN)
		{
			g_State = FP_SL_PLAY;
			return true;
		}
		break;*/

		//##################################
		//		LEVEL DEATH
		//##################################
	case FP_SL_LOSE:
		if((_eventPtr->type == K_EVENT_KEYUP) && (_eventPtr->ptkKey == K_VK_RETURN))
		{
			char _buffer[15] = {0};
			sprintf(_buffer,"map%02i.map",g_level);
			loadBFile(_buffer);
			g_State = FP_SL_START;
		}
		return true;

		//##################################
		//		LEVEL WIN
		//##################################
	case FP_SL_WIN:
		if((_eventPtr->type == K_EVENT_KEYUP) && (_eventPtr->ptkKey == K_VK_RETURN))
		{
			//--------------------------------
			//	Save Highscore
			//--------------------------------
			unsigned int _finalTime = g_Timer / 100000;
			if((g_Highscore[g_level] == 0) || (_finalTime < g_Highscore[g_level]))
			{
				g_Highscore[g_level] = _finalTime;
			}
			//--------------------------------
			//	Load Next Map
			//--------------------------------
			++g_level;
			if(g_level < FP_LEVEL_COUNT)
			{
				char _buffer[15] = {0};
				sprintf(_buffer,"map%02i.map",g_level);
				loadBFile(_buffer);
				g_State = FP_SL_START;
			}
			else
			{
				g_State = FP_SM_MENU;
			}
		}
		return true;
	};
	//----------------------------
	//	Process KEY
	//----------------------------
	if((_eventPtr->type == K_EVENT_KEYDOWN) && ((g_State == FP_SL_PLAY) || (g_State == FP_SL_PAUSE)))
	{
		//---------------------------
		//	Process Key
		//---------------------------
		switch(_eventPtr->ptkKey)
		{
		//------------------------------------
		//	QUIT
		//------------------------------------
		case K_VK_ESCAPE:
			{
				g_State = FP_SM_MENU;
			}
			break;

		//------------------------------------
		//	PAUSE
		//------------------------------------
		case K_VK_P:
			{
				g_State = (g_State - 4) % 2 + 5;// FP_SL_PAUSE;
			}
			break;

		//------------------------------------
		//	SCROLL KEYS
		//------------------------------------
		case K_VK_UP:		g_bOff.y += 20;		break;
		case K_VK_DOWN:		g_bOff.y -= 20;		break;
		case K_VK_LEFT:		g_bOff.x += 20;		break;
		case K_VK_RIGHT:	g_bOff.x -= 20;		break;

		//------------------------------------
		//	SAVE / LOAD FILE
		//------------------------------------
		case K_VK_F4:
			{
				memset(g_Tiles,	0,sizeof(int)*FP_BSIZE);
				CalcField();
			}
			break;
		case K_VK_F5:
			{
				char _buffer[15] = {0};
				sprintf(_buffer,"map%02i.map",g_level);
				saveBFile(_buffer);
			}
			break;
		case K_VK_S:
			{
				saveBFile("edit.map");
			}
			break;
		case K_VK_L:
			{
				loadBFile("edit.map");
			}
			break;
		//------------------------------------
		//	QUICK CHANGE MODE
		//------------------------------------
		case K_VK_Z:	g_ballMode = 0;	break;
		case K_VK_X:	g_ballMode = 1;	break;
		case K_VK_C:	g_ballMode = 2;	break;
		//------------------------------------
		//	Toggle Field Lines
		//------------------------------------
		case K_VK_F:
			{
				g_showLines = !g_showLines;
				if(g_showLines)		CalcField();
			}
			break;
		//------------------------------------
		//	Toggle ISO / FLAT
		//------------------------------------
		case K_VK_R:
			{
				g_showISO = !g_showISO;
			}
			break;
		//------------------------------------
		//	PLACE TILE
		//------------------------------------
		case K_VK_0:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 0;}	break;
		case K_VK_1:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 1;}	break;
		case K_VK_2:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 2;}	break;
		case K_VK_3:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 3;}	break;

		case K_VK_4:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 11;}	break;
		case K_VK_5:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 12;}	break;
		case K_VK_6:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 13;}	break;
		case K_VK_7:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 14;}	break;
		case K_VK_8:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 15;}	break;
		case K_VK_9:	{g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = 16;}	break;

		//------------------------------------
		//	PLACE FLAG
		//------------------------------------
		case K_VK_INSERT:
			{
				g_flags += 1;
				g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = FP_X_FLAG;
			}	
			break;
		//------------------------------------
		//	PLACE DROP ZONE
		//------------------------------------
		case K_VK_DEL:
			{
				g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = FP_X_DROP;
			}	
			break;
			
		//------------------------------------
		//	PLACE START
		//------------------------------------
		case K_VK_HOME:
			{
				//--------------------------------------
				//	Remove Old Start
				//--------------------------------------
				g_Tiles[g_bStart.x + g_bStart.y*FP_BOARDW] = 0;

				//--------------------------------------
				//	Set New Start
				//--------------------------------------
				g_bStart.x = g_bCursor[0];
				g_bStart.y = g_bCursor[1];
				g_Tiles[g_bStart.x + g_bStart.y*FP_BOARDW] = FP_X_START;

				//--------------------------------------
				//	Place Ball at Start
				//--------------------------------------
				g_ballPos.x = (g_bStart.x)*32.0f + 16.0f;
				g_ballPos.y = (g_bStart.y)*32.0f + 16.0f;
				g_ballDir.x = 0.0f;
				g_ballDir.y = 0.0f;
				g_ballMode = 0;				
			}	
			break;
		//------------------------------------
		//	PLACE END
		//------------------------------------
		case K_VK_END:
			{
				//--------------------------------------
				//	Remove Old End
				//--------------------------------------
				g_Tiles[g_bEnd.x + g_bEnd.y*FP_BOARDW] = 0;

				//--------------------------------------
				//	Set New End
				//--------------------------------------
				g_bEnd.x = g_bCursor[0];
				g_bEnd.y = g_bCursor[1];
				g_Tiles[g_bEnd.x + g_bEnd.y*FP_BOARDW] = FP_X_END;
			}	
			break;
		//------------------------------------
		//	Set Gold, Silver and Bronze
		//------------------------------------
		case K_VK_Q:	g_Gold   = g_Timer / 100000;		break;
		case K_VK_W:	g_Silver = g_Timer / 100000;		break;
		case K_VK_E:	g_Bronze = g_Timer / 100000;		break;
		};
	}
	//--------------------------
	//	Mouse Up
	//	Place Tile
	//--------------------------
	/*
	else if (_eventPtr->type == K_EVENT_MOUSEUP)
	{
		switch(_eventPtr->buttonIndex)
		{
		case K_LBUTTON:
			{
				g_ballMode = (g_ballMode + 1) % 3;
			}
			break;
		case K_RBUTTON:
			{
				g_ballMode = (g_ballMode + 2) % 3;
			}
		};
	}*/
	return true;
}
//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	/*
	for(int c = 5; c < 8; ++c)
	{
		char _buffer[15] = {0};
		sprintf(_buffer,"map%02i.txt",c);
		loadTFile(_buffer);
		sprintf(_buffer,"map%02i.map",c);
		saveBFile(_buffer);
	}
	return 1;/**/
	//-----------------------------------
	//	Init Window
	//-----------------------------------
	unsigned long _oldTime = KMiscTools::getMilliseconds();
	unsigned long _newTime = 0;
	srand(_oldTime);																// Seed Random

	//-----------------------------------
	//	Create Window
	//-----------------------------------
	g_gameWin = KPTK::createKWindow(K_OPENGL);													// Init Window
	g_gameWin->createGameWindow(FP_SCREENW,FP_SCREENH,32,true,FP_TITLE);						// Create Window
	g_gameWin->setPTKCallBack(&EventHandler);

	//-----------------------------------
	//	Get Font
	//-----------------------------------
	KTrueText _font("5by5.ttf");																// Load Font
	g_gameTxt = &_font;																			// Link PTR
	g_gameTxt->setHeightPix(30);
	g_gameTxt->setColor(0.0f,0.0f,0.0f,1.0);

	//-----------------------------------
	//	Graphic
	//-----------------------------------
	g_gfxBack = KPTK::createKGraphic("back.png");												// Background Graphic
	g_gfxBall[0] = KPTK::createKGraphic();			g_gfxBall[0]->loadPicture("ball_1.png", true, true);
	g_gfxBall[1] = KPTK::createKGraphic();			g_gfxBall[1]->loadPicture("ball_2.png", true, true);
	g_gfxBall[2] = KPTK::createKGraphic();			g_gfxBall[2]->loadPicture("ball_3.png", true, true);
	g_gfxBall[3] = KPTK::createKGraphic();			g_gfxBall[3]->loadPicture("ball_4.png", true, true);
	g_gfxBall[4] = KPTK::createKGraphic();			g_gfxBall[4]->loadPicture("ball_5.png", true, true);
	g_gfxBall[5] = KPTK::createKGraphic();			g_gfxBall[5]->loadPicture("ball_6.png", true, true);

	g_gfxTile[ 0] = KPTK::createKGraphic();			g_gfxTile[ 0]->loadPicture("tile00.png", true, true);
	g_gfxTile[ 1] = KPTK::createKGraphic();			g_gfxTile[ 1]->loadPicture("tile01.png", true, true);
	g_gfxTile[ 2] = KPTK::createKGraphic();			g_gfxTile[ 2]->loadPicture("tile02.png", true, true);
	g_gfxTile[ 3] = KPTK::createKGraphic();			g_gfxTile[ 3]->loadPicture("tile03.png", true, true);
	g_gfxTile[ 4] = KPTK::createKGraphic();			g_gfxTile[ 4]->loadPicture("tile04.png", true, true);
	g_gfxTile[ 5] = KPTK::createKGraphic();			g_gfxTile[ 5]->loadPicture("tile05.png", true, true);
	g_gfxTile[ 6] = KPTK::createKGraphic();			g_gfxTile[ 6]->loadPicture("tile06.png", true, true);
	g_gfxTile[ 7] = KPTK::createKGraphic();			g_gfxTile[ 7]->loadPicture("tile07.png", true, true);
	g_gfxTile[ 8] = KPTK::createKGraphic();			g_gfxTile[ 8]->loadPicture("tile08.png", true, true);
	g_gfxTile[ 9] = KPTK::createKGraphic();			g_gfxTile[ 9]->loadPicture("tile09.png", true, true);
	g_gfxTile[10] = KPTK::createKGraphic();			g_gfxTile[10]->loadPicture("tile10.png", true, true);
	g_gfxTile[11] = KPTK::createKGraphic();			g_gfxTile[11]->loadPicture("tile11.png", true, true);
	g_gfxTile[12] = KPTK::createKGraphic();			g_gfxTile[12]->loadPicture("tile12.png", true, true);
	g_gfxTile[13] = KPTK::createKGraphic();			g_gfxTile[13]->loadPicture("tile13.png", true, true);
	g_gfxTile[14] = KPTK::createKGraphic();			g_gfxTile[14]->loadPicture("tile14.png", true, true);

	g_gfxFlat = KPTK::createKGraphic();				g_gfxFlat->loadPicture("flat.png", true, true);

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_State = FP_SM_MENU;
	g_showISO = false;
	g_showLines = false;
	memset(g_bCursor,	0,sizeof(int)*2);
	memset(g_Tiles,		0,sizeof(int)*FP_BSIZE);
	memset(g_Highscore,	0,sizeof(unsigned int)*FP_LEVEL_COUNT);

	g_ballMode = 2;
	g_ballPos.x = 250.0f;
	g_ballPos.y = 250.0f;

	g_Timer = 0;
	g_menu = 0;

	//-----------------------------------
	//	Load High Score File
	//-----------------------------------
	loadHighScore("score.txt");

	//-----------------------------------
	//	Load Map
	//-----------------------------------
	loadBFile("map00.map");

	//-----------------------------------
	//	Game Loop
	//-----------------------------------
	while((g_State > 0) && (g_gameWin->isQuit() == false))
	{
		//-------------------------
		//	Update Delta
		//-------------------------
		{
			_newTime = KMiscTools::getMilliseconds();
			g_Delta = (_oldTime - _newTime) / 100000;
			_oldTime = _newTime;
		}
		//-------------------------
		//	Get Cursor
		//-------------------------
		if(!g_showISO)
		{
			g_bCursor[0] = min(FP_BOARDW - 1,max(0,((KInput::getMouseX() - g_bOff.x) / 32)));
			g_bCursor[1] = min(FP_BOARDH - 1,max(0,((KInput::getMouseY() - g_bOff.y) / 32)));
		}
		//-------------------------
		//	Move Ball
		//-------------------------
		if(g_State == FP_SL_PLAY)
		{
			g_Timer += g_Delta;
			MoveBall();
			CheckPickups();
		}
		//-------------------------
		//	Render Menu
		//-------------------------
		if(g_State == FP_SM_MENU)
		{
			RenderMENU();
		}
		//-------------------------
		//	Render Play Area
		//-------------------------
		else if(g_State > 3)
		{
			RenderGAME();
		}
		//-------------------------
		// Buffer Flip
		//-------------------------
		g_gameWin->flipBackBuffer();
		//-----------------------------
		//	Process Events
		//-----------------------------
		g_gameWin->processEvents();
	}

	//------------------------------
	//	Save High Score
	//------------------------------
	saveHighScore("score.txt");
}
//========================================================================
//		Render Game
//========================================================================
void RenderMINI()
{
	//------------------------
	//	Draw MiniMap
	//------------------------
	g_gfxBack->drawRect(0,0,3.0f*FP_BOARDW,3.0f*FP_BOARDH,0.0f,0.0f,0.0f,0.5f);
	for(int x = 0; x < FP_BOARDW;  ++x)
	{
		for(int y = 0; y < FP_BOARDH; ++y)
		{
			//---------------------
			//	Get Tile Value
			//---------------------
			int& _tile = g_Tiles[(x) + (y)*FP_BOARDW];
			//---------------------
			//	Draw Tile
			//---------------------
			float xM = x*3.0f;
			float yM = y*3.0f;

			if(_tile & FP_X_FLAG)
				g_gfxBack->drawRect(xM,yM,xM+3,yM+3,  1.0f,0.0f,0.0f,1.0f);
			else if(_tile & FP_X_DROP)
				g_gfxBack->drawRect(xM,yM,xM+3,yM+3,  0.0f,0.0f,0.0f,1.0f);
			else if(_tile & 127)
				g_gfxBack->drawRect(xM,yM,xM+3,yM+3,  0.0f,1.0f,1.0f,1.0f);
		}
	}
}

//========================================================================
//		Render Game
//========================================================================
void RenderGAME()
{
	if(g_showISO)
		RenderISO2();//RenderISO();
	else
		RenderFLAT2();


	//-------------------------
	//	Overlay any Graphics
	//-------------------------
	switch(g_State)
	{
	case FP_SL_PAUSE:
		g_gfxBack->drawRect(0,0,800.0f,600.0f,0.5f,0.5f,0.5f,0.6f);
		break;
	case FP_SL_START:
		g_gfxBack->drawRect(0,0,800.0f,600.0f,0.0f,0.0f,1.0f,0.2f);
		break;
	case FP_SL_WIN:
		g_gfxBack->drawRect(0,0,800.0f,600.0f,0.0f,1.0f,0.0f,0.2f);
		break;
	case FP_SL_LOSE:
		g_gfxBack->drawRect(0,0,800.0f,600.0f,1.0f,0.0f,0.0f,0.2f);
		break;
	};

	//--------------------------
	//	Render Mini Map
	//--------------------------
	RenderMINI();

	//-------------------------
	//	Print Reminaing Flag
	//-------------------------
	g_gameTxt->setHeightPix(30);
	if(g_flags)
	{
		g_gameTxt->setColor(1.0f,0.0f,0.0f,1.0f);
		char _buffer[50] = {0};
		sprintf(_buffer,"%i Flags Left",g_flags);
		g_gameTxt->drawStringCentered(_buffer,0,800,20);
	}

	//-------------------------
	//	Print Time
	//-------------------------
	//g_gameTxt->setHeightPix(30);

	unsigned int _inSecs = g_Timer / 100000;
	unsigned int _target = 0;

	if(_inSecs < g_Gold)
	{
		g_gameTxt->setColor(1.0f,1.0f,0.0f,1.0f);
		_target = g_Gold;
	}
	else if(_inSecs < g_Silver)
	{
		g_gameTxt->setColor(0.8f,0.8f,0.8f,1.0f);
		_target = g_Silver;
	}
	else if(_inSecs < g_Bronze)
	{
		g_gameTxt->setColor(0.6f,0.6f,0.0f,1.0f);
		_target = g_Bronze;
	}
	else
	{
		g_gameTxt->setColor(1.0f,1.0f,1.0f,1.0f);
		_target = 0;
	}

	char* _buffer = getTime(_inSecs);
	g_gameTxt->drawStringCentered(_buffer,0,800,0);
	delete[] _buffer;

	if(_target > 0)
	{
		char* _buffer = getTime(_target);
		g_gameTxt->drawStringCentered(_buffer,400,800,0);
		delete[] _buffer;
	}
}
//========================================================================
//		Render Game
//========================================================================
void RenderFLAT2()
{
	//-------------------------
	//	Clean
	//-------------------------
	g_gameWin->setDefaultWorldView();
	g_gameWin->setClearColor(0,0,1,0);

	g_gfxBack->drawRect(0,0,FP_SCREENW,FP_SCREENH, 0.2f,0.2f,0.2f,1.0f);


	//-----------------------------------
	//	OFFSETS
	//	Follow Ball only while in Play
	//-----------------------------------
	if((g_State == FP_SL_PLAY) || (g_State == FP_SL_START))
	{
		g_bOff.x = (FP_SCREENW/2 - int(g_ballPos.x));
		g_bOff.y = (FP_SCREENH/2 - int(g_ballPos.y));
	}

	//----------------------------
	//	Draw Board
	//----------------------------
	for(int y = 0; y < FP_BOARDH; ++y)
	for(int x = 0; x < FP_BOARDW; ++x)
	{
		//----------------------------
		//	Calc Tile Pos
		//----------------------------
		float u = x*32.0f + g_bOff.x;
		float v = y*32.0f + g_bOff.y;

		if((u > -32) && (v > -32) && (u < FP_SCREENW) && (v < FP_SCREENH))
		{
			//---------------------
			//	Get Tile Value
			//---------------------
			int _tile = g_Tiles[(x) + (y)*FP_BOARDW];
			//---------------------
			//	Draw Tile
			//---------------------
			if((_tile & 127) > 0)
			{
				switch(_tile & 127)
				{
				case 1:				
				case 2:
				case 3:
					{
						int _anim = (g_ballMode+_tile)%3;
						g_gfxFlat->blitRect(_anim*32.0f,_tile*32.0f,(_anim+1)*32.0f,(_tile+1)*32.0f,  short(u),short(v));
					}
					break;

				case 11:		
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
					g_gfxFlat->blitRect(0,4*32.0f,32.0f,(5)*32.0f,short(u),short(v));
					break;
				};
			}
			//------------------------------------------
			//	Draw Special Tiles
			//------------------------------------------
			if(_tile & FP_X_START)
			{
				g_gfxFlat->blitRect(0,5*32.0f,32.0f,6*32.0f,short(u),short(v));
			}
			else if(_tile & FP_X_FLAG)
			{
				g_gfxFlat->blitRect(0,7*32.0f,32.0f,8*32.0f,short(u),short(v));
			}
			else if(_tile & FP_X_END)
			{
				if(g_flags)
					g_gfxFlat->blitRect(0,6*32.0f,32.0f,7*32.0f,short(u),short(v));
				else
					g_gfxFlat->blitRect(32.0f,6*32.0f,64.0f,7*32.0f,short(u),short(v));
			}
			else if(_tile & FP_X_DROP)
			{
				g_gfxFlat->blitRect(0,8*32.0f,32.0f,9*32.0f,short(u),short(v));
			}

			//----------------------------
			//	Display Magnet Lines
			//----------------------------
			if(g_showLines)
			{
				FPVec2& _res = g_Force[x + y*FP_BOARDW + g_ballMode*FP_BSIZE];
				g_gfxBack->drawLine(u + 16.0f,v + 16.0f,u + _res.x + 16.0f,v + _res.y + 16.0f,
					(g_ballMode == 0), (g_ballMode == 1), (g_ballMode == 2), 1.0f);
			}
		}
	}
	//-------------------------
	//	Draw Ball
	//-------------------------
	if(g_State != FP_SL_PAUSE)
	{
		g_gfxBall[g_ballMode]->blitAlphaRect(0,0,32,32, FP_SCREENW/2 - 16,FP_SCREENH/2 - 16);
	}
	//-------------------------
	//	Draw Mouse Cursor
	//-------------------------
	if(g_State == FP_SL_PAUSE)
	{
		float u = (g_bCursor[0])*32.0f + g_bOff.x;
		float v = (g_bCursor[1])*32.0f + g_bOff.y;
		if((u > -32) && (v > -32) && (u < FP_SCREENW) && (v < FP_SCREENH))
		{
			g_gfxBack->drawRect(u,v,u+32.0f,v+32.0f,  1.0f,1.0f,1.0f,1.0f);
		}
	}
}

//========================================================================
//		Render Game
//========================================================================
void RenderISO2()
{
	//-------------------------
	//	Clean
	//-------------------------
	g_gameWin->setDefaultWorldView();
	g_gameWin->setClearColor(0,0,0,0);
	g_gfxBack->drawRect(0,0,800.0f,600.0f, 0.2f,0.2f,0.2f,1.0f);

	//-------------------------
	//	Calc Ball
	//-------------------------
	int _bU = int((((g_ballPos.y + g_ballPos.x)/32.0f)  )*28.62f);
	int _bV = int((((g_ballPos.y - g_ballPos.x)/32.0f)-2)*14.31f);

	//-------------------------
	//	OFFSETS
	//-------------------------
	g_bOff.x = (FP_SCREENW/2 - _bU);
	g_bOff.y = (FP_SCREENH/2 - _bV);

	_bU = _bU + g_bOff.x;
	_bV = _bV + g_bOff.y;
	//----------------------------
	//	Draw Board
	//----------------------------
	for(int y = 0; y < FP_BOARDH; ++y)
		for(int x = (FP_BOARDW - 1); x >= 0; --x)
		{
			//----------------------------------------
			//	Calculate Tile Pos
			//----------------------------------------
			int u = int((y+x)*28.62f) + g_bOff.x;
			int v = int((y-x)*14.31f) + g_bOff.y;

			if((u > -64) && (v > -32) && (u < FP_SCREENW) && (v < FP_SCREENH))
			{
				//----------------------------------------
				//	Get Tile Value
				//----------------------------------------
				int _tile = g_Tiles[(x) + (y)*FP_BOARDW];

				//----------------------------------------
				//	Draw Tile
				//----------------------------------------
				if((_tile & 127) > 3)
					g_gfxTile[10]->blitAlphaRect(0,0,64,48,u,v - 15);
				else if((_tile & 127) > 0)
				{
					switch(g_ballMode*10 + (_tile & 127))
					{
						case  1:	g_gfxTile[_tile+0]->blitAlphaRect(0,0,64,48,u,v - 0);	break;
						case  2:	g_gfxTile[_tile+3]->blitAlphaRect(0,0,64,64,u,v -20);	break;
						case  3:	g_gfxTile[_tile+6]->blitAlphaRect(0,0,64,64,u,v -16);	break;
						case 11:	g_gfxTile[_tile+6]->blitAlphaRect(0,0,64,64,u,v -16);	break;
						case 12:	g_gfxTile[_tile  ]->blitAlphaRect(0,0,64,48,u,v - 0);	break;
						case 13:	g_gfxTile[_tile+3]->blitAlphaRect(0,0,64,64,u,v -20);	break;
						case 21:	g_gfxTile[_tile+3]->blitAlphaRect(0,0,64,64,u,v -20);	break;
						case 22:	g_gfxTile[_tile+6]->blitAlphaRect(0,0,64,64,u,v -16);	break;
						case 23:	g_gfxTile[_tile  ]->blitAlphaRect(0,0,64,48,u,v - 0);	break;
					};
				}
				else if((_tile & 127) == 0)
					g_gfxTile[0]->blitAlphaRect(0,0,64,48,u,v);
				//----------------------------------------
				//	Draw Special Bit
				//----------------------------------------
				if(_tile & FP_X_START)
				{
					g_gfxTile[11]->blitAlphaRect(0,0,64,48,u,v - 0);
				}
				else if(_tile & FP_X_END)
				{
					if(g_flags > 0)
						g_gfxTile[12]->blitAlphaRect(0,0,64,48,u,v -16);
					else
						g_gfxTile[12]->blitAlphaRect(64,0,128,48,u,v -16);
				}
				else if(_tile & FP_X_FLAG)
				{
					g_gfxTile[13]->blitAlphaRect(0,0,64,48,u,v - 16);
				}
				else if(_tile & FP_X_DROP)
				{
					g_gfxTile[14]->blitAlphaRect(0,0,64,48,u,v);
				}
			}
		}
	//----------------------------
	//	Draw Ball
	//----------------------------
	{
		g_gfxBall[g_ballMode+3]->blitAlphaRect(0,0,64,64,int(_bU)-16,int(_bV));
	}
}
//========================================================================
//		Render Menu
//========================================================================
void RenderMENU()
{
	//-------------------------
	//	Clean
	//-------------------------
	g_gameWin->setDefaultWorldView();
	g_gameWin->setClearColor(0,0,1,0);

	//-------------------------
	//	Draw Board
	//-------------------------
	g_gfxBack->drawRect(0,0,800.0f,600.0f, 0.2f,0.2f,0.2f,1.0f);

	//-------------------------
	//	Menu
	//-------------------------
	if(g_menu < 10)
	{
		g_gameTxt->setHeightPix(80);
		//-------------------------
		//	Option - START GAME
		//-------------------------
		g_gameTxt->setColor(1.0f,1.0f,1.0f - (g_menu == 0)*0.5f,0.5f + (g_menu == 0)*0.5f);
		g_gameTxt->drawStringCentered("Start",0,800,100);
		//-------------------------
		//	Option - LEVEL
		//-------------------------
		g_gameTxt->setColor(1.0f,1.0f,1.0f - (g_menu == 1)*0.5f, 0.5f + (g_menu == 1)*0.5f);
		g_gameTxt->drawStringCentered("Level",0,800,200);
		char _buffer[9] = {0};
		sprintf(_buffer,"< %02i >",g_level);
		g_gameTxt->drawStringCentered(_buffer,0,800,260);
		//-------------------------
		//	Option - HIGHSCORES
		//-------------------------
		g_gameTxt->setColor(1.0f,1.0f,1.0f - (g_menu == 2)*0.5f,0.5f + (g_menu == 2)*0.5f);
		g_gameTxt->drawStringCentered("Highscores",0,800,370);
		//-------------------------
		//	Option - QUIT GAME
		//-------------------------
		g_gameTxt->setColor(1.0f,1.0f,1.0f - (g_menu == 3)*0.5f,0.5f + (g_menu == 3)*0.5f);
		g_gameTxt->drawStringCentered("Quit",0,800,480);

		//-------------------------
		//	Render Mini Map
		//-------------------------
		RenderMINI();
	}
	//------------------------------------
	//	Draw Highscores
	//------------------------------------
	else if(g_menu >= 100)
	{
		g_gameTxt->setHeightPix(40);

		for(int c = 0; c < FP_LEVEL_COUNT; ++c)
		{
			char _buffer[50] = {0};
			char* _score = getTime(g_Highscore[c]);
			sprintf(_buffer,"[%i] ---  %s",c,_score);
			g_gameTxt->drawStringCentered(_buffer,0,800,c*40);
			delete[] _score;
		}
	}
}

//========================================================================
//		Calculate Force at Point
//	FORCE = FP_MAGCONST * (MASS1 * MASS2) / SQRDDIST
//========================================================================
FPVec2 CalcForce(FPVec2 _pos, int _mode)
{
	if(_mode < 0)
		return FPVec2(0,0);

	FPVec2 _total;
	int _pull = ((_mode+1) % 3) + 1;
	int _push = ((_mode+2) % 3) + 1;
	//-----------------------------------
	//	Loop Through Map
	//-----------------------------------
	for(int x = 0; x < FP_BOARDW;  ++x)
	{
		for(int y = 0; y < FP_BOARDH; ++y)
		{
			//---------------------
			//	Get Tile Value
			//---------------------
			int& _tile = g_Tiles[(x) + (y)*FP_BOARDW];
			//---------------------
			//	PULL
			//---------------------
			if(_tile == _pull)
			{
				FPVec2 _delta = FPVec2((x*32.0f + 16.0f),(y*32.0f + 16.0f)) - _pos;
				float _force = FP_MAGCONST / _delta.SqrdMag();
				_delta *= (_force / _delta.Magnitude());
				_total += _delta;
			}
			//---------------------
			//	PUSH
			//---------------------
			else if(_tile == _push)
			{
				FPVec2 _delta = FPVec2((x*32.0f + 16.0f),(y*32.0f + 16.0f)) - _pos;
				float _force = FP_MAGCONST / _delta.SqrdMag();
				_delta *= (_force / _delta.Magnitude());
				_total -= _delta;
			}
		}
	}

	return _total;
}
//========================================================================
//	Move Ball
//========================================================================
void MoveBall()
{
	//------------------------------
	//	Calc Accel
	//------------------------------
	FPVec2 _total = CalcForce(g_ballPos,g_ballMode);
	if(_total.SqrdMag() > 0.0f)
		_total.Normalize();

	//------------------------------
	//	Change Direction
	//------------------------------
	g_ballDir = g_ballDir + _total * 2.0f;
	float _dirMag = g_ballDir.Magnitude();

	if(_dirMag > 20.0f)
		g_ballDir *= (20.0f /_dirMag);

	//------------------------------
	//	Collision Detection
	//------------------------------
	{
		int x = int((g_ballPos.x + g_ballDir.x * (g_Delta / 100000.0f)) / 32.0f);
		int y = int((g_ballPos.y + g_ballDir.y * (g_Delta / 100000.0f)) / 32.0f);

        //-------------------------------
		//	Check | Walls
		//-------------------------------
		if(g_ballDir.x > 0.0f)
		{
			if(CollideBlock(x+1,y))		
				g_ballDir.x *= -0.8f;
		}
		else
		{
			if(CollideBlock(x-1,y))		
				g_ballDir.x *= -0.8f;
		}

		//-------------------------------
		//	Check -- Walls
		//-------------------------------
		if(g_ballDir.y > 0.0f)
		{
			if(CollideBlock(x,y+1))		
				g_ballDir.y *= -0.8f;
		}
		else
		{
			if(CollideBlock(x,y-1))		
				g_ballDir.y *= -0.8f;
		}


		//-------------------------------
		//	Angular Collisions
		//-------------------------------
		bool _collide = CollideBlock(x,y);
		if(g_ballDir.x > 0.0f)
		{
			if(g_ballDir.y > 0.0f)
			{
				_collide |= CollideBlock(x+1,y+1);
			}
			else
			{
				_collide |= CollideBlock(x+1,y-1);
			}
		}
		else
		{
			if(g_ballDir.y > 0.0f)
			{
				_collide |= CollideBlock(x-1,y+1);
			}
			else
			{
				_collide |= CollideBlock(x-1,y-1);
			}
		}

		if(_collide)
		{
			g_ballDir.x = 0;
			g_ballDir.y = 0;
		}
	}
	//------------------------------
	//	Change Position
	//------------------------------
	g_ballPos += g_ballDir * (g_Delta / 100000.0f);
	g_ballPos.x = min(FP_BOARDW*32.0f,max(0.0f,g_ballPos.x));
	g_ballPos.y = min(FP_BOARDH*32.0f,max(0.0f,g_ballPos.y));
}
//========================================================================
//	Check for Pickups
//========================================================================
void CheckPickups()
{
	//------------------------------
	//	Look for Pickups
	//------------------------------
	g_flags = 0;
	for(int x = 0; x < FP_BOARDW; ++x)
	for(int y = 0; y < FP_BOARDH; ++y)
	{
		//------------------------------
		//	Count Flags
		//------------------------------
		if((g_Tiles[x+y*FP_BOARDW] & FP_X_FLAG) > 0)
		{
			FPVec2 _deltaFlag(g_ballPos.x - (x*32.0f + 16.0f),g_ballPos.y - (y*32.0f + 16.0f));
			_deltaFlag /= 32.0f;

			//---------------------------
			//	Pickup Flag
			//---------------------------
			if(_deltaFlag.SqrdMag() < 1.5f)
			{
				g_Tiles[x+y*FP_BOARDW] -= FP_X_FLAG;
			}
			//---------------------------
			//	Add Flag to left list
			//---------------------------
			else
			{
				g_flags += 1;
			}
		}
		//------------------------------
		//	Drop Zones
		//------------------------------
		else if((g_Tiles[x+y*FP_BOARDW] & FP_X_DROP) > 0)
		{
			FPVec2 _deltaFlag(g_ballPos.x - (x*32.0f + 16.0f),g_ballPos.y - (y*32.0f + 16.0f));
			_deltaFlag /= 32.0f;

			//---------------------------
			//	Fall Down Hole
			//---------------------------
			if(_deltaFlag.SqrdMag() < 1.5f)
			{
				g_ballPos.x = g_bStart.x*32.0f + 16.0f;
				g_ballPos.y = g_bStart.y*32.0f + 16.0f;
			}
		}
	}
	//------------------------------
	//	Check END
	//	ONLY if no flags left
	//------------------------------
	if(g_flags == 0)
	{
		FPVec2 _deltaEnd(g_ballPos.x - (g_bEnd.x*32.0f + 16.0f),g_ballPos.y - (g_bEnd.y*32.0f + 16.0f));
		_deltaEnd /= 32.0f;
		if(_deltaEnd.SqrdMag() < 1.5f)
		{
			//-------------------------------
			//	LEVEL COMPELTED
			//-------------------------------
			g_State = FP_SL_WIN;
		}
	}
}

//========================================================================
//	Collision Test
//========================================================================
bool CollideBlock(int x, int y)
{
	//--------------------------
	//	Check Block is Solid
	//--------------------------
	int _tile = g_Tiles[(x) + (y)*FP_BOARDW];
	_tile = _tile & 127;
	if(_tile <= 0)
		return false;
	//--------------------------
	//	Get Vectors
	//--------------------------
	FPVec2 _tilePos(x*32.0f + 16.0f,y*32.0f + 16.0f);
	FPVec2 _delta = _tilePos - g_ballPos;

	//--------------------------
	//	Quick Check
	//--------------------------
	float _dMag = _delta.Magnitude();
	if(_dMag < 32.0f)		return true;
	if(_dMag > 50.0f)		return false;

	//--------------------------
	//	Crop Delta
	//--------------------------
	_delta.x = max(-16.0f,min(16.0f,_delta.x));
	_delta.y = max(-16.0f,min(16.0f,_delta.y));

	//--------------------------
	//	Test Edge / Corner
	//--------------------------
	float _dist = (g_ballPos - (_tilePos - _delta)).Magnitude();
	return(_dist < 16.0f);
}

//========================================================================
//	SAVE FILE
//========================================================================
void saveTFile(const char* _filename)
{
	FILE* _file = fopen(_filename,"wt");
	//-------------------------------
	//	Map Header
	//-------------------------------
	fprintf(_file,"[ MAP_WIDTH = %i ] \n",FP_BOARDW);
	fprintf(_file,"[ MAP_HEIGHT = %i ] \n",FP_BOARDH);
	fprintf(_file,"[ MAP_START = %i : %i] \n",	g_bStart.x,	g_bStart.y);
	fprintf(_file,"[ MAP_END = %i : %i] \n",	g_bEnd.x,	g_bEnd.y);
	fprintf(_file,"[ GOLD   = %u ] \n",	g_Gold);
	fprintf(_file,"[ SILVER = %u ] \n",	g_Silver);
	fprintf(_file,"[ BRONZE = %u ] \n",	g_Bronze);

	//-------------------------------
	//	Map Data
	//-------------------------------
	fputs("[START :",_file);
	for(int c = 0; c < FP_BSIZE; ++c)
	{
		fputc(g_Tiles[c] + 100,_file);	// ADD 100 to avoid control char
	}
	fputs(": END]",_file);

	fclose(_file);
}
//========================================================================
//	SAVE FILE
//========================================================================
void saveBFile(const char* _filename)
{
	FILE* _file = fopen(_filename,"wb");
	//-------------------------------
	//	Map Header
	//-------------------------------
	fputc(FP_BOARDW,_file);
	fputc(FP_BOARDH,_file);
	fputc(g_bStart.x,_file);
	fputc(g_bStart.y,_file);
	fputc(g_bEnd.x,_file);
	fputc(g_bEnd.y,_file);
	fwrite(&g_Gold,sizeof(unsigned int),1,_file);
	fwrite(&g_Silver,sizeof(unsigned int),1,_file);
	fwrite(&g_Bronze,sizeof(unsigned int),1,_file);
	fwrite(g_Tiles,sizeof(int),FP_BSIZE,_file);

	fclose(_file);
}
//========================================================================
//	LOAD FILE
//========================================================================
void loadBFile(const char* _filename)
{
	//----------------------------------
	//	Reset Data
	//----------------------------------
	memset(g_Tiles,		0,sizeof(int)*FP_BSIZE);
	g_ballMode = 2;
	g_Timer = 0;

	FILE* _file = fopen(_filename,"rb");
	if (_file!=NULL)
	{
		//-------------------------------
		//	Map Header
		//-------------------------------
		fgetc(_file);
		fgetc(_file);
		g_bStart.x = fgetc(_file);
		g_bStart.y = fgetc(_file);
		g_bEnd.x = fgetc(_file);
		g_bEnd.y = fgetc(_file);
		fread(&g_Gold,sizeof(unsigned int),1,_file);
		fread(&g_Silver,sizeof(unsigned int),1,_file);
		fread(&g_Bronze,sizeof(unsigned int),1,_file);
		fread(g_Tiles,sizeof(int),FP_BSIZE,_file);
	}
    fclose(_file);


	//------------------------------------
	//	Add BITS for Special Tiles
	//------------------------------------
	{
		g_Tiles[g_bStart.x + g_bStart.y*FP_BOARDW] = FP_X_START;
		g_Tiles[g_bEnd.x + g_bEnd.y*FP_BOARDW] = FP_X_END;
	}

	//-------------------------------
	//	Setup Ball
	//-------------------------------
	g_ballPos.x = (g_bStart.x)*32.0f + 16.0f;
	g_ballPos.y = (g_bStart.y)*32.0f + 16.0f;
	g_ballDir.x = 0.0f;
	g_ballDir.y = 0.0f;
	g_ballMode = 0;

	//--------------------------------
	//	Precalulate Forces for Tiles
	//--------------------------------
	CalcField();
	CheckPickups();
}
//========================================================================
//	LOAD FILE
//========================================================================
void loadTFile(const char* _filename)
{
	//----------------------------------
	//	Reset Data
	//----------------------------------
	memset(g_Tiles,		0,sizeof(int)*FP_BSIZE);
	g_ballMode = 2;
	g_Timer = 0;

	FILE* _file = fopen(_filename,"rt");
	if (_file!=NULL)
	{
		//--------------------------
		//	Measure File
		//--------------------------
		fseek(_file,0, SEEK_END);
		long _fSize = ftell(_file);
		rewind(_file);

		//--------------------------
		//	Allocate and Copy
		//--------------------------
		char* _buffer = new char[_fSize];
		memset(_buffer,0,(_fSize)*sizeof(char));
		fread(_buffer,1,(_fSize)*sizeof(char),_file);
		char* _cursor = _buffer;

		//-------------------------------
		//	Map Header
		//-------------------------------
		_cursor = strstr(_buffer,"[ MAP_START");
		if(_cursor)		sscanf(_cursor,"[ MAP_START = %i : %i] \n", &(g_bStart.x), &(g_bStart.y));
		_cursor = strstr(_buffer,"[ MAP_END");
		if(_cursor)		sscanf(_cursor,"[ MAP_END = %i : %i] \n", &(g_bEnd.x), &(g_bEnd.y));
		_cursor = strstr(_buffer,"[ GOLD");		
		if(_cursor)		sscanf(_cursor,"[ GOLD   = %u ] \n",	&g_Gold);
		_cursor = strstr(_buffer,"[ SILVER");
		if(_cursor)		sscanf(_cursor,"[ SILVER = %u ] \n",	&g_Silver);
		_cursor = strstr(_buffer,"[ BRONZE");
		if(_cursor)		sscanf(_cursor,"[ BRONZE = %u ] \n",	&g_Bronze);

		//-------------------------------
		//	Map Data
		//-------------------------------
		_cursor = strstr(_buffer,"[START :") + 8;

		for(int c = 0; c < FP_BSIZE; ++c)
		{
			g_Tiles[c] = _cursor[c] - 100;
		}

		//--------------------------
		//	Terminate
		//--------------------------		
		fclose(_file);
		delete[] _buffer;
	}
	//------------------------------------
	//	Add BITS for Special Tiles
	//------------------------------------
	{
		g_Tiles[g_bStart.x + g_bStart.y*FP_BOARDW] = FP_X_START;
		g_Tiles[g_bEnd.x + g_bEnd.y*FP_BOARDW] = FP_X_END;
	}

	//-------------------------------
	//	Setup Ball
	//-------------------------------
	g_ballPos.x = (g_bStart.x)*32.0f + 16.0f;
	g_ballPos.y = (g_bStart.y)*32.0f + 16.0f;
	g_ballDir.x = 0.0f;
	g_ballDir.y = 0.0f;
	g_ballMode = 0;

	//--------------------------------
	//	Precalulate Forces for Tiles
	//--------------------------------
	CalcField();
	CheckPickups();
}

//========================================================================
//	Convert Time
//========================================================================
char* getTime(unsigned int _milli)
{
	int _outMin  = (_milli/(60*60));	
	int _outSecs = (_milli/60) - (60*_outMin);
	int _outMili = _milli%60;
	char* _buffer = new char[50];
	sprintf(_buffer,"%02i : %02i : %02i",_outMin,_outSecs,_outMili);
	return _buffer;
}

//========================================================================
//	Save High Score
//========================================================================
void saveHighScore(const char* _filename)
{
	FILE* _file = fopen(_filename,"wt");
	//-------------------------------
	//	Save High Scores
	//-------------------------------
	for(int c = 0; c < FP_LEVEL_COUNT; ++c)
	{
		fprintf(_file," [%i]%u \n",c,g_Highscore[c]);
	}

	fclose(_file);
}
//========================================================================
//	Load High Score
//========================================================================
void loadHighScore(const char* _filename)
{
	FILE* _file = fopen(_filename,"rt");
	if (_file!=NULL)
	{
		while(feof(_file) == 0)
		{
			//--------------------------
			//	Load High Score
			//--------------------------
			int _index;
			unsigned int _score;
			fscanf(_file," [%i]%u \n",&_index,&_score);
			g_Highscore[_index] = _score;
		}
		//--------------------------
		//	Terminate
		//--------------------------		
		fclose(_file);
	}
}
//========================================================================
//	Precalculate Field Lines
//========================================================================
void CalcField()
{
	//--------------------------------------
	//	Clean Memory
	//--------------------------------------
	memset(g_Force,0,sizeof(FPVec2)*FP_BSIZE*3);

	//-------------------------------
	//	Loop Field
	//-------------------------------
	for(int y = 0; y < FP_BOARDH; ++y)
	for(int x = 0; x < FP_BOARDW; ++x)
	{
		//-------------------------------
		//	Get slightly off centre
		//-------------------------------
		FPVec2 _realPos(x*32.0f + 16.01f, y*32.0f + 16.01f);

		//-------------------------------
		//	Loop Through Modes
		//-------------------------------
		for(int c = 0; c < 3; ++c)
		{
			FPVec2 _res = CalcForce(_realPos,c);
			if(_res.SqrdMag() > 0.0f)
				_res.Normalize();

			_res *= 15.0f;
			g_Force[x + y*FP_BOARDW + c*FP_BSIZE] = _res;
		}
	}
}