/**************************************************************************
*
* File:		GaGunManager.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GAGUNMANAGER_H__
#define __GAGUNMANAGER_H__

#include "BcSingleton.h"

#include "PhObject.h"
#include "PhField.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// Forward Declarations
class GaGunManager;

//////////////////////////////////////////////////
// GaGun
class GaGun:
	public PhObject
{
protected:
	friend class GaGunManager;
	BcBool bActive_;
	BcReal LifeSpan_;
};

//////////////////////////////////////////////////
// GaGunMagnet
class GaGunMagnet:
	public GaGun,
	public PhField
{
protected:
	friend class GaGunManager;

	BcU32 iRot_;

};

//////////////////////////////////////////////////
// GaGunManager
class GaGunManager:
	public BcSingleton< GaGunManager >
{
public:
	enum
	{
		MAX_GUNS = 256,
		MAX_MAGNETS = 64
	};
public:
	GaGunManager();
	virtual ~GaGunManager();

	void update();
	void render( const BcVec2d& WorldPos );

	void shoot( const BcVec2d& Position, const BcVec2d& Velocity, BcReal LifeSpan );
	void shootMagnet( const BcVec2d& Position, const BcVec2d& Velocity, BcReal LifeSpan );

	void reset();

private:
	GaGun Gun_[ MAX_GUNS ];
	GaGunMagnet GunMagnet_[ MAX_MAGNETS ];

	RcSprite* pBullet_;
	RcSprite* pMagnet_;

};

//////////////////////////////////////////////////
// Inlines
inline void GaGunManager::reset()
{
	memset( &Gun_, 0, sizeof( Gun_ ) );
	memset( &GunMagnet_, 0, sizeof( GunMagnet_ ) );
}

#endif