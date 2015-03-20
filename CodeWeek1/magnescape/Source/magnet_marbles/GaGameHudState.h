/**************************************************************************
*
* File:		GaGameHudState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GAGAMEHUDSTATE_H__
#define __GAGAMEHUDSTATE_H__

#include "BcBaseState.h"

#include "ObjMarble.h"
#include "ObjMagnet.h"

//////////////////////////////////////////////////
// Enums
enum
{
	gaHUD_BACKGROUND = 0,
	gaHUD_BTNUP,
	gaHUD_BTNDN,
	gaHUD_BTNOV,
	gaHUD_MAX
};

//////////////////////////////////////////////////
// Enum
enum
{
	gaHUD_PARAM_SCORE = 0,

};
//////////////////////////////////////////////////
// GaGameHudState
class GaGameHudState:
	public BcBaseState
{
public:
	enum
	{
		MAX_BUTTONS = 16
	};

public:
	GaGameHudState();
	virtual ~GaGameHudState();

	BcU32 addButton( const BcChar* pText );

	BcU32 mouseOver( const BcVec2d& MousePos );
	BcU32 mouseClick( const BcVec2d& MousePos );

	void health( BcReal Health );
	void score( BcReal Score );

private:
	virtual void execute();
	virtual void render();

private:
	struct GaButton
	{
		BcReal DownTimer_;

		RcSprite* pCurrSprite_;
		RcSprite* pText_;

		BcVec2d Position_;
		BcVec2d Dimensions_;
	};

	RcSprite* aSprites_[ gaHUD_MAX ];

	BcU32 nButtons_;
	GaButton Buttons_[ MAX_BUTTONS ];

	//
	BcBool bUpdateHUD_;
	BcU32 Score_;
	BcU32 Health_;	

	RcSprite* pScore_;
	RcSprite* pHealth_;
};

//////////////////////////////////////////////////
// Inlines

inline void GaGameHudState::health( BcReal Health )
{
	if( Health_ != static_cast< BcU32 >( Health ) )
	{
		bUpdateHUD_;
	}
	Health_ = static_cast< BcU32 >( Health );
}

inline void GaGameHudState::score( BcReal Score )
{
	if( Score_ != Score )
	{
		bUpdateHUD_ = BcTrue;
	}

	Score_ = Score;
}

#endif
