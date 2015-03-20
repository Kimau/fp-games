/*					Coin Pile
	A pile of pennies, last to take a penny loses.
	Uses PTK lib located at http://www.phelios.com/ptk/

			http://www.flammablepenguins.com/	- Claire Blackshaw
*/
//--------------------------------
//	Includes
//--------------------------------
#include <vector>
#include "ptk.h"
#include "KPTK.h"
#include "KSound.h"

//--------------------------------
//	Globals
//--------------------------------
KWindow*	g_gameWin;			// PTK Window
KTrueText*	g_gameTxt;			// PTK Text Instance
KGraphic*	g_gfxBack;			// Graphic Back
KGraphic*	g_gfxCoin;			// Graphic Coin
KGraphic*	g_gfxDie[6];		// Graphic Coin

int			g_State;			// Game State
int			g_coins;			// Coin Total
int			g_die[2];			// Die
int			g_ai;				// AI Last Choice
int			g_last;				// Your Last Choice

using namespace std;

//========================================================================
//	Handels Events while in Menu
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	//---------------------------------------
	//	Restart Game
	//---------------------------------------
	if((_eventPtr->type == K_EVENT_KEYUP) && (_eventPtr->ptkKey == K_VK_R))
	{
		g_coins = rand() % 30 + 30;		// Roll new coin Title
		g_State = 1;					// Set it to Player Turn
		g_die[0] = rand() % 6 + 1;		// Roll New Set of Die
		g_die[1] = rand() % 6 + 1;
	}
	//---------------------------------------
	//	Press a Key to continue
	//---------------------------------------
	if((g_State == 2) && (_eventPtr->type == K_EVENT_KEYUP))
	{
		g_coins -= g_ai;
		g_State = 1;					// Set State to Player
		g_die[0] = rand() % 6 + 1;		// Roll New Set of Die
		g_die[1] = rand() % 6 + 1;

		if(g_coins <= 0)
		{
			g_State = 6;
		}

		return true;
	}
	//----------------------------
	//	Normal State
	//----------------------------
	if((g_State == 1) && (_eventPtr->type == K_EVENT_KEYUP))
	{
		//---------------------------
		//	Process Key
		//---------------------------
		switch(_eventPtr->ptkKey)
		{
		case K_VK_LEFT:
			{
				g_last = g_die[0];		// Store Dice
				g_coins -= g_die[0];	// Remove Dice
				g_State = 3;			// Ask AI
			}
			break;
		case K_VK_RIGHT:
			{
				g_last = g_die[1];		// Store Dice
				g_coins -= g_die[1];	// Remove Dice
				g_State = 3;			// Ask AI
			}
			break;
		case K_VK_ESCAPE:
			{
				g_State = 0;			// Quit Game
			}
			break;
		};

		if(g_coins <= 0)
		{
			g_State = 5;
		}
		return true;
	}
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
	char _buffer[50] = {0};											// Temp Buffer
	srand(KMiscTools::getMilliseconds());							// Seed Random
	g_State = 1;													// Set State to Player
	g_ai = 0;
	g_last = 0;
	//-----------------------------------
	//	Create Window
	//-----------------------------------
	g_gameWin = KPTK::createKWindow(K_OPENGL);						// Init Window
	g_gameWin->createGameWindow(640,480,32,true,"FP Coin Grabber");	// Create Window
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
	g_gfxCoin = KPTK::createKGraphic("coin.png");					// Coin Graphic	
	g_gfxDie[0] = KPTK::createKGraphic("die1.png");					// Load Dice
	g_gfxDie[1] = KPTK::createKGraphic("die2.png");					// More efficient if a bit cludgy to type it out
	g_gfxDie[2] = KPTK::createKGraphic("die3.png");
	g_gfxDie[3] = KPTK::createKGraphic("die4.png");
	g_gfxDie[4] = KPTK::createKGraphic("die5.png");
	g_gfxDie[5] = KPTK::createKGraphic("die6.png");

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_coins = rand() % 30 + 30;
	g_die[0] = rand() % 6 + 1;
	g_die[1] = rand() % 6 + 1;
	g_gameTxt->setHeightPix(30);
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

		//-------------------------
		//	Display Coin Total
		//-------------------------
		{
			int c = 0;
			for(c = 0; c < g_coins; ++c)
			{
				g_gfxCoin->blitAlphaRect(0,0,76,7,280+((c/5)%2)*8,460 - (c*6));
			}
			itoa(g_coins,_buffer,10);
			g_gameTxt->drawStringFromLeft(_buffer,300,430 - (c*6));
		}

		//-------------------------
		//	Last Choice Choice
		//-------------------------
		g_gameTxt->drawStringFromLeft("Last Choice",50,300);
		itoa(g_last,_buffer,10);
		g_gameTxt->drawStringFromLeft(_buffer,100,330);
		//-------------------------
		//	AI Choice
		//-------------------------
		g_gameTxt->drawStringFromLeft("AI Choice",420,300);
		itoa(g_ai,_buffer,10);
		g_gameTxt->drawStringFromLeft(_buffer,470,330);

		//==================================
		//	Chose State
		//==================================
		switch(g_State)
		{
			//=================================
			//		Await Player Choice
			//=================================
			case 1:
			{
				//-------------------------
				//	Display Dice
				//-------------------------
				g_gfxDie[g_die[0] - 1]->blitAlphaRect(0,0,64,64,100,160);
				g_gfxDie[g_die[1] - 1]->blitAlphaRect(0,0,64,64,170,140);

				g_gameTxt->drawStringFromLeft("Use <- or -> to select dice",80,10);
			}break;

			//=================================
			//		Show AI Choice
			//=================================
			case 2:
			{
				//-------------------------
				//	Display AI Choice
				//-------------------------
				g_gfxDie[g_die[0] - 1]->blitAlphaRect(0,0,64,64,640-155-64,140);	// Compiler will optimize the sum
				g_gfxDie[g_die[1] - 1]->blitAlphaRect(0,0,64,64,640-90-64,160);

				g_gameTxt->drawStringFromLeft("Press ANY key",200,10);
			}break;

			//=================================
			//		Ask AI
			//=================================
			case 3:
			{
				g_die[0] = rand() % 6 + 1;
				g_die[1] = rand() % 6 + 1;
				int _ideal = (g_coins - 1) % 6;

				if((g_die[0] - _ideal) < (g_die[1] - _ideal))
				{
					g_ai = g_die[0];
				}
				else
				{
					g_ai = g_die[1];
				}
				g_State = 2;

			}break;
			//=================================
			//		GAME OVER
			//=================================
			case 5:	
				g_gameTxt->drawStringFromLeft("WINNER",440,160);					// COMPUTER WIN!
				g_gameTxt->drawStringFromLeft("Press R to Restart",180,10);			
				break;
			case 6:	
				g_gameTxt->drawStringFromLeft("WINNER",110,160);					// PLAYER WIN!
				g_gameTxt->drawStringFromLeft("Press R to Restart",180,10);			
				break;
		};
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