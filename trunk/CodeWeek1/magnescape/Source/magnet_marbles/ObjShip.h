/**************************************************************************
*
* File:		ObjShip.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __ObjShip_H__
#define __ObjShip_H__

#include "PhObject.h"
#include "ObjBase.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// ObjShip
class ObjShip:
	public ObjBase,
	public PhObject
{
public:
	ObjShip();
	virtual ~ObjShip();

	virtual void update();
	virtual void render( const BcVec2d& WorldPos );

	virtual void damage();

	RcSprite*		pSprite_;
	BcU32			iSprite_;

	BcReal			NextBullet_;
	BcReal			NextMagnet_;

	BcReal			Health_;

	BcBool			bWin_;

	BcS32			Score_;
};


#endif