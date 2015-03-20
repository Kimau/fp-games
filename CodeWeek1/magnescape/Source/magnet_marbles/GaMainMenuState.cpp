/**************************************************************************
*
* File:		GaMainMenuState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaMainMenuState.h"
#include "GaGameHudState.h"

#include "GaGameManagementState.h"
#include "GaGameState.h"
#include "GaEditState.h"

#include "BcMain.h"

#include "RcSpriteManager.h"

#include "GaMusic.h"

//////////////////////////////////////////////////
// Ctor
GaMainMenuState::GaMainMenuState():
	GameStateInput_( gaGS_INP_IDLE ),
	SelectedTile_( 0 )
{
	//
	WorldPos_ = BcVec2d( 0.0f, 7712.0f );

	//
	pMap_ = new GaMap();
	pMap_->reset();

	//
	GaGameHudState* pHudState = new GaGameHudState();

	pHudState->addButton( "Start Game" );
	pHudState->addButton( "Play Map" );
	pHudState->addButton( "Edit Map" );

	GaMusic::pInstance()->play( 52, 54 );

	pTitle_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Magnescape!", 32 );
	pName_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "By Neil Richardson!", 23 );

	spawnState( pHudState, BcFalse );
}

//////////////////////////////////////////////////
// Dtor
GaMainMenuState::~GaMainMenuState()
{
	// Screw it - memory leak. This is what I get for
	// hacking all weekend!
	//PhManager::pInstance()->reset();
	//ObjManager::pInstance()->reset();
	//GaGunManager::pInstance()->reset();
	//GaParticleManager::pInstance()->reset();
}

//////////////////////////////////////////////////
// execute
void GaMainMenuState::execute()
{
	pMap_->update();

	handleGameStateInput();
}

//////////////////////////////////////////////////
// render
void GaMainMenuState::render()
{
	pMap_->render( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );

	pTitle_->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 128.0f ) );
	pName_->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 192.0f ) );
}

//////////////////////////////////////////////////
// handleGameStateInput
void GaMainMenuState::handleGameStateInput()
{
	BcVec2d MousePos = BcMain::pInstance()->cursorPos();

	if( MousePos.X_ < 192.0f )
	{
		GaGameHudState* pHudState = dynamic_cast< GaGameHudState* >( pChild() );

		//
		if( pHudState != NULL )
		{
			BcU32 ButtonOver = BcErrorCode;
			BcU32 ButtonClick = BcErrorCode;

			ButtonOver = pHudState->mouseOver( MousePos );

			if( BcMain::pInstance()->mousePressed( 1 ) )
			{
				ButtonClick = pHudState->mouseClick( MousePos );

				switch( ButtonClick )
				{
				case 0:
					pParent()->spawnState( new GaGameManagementState(), BcFalse );
					break;

				case 1:
					pParent()->spawnState( new GaGameState( "data/map_custom.map" ), BcFalse );
					break;

				case 2:
					pParent()->spawnState( new GaEditState(), BcFalse );
					break;

				default:
					break;
				}
			}
		}
	}
}

