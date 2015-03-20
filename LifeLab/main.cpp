/*					pLife
	Uses PTK lib located at http://www.phelios.com/ptk/

			http://www.flammablepenguins.com/	- Claire Blackshaw
*/
//--------------------------------
//	Includes
//--------------------------------
#include <vector>
#include "ptk.h"

#define PL_HEIGHT	40
#define PL_WIDTH	60
#define PL_SIZE		(PL_WIDTH * PL_HEIGHT)
#define PL_STATES	3

//--------------------------------
//	Life Cycle
//--------------------------------
void LifeCycle();

//--------------------------------
//	Globals
//--------------------------------
KWindow*	g_gameWin;			// PTK Window
KTrueText*	g_gameTxt;			// PTK Text Instance
KGraphic*	g_gfxBack;			// Graphic Back

int			g_State;					// Game State
int			g_Gen;						// Which Side of the Pool
int 		g_Pool[PL_SIZE*PL_STATES];	// Pool

using namespace std;

//========================================================================
//	Handels Events while in Menu
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	//----------------------------
	//	Process KEY
	//----------------------------
	if(_eventPtr->type == K_EVENT_KEYDOWN)
	{
		//---------------------------
		//	Process Key
		//---------------------------
		switch(_eventPtr->ptkKey)
		{
		case K_VK_W:
			{
				LifeCycle();
			}
			break;
		case K_VK_R:
			{
				//-----------------------------------
				//	Fill Board
				//-----------------------------------
				memset(g_Pool,0,sizeof(int)*PL_SIZE*PL_STATES);
				g_Gen = 0;
				for(int c = 0; c < 500; ++c)
				{
					int x = rand() % PL_WIDTH;
					int y = rand() % PL_HEIGHT;
					g_Pool[x + y*PL_WIDTH] = 1;
				}
			}
			break;
		case K_VK_ESCAPE:
			{
			}
			break;
		};
	}
	return true;
}
//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	//-----------------------------------
	//	Init Window
	//-----------------------------------
	srand(KMiscTools::getMilliseconds());							// Seed Random

	//-----------------------------------
	//	Create Window
	//-----------------------------------
	g_gameWin = KPTK::createKWindow(K_OPENGL);						// Init Window
	g_gameWin->createGameWindow(640,480,32,true,"Flammable Penguins: Conways Game of Life");	// Create Window
	g_gameWin->setPTKCallBack(&EventHandler);

	//-----------------------------------
	//	Get Font
	//-----------------------------------
	KTrueText _font("5by5.ttf");									// Load Font
	g_gameTxt = &_font;												// Link PTR

	//-----------------------------------
	//	Graphic
	//-----------------------------------
	g_gfxBack = KPTK::createKGraphic("back.png");					// Background Graphic

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	memset(g_Pool,0,sizeof(int)*PL_SIZE*PL_STATES);
	g_gameTxt->setHeightPix(30);
	g_gameTxt->setColor(0.0f,0.0f,0.0f,0.5);
	g_State = 1;
	g_Gen = 0;

	//-----------------------------------
	//	Fill Board
	//-----------------------------------
	for(int c = 0; c < 500; ++c)
	{
		int x = rand() % PL_WIDTH;
		int y = rand() % PL_HEIGHT;
		g_Pool[x + y*PL_WIDTH] = 1;
	}

	//-----------------------------------
	//	Game Loop
	//-----------------------------------
	while((g_State) && (g_gameWin->isQuit() == false))
	{
		//-------------------------
		//	Draw
		//-------------------------
		g_gameWin->setDefaultWorldView();
		g_gameWin->setClearColor(0,0,1,0);

		g_gfxBack->blitRect(0,0,640,480,0,0);
		int _popCount = 0;
		//------------------------------------------------------------------
		//	Draw Pool
		//
		//	TOP LINE	:: (((c / PL_WIDTH)%PL_HEIGHT) == 0)
		//	BOTTOM LINE	:: (((c / PL_WIDTH)%PL_HEIGHT) == (PL_HEIGHT-1))
		//	LEFT LINE	:: ((c % PL_WIDTH) == 0)
		//	RIGHT LINE	:: ((c % PL_WIDTH) == (PL_WIDTH - 1))
		//------------------------------------------------------------------
		for(int x = 0; x < PL_WIDTH; ++x)
		for(int y = 0; y < PL_HEIGHT; ++y)
		{
			if(g_Pool[x + y*PL_WIDTH + ((PL_STATES+g_Gen-2)%PL_STATES)*PL_SIZE] != 0)
				g_gfxBack->drawRect(
				(x)*10.0f + 15.0f,(y)*10.0f + 5.0f,(x+1)*10.0f + 15.0f,(y+1)*10.0f + 5.0f,
				0.0f,0.4f,0.0f,1.0f);

			if(g_Pool[x + y*PL_WIDTH + ((PL_STATES+g_Gen-1)%PL_STATES)*PL_SIZE] != 0)
				g_gfxBack->drawRect(
				(x)*10.0f + 15.0f,(y)*10.0f + 5.0f,(x+1)*10.0f + 15.0f,(y+1)*10.0f + 5.0f,
				0.0f,0.6f,0.0f,1.0f);

			if(g_Pool[x + y*PL_WIDTH + (g_Gen%PL_STATES)*PL_SIZE] != 0)
			{
				g_gfxBack->drawRect(
				(x)*10.0f + 15.0f,(y)*10.0f + 5.0f,(x+1)*10.0f + 15.0f,(y+1)*10.0f + 5.0f,
				0.0f,1.0f,0.0f,1.0f);
				++_popCount;
			}
		}
		g_gameTxt->setHeightPix(30);
		char _buffer[25];
		sprintf(_buffer,"Gen: %i",g_Gen);				g_gameTxt->drawStringFromLeft(_buffer,5,410);
		sprintf(_buffer,"Pop Size: %i",_popCount);		g_gameTxt->drawStringFromLeft(_buffer,5,445);
		g_gameTxt->setHeightPix(10);
		g_gameTxt->drawStringFromLeft("[W] Advance Generation",												 250,405);
		g_gameTxt->drawStringFromLeft("[R] Reset Simulation",												 400,405);
		g_gameTxt->drawStringFromLeft("Conways game of life is a simple demostration of emergent behaviour.",210,415);
		g_gameTxt->drawStringFromLeft("The simulation tends towards a inert or stable state.",				 210,425);
		g_gameTxt->drawStringFromLeft("RULES...",															 210,435);
		g_gameTxt->drawStringFromLeft("1. An empty cell with exactly 3 neighbours births a new cell.",		 210,445);
		g_gameTxt->drawStringFromLeft("2. When a populated cell has less than 2 neighbours it dies.",		 210,455);
		g_gameTxt->drawStringFromLeft("3. When a populated cell has 4 or more neighbours it dies.",			 210,465);
		//-------------------------
		// Buffer Flip
		//-------------------------
		g_gameWin->flipBackBuffer();
		//-----------------------------
		//	Process Events
		//-----------------------------
		g_gameWin->processEvents();
	}
}

