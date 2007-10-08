/**************************************************************************
*
* File:		RcSpriteManager.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "RcSpriteManager.h"

//////////////////////////////////////////////////
// Ctor
RcSpriteManager::RcSpriteManager()
{
	TTF_Init();
}

//////////////////////////////////////////////////
// Dtor
RcSpriteManager::~RcSpriteManager()
{

}

//////////////////////////////////////////////////
// Ctor
RcSprite* RcSpriteManager::pSprite( const BcChar* pFileName, BcU32 nSprites )
{
	RcString SpriteName = RcString( pFileName );

	RcSprite* pSprite = SpriteMap_[ SpriteName ];
	if( pSprite == NULL )
	{
		pSprite = new RcSprite();
		pSprite->load( pFileName, nSprites );

		SpriteMap_[ SpriteName ] = pSprite;
	}

	return pSprite;
}

//////////////////////////////////////////////////
// pString
RcSprite* RcSpriteManager::pString( const BcChar* pFileName, const BcChar* pText, BcU32 Size )
{
	BcChar SizeBuffer[32];
	itoa( Size, SizeBuffer, 10 );

	RcString FontName = RcString( "_" ) + RcString( SizeBuffer );
	RcString StringName = FontName + RcString( "_" ) + RcString( pText );

	TTF_Font* pFont = FontMap_[ FontName ];
	RcSprite* pString = StringMap_[ StringName ];

	if( pFont == NULL )
	{
		// Not loaded. Load it.
		pFont = TTF_OpenFont( pFileName, Size );
		BcAssert( pFont != NULL );

		FontMap_[ FontName ] = pFont;
	}

	if( pString == NULL )
	{
		// Not already rendered and cached. Render it.
		pString = new RcSprite();

		SDL_Color Colour;
		Colour.r = 0xff;
		Colour.g = 0xff;
		Colour.b = 0xff;

		pString->pSurface_ = TTF_RenderUTF8_Solid( pFont, pText, Colour );
		pString->nSprites_ = 1;

		StringMap_[ StringName ] = pString;
	}

	return pString;
}
