/**************************************************************************
*
* File:		GaMap.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GAMAP_H__
#define __GAMAP_H__

#include "BcVec2d.h"
#include "RcSprite.h"

//////////////////////////////////////////////////
// Enums
enum
{
	gaMAP_TILE_BLANK = 0,
	gaMAP_TILE_WALL,
	gaMAP_TILE_ENEMY,
	gaMAP_TILE_PORTAL,
	gaMAP_TILE_MAX
};

enum
{
	gaMAP_FUNCTION_NORMAL = 0,
	gaMAP_FUNCTION_HOLE
};

//////////////////////////////////////////////////
// GaMap
class GaMap
{
public:
	enum
	{
		MAX_XSIZE = 7,
		MAX_YSIZE = 128
	};

public:
	GaMap();
	virtual ~GaMap();

	void reset();

	void update();
	void render( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom );
	void renderField( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom );
	void renderEnemies( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom );
	void renderPortal( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom );

	BcBool testCircle( BcU32 TileType, const BcVec2d& Pos, BcReal Rad, BcVec2d* pNormal );

	BcBool setTile( BcU32 TileType, BcS32 iX, BcS32 iY );

	//
	void save( const BcChar* pFileName );
	void load( const BcChar* pFileName );

	//
	void addEnemy( const BcVec2d& Position );

	BcU32 nEnemies() const;
	const BcVec2d& getEnemy( BcU32 i );

	void setPortal( const BcVec2d& Position );
	const BcVec2d& getPortal();

private:
	struct GaMapTile
	{
		BcU32 TileType_;
		BcU32 Function_;
		BcU32 Param_;
	};

	GaMapTile MapTiles_[MAX_XSIZE][MAX_YSIZE];

	RcSprite* pSprites_;

	RcSprite* pGameBGA_;
	RcSprite* pGameBGB_;

	RcSprite* pEnemy_;
	RcSprite* pPortal_;

	BcVec2d ScrollingA_;
	BcVec2d ScrollingB_;

	BcU32 nEnemies_;
	BcVec2d Enemies_[32];

	BcVec2d Portal_;

};

//////////////////////////////////////////////////
// Inlines
inline void GaMap::addEnemy( const BcVec2d& Position )
{
	if( nEnemies_ < 32 )
	{
		Enemies_[ nEnemies_++ ] = Position;
	}
}

inline BcU32 GaMap::nEnemies() const
{
	return nEnemies_;
}

inline const BcVec2d& GaMap::getEnemy( BcU32 i )
{
	return Enemies_[ i ];
}

inline void GaMap::setPortal( const BcVec2d& Position )
{
	Portal_ = Position;
}

inline const BcVec2d& GaMap::getPortal()
{
	return Portal_;
}

#endif