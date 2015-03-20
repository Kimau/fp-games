/**************************************************************************
*
* File:		ObjPlasma.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __OBJPLASMA_H__
#define __OBJPLASMA_H__

#include "PhObject.h"
#include "ObjBase.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// ObjPlasma
class ObjPlasma:
	public ObjBase,
	public PhObject
{
public:
	ObjPlasma( const BcVec2d& Pos );
	virtual ~ObjPlasma();

	virtual void update();
	virtual void render( const BcVec2d& WorldPos );

	RcSprite*		pSprite_[4];

	BcReal			Timer_;
};


#endif