/**************************************************************************
*
* File:		GaEditState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaEditState.h"
#include "GaGameHudState.h"
#include "GaMainMenuState.h"

#include "BcMain.h"

#include "ObjManager.h"
#include "PhManager.h"

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
GaEditState::GaEditState():
	GameStateInput_( gaGS_INP_IDLE ),
	SelectedTile_( 0 )
{
	//
	WorldPos_ = BcVec2d( 0.0f, 7712.0f );

	//
	pMap_ = new GaMap();
	pMap_->reset();

	ObjManager::pInstance()->pMap( pMap_ );

	//
	GaGameHudState* pHudState = new GaGameHudState();

	pHudState->addButton( "Select Blank" );
	pHudState->addButton( "Select Wall" );
	pHudState->addButton( "Add Enemy" );
	pHudState->addButton( "Set Portal" );
	pHudState->addButton( "Save Map" );

	spawnState( pHudState, BcFalse );
}

//////////////////////////////////////////////////
// Dtor
GaEditState::~GaEditState()
{

}

//////////////////////////////////////////////////
// execute
void GaEditState::execute()
{
	handleGameStateInput();

	//
	pMap_->update();

	//
	if( BcMain::pInstance()->keyPressed( SDLK_ESCAPE ) )
	{
		pParent()->spawnState( new GaMainMenuState(), BcFalse );
	}
}

//////////////////////////////////////////////////
// render
void GaEditState::render()
{
	pMap_->render( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );
	pMap_->renderEnemies( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );
	pMap_->renderPortal( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );
}

//////////////////////////////////////////////////
// handleGameStateInput
void GaEditState::handleGameStateInput()
{
	BcVec2d MousePos = BcMain::pInstance()->cursorPos();

	switch( GameStateInput_ )
	{
	case gaGS_INP_IDLE:
		{
			if( MousePos.X_ > 192.0f )
			{
				// Left mouse, put piece.
				if( BcMain::pInstance()->mouseDown( 1 ) )
				{
					BcVec2d RenderFrom = ( MousePos - BcVec2d( 192.0f, 0.0f ) ) + WorldPos_;

					BcS32 iX = static_cast< BcS32 >( RenderFrom.X_ / 64.0f );
					BcS32 iY = static_cast< BcS32 >( RenderFrom.Y_ / 64.0f );

					if( SelectedTile_ < gaMAP_TILE_ENEMY )
					{
						if( ObjManager::pInstance()->pMap()->setTile( SelectedTile_, iX, iY ) )
						{
							// Success
						}
						else
						{
							// Failure
						}
					}
				}

				if( BcMain::pInstance()->mousePressed( 1 ) )
				{
					BcVec2d RenderFrom = ( MousePos - BcVec2d( 192.0f, 0.0f ) ) + WorldPos_;

					BcS32 iX = static_cast< BcS32 >( RenderFrom.X_ / 64.0f );
					BcS32 iY = static_cast< BcS32 >( RenderFrom.Y_ / 64.0f );

					if( SelectedTile_ == gaMAP_TILE_ENEMY )
					{
						ObjManager::pInstance()->pMap()->addEnemy( RenderFrom );
					}

					if( SelectedTile_ == gaMAP_TILE_PORTAL )
					{
						ObjManager::pInstance()->pMap()->setPortal( RenderFrom );
					}
				}
				// Middle mouse, move world.
				if( BcMain::pInstance()->mouseDown( 2 ) )
				{
					OldWorldPos_ = WorldPos_;
					MouseDownPos_ = MousePos;
					GameStateInput_ = gaGS_INP_MOVE;
				}

				//

			}

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

						if( ButtonClick < gaMAP_TILE_MAX )
						{
							SelectedTile_ = ButtonClick;
						}
						else
						{
							switch( ButtonClick )
							{
							case 4:
								ObjManager::pInstance()->pMap()->save( "data/map_custom.map" );
								break;

							default:
								break;
							}
						}
					}
				}
			}
		}
		break;

	case gaGS_INP_MOVE:
		{
			// Move world.
			if( BcMain::pInstance()->mouseDown( 2 ) )
			{
				BcVec2d DeltaPos = ( MouseDownPos_ - MousePos ) * BcMain::pInstance()->tick() * 8.0f;

				WorldPos_ -= DeltaPos;

				if( WorldPos_.X_ < 0.0f )
				{
					WorldPos_.X_ = 0.0f;
				}

				if( WorldPos_.Y_ < 0.0f )
				{
					WorldPos_.Y_ = 0.0f;
				}

				if( WorldPos_.X_ > ( GaMap::MAX_XSIZE * 64.0f - 448.0f ) )
				{
					WorldPos_.X_ = ( GaMap::MAX_XSIZE * 64.0f - 448.0f );
				}

				if( WorldPos_.Y_ > ( GaMap::MAX_YSIZE * 64.0f - 480.0f ) )
				{
					WorldPos_.Y_ = ( GaMap::MAX_YSIZE * 64.0f - 480.0f );
				}
			}
			else
			{
				GameStateInput_ = gaGS_INP_IDLE;
			}
		}
		break;

	}
}

