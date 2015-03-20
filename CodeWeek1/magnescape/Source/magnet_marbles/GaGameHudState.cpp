/**************************************************************************
*
* File:		GaGameHudState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaGameHudState.h"

#include "BcMain.h"

#include "ObjManager.h"
#include "PhManager.h"

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
GaGameHudState::GaGameHudState()
{
	aSprites_[ gaHUD_BACKGROUND ] = RcSpriteManager::pInstance()->pSprite( "data/hud_bg.tga", 1 );
	aSprites_[ gaHUD_BTNUP ] = RcSpriteManager::pInstance()->pSprite( "data/hud_btnup.tga", 1 );
	aSprites_[ gaHUD_BTNDN ] = RcSpriteManager::pInstance()->pSprite( "data/hud_btndn.tga", 1 );
	aSprites_[ gaHUD_BTNOV ] = RcSpriteManager::pInstance()->pSprite( "data/hud_btnov.tga", 1 );

	nButtons_ = 0;

	Score_ = 0;
	Health_ = 100;
	bUpdateHUD_ = BcTrue;
}

//////////////////////////////////////////////////
// Dtor
GaGameHudState::~GaGameHudState()
{

}

//////////////////////////////////////////////////
// addButton
BcU32 GaGameHudState::addButton( const BcChar* pText )
{
	BcAssert( nButtons_ != MAX_BUTTONS );

	GaButton* pButton = &Buttons_[ nButtons_++ ];

	BcVec2d Position = BcVec2d( 32.0f, ( nButtons_ + 4 ) * 48.0f );

	pButton->pText_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", pText, 18 );
	pButton->DownTimer_ = -1.0f;
	pButton->pCurrSprite_ = aSprites_[ gaHUD_BTNUP ];
	pButton->Position_ = Position ;
	pButton->Dimensions_ = BcVec2d( 128.0f, 32.0f );

	return ( nButtons_ - 1 );
}

//////////////////////////////////////////////////
// execute
void GaGameHudState::execute()
{
	// Time out down buttons
	for( BcU32 i = 0; i < nButtons_; ++i )
	{
		GaButton* pButton = &Buttons_[ i ];

		if( pButton->DownTimer_ > 0.0f )
		{
			pButton->DownTimer_ -= BcMain::pInstance()->tick();

			if( pButton->DownTimer_ < 0.0f )
			{
				pButton->DownTimer_ = -1.0f;
				pButton->pCurrSprite_ = aSprites_[ gaHUD_BTNUP ];
			}
		}
	}

	if( bUpdateHUD_ )
	{
		BcChar Buffer[128];

		sprintf( Buffer, "Score: %u", Score_ );
		pScore_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", Buffer, 18 );

		sprintf( Buffer, "Health: %u%%", Health_ );
		pHealth_ = RcSpriteManager::pInstance()->pString( "data/jaggeddreams.ttf", Buffer, 18 );
	}

}

//////////////////////////////////////////////////
// render
void GaGameHudState::render()
{
	aSprites_[ gaHUD_BACKGROUND ]->draw( 0, BcVec2d( 0.0f, 0.0f ) );

	for( BcU32 i = 0; i < nButtons_; ++i )
	{
		GaButton* pButton = &Buttons_[ i ];

		// Button
		pButton->pCurrSprite_->draw( 0, pButton->Position_ );

		// Text
		BcVec2d TextPos = pButton->Position_ +  pButton->Dimensions_ * 0.5f ;

		if( pButton->DownTimer_ > 0.0f )
		{
			TextPos += BcVec2d( 2.0f, 2.0f );
		}

		//
		pButton->pText_->drawCentered( 0, TextPos );
	}

	pScore_->draw( 0, BcVec2d( 32.0f, 48.0f ) );
	pHealth_->draw( 0, BcVec2d( 32.0f, 96.0f ) );
}

//////////////////////////////////////////////////
// mouseOver
BcU32 GaGameHudState::mouseOver( const BcVec2d& MousePos )
{
	// Find buttons..
	for( BcU32 i = 0; i < nButtons_; ++i )
	{
		GaButton* pButton = &Buttons_[ i ];

		BcVec2d TL = pButton->Position_;
		BcVec2d BR = pButton->Position_ + pButton->Dimensions_;

		if( MousePos.X_ > TL.X_ && MousePos.X_ < BR.X_ &&
		    MousePos.Y_ > TL.Y_ && MousePos.Y_ < BR.Y_ )
		{
			if( pButton->DownTimer_ < 0.0f )
			{
				pButton->pCurrSprite_ = aSprites_[ gaHUD_BTNOV ];
			}

			return i;
		}
		else
		{
			if( pButton->DownTimer_ < 0.0f )
			{
				pButton->DownTimer_ = -1.0f;
				pButton->pCurrSprite_ = aSprites_[ gaHUD_BTNUP ];
			}
		}
	}

	return BcErrorCode;
}

//////////////////////////////////////////////////
// mouseClick
BcU32 GaGameHudState::mouseClick( const BcVec2d& MousePos )
{
	// Find buttons..

	// Find buttons..
	for( BcU32 i = 0; i < nButtons_; ++i )
	{
		GaButton* pButton = &Buttons_[ i ];

		BcVec2d TL = pButton->Position_;
		BcVec2d BR = pButton->Position_ + pButton->Dimensions_;

		if( MousePos.X_ > TL.X_ && MousePos.X_ < BR.X_ &&
			MousePos.Y_ > TL.Y_ && MousePos.Y_ < BR.Y_ )
		{
			pButton->DownTimer_ = 0.25f;
			pButton->pCurrSprite_ = aSprites_[ gaHUD_BTNDN ];

			return i;
		}
	}

	return BcErrorCode;
}