//==========================================================================
//	Process a Cycle of Life
//==========================================================================
void LifeCycle()
{
	int* _old = &(g_Pool[(g_Gen % PL_STATES)*PL_SIZE]);
	g_Gen = g_Gen + 1;
	int* _new = &(g_Pool[(g_Gen % PL_STATES)*PL_SIZE]);
	memset(_new,0,sizeof(int)*PL_SIZE);

	for(int x = 0; x < PL_WIDTH; ++x)
	for(int y = 0; y < PL_HEIGHT; ++y)
	{
		bool _pop =  (_old[(x + y*PL_WIDTH)] > 0);
		
		//----------------------------------
		//	Count Neighbours
		//----------------------------------
		int _total = 0;
		_total += _old[((x-1)%PL_WIDTH) + ((y+1)%PL_HEIGHT)*PL_WIDTH];
		_total += _old[((x-1)%PL_WIDTH) + (	y			  )*PL_WIDTH];
		_total += _old[((x-1)%PL_WIDTH) + ((y-1)%PL_HEIGHT)*PL_WIDTH];
		_total += _old[	(x)				+ ((y+1)%PL_HEIGHT)*PL_WIDTH];
		_total += _old[	(x)				+ ((y-1)%PL_HEIGHT)*PL_WIDTH];
		_total += _old[((x+1)%PL_WIDTH) + ((y+1)%PL_HEIGHT)*PL_WIDTH];
		_total += _old[((x+1)%PL_WIDTH) + (	y			  )*PL_WIDTH];
		_total += _old[((x+1)%PL_WIDTH) + ((y-1)%PL_HEIGHT)*PL_WIDTH];

		//----------------------------------
		//	Check if Populated
		//----------------------------------
		if(_pop)
		{
			//----------------------------------
			//	Check Survival
			//----------------------------------
			switch(_total)
			{
			case 0:
			case 1:	// LONELY
				_new[(x + y*PL_WIDTH)] = 0;
				break;
			case 2:	// IDEAL
			case 3:
				_new[(x + y*PL_WIDTH)] = 1;
				break;
			case 4:	// OVER POPULATION	
			case 5:
			case 6:
			case 7:
			case 8:
				_new[(x + y*PL_WIDTH)] = 0;
				break;
			};
		}
		//----------------------------------
		//	If 3 Parents exsists, SPAWN
		//----------------------------------
		else if(_total == 3)
		{
			_new[(x + y*PL_WIDTH)] = 1;
		}
	}
}