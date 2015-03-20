/**************************************************************************
*
* File:		GaEditState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaEditState_H__
#define __GaEditState_H__

#include "BcBaseState.h"

#include "GaMap.h"

#include "ObjMarble.h"
#include "ObjMagnet.h"

//////////////////////////////////////////////////
// GaEditState
class GaEditState:
	public BcBaseState
{
public:
	GaEditState();
	virtual ~GaEditState();

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

};

#endif
