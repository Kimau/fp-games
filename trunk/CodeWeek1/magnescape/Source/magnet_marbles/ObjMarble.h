/**************************************************************************
*
* File:		ObjMarble.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __OBJMARBLE_H__
#define __OBJMARBLE_H__

#include "PhObject.h"
#include "ObjBase.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// ObjMarble
class ObjMarble:
	public ObjBase,
	public PhObject
{
public:
	ObjMarble();
	virtual ~ObjMarble();

	virtual void update();
	virtual void render( const BcVec2d& WorldPos );

	enum
	{
		objPOLE_RED = 0,
		objPOLE_BLUE,
	};

	BcU32			Pole_;
	RcSprite*		pSprite_;
	BcU32			iSprite_;
	BcReal			SpriteTick_;
};


#endif