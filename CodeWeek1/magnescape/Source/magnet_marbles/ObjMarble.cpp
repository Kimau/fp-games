/**************************************************************************
*
* File:		ObjMarble.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "ObjMarble.h"

#include "ObjManager.h"

#include "BcMain.h"

#include "RcSpriteManager.h"

#include "PhManager.h"


//////////////////////////////////////////////////
// Ctor
ObjMarble::ObjMarble()
{
	Pos_ = BcVec2d( 128.0f, 768.0f );

	//
	pSprite_ = RcSpriteManager::pInstance()->pSprite( "data/ball_red.tga", 4 );
	iSprite_ = 0;

	//
	Pole_ = objPOLE_RED;
}

//////////////////////////////////////////////////
// Dtor
ObjMarble::~ObjMarble()
{
	
}

//////////////////////////////////////////////////
// update
void ObjMarble::update()
{
	BcVec2d Normal;
	if( ObjManager::pInstance()->pMap()->testCircle( gaMAP_TILE_WALL, Pos_, 16.0f, &Normal ) )
	{
		Pos_ = OldPos_;
		Vel_ = Vel_ - Normal * ( 2.0f * ( Vel_.dot( Normal ) ) );

		Vel_ -= ( Vel_ * 0.05f );
	}

	// Poles
	switch( Pole_ )
	{
	case objPOLE_RED:
		Acc_ = -PhManager::pInstance()->fieldDir( Pos_ );
		break;

	case objPOLE_BLUE:
		Acc_ = PhManager::pInstance()->fieldDir( Pos_ );
		break;
	}

	// Gravity
	Acc_ += BcVec2d( 0.0f, 100.0f );

	//
	SpriteTick_ -= BcMain::pInstance()->tick();
	if( SpriteTick_ < 0.0f )
	{
		SpriteTick_ = 0.1f;
		iSprite_ = ( iSprite_ + 1 ) % 4;
	}
}

//////////////////////////////////////////////////
// render
void ObjMarble::render( const BcVec2d& WorldPos )
{
	pSprite_->drawCentered( iSprite_, Pos_ - WorldPos );
}

