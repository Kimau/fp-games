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

#include "ObjMagnet.h"
#include "ObjPlasma.h"

//////////////////////////////////////////////////
// GaGameState
class GaGameState:
	public BcBaseState
{
public:
	GaGameState();
	virtual ~GaGameState();

private:
	virtual void execute();
	virtual void render();

	void handleGameStateInput();

private:
	RcSprite*		pSprite_;
	
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

	enum eGamePlaceMode
	{
		gaGP_INP_NONE = 0,
		gaGP_INP_PLACE,
		gaGP_INP_REMOVE
	};

	eGameStateInput GameStateInput_;
	eGamePlaceMode GamePlaceMode_;

	// gaGS_INP_MOVE
	BcVec2d OldWorldPos_;
	BcVec2d MouseDownPos_;

	// gaGS_INP_PLACE
	BcReal MagnetAngle_;

	BcBool bRunGame_;

};

#endif
