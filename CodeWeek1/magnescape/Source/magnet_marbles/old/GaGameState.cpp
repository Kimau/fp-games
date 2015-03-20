/**************************************************************************
*
* File:		GaGameState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaGameState.h"
#include "GaGameHudState.h"

#include "BcMain.h"

#include "ObjManager.h"
#include "PhManager.h"

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
GaGameState::GaGameState():
	GameStateInput_( gaGS_INP_IDLE ),
	bRunGame_( BcFalse ),
	MagnetAngle_( 0.0f )
{
	//
	pMap_ = new GaMap();
	pMap_->reset();
	pMap_->load( "data/map_custom.map" );

	ObjManager::pInstance()->pMap( pMap_ );

	//
	pSprite_ = RcSpriteManager::pInstance()->pSprite( "data/magnet.tga", 1 );

	//
	GaGameHudState* pHudState = new GaGameHudState();

	pHudState->addButton( "Place Magnet" );
	pHudState->addButton( "Remove Magnet" );
	pHudState->addButton( "GO!" );

	spawnState( pHudState, BcFalse );
}

//////////////////////////////////////////////////
// Dtor
GaGameState::~GaGameState()
{
	ObjManager::pInstance()->delObject( pMagnet_ );
	PhManager::pInstance()->delField( pMagnet_ );
}

//////////////////////////////////////////////////
// execute
void GaGameState::execute()
{
	handleGameStateInput();

	//
	pMap_->update();

	const BcReal TimeStep = 0.005f;
	static BcReal TimeLeft = 0.0f;

	/*TimeLeft += BcMain::pInstance()->tick();
	while( TimeLeft > TimeStep )
	{
		PhManager::pInstance()->step( TimeStep );
		ObjManager::pInstance()->update();	

		TimeLeft -= TimeStep;
	}*/

	if( bRunGame_ )
	{
		for( BcU32 i = 0; i < 2; ++i )
		{
			PhManager::pInstance()->step( BcMain::pInstance()->tick() );
			ObjManager::pInstance()->update();
		}
	}
}

//////////////////////////////////////////////////
// render
void GaGameState::render()
{
	pMap_->render( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );
	pMap_->renderField( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );

	ObjManager::pInstance()->render( WorldPos_ );

	if( GamePlaceMode_ == gaGP_INP_PLACE )
	{
		{
			pSprite_->drawCentered( 0, BcMain::pInstance()->cursorPos(), BcU32( MagnetAngle_ ) );
		}
	}
}

//////////////////////////////////////////////////
// handleGameStateInput
void GaGameState::handleGameStateInput()
{
	BcVec2d MousePos = BcMain::pInstance()->cursorPos();

	switch( GameStateInput_ )
	{
	case gaGS_INP_IDLE:
		{
			if( MousePos.X_ > 192.0f )
			{
				// Middle mouse, move world.
				if( BcMain::pInstance()->mouseDown( 2 ) )
				{
					OldWorldPos_ = WorldPos_;
					MouseDownPos_ = MousePos;
					GameStateInput_ = gaGS_INP_MOVE;
				}

				switch( GamePlaceMode_ )
				{
				case gaGP_INP_PLACE:
					{
						if( BcMain::pInstance()->mousePressed( 1 ) )
						{
							BcVec2d RenderFrom = ( MousePos - BcVec2d( 192.0f, 0.0f ) ) + WorldPos_;

							BcReal Angle = ( ( ( 2.0f * BcReal( M_PI ) ) ) / 32.0f ) * MagnetAngle_;
							BcVec2d FieldDir( sinf( Angle ), cosf( Angle ) );

							pMagnet_ = new ObjPlasma( RenderFrom, FieldDir, 10000.0f );
							ObjManager::pInstance()->addObject( pMagnet_ );
							PhManager::pInstance()->addField( pMagnet_ );

						}
						if( BcMain::pInstance()->mouseDown( 3 ) )
						{
							GamePlaceMode_ = gaGP_INP_NONE;
						}

						if( BcMain::pInstance()->mousePressed( 4 ) )
						{
							MagnetAngle_ += 1.0f;

							if( MagnetAngle_ >= 32.0f )
							{
								MagnetAngle_ -= 32.0f;
							}
						}

						if( BcMain::pInstance()->mousePressed( 5 ) )
						{
							MagnetAngle_ -= 1.0f;

							if( MagnetAngle_ < 0.0f )
							{
								MagnetAngle_ += 32.0f;
							}
						}
					}
					break;

				case gaGP_INP_REMOVE:
					{
						if( BcMain::pInstance()->mouseDown( 3 ) )
						{
							GamePlaceMode_ = gaGP_INP_NONE;
						}
					}
					break;
				}
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

						switch( ButtonClick )
						{
							// Place
						case 0:
							GamePlaceMode_ = gaGP_INP_PLACE;
							break;

							// Remove
						case 1:
							GamePlaceMode_ = gaGP_INP_REMOVE;
							break;

							// Go
						case 2:
							bRunGame_ = BcTrue;
							break;

						default:
							break;
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

