/**************************************************************************
*
* File:		GaParticleManager.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaParticleManager.h"

#include "BcMain.h"
#include "PhManager.h"

#include "ObjManager.h"
#include "GaMap.h"

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
GaParticleManager::GaParticleManager()
{
	for( BcU32 i = 0; i < MAX_PARTICLES; ++i )
	{
		Particles_[ i ].bActive_ = BcFalse;
	}

	pParticle_ = RcSpriteManager::pInstance()->pSprite( "data/particles.tga", 4 );
}

//////////////////////////////////////////////////
// Dtor
GaParticleManager::~GaParticleManager()
{

}

//////////////////////////////////////////////////
// update
void GaParticleManager::update()
{
	for( BcU32 i = 0; i < MAX_PARTICLES; ++i )
	{
		if( Particles_[ i ].bActive_ == BcTrue )
		{
			Particles_[ i ].LifeSpan_ -= BcMain::pInstance()->tick();
			Particles_[ i ].Vel_ += BcVec2d( 0.0f, 2000.0f ) * BcMain::pInstance()->tick();
			Particles_[ i ].Pos_ += Particles_[ i ].Vel_ * BcMain::pInstance()->tick();

			if( Particles_[ i ].LifeSpan_ <= 0.0f )
			{
				Particles_[ i ].bActive_ = BcFalse;
			}

			if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Particles_[ i ].Pos_, 4.0f, NULL ) )
			{
				Particles_[ i ].bActive_ = BcFalse;
			}

		}
	}
}

//////////////////////////////////////////////////
// render
void GaParticleManager::render( const BcVec2d& WorldPos )
{
	for( BcU32 i = 0; i < MAX_PARTICLES; ++i )
	{
		if( Particles_[ i ].bActive_ == BcTrue )
		{
			pParticle_->drawCentered( Particles_[ i ].iParticle_, Particles_[ i ].Pos_ - ( WorldPos - BcVec2d( 192.0f, 0.0f ) ) );
		}
	}
}

//////////////////////////////////////////////////
// spawn
void GaParticleManager::spawn( const BcVec2d& Position, const BcVec2d& Direction, BcReal Spread, BcReal LifeSpan, BcU32 Count )
{
	BcAssert( Count > 0 );

	for( BcU32 i = 0; i < MAX_PARTICLES; ++i )
	{
		if( Particles_[ i ].bActive_ == BcFalse )
		{
			Particles_[ i ].Pos_ = Position;
			Particles_[ i ].Vel_ = Direction + BcVec2d( BcRandReal(), BcRandReal() ) * Spread;
			Particles_[ i ].bActive_ = BcTrue;
			Particles_[ i ].LifeSpan_ = ( LifeSpan * 0.5f ) + ( BcRandReal() * LifeSpan * 0.5f );
			Particles_[ i ].iParticle_ = BcU32( rand() ) % 4;

			Count--;
		}

		if( Count == 0 )
		{
			break;
		}
	}
}
