//--------------------------------
//	Includes
//--------------------------------
#include <vector>
#include "ptk.h"
#include "gameTile.h"

//--------------------------------
//	Define
//--------------------------------
#define		FP_SCREENW	800
#define		FP_SCREENH	600
#define		FP_TITLE	"FP Tile Builder"
#define		FP_VISIW		9				// Visible Area
#define		FP_VISIH		9
#define		FP_BOARDW		25				// Entire Play Board
#define		FP_BOARDH		25
#define		FP_BSIZE	(FP_BOARDW * FP_BOARDH)
#define		FP_XOFFSET		200
#define		FP_YOFFSET		15

//--------------------------------
//	Predeclare
//--------------------------------
void Render();
bool CheckTile(int x, int y, gameTile _tile);

//--------------------------------
//	Globals
//--------------------------------
KWindow*	g_gameWin;			// PTK Window
KTrueText*	g_gameTxt;			// PTK Text Instance
KGraphic*	g_gfxBack;			// Graphic Back
KGraphic*	g_gfxTile;			// Graphic Tile

int			g_State;			// Game State
int			g_bCursor[2];		// Game Board Cursor 
int			g_bScroll[2];		// Game Board Scroll
gameTile	g_Tiles[FP_BSIZE];	// Game Board
gameTile	g_cTile;			// Current Tile

bool		g_validTile;

using namespace std;

//========================================================================
//	Handels Events while in Menu
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	switch(_eventPtr->type)
	{
	//--------------------------
	//	Event Key Down
	//--------------------------
	case K_EVENT_KEYDOWN:
		{
			switch(_eventPtr->ptkKey)
			{
			//------------------------------------
			//	Scrolling Keys
			//------------------------------------
			case K_VK_LEFT:
				{
					g_bScroll[0] = max(0,g_bScroll[0] - 1);
					break;
				}
			case K_VK_RIGHT:
				{
					g_bScroll[0] = min(FP_BOARDW - FP_VISIW,g_bScroll[0] + 1);
					break;
				}
			case K_VK_UP:
				{
					g_bScroll[1] = max(0,g_bScroll[1] - 1);
					break;
				}
			case K_VK_DOWN:
				{
					g_bScroll[1] = min(FP_BOARDH - FP_VISIH,g_bScroll[1] + 1);
					break;
				}
			};
		break;
		}
	//--------------------------
	//	Mouse Up
	//	Place Tile
	//--------------------------
	case K_EVENT_MOUSEUP:
		{
			switch(_eventPtr->buttonIndex)
			{
			case K_LBUTTON:
				{
					//--------------------------
					//	Place if Valid
					//--------------------------
					if(g_validTile)
					{
						g_Tiles[g_bCursor[0] + g_bCursor[1]*FP_BOARDW] = g_cTile;
						g_cTile.Generate();
					}
					break;
				}
			case K_RBUTTON:
				//--------------------------
				//	Rotate Piece
				//--------------------------
				{
					g_cTile.RotateCW();
					break;
				}				
			};
			break;
		}
	};
	return true;
}

