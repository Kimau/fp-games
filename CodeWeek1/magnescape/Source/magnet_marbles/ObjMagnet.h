/**************************************************************************
*
* File:		ObjMagnet.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __ObjMagnet_H__
#define __ObjMagnet_H__

#include "PhField.h"
#include "ObjBase.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// ObjMagnet
class ObjMagnet:
	public ObjBase,
	public PhField
{
public:
	ObjMagnet( const BcVec2d& Pos, const BcVec2d& Ori, BcReal Mag );
	virtual ~ObjMagnet();

	virtual void update();
	virtual void render( const BcVec2d& WorldPos );

	RcSprite*		pSprite_;
};


#endif