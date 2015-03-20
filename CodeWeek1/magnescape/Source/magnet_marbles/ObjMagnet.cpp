/**************************************************************************
*
* File:		ObjMagnet.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "ObjMagnet.h"

#include "BcMain.h"

#include "RcSpriteManager.h"
//////////////////////////////////////////////////
// Ctor
ObjMagnet::ObjMagnet( const BcVec2d& Pos, const BcVec2d& Ori, BcReal Mag )
{
	Pos_ = Pos;
	Ori_ = Ori;
	Mag_ = Mag;

	//
	pSprite_ = RcSpriteManager::pInstance()->pSprite( "data/magnet.tga", 1 );

}

//////////////////////////////////////////////////
// Dtor
ObjMagnet::~ObjMagnet()
{

}

//////////////////////////////////////////////////
// update
void ObjMagnet::update()
{

}

//////////////////////////////////////////////////
// render
void ObjMagnet::render( const BcVec2d& WorldPos )
{
	BcReal Angle = ( atan2f( Ori_.X_, Ori_.Y_ ) / ( 2.0f * BcReal( M_PI ) ) ) * 32.0f;

	if( Angle < 0.0f )
	{
		Angle += 32.0f;
	}

	pSprite_->drawCentered( 0, Pos_ - WorldPos, BcU32( Angle ) );
}

