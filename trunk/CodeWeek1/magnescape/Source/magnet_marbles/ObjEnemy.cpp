/**************************************************************************
*
* File:		ObjEnemy.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "ObjEnemy.h"

#include "ObjManager.h"

#include "BcMain.h"

#include "RcSpriteManager.h"

#include "PhManager.h"

#include "GaGunManager.h"
#include "GaParticleManager.h"

#include "SoManager.h"

ObjShip* ObjEnemy::pPlayer_ = NULL;

//////////////////////////////////////////////////
// Ctor
ObjEnemy::ObjEnemy( const BcVec2d& Pos )
{
	Pos_ = Pos;

	//
	pSprite_ = RcSpriteManager::pInstance()->pSprite( "data/enemy.tga", 8 );
	iSprite_ = 0;

	NextBullet_ = 0.0f;
	NextMagnet_ = 0.0f;

	Health_ = 50.0f;
}

//////////////////////////////////////////////////
// Dtor
ObjEnemy::~ObjEnemy()
{
	PhManager::pInstance()->delObject( this );
}

//////////////////////////////////////////////////
// update
void ObjEnemy::update()
{
	BcVec2d Normal;

	if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_, 16.0f, &Normal ) )
	{
		Pos_ = OldPos_;
		Vel_ = Vel_ - Normal * ( 2.0f * ( Vel_.dot( Normal ) ) );

		Vel_ -= ( Vel_ * 0.75f );

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

	// AI

	// Head for player
	if( ( pPlayer_->Pos_ - Pos_ ).magnitude() < 512.0f )
	{
		if( pPlayer_->Pos_.X_ < Pos_.X_ )
		{
			Acc_.X_ = -ShipSpeed;
		}

		if( pPlayer_->Pos_.X_ > Pos_.X_ )
		{
			Acc_.X_ = ShipSpeed;
		}

		if( pPlayer_->Pos_.Y_ < ( Pos_.Y_ + 128.0f ) )
		{
			Acc_.Y_ = -ShipSpeed;
		}

		if( pPlayer_->Pos_.Y_ > ( Pos_.Y_ + 128.0f ) )
		{
			Acc_.Y_ = ShipSpeed;
		}


		if( fabs( pPlayer_->Pos_.X_ - Pos_.X_ ) < 32.0f && fabs( pPlayer_->Pos_.Y_ - Pos_.Y_ ) < 192.0f )
		{
			doShoot();
		}
	}

	// Avoid collisions.
	//
	BcBool bTestWorldCol = BcTrue;

	{
		ObjBase* pCollision = ObjManager::pInstance()->testCollision( this, Pos_, 64.0f, &Normal );

		if( pCollision != NULL )
		{
			//
			PhObject* pCollisionObject = dynamic_cast< PhObject* >( pCollision );

			// If its a physics object, avoid.
			if( pCollisionObject != NULL )
			{
				ObjShip* pShip = dynamic_cast< ObjShip* >( pCollision );

				// If its the players ship, we don't really care. Suicide FTW!
				if( pShip != NULL )
				{
					Acc_ = Normal * ShipSpeed * 0.5f;
				}
				else
				{
					Acc_ = Normal * ShipSpeed * 2.0f;
				}
			}
		}
		else
		{

		}
	}

	// If we hit someone else we knock a lot of health off
	{
		ObjBase* pCollision = ObjManager::pInstance()->testCollision( this, Pos_, 16.0f, &Normal );

		if( pCollision != NULL )
		{
			//
			PhObject* pCollisionObject = dynamic_cast< PhObject* >( pCollision );

			// If its a physics object, avoid.
			if( pCollisionObject != NULL )
			{
				Pos_ = OldPos_;
				Vel_ = Vel_ - Normal * ( 2.0f * ( Vel_.dot( Normal ) ) );

				Vel_ -= ( Vel_ * 0.75f );

				GaParticleManager::pInstance()->spawn( Pos_ - Normal * 14.0f, Normal * 200.0f, 200.0f, 0.25f, 32 );
				Health_ -= 40.0f;
			}
		}
		else
		{

		}
	}

	if( bTestWorldCol )
	{
		if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_ - BcVec2d( 48.0f, 0.0f ), 32.0f, &Normal ) )
		{
			Acc_.X_ = ShipSpeed * 2.0f;
		}

		if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_ + BcVec2d( 48.0f, 0.0f ), 32.0f, &Normal ) )
		{
			Acc_.X_ = -ShipSpeed * 2.0f;
		}

		if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_ - BcVec2d( 0.0f, 48.0f ), 32.0f, &Normal ) )
		{
			Acc_.Y_ = ShipSpeed * 2.0f;
		}

		if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_ + BcVec2d( 0.0f, 48.0f ), 32.0f, &Normal ) )
		{
			Acc_.Y_ = -ShipSpeed * 2.0f;
		}
	}

	// Do sprite
	iSprite_ = 0;

	if( Acc_.Y_ > 0.0f )
	{
		iSprite_ = 1;
	}
	else if( Acc_.Y_ < 0.0f )
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

	// Slow down if we are near others
	if( ObjManager::pInstance()->testCollision( this, Pos_, 96.0f, &Normal ) )
	{
		Acc_ = Acc_ * 0.5f;
	}

	// Friction
	Vel_ -= ( Vel_ * 0.01f );

	if( Health_ < 0.0f )
	{
		GaParticleManager::pInstance()->spawn( Pos_, BcVec2d( 0.0f, -300.0f ), 200.0f, 0.5f, 512 );
		pPlayer_->Score_ += 50;
		bAlive_ = BcFalse;

		SoManager::pInstance()->play( 4 );
		SoManager::pInstance()->play( 5 );

	}
}

//////////////////////////////////////////////////
// render
void ObjEnemy::render( const BcVec2d& WorldPos )
{
	pSprite_->drawCentered( iSprite_, Pos_ - WorldPos );
}

//////////////////////////////////////////////////
// doShoot
void ObjEnemy::doShoot()
{
	if( NextBullet_ < 0.0f )
	{
		GaGunManager::pInstance()->shoot( Pos_ + BcVec2d( 0.0f, 24.0f ), BcVec2d( 0.0f, 512.0f ), 2.8f );

		NextBullet_ = 0.1f;
	}
}

//////////////////////////////////////////////////
// damage
void ObjEnemy::damage()
{
	Health_ -= 0.25f;

	pPlayer_->Score_ += 2;

	SoManager::pInstance()->play( 6 );
}