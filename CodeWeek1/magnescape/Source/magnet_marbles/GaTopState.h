/**************************************************************************
*
* File:		GaTopState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GATOPSTATE_H__
#define __GATOPSTATE_H__

#include "BcBaseState.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// GaTopState
class GaTopState:
	public BcBaseState
{
public:
	GaTopState();
	virtual ~GaTopState();

private:
	virtual void execute();
	virtual void render();

private:

};

#endif
