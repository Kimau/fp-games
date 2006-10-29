/*====================================================================================
			Basic Game Class
	All game states have common functions to interface.
	OPEN	: Start the game state
	STEP	: Iterate through the Game
	INPUT	: Pass on Mouse and Keyboard Input	
	EXIT	: Close the game state
====================================================================================*/
#ifndef BASIC_GAME_CLASS_OSIE
#define BASIC_GAME_CLASS_OSIE

#include "GLHeader.h"
#include <vector>

class gameBlock abstract
{
public:
	// Interface Functions
	virtual int Open() = 0;
	virtual int Step() = 0;
	virtual int Input(UINT _iMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual int Exit() = 0;
	// Inherited Varibles
protected:
	int m_State;
};

#endif

extern std::vector<gameBlock*> g_BlockPTR;