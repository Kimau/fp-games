/**************************************************************************
*
* File:		GaGameManagementState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaGameManagementState.h"

#include "BcMain.h"

#include "GaGameState.h"
#include "GaGameHudState.h"

#include "RcSpriteManager.h"

#include "GaMusic.h"
 

//////////////////////////////////////////////////
// Ctor
GaGameManagementState::GaGameManagementState():
	State_( gaGM_PREGAME ),
	Score_( 0 ),
	LevelScore_( 0 ),
	iCurrentLevel_( 0 )
{
	spawnState( new GaGameHudState(), BcFalse );

	pGameTexts_[ 0 ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Press Space to Begin", 24 );
	pGameTexts_[ 1 ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Press Space to Begin", 24 );
	pGameTexts_[ 2 ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Press Space to Advance", 24 );
	pGameTexts_[ 3 ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Press Space to Retry", 24 );
	pGameTexts_[ 4 ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Game Over...", 24 );

	for( BcU32 i = 0; i < 10; ++i )
	{
		BcChar LevelFilename[128];

		sprintf( LevelFilename, "Mission %i:", i + 1 );
		pLevelTexts_[ i ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", LevelFilename, 32 );
	}

	for( BcU32 i = 0; i < 10; ++i )
	{
		BcChar LevelFilename[128];

		sprintf( LevelFilename, "%i", i );
		pNumbers_[ i ] = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", LevelFilename, 24 );
	}

	pScore_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Score: ", 24 );
	pLevelScore_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", "Level Score: ", 24 );

	GaMusic::pInstance()->play( 50, 52 );
}

//////////////////////////////////////////////////
// Dtor
GaGameManagementState::~GaGameManagementState()
{

}

//////////////////////////////////////////////////
// execute
void GaGameManagementState::execute()
{
	switch( State_ )
	{
	case gaGM_PREGAME:
		if( BcMain::pInstance()->keyPressed( SDLK_SPACE ) )
		{	
			spawnLevel( iCurrentLevel_ );
		}
		break;

	case gaGM_INGAME:
		break;

	case gaGM_WINLEVEL:
		if( BcMain::pInstance()->keyPressed( SDLK_SPACE ) )
		{
			++iCurrentLevel_;
			State_ = gaGM_PREGAME;

			if( iCurrentLevel_ == 6 )
			{
				State_ = gaGM_THANKS;
			}

			GaMusic::pInstance()->play( 50, 52 );
		}
		break;

	case gaGM_LOSELEVEL:
		if( BcMain::pInstance()->keyPressed( SDLK_SPACE ) )
		{
			spawnLevel( iCurrentLevel_ );
		}
		break;
	}
}

//////////////////////////////////////////////////
// render
void GaGameManagementState::render()
{
	BcVec2d Pos = BcVec2d( 192.0f + 224.0f, 128.0f );

	switch( State_ )
	{
	case gaGM_PREGAME:
		pLevelTexts_[ iCurrentLevel_ ]->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 128.0f ) );
		pGameTexts_[ State_ ]->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 320.0f ) );
		break;

	case gaGM_INGAME:
		{

		}
		break;

	case gaGM_WINLEVEL:
		pLevelTexts_[ iCurrentLevel_ ]->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 128.0f ) );
		pGameTexts_[ State_ ]->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 320.0f ) );

		pScore_->draw( 0, BcVec2d( 256.0f, 200.0f ) );
		printNumber( BcVec2d( pScore_->pSurface_->w + 256.0f, 200.0f ), Score_ );

		pLevelScore_->draw( 0, BcVec2d( 256.0f, 232.0f ) );
		printNumber( BcVec2d( pLevelScore_->pSurface_->w + 256.0f, 232.0f ), LevelScore_ );
		break;

	case gaGM_LOSELEVEL:
		pLevelTexts_[ iCurrentLevel_ ]->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 128.0f ) );
		pGameTexts_[ State_ ]->drawCentered( 0, BcVec2d( 192.0f + 224.0f, 320.0f ) );
		break;

	case gaGM_THANKS:
		printText( Pos, "Thanks for playing!" );								Pos += BcVec2d( 0.0f, 64.0f );
		printText( Pos, "This was the Weekend of Code entry by Neilo." );		Pos += BcVec2d( 0.0f, 48.0f );
		printText( Pos, "Special thanks to my house mates" );					Pos += BcVec2d( 0.0f, 32.0f );
		printText( Pos, "for letting me sleep in 2 days" );						Pos += BcVec2d( 0.0f, 32.0f );
		printText( Pos, "running, and to Claire for hosting" );					Pos += BcVec2d( 0.0f, 32.0f );
		printText( Pos, "this fine competition!" );								Pos += BcVec2d( 0.0f, 32.0f );
		break;
	}

	GaGameState* pGameState = dynamic_cast< GaGameState* >( pChild() );

	if( pGameState != NULL )
	{
		GaGameHudState* pGameHudState = dynamic_cast< GaGameHudState* >( pGameState->pChild() );

		if( pGameHudState != NULL )
		{
			if( pGameState->pShip_ != NULL )
			{
				LevelScore_ = pGameState->pShip_->Score_;
				pGameHudState->health( pGameState->pShip_->Health_ );
			}

			pGameHudState->score( Score_ + LevelScore_ );
		}
	}
	else
	{
		GaGameHudState* pGameHudState = dynamic_cast< GaGameHudState* >( pChild() );

		if( pGameHudState != NULL )
		{
			pGameHudState->score( Score_ );
		}
	}
}

//////////////////////////////////////////////////
//
void GaGameManagementState::winLevel( BcU32 Score )
{
	spawnState( new GaGameHudState(), BcFalse );

	LevelScore_ = Score;
	Score_ += LevelScore_;
	State_ = gaGM_WINLEVEL;

	GaMusic::pInstance()->play( 50, 52 );

}

//////////////////////////////////////////////////
//
void GaGameManagementState::loseLevel()
{
	spawnState( new GaGameHudState(), BcFalse );

	State_ = gaGM_LOSELEVEL;

	GaMusic::pInstance()->play( 50, 52 );
}

//////////////////////////////////////////////////
// spawnLevel
void GaGameManagementState::spawnLevel( BcU32 iLevel )
{
	BcChar LevelFilename[128];

	sprintf( LevelFilename, "data/level%i.map", iLevel );

	State_ = gaGM_INGAME;

	GaMusic::pInstance()->play( 0, 48 );

	spawnState( new GaGameState( LevelFilename ), BcFalse );
}

//////////////////////////////////////////////////
// printNumber
void GaGameManagementState::printNumber( const BcVec2d& Pos, BcU32 Number )
{
	BcU32 Divisor = 1000000;
	BcVec2d CharPos = Pos;

	for( BcU32 i = 0; i < 7; ++i )
	{
		BcU32 iNum = ( Number / Divisor ) % 10;
		Number -= ( iNum * Divisor );
		Divisor = Divisor / 10;

		RcSprite* pSprite = pNumbers_[ iNum ];

		pSprite->draw( 0, CharPos );

		CharPos.X_ += static_cast< BcReal >( pSprite->pSurface_->w );
	}
}

//////////////////////////////////////////////////
//
void GaGameManagementState::printText( const BcVec2d& Pos, const BcChar* Text )
{
	RcSprite* pSprite = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", Text, 18 );
	pSprite->drawCentered( 0, Pos );
}
