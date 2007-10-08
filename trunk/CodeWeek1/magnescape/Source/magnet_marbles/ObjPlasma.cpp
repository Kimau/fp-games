/**************************************************************************
*
* File:		ObjPlasma.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "ObjPlasma.h"

#include "BcMain.h"

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
ObjPlasma::ObjPlasma( const BcVec2d& Pos )
{
	//
	pSprite_[ 0 ] = RcSpriteManager::pInstance()->pSprite( "data/portal_1.tga", 1 );
	pSprite_[ 1 ] = RcSpriteManager::pInstance()->pSprite( "data/portal_2.tga", 1 );
	pSprite_[ 2 ] = RcSpriteManager::pInstance()->pSprite( "data/portal_3.tga", 1 );
	pSprite_[ 3 ] = RcSpriteManager::pInstance()->pSprite( "data/portal_4.tga", 1 );

	Pos_ = Pos;
	Timer_ = 0.0f;
}

//////////////////////////////////////////////////
// Dtor
ObjPlasma::~ObjPlasma()
{

}

//////////////////////////////////////////////////
// update
void ObjPlasma::update()
{
	Timer_ += BcMain::pInstance()->tick();
}

//////////////////////////////////////////////////
// render
void ObjPlasma::render( const BcVec2d& WorldPos )
{
	BcVec2d Pos;

	Pos = Pos_ - WorldPos + ( BcVec2d( sinf( Timer_ ), cosf( Timer_ ) ) * 2.0f );
	pSprite_[ 0 ]->drawCentered( 0, Pos );

	Pos = Pos_ - WorldPos + ( BcVec2d( sinf( Timer_ * 2.1f ), cosf( -Timer_ ) ) * 2.0f );
	pSprite_[ 1 ]->drawCentered( 0, Pos );

	Pos = Pos_ - WorldPos + ( BcVec2d( -sinf( Timer_ * 0.1f ), cosf( Timer_ * 0.24f ) ) * 2.0f );
	pSprite_[ 2 ]->drawCentered( 0, Pos );

	Pos = Pos_ - WorldPos + ( BcVec2d( sinf( Timer_ * 0.4f ), cosf( Timer_ * 2.4f ) ) * 2.0f );
	pSprite_[ 3 ]->drawCentered( 0, Pos );
}

