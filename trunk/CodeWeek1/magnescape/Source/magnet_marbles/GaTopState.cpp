/**************************************************************************
*
* File:		GaTopState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTopState.h"

#include "GaMainMenuState.h"
#include "GaGameState.h"
#include "GaEditState.h"

#include "BcMain.h"

#include "GaMusic.h"
#include "SoManager.h"

//////////////////////////////////////////////////
// Sounds
SoSynthParam gSoundEffects[] =
{
	{ soSYNTH_PULSE, 880,  440, 0.2f, 50,  100, 500, 50 },
	{ soSYNTH_SINE,   100, 400, 0.4f, 500, 100, 1000, 5000 },
	{ soSYNTH_SINE,   400, 100, 0.5f, 500, 100, 1000, 5000 },
	{ soSYNTH_NOISE,   0,    0, 0.3f, 1000, 100, 100, 100 },

	{ soSYNTH_SINE,   1660, 880, 0.4f, 50, 2000, 1000, 50 },
	{ soSYNTH_NOISE,   0,    0, 0.3f, 100, 2000, 100, 100 },

	{ soSYNTH_PULSE,  2000, 200, 0.3f, 10, 20, 10, 1000 },
	{ soSYNTH_PULSE,  3000, 300, 0.3f, 10, 50, 10, 1000 },

	{ soSYNTH_SINE,   110,  110, 0.05f, 10000, 20, 1000, 100 },
	{ soSYNTH_SINE,   109,  108, 0.05f, 10000, 20, 1000, 100 },
};

//////////////////////////////////////////////////
// externInit
void externInit()
{
	BcMain::pInstance()->topState( new GaTopState() );
}

//////////////////////////////////////////////////
// Ctor
GaTopState::GaTopState()
{
	spawnState( new GaMainMenuState(), BcFalse );

	for( BcU32 i = 0; i < 10; ++i )
	{
		SoManager::pInstance()->addParams( gSoundEffects[ i ] );
	}

	GaMusic::pInstance()->initialise();
}

//////////////////////////////////////////////////
// Dtor
GaTopState::~GaTopState()
{
	GaMusic::pInstance()->shutdown();
}

//////////////////////////////////////////////////
// execute
void GaTopState::execute()
{
	GaMusic::pInstance()->update();
}

//////////////////////////////////////////////////
// render
void GaTopState::render()
{
	
}