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
#include "GaGameManagementState.h"
#include "GaMainMenuState.h"
#include "BcMain.h"

#include "ObjManager.h"
#include "PhManager.h"

#include "GaGunManager.h"
#include "GaParticleManager.h"

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
GaGameState::GaGameState( const BcChar* pFileName )
{
	//
	WorldPos_ = BcVec2d( 0.0f, 7712.0f );

	//
	pMap_ = new GaMap();
	pMap_->reset();
	pMap_->load( pFileName );

	ObjManager::pInstance()->pMap( pMap_ );

	//
	pShip_ = new ObjShip();
	ObjManager::pInstance()->addObject( pShip_ );
	PhManager::pInstance()->addObject( pShip_ );

	ObjManager::pInstance()->addObject( new ObjPlasma( pMap_->getPortal() ) );

	//
	for( BcU32 i = 0; i < pMap_->nEnemies(); ++i )
	{
		pEnemies_[ i ] = new ObjEnemy( pMap_->getEnemy( i ) );
		ObjManager::pInstance()->addObject( pEnemies_[ i ] );
		PhManager::pInstance()->addObject( pEnemies_[ i ] );
	}

	//
	GaGameHudState* pHudState = new GaGameHudState();

	//pHudState->addButton( "Place Magnet" );
	//pHudState->addButton( "Remove Magnet" );
	//pHudState->addButton( "GO!" );

	spawnState( pHudState, BcFalse );
}

//////////////////////////////////////////////////
// Dtor
GaGameState::~GaGameState()
{
	// Screw it - memory leak. This is what I get for
	// hacking all weekend!
	PhManager::pInstance()->reset();
	ObjManager::pInstance()->reset();
	GaGunManager::pInstance()->reset();
	GaParticleManager::pInstance()->reset();
}

//////////////////////////////////////////////////
// execute
void GaGameState::execute()
{
	//
	pMap_->update();

	PhManager::pInstance()->step( BcMain::pInstance()->tick() );
	ObjManager::pInstance()->update();
	GaGunManager::pInstance()->update();
	GaParticleManager::pInstance()->update();

	/*
	const BcReal TimeStep = 0.005f;
	static BcReal TimeLeft = 0.0f;

	TimeLeft += BcMain::pInstance()->tick();
	while( TimeLeft > TimeStep )
	{
	PhManager::pInstance()->step( TimeStep );
	ObjManager::pInstance()->update();	

	TimeLeft -= TimeStep;
	}
	*/

	if( pShip_ != NULL )
	{
		if( pShip_->bAlive_ == BcFalse )
		{
			pShip_ = NULL;
		}
	}

	if( pShip_ != NULL )
	{
		BcReal TargetY = pShip_->Pos_.Y_ - 240.0f;

		if( fabs( WorldPos_.Y_ - TargetY ) > 1.0f )
		{
			BcReal Vel = ( TargetY - WorldPos_.Y_ ) * BcMain::pInstance()->tick() * 4.0f;

			WorldPos_.Y_ += Vel;
		}

		if( WorldPos_.Y_ > 7712.0f )
		{
			WorldPos_.Y_ = 7712.0f;
		}

		if( WorldPos_.Y_ < 0.0f )
		{
			WorldPos_.Y_ = 0.0f;
		}
	}

	// Manage game if need be.
	GaGameManagementState* pManager = dynamic_cast< GaGameManagementState* >( pParent() );

	if( pManager != NULL )
	{
		if( pShip_ == NULL )
		{
			pManager->loseLevel();
		}
		else
		{
			if( pShip_->bWin_ == BcTrue )
			{
				pManager->winLevel( pShip_->Score_ );
			}
		}
	}
	else
	{
		if( BcMain::pInstance()->keyPressed( SDLK_ESCAPE ) )
		{
			pParent()->spawnState( new GaMainMenuState(), BcFalse );
		}
	}
}

//////////////////////////////////////////////////
// render
void GaGameState::render()
{
	pMap_->render( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );
	//pMap_->renderField( BcVec2d( 192.0f, 0.0f ), BcVec2d( 448.0f, 480.0f ), WorldPos_ );

	ObjManager::pInstance()->render( WorldPos_ );
	GaGunManager::pInstance()->render( WorldPos_ );
	GaParticleManager::pInstance()->render( WorldPos_ );
}
