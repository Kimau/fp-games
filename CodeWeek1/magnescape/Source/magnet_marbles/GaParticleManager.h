/**************************************************************************
*
* File:		GaParticleManager.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaParticleManager_H__
#define __GaParticleManager_H__

#include "BcSingleton.h"

#include "PhObject.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// GaParticleManager
class GaParticleManager:
	public BcSingleton< GaParticleManager >
{
public:
	enum
	{
		MAX_PARTICLES = 1024
	};
public:
	GaParticleManager();
	virtual ~GaParticleManager();

	void update();
	void render( const BcVec2d& WorldPos );

	void spawn( const BcVec2d& Position, const BcVec2d& Direction, BcReal Spread, BcReal LifeSpan, BcU32 Count );

	void reset();

private:
	struct GaParticle
	{
		BcBool bActive_;
		BcReal LifeSpan_;
		BcU32 iParticle_;
		BcVec2d Pos_;
		BcVec2d Vel_;
	};

	GaParticle Particles_[ MAX_PARTICLES ];

	RcSprite* pParticle_;

};

//////////////////////////////////////////////////
// Inlines
inline void GaParticleManager::reset()
{
	memset( &Particles_, 0, sizeof( Particles_ ) );
}

#endif