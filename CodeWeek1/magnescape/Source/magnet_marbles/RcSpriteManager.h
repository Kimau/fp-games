/**************************************************************************
*
* File:		RcSpriteManager.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __RCSPRITEMANAGER_H__
#define __RCSPRITEMANAGER_H__

#include "BcSingleton.h"
#include "RcSprite.h"

#include <string>
#include <map>

//////////////////////////////////////////////////
// RcSpriteManager
class RcSpriteManager:
	public BcSingleton< RcSpriteManager >
{
public:
	RcSpriteManager();
	virtual ~RcSpriteManager();

	RcSprite* pSprite( const BcChar* pFileName, BcU32 nSprites );
	RcSprite* pString( const BcChar* pFileName, const BcChar* pText, BcU32 Size );

private:
	typedef std::string RcString;
	typedef std::map< RcString, RcSprite* > RcSpriteMap;
	typedef std::map< RcString, TTF_Font* > RcTTFMap;
	
	RcSpriteMap SpriteMap_;
	RcSpriteMap StringMap_;
	RcTTFMap FontMap_;


};


#endif