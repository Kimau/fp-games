/**************************************************************************
*
* File:		GaMainMenuState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaMainMenuState_H__
#define __GaMainMenuState_H__

#include "BcBaseState.h"

#include "GaMap.h"

#include "ObjMarble.h"
#include "ObjMagnet.h"

//////////////////////////////////////////////////
// GaMainMenuState
class GaMainMenuState:
	public BcBaseState
{
public:
	GaMainMenuState();
	virtual ~GaMainMenuState();

private:
	virtual void execute();
	virtual void render();

	void handleGameStateInput();

private:
	//
	GaMap*			pMap_;

	//
	BcVec2d			WorldPos_;

	// Input
	enum eGameStateInput
	{
		gaGS_INP_IDLE = 0,
		gaGS_INP_MOVE,
	};

	eGameStateInput GameStateInput_;

	// gaGS_INP_MOVE
	BcVec2d OldWorldPos_;
	BcVec2d MouseDownPos_;

	//
	BcU32 SelectedTile_;

	//
	RcSprite* pTitle_;
	RcSprite* pName_;


};

#endif