//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	//-----------------------------------
	//	Init Game
	//-----------------------------------
	srand(KMiscTools::getMilliseconds());							// Seed Random
	g_State = 0;													// Set State to Player
	//-----------------------------------
	//	Create Window
	//-----------------------------------
	g_gameWin = KPTK::createKWindow(K_OPENGL);						// Init Window
	g_gameWin->createGameWindow(FP_SCREENW,FP_SCREENH,32,true,FP_TITLE);	// Create Window
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
	g_gfxTile = KPTK::createKGraphic("tile.png");					// Tile Graphic

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_gameTxt->setHeightPix(30);
	g_State = 1;
	memset(g_Tiles,0,FP_BSIZE * sizeof(gameTile));
	memset(g_bCursor,0,2 * sizeof(int));
	memset(g_bScroll,0,2 * sizeof(int));
	
	//----------------------------------
	//	Random Tile
	//----------------------------------
	for(int c = 0; c < 8; ++c)
	{
		g_Tiles[rand() % FP_BSIZE].Generate();
	}
	g_cTile.Generate();
	//-----------------------------------
	//	FULL BOARD
	//	USED FOR TESTING
	//-----------------------------------
	/*int _failed = 0;
	while(_failed < 3)
	{
		g_cTile.Generate();
		int c = 0;
		int t = 0;
		while((c < FP_BSIZE) && (t < 4))
		{
			if(CheckTile(c%FP_BOARDW,c/FP_BOARDW,g_cTile))
			{
				g_Tiles[c] = g_cTile;
				--_failed;
				break;
			}
			++c;

			if(c == FP_BSIZE)
			{
				c = 0;
				++t;
				g_cTile.RotateCW();
			}
		}
		++_failed;
	}*/
	//-----------------------------------
	//	Game Loop
	//-----------------------------------
	while((g_State) && (g_gameWin->isQuit() == false))
	{
		//-------------------------
		//	Get Cursor
		//-------------------------
		g_bCursor[0] = g_bScroll[0] + (KInput::getMouseX()-FP_XOFFSET) / 64;
		g_bCursor[0] = max(g_bScroll[0],min(FP_BOARDW,min(g_bScroll[0]+FP_VISIW-1,g_bCursor[0])));
		g_bCursor[1] = g_bScroll[1] + (KInput::getMouseY()-FP_YOFFSET) / 64;
		g_bCursor[1] = max(g_bScroll[1],min(FP_BOARDH,min(g_bScroll[1]+FP_VISIH-1,g_bCursor[1])));

		g_validTile = (CheckTile((g_bCursor[0]),(g_bCursor[1]),g_cTile));
		//-----------------------------
		//	Render Board
		//-----------------------------
		Render();
		//-----------------------------
		//	Process Events
		//-----------------------------
		g_gameWin->processEvents();
	}
}

