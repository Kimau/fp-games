/**************************************************************************
*
* File:		ObjEnemy.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __ObjEnemy_H__
#define __ObjEnemy_H__

#include "PhObject.h"
#include "ObjBase.h"

#include "RcSprite.h"

#include "ObjShip.h"

//////////////////////////////////////////////////
// ObjEnemy
class ObjEnemy:
	public ObjBase,
	public PhObject
{
public:
	ObjEnemy( const BcVec2d& Pos );
	virtual ~ObjEnemy();

	virtual void update();
	virtual void render( const BcVec2d& WorldPos );

	virtual void damage();

	void doShoot();

	RcSprite*		pSprite_;
	BcU32			iSprite_;

	BcReal			NextBullet_;
	BcReal			NextMagnet_;

	BcReal			Health_;

	static ObjShip*	pPlayer_;
};



#endif