/**************************************************************************
*
* File:		RcSprite.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "RcSprite.h"

#include "BcMain.h"

//////////////////////////////////////////////////
// Statics
SDL_Surface*		RcSprite::pScreenSurface_ = NULL;

//////////////////////////////////////////////////
// Ctor
RcSprite::RcSprite():
	pSurface_( NULL )
{
	if( pScreenSurface_ == NULL )
	{
		pScreenSurface_ = BcMain::pInstance()->pScreenSurface();
	}
}

//////////////////////////////////////////////////
// Dtor
RcSprite::~RcSprite()
{
	if( pSurface_ != NULL )
	{
		SDL_FreeSurface( pSurface_ );
	}
}

//////////////////////////////////////////////////
// Dtor
void RcSprite::load( const BcChar* pFilename, BcU32 nSprites )
{
	BcAssert( pSurface_ == NULL );

	pSurface_ = IMG_Load( pFilename );
	BcAssert( pSurface_ != NULL );

	nSprites_ = nSprites;

	DstRect_.w = pSurface_->w / nSprites_;
	DstRect_.h = pSurface_->h;

	for( BcU32 i = 0; i < 32; ++i )
	{
		//pRotations_[ i ] = SDL_CreateRGBSurface( SDL_SWSURFACE, DstRect_.w, DstRect_.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 );

		BcReal Angle = ( 360.0f / 32.0f ) * static_cast< BcReal >( i );

		pRotations_[ i ] = rotozoomSurface( pSurface_, Angle, 1.0f, 1 );
	}
}

//////////////////////////////////////////////////
// draw
void RcSprite::draw( BcU32 iIndex, const BcVec2d& Position )
{
	DstRect_.w = pSurface_->w / nSprites_;
	DstRect_.h = pSurface_->h;
	DstRect_.x = static_cast< Sint16 >( Position.X_ );
	DstRect_.y = static_cast< Sint16 >( Position.Y_ );

	SrcRect_ = DstRect_;
	SrcRect_.y = 0;
	SrcRect_.x = DstRect_.w * iIndex;

	SDL_BlitSurface( pSurface_, &SrcRect_, pScreenSurface_, &DstRect_ );
}

//////////////////////////////////////////////////
// drawCentered
void RcSprite::drawCentered( BcU32 iIndex, const BcVec2d& Position, BcU32 iRotation/* = 0*/ )
{
	if( iRotation == 0 )
	{
		DstRect_.w = pSurface_->w / nSprites_;
		DstRect_.h = pSurface_->h;
		DstRect_.x = static_cast< Sint16 >( Position.X_ - ( DstRect_.w / 2 ) );
		DstRect_.y = static_cast< Sint16 >( Position.Y_ - ( DstRect_.h / 2 ) );

		SrcRect_ = DstRect_;
		SrcRect_.y = 0;
		SrcRect_.x = DstRect_.w * iIndex;

		SDL_BlitSurface( pSurface_, &SrcRect_, pScreenSurface_, &DstRect_ );
	}
	else
	{
		DstRect_.w = pRotations_[ iRotation ]->w / nSprites_;
		DstRect_.h = pRotations_[ iRotation ]->h;
		DstRect_.x = static_cast< Sint16 >( Position.X_ - ( DstRect_.w / 2 ) );
		DstRect_.y = static_cast< Sint16 >( Position.Y_ - ( DstRect_.h / 2 ) );

		SrcRect_ = DstRect_;
		SrcRect_.y = 0;
		SrcRect_.x = DstRect_.w * iIndex;

		SDL_BlitSurface( pRotations_[ iRotation ], &SrcRect_, pScreenSurface_, &DstRect_ );
	}
}