//========================================================================
//		Render Game
//========================================================================
void Render()
{
	//-------------------------
	//	Draw
	//-------------------------
	g_gameWin->setDefaultWorldView();
	g_gameWin->setClearColor(0,0,1,0);

	//-------------------------
	//	Draw Board
	//-------------------------
	g_gfxBack->blitRect(0,0,800,600,0,0);
	g_gfxBack->drawRect(FP_XOFFSET,FP_YOFFSET,FP_XOFFSET + FP_VISIW*64.0f,FP_YOFFSET + FP_VISIH*64.0f,0.0f,0.0f,0.0f,0.3f);
	for(int x = 0; x < FP_VISIW;  ++x)
	{
		for(int y = 0; y < FP_VISIH; ++y)
		{
			//---------------------
			//	Get Tile Value
			//---------------------
			gameTile& _tile = g_Tiles[(g_bScroll[0] + x) + (g_bScroll[1] + y)*FP_BOARDW];
			//---------------------
			//	Draw Tile
			//---------------------
			if(!(_tile.Empty()))
			{
				int u = _tile.m_north + _tile.m_west  * 3;
				int v = _tile.m_east  + _tile.m_south * 3;

				int xB = x*64 + FP_XOFFSET;
				int yB = y*64 + FP_YOFFSET;
				g_gfxTile->blitRect(u*64.0f, v*64.0f,(u+1)*64.0f,(v+1)*64.0f,xB,yB);
			}
		}
	}
	//---------------------
	//	Draw Cursor
	//---------------------
	{
		float x = (g_bCursor[0] - g_bScroll[0])*64.0f + FP_XOFFSET;
		float y = (g_bCursor[1] - g_bScroll[1])*64.0f + FP_YOFFSET;
		int u = g_cTile.m_north + g_cTile.m_west  * 3;
		int v = g_cTile.m_east  + g_cTile.m_south * 3;

		if(!g_validTile)
			g_gfxBack->drawRect(x - 10.0f,y - 10.0f,x+74.0f,y+74.0f,1.0f,0.0f,0.0f,1.0f);

		g_gfxTile->blitRect(u*64.0f, v*64.0f,(u+1)*64.0f,(v+1)*64.0f,(short)x,(short)y);
	}
	//------------------------
	//	Draw MiniMap
	//------------------------
	g_gfxTile->drawRect(0,0,6*FP_BOARDW,6*FP_BOARDH,0.0f,0.0f,0.0f,0.5f);
	for(int x = 0; x < FP_BOARDW;  ++x)
	{
		for(int y = 0; y < FP_BOARDH; ++y)
		{
			//---------------------
			//	Get Tile Value
			//---------------------
			gameTile& _tile = g_Tiles[(x) + (y)*FP_BOARDW];
			//---------------------
			//	Draw Tile
			//---------------------
			if(!(_tile.Empty()))
			{
				int u = _tile.m_north + _tile.m_west  * 3;
				int v = _tile.m_east  + _tile.m_south * 3;

				int xM = x*6-28;
				int yM = y*6-28;
				g_gfxTile->blitRectFx(u*64.0f, v*64.0f,(u+1)*64.0f,(v+1)*64.0f,xM,yM,0.0f,0.1f);
			}
		}
	}
	{
		g_gfxTile->drawLine( g_bScroll[0]			 * 6.0f, g_bScroll[1]			 * 6.0f, g_bScroll[0]				* 6.0f,(g_bScroll[1] + FP_VISIH)	* 6.0f,1.0f,0.0f,0.0f,1.0f);
		g_gfxTile->drawLine( g_bScroll[0]			 * 6.0f, g_bScroll[1]			 * 6.0f,(g_bScroll[0] + FP_VISIW)	* 6.0f, g_bScroll[1]				* 6.0f,1.0f,0.0f,0.0f,1.0f);
		g_gfxTile->drawLine((g_bScroll[0] + FP_VISIW)* 6.0f, g_bScroll[1]			 * 6.0f,(g_bScroll[0] + FP_VISIW)	* 6.0f,(g_bScroll[1] + FP_VISIH)	* 6.0f,1.0f,0.0f,0.0f,1.0f);
		g_gfxTile->drawLine( g_bScroll[0]			 * 6.0f,(g_bScroll[1] + FP_VISIH)* 6.0f,(g_bScroll[0] + FP_VISIW)	* 6.0f,(g_bScroll[1] + FP_VISIH)	* 6.0f,1.0f,0.0f,0.0f,1.0f);
	}
	//-------------------------
	// Buffer Flip
	//-------------------------
	g_gameWin->flipBackBuffer();
}
//========================================================================
//	Check if Tile Works
//========================================================================
bool CheckTile(int x, int y, gameTile _tile)
{
	if(!(_tile.Empty()))
	{
		//---------------------------
		//		EMPTY
		//---------------------------
		if(!(g_Tiles[x+y*FP_BOARDW].Empty()))
		{
			return false;
		}
		//---------------------------
		//		Must have neighbour
		//---------------------------
		if(
			(g_Tiles[(x-1)+(y  )*FP_BOARDW].Empty()) &&
			(g_Tiles[(x+1)+(y  )*FP_BOARDW].Empty()) &&
			(g_Tiles[(x  )+(y-1)*FP_BOARDW].Empty()) &&
			(g_Tiles[(x  )+(y+1)*FP_BOARDW].Empty()))
		{
			return false;
		}
		//---------------------------
		//		NORTH
		//--------------------------- 
		if(y != 0)
		{
			if(!(g_Tiles[x+(y-1)*FP_BOARDW].Empty()) && (_tile.m_north != (g_Tiles[x+(y-1)*FP_BOARDW].m_south) ))
			{
				return false;
			}
		}
		//---------------------------
		//		SOUTH
		//---------------------------
		if(y != (FP_BOARDH-1))
		{
			if(!(g_Tiles[x+(y+1)*FP_BOARDW].Empty()) && (_tile.m_south != (g_Tiles[x+(y+1)*FP_BOARDW].m_north) ))
			{
				return false;
			}
		}
		//---------------------------
		//		WEST
		//---------------------------
		if(x != 0)
		{
			if(!(g_Tiles[(x-1)+y*FP_BOARDW].Empty()) && (_tile.m_west != (g_Tiles[(x-1)+y*FP_BOARDW].m_east) ))
			{
				return false;
			}
		}
		//---------------------------
		//		EAST
		//---------------------------
		if(x != (FP_BOARDW-1))
		{
			if(!(g_Tiles[(x+1)+y*FP_BOARDW].Empty()) && (_tile.m_east != (g_Tiles[(x+1)+y*FP_BOARDW].m_west) ))
			{
				return false;
			}
		}
	}
	return true;
}










