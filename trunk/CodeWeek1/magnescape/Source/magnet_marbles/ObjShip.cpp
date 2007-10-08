/**************************************************************************
*
* File:		ObjShip.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "ObjShip.h"
#include "ObjEnemy.h"
#include "ObjPlasma.h"

#include "ObjManager.h"

#include "BcMain.h"

#include "RcSpriteManager.h"

#include "PhManager.h"

#include "GaGunManager.h"
#include "GaParticleManager.h"

#include "SoManager.h"

//////////////////////////////////////////////////
// Ctor
ObjShip::ObjShip()
{
	Pos_ = BcVec2d( 224.0f, 8064.0f );

	//
	pSprite_ = RcSpriteManager::pInstance()->pSprite( "data/ship.tga", 8 );
	iSprite_ = 0;

	NextBullet_ = 0.0f;
	NextMagnet_ = 0.2f;

	// DIRTY
	ObjEnemy::pPlayer_ = this;

	Health_ = 100.0f;

	bWin_ = BcFalse;

	Score_ = 0;
}

//////////////////////////////////////////////////
// Dtor
ObjShip::~ObjShip()
{
	PhManager::pInstance()->delObject( this );
}

//////////////////////////////////////////////////
// update
void ObjShip::update()
{
	if( bWin_ == BcFalse )
	{
		BcVec2d Normal;

		if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_, 16.0f, &Normal ) )
		{
			Pos_ = OldPos_;
			Vel_ = Vel_ - Normal * ( 2.0f * ( Vel_.dot( Normal ) ) );

			Vel_ -= ( Vel_ * 0.75f );

			SoManager::pInstance()->play( 7 );

			if( Vel_.magnitude() > 16.0f )
			{
				GaParticleManager::pInstance()->spawn( Pos_ - Normal * 14.0f, Normal * 200.0f, 200.0f, 0.25f, 32 );
				Health_ -= Vel_.magnitude();
			}
		}

		const BcReal ShipSpeed = 256.0f;

		Acc_ = BcVec2d( 0.0f, 0.0f );

		NextBullet_ -= BcMain::pInstance()->tick();
		NextMagnet_ -= BcMain::pInstance()->tick();

		if( BcMain::pInstance()->keyPressed( SDLK_a ) )
		{
			SoManager::pInstance()->play( 8 );
			SoManager::pInstance()->play( 9 );
		}
		
		if( BcMain::pInstance()->keyPressed( SDLK_d ) )
		{
			SoManager::pInstance()->play( 8 );
			SoManager::pInstance()->play( 9 );
		}

		if( BcMain::pInstance()->keyPressed( SDLK_w ) )
		{
			SoManager::pInstance()->play( 8 );
			SoManager::pInstance()->play( 9 );
		}

		if( BcMain::pInstance()->keyPressed( SDLK_s ) )
		{
			SoManager::pInstance()->play( 8 );
			SoManager::pInstance()->play( 9 );
		}

		if( BcMain::pInstance()->keyDown( SDLK_a ) )
		{
			Acc_ -= BcVec2d( ShipSpeed, 0.0f );
		}

		if( BcMain::pInstance()->keyDown( SDLK_d ) )
		{
			Acc_ += BcVec2d( ShipSpeed, 0.0f );
		}

		if( BcMain::pInstance()->keyDown( SDLK_w ) )
		{
			Acc_ -= BcVec2d( 0.0f, ShipSpeed );
		}

		if( BcMain::pInstance()->keyDown( SDLK_s ) )
		{
			Acc_ += BcVec2d( 0.0f, ShipSpeed );
		}

		if( BcMain::pInstance()->keyDown( SDLK_LCTRL ) )
		{
			if( NextBullet_ < 0.0f )
			{
				GaGunManager::pInstance()->shoot( Pos_ - BcVec2d( 0.0f, 24.0f ), BcVec2d( 0.0f, -512.0f ), 2.8f );

				NextBullet_ = 0.1f;
			}
		}

		if( BcMain::pInstance()->keyDown( SDLK_SPACE ) )
		{
			if( NextMagnet_ < 0.0f )
			{
				GaGunManager::pInstance()->shootMagnet( Pos_ - BcVec2d( 0.0f, 24.0f ), BcVec2d( 0.0f, -64.0f ), 4.0f );

				NextMagnet_ = 1.0f;
			}
		}

		// Do sprite
		iSprite_ = 0;

		if( Acc_.Y_ < 0.0f )
		{
			iSprite_ = 1;
		}
		else if( Acc_.Y_ > 0.0f )
		{
			iSprite_ = 2;
		}
		else if( Acc_.X_ > 0.0f )
		{
			iSprite_ = 3;
		}
		else if( Acc_.X_ < 0.0f )
		{
			iSprite_ = 4;
		}

		// If we hit someone else we knock a lot of health off
		{
			ObjBase* pCollision = ObjManager::pInstance()->testCollision( this, Pos_, 16.0f, &Normal );

			if( pCollision != NULL )
			{
				//
				PhObject* pCollisionObject = dynamic_cast< PhObject* >( pCollision );
				ObjPlasma* pPlasmaObject = dynamic_cast< ObjPlasma* >( pCollision );

				// If its a physics object, avoid.
				if( pCollisionObject != NULL )
				{
					ObjEnemy* pEnemy = dynamic_cast< ObjEnemy* >( pCollision );

					if( pEnemy != NULL )
					{
						Pos_ = OldPos_;
						Vel_ = Vel_ - Normal * ( 2.0f * ( Vel_.dot( Normal ) ) );

						Vel_ -= ( Vel_ * 0.75f );

						GaParticleManager::pInstance()->spawn( Pos_ - Normal * 14.0f, Normal * 200.0f, 200.0f, 0.25f, 32 );
						Health_ -= 20.0f;
						Score_ -= 10;

						SoManager::pInstance()->play( 7 );
					}
				}

				// If its a portal
				if( pPlasmaObject != NULL )
				{
					GaParticleManager::pInstance()->spawn( Pos_, BcVec2d( 0.0f, -300.0f ), 500.0f, 1.5f, 1024 );
					Acc_ = BcVec2d( 0.0f, 0.0f );
					Vel_ = BcVec2d( 0.0f, -250.0f );
					bWin_ = BcTrue;
					Score_ += 500;
				}
			}
			else
			{

			}
		}

		// Friction
		Vel_ -= ( Vel_ * 0.01f );

		//
		if( Health_ < 0.0f )
		{
			GaParticleManager::pInstance()->spawn( Pos_, BcVec2d( 0.0f, -300.0f ), 200.0f, 0.5f, 512 );
			bAlive_ = BcFalse;
			Score_ = 0;

			SoManager::pInstance()->play( 4 );
			SoManager::pInstance()->play( 5 );
		}
	}

	if( Score_ < 0 )
	{
		Score_ = 0;
	}

	if( Health_ < 5.0f )
	{
		Health_ += BcMain::pInstance()->tick() * 0.25f;
	}
}

//////////////////////////////////////////////////
// render
void ObjShip::render( const BcVec2d& WorldPos )
{
	if( bWin_ == BcFalse )
	{
		pSprite_->drawCentered( iSprite_, Pos_ - WorldPos );
	}
}

//////////////////////////////////////////////////
// damage
void ObjShip::damage()
{
	Health_ -= 0.25f;

	SoManager::pInstance()->play( 7 );
}