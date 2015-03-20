/**************************************************************************
*
* File:		GaGunManager.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaGunManager.h"

#include "BcMain.h"
#include "PhManager.h"

#include "ObjManager.h"
#include "GaMap.h"

#include "RcSpriteManager.h"

#include "GaParticleManager.h"

#include "SoManager.h"

//////////////////////////////////////////////////
// Globals
static BcVec2d gDirections[] =
{
	BcVec2d(  0.0f, -1.0f ),
	BcVec2d(  1.0f, -1.0f ),
	BcVec2d(  1.0f,  0.0f ),
	BcVec2d(  1.0f,  1.0f ),
	BcVec2d(  0.0f,  1.0f ),
	BcVec2d( -1.0f,  1.0f ),
	BcVec2d( -1.0f,  0.0f ),
	BcVec2d( -1.0f, -1.0f ),
};

//////////////////////////////////////////////////
// Ctor
GaGunManager::GaGunManager()
{
	for( BcU32 i = 0; i < MAX_GUNS; ++i )
	{
		Gun_[ i ].bActive_ = BcFalse;
	}

	for( BcU32 i = 0; i < MAX_MAGNETS; ++i )
	{
		GunMagnet_[ i ].bActive_ = BcFalse;
	}

	for( BcU32 i = 0; i < 8; ++i )
	{
		gDirections[ i ].normalise();
	}

	pBullet_ = RcSpriteManager::pInstance()->pSprite( "data/bullet.tga", 1 );
	pMagnet_ = RcSpriteManager::pInstance()->pSprite( "data/magnet.tga", 1 );
}

//////////////////////////////////////////////////
// Dtor
GaGunManager::~GaGunManager()
{
	for( BcU32 i = 0; i < MAX_GUNS; ++i )
	{
		if( Gun_[ i ].bActive_ == BcTrue )
		{
			PhManager::pInstance()->delObject( &Gun_[ i ] );
		}
	}
}

//////////////////////////////////////////////////
// update
void GaGunManager::update()
{
	BcVec2d Normal;

	// GaGun
	for( BcU32 i = 0; i < MAX_GUNS; ++i )
	{
		if( Gun_[ i ].bActive_ == BcTrue )
		{
			Gun_[ i ].LifeSpan_ -= BcMain::pInstance()->tick();

			BcVec2d NewAcc = PhManager::pInstance()->fieldDir( Gun_[ i ].Pos_ );

			Gun_[ i ].Acc_ = NewAcc;

			if( Gun_[ i ].LifeSpan_ <= 0.0f )
			{
				Gun_[ i ].bActive_ = BcFalse;
				PhManager::pInstance()->delObject( &Gun_[ i ] );
			}

			ObjBase* pCollision = ObjManager::pInstance()->testCollision( NULL, Gun_[ i ].Pos_, 4.0f, &Normal );

			if( pCollision )
			{
				pCollision->damage();
				GaParticleManager::pInstance()->spawn( Gun_[ i ].Pos_, Normal * 100.0f, 100.0f, 0.1f, 4 );
			}


			if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Gun_[ i ].Pos_, 4.0f, &Normal ) )
			{
				Gun_[ i ].bActive_ = BcFalse;
				PhManager::pInstance()->delObject( &Gun_[ i ] );

				GaParticleManager::pInstance()->spawn( Gun_[ i ].Pos_, Normal * 100.0f, 100.0f, 0.1f, 4 );
			}
		}
	}

	// GaGunMagnet
	for( BcU32 i = 0; i < MAX_MAGNETS; ++i )
	{
		if( GunMagnet_[ i ].bActive_ == BcTrue )
		{
			GunMagnet_[ i ].LifeSpan_ -= BcMain::pInstance()->tick();
			GunMagnet_[ i ].iRot_ = ( GunMagnet_[ i ].iRot_ + 1 ) % 32;
			if( GunMagnet_[ i ].LifeSpan_ <= 0.0f )
			{
				GunMagnet_[ i ].bActive_ = BcFalse;
				PhManager::pInstance()->delObject( &GunMagnet_[ i ] );
				PhManager::pInstance()->delField( &GunMagnet_[ i ] );

				SoManager::pInstance()->play( 2 );

				GaParticleManager::pInstance()->spawn( GunMagnet_[ i ].PhObject::Pos_, GunMagnet_[ i ].PhObject::Vel_ * 3.0f, 100.0f, 0.4f, 64 );
			}

			GunMagnet_[ i ].PhField::Pos_ = GunMagnet_[ i ].PhObject::Pos_;

			if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, GunMagnet_[ i ].PhObject::Pos_, 4.0f, &Normal ) )
			{
				GunMagnet_[ i ].bActive_ = BcFalse;
				PhManager::pInstance()->delObject( &GunMagnet_[ i ] );
				PhManager::pInstance()->delField( &GunMagnet_[ i ] );

				SoManager::pInstance()->play( 3 );

				GaParticleManager::pInstance()->spawn( GunMagnet_[ i ].PhObject::Pos_, Normal * 100.0f, 100.0f, 0.4f, 32 );
			}
		}
	}
}

//////////////////////////////////////////////////
// render
void GaGunManager::render( const BcVec2d& WorldPos )
{
	for( BcU32 i = 0; i < MAX_GUNS; ++i )
	{
		if( Gun_[ i ].bActive_ == BcTrue )
		{
			pBullet_->drawCentered( 0, Gun_[ i ].Pos_ - ( WorldPos - BcVec2d( 192.0f, 0.0f ) ) );
		}
	}

	for( BcU32 i = 0; i < MAX_MAGNETS; ++i )
	{
		if( GunMagnet_[ i ].bActive_ == BcTrue )
		{
			pMagnet_->drawCentered( 0, GunMagnet_[ i ].PhObject::Pos_ - ( WorldPos - BcVec2d( 192.0f, 0.0f ) ), GunMagnet_[ i ].iRot_ );
		}
	}
}

//////////////////////////////////////////////////
// shoot
void GaGunManager::shoot( const BcVec2d& Position, const BcVec2d& Velocity, BcReal LifeSpan )
{
	for( BcU32 i = 0; i <  MAX_GUNS; ++i )
	{
		if( Gun_[ i ].bActive_ == BcFalse )
		{
			Gun_[ i ].Pos_ = Position;
			Gun_[ i ].Vel_ = Velocity;
			Gun_[ i ].Acc_ = BcVec2d( 0.0f, 0.0f );
			Gun_[ i ].bActive_ = BcTrue;
			Gun_[ i ].LifeSpan_ = LifeSpan;

			PhManager::pInstance()->addObject( &Gun_[ i ] );

			SoManager::pInstance()->play( 0 );
			break;
		}
	}
}

//////////////////////////////////////////////////
// shootMagnet
void GaGunManager::shootMagnet( const BcVec2d& Position, const BcVec2d& Velocity, BcReal LifeSpan )
{
	for( BcU32 i = 0; i <  MAX_MAGNETS; ++i )
	{
		if( GunMagnet_[ i ].bActive_ == BcFalse )
		{
			GunMagnet_[ i ].PhObject::Pos_ = Position;
			GunMagnet_[ i ].PhField::Pos_ = Position;
			GunMagnet_[ i ].PhField::Ori_ = BcVec2d( 0.0f, 1.0f );
			GunMagnet_[ i ].PhField::Mag_ = 100000.0f;
			GunMagnet_[ i ].Vel_ = Velocity;
			GunMagnet_[ i ].Acc_ = BcVec2d( 0.0f, 0.0f );
			GunMagnet_[ i ].bActive_ = BcTrue;
			GunMagnet_[ i ].LifeSpan_ = LifeSpan;
			GunMagnet_[ i ].iRot_ = 0;

			PhManager::pInstance()->addObject( &GunMagnet_[ i ] );
			PhManager::pInstance()->addField( &GunMagnet_[ i ] );

			SoManager::pInstance()->play( 1 );

			break;
		}
	}
}
