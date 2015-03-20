/**************************************************************************
*
* File:		GaGameManagementState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GAGAMEMANAGEMENTSTATE_H__
#define __GAGAMEMANAGEMENTSTATE_H__

#include "BcBaseState.h"

#include "RcSprite.h"

//////////////////////////////////////////////////
// GaGameManagementState
class GaGameManagementState:
	public BcBaseState
{
public:
	GaGameManagementState();
	virtual ~GaGameManagementState();

	virtual void execute();
	virtual void render();

	void winLevel( BcU32 Score );
	void loseLevel();

	void spawnLevel( BcU32 iLevel );

	void printNumber( const BcVec2d& Pos, BcU32 Number );
	void printText( const BcVec2d& Pos, const BcChar* Text );

	enum
	{
		gaGM_PREGAME = 0,
		gaGM_INGAME,
		gaGM_WINLEVEL,
		gaGM_LOSELEVEL,
		gaGM_THANKS,
		gaGM_MAX
	};

	BcU32 State_;

	BcU32 Score_;
	BcU32 LevelScore_;

	BcU32 iCurrentLevel_;

	RcSprite* pGameTexts_[ gaGM_MAX ];

	RcSprite* pLevelTexts_[ 10 ];

	RcSprite* pNumbers_[ 10 ];

	RcSprite* pScore_;
	RcSprite* pLevelScore_;

};

#endif
