/**************************************************************************
*
* File:		RcSprite.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __RCSPRITE_H__
#define __RCSPRITE_H__

#include "BcTypes.h"

#include "BcVec2d.h"

//////////////////////////////////////////////////
// Forward Declarations
class RcSpriteManager;

//////////////////////////////////////////////////
// RcSprite
class RcSprite
{
public:
	RcSprite();
	~RcSprite();

	void load( const BcChar* pFilename, BcU32 nSprites );
	void draw( BcU32 iIndex, const BcVec2d& Position );
	
	void drawCentered( BcU32 iIndex, const BcVec2d& Position, BcU32 iRotation = 0 );

	static SDL_Surface*		pScreenSurface_;

	SDL_Surface*			pSurface_;
	BcU32					nSprites_;
	SDL_Rect				DstRect_;
	SDL_Rect				SrcRect_;

	SDL_Surface*			pRotations_[32];

};

#endif