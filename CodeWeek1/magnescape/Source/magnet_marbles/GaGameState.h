/**************************************************************************
*
* File:		GaGameState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GAGAMESTATE_H__
#define __GAGAMESTATE_H__

#include "BcBaseState.h"

#include "GaMap.h"

#include "ObjShip.h"
#include "ObjEnemy.h"
#include "ObjPlasma.h"

//////////////////////////////////////////////////
// GaGameState
class GaGameState:
	public BcBaseState
{
public:
	GaGameState( const BcChar* pFileName );
	virtual ~GaGameState();

	virtual void execute();
	virtual void render();

	//
	GaMap*			pMap_;

	// Objects
	ObjShip*		pShip_;
	ObjEnemy*		pEnemies_[ 128 ];

	//
	BcVec2d			WorldPos_;
};

#endif
