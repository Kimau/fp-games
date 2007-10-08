/**************************************************************************
*
* File:		GaMap.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaMap.h"

#include "RcSpriteManager.h"

#include "PhManager.h"

#include "BcMain.h"

//////////////////////////////////////////////////
// Globals
static BcVec2d gEdgeOffsets[] =
{
	BcVec2d(  0.0f,  0.0f ),
	BcVec2d(  0.0f, -1.0f ),
	BcVec2d(  1.0f, -1.0f ),
	BcVec2d(  1.0f,  0.0f ),
	BcVec2d(  1.0f,  1.0f ),
	BcVec2d(  0.0f,  1.0f ),
	BcVec2d( -1.0f,  1.0f ),
	BcVec2d( -1.0f,  0.0f ),
	BcVec2d( -1.0f, -1.0f ),
};

//////////////////////////////////////////////////
// Ctor
GaMap::GaMap()
{
	pSprites_ = RcSpriteManager::pInstance()->pSprite( "data/tiles.tga", 8 );
	pGameBGA_ = RcSpriteManager::pInstance()->pSprite( "data/game_bg.tga", 1 );
	pGameBGB_ = RcSpriteManager::pInstance()->pSprite( "data/game_bgb.tga", 1 );
	pEnemy_ = RcSpriteManager::pInstance()->pSprite( "data/enemy.tga", 8 );
	pPortal_ = RcSpriteManager::pInstance()->pSprite( "data/portal_1.tga", 1 );

	// Normalise..
	for( BcU32 i = 1; i < 9; ++i )
	{
		gEdgeOffsets[ i ].normalise();
	}

	nEnemies_ = 0;
}

//////////////////////////////////////////////////
// Dtor
GaMap::~GaMap()
{

}

//////////////////////////////////////////////////
// reset
void GaMap::reset()
{
	for( BcU32 i = 0; i < MAX_XSIZE; ++i )
	{
		for( BcU32 j = 0; j < MAX_YSIZE; ++j )
		{
			/*
			if( i == 0 || i == ( MAX_XSIZE - 1 ) ||
				j == 0 || j == ( MAX_YSIZE - 1 ) )
			{
				MapTiles_[ i ][ j ].TileType_ = gaMAP_TILE_WALL;
			}
			else
			{
				MapTiles_[ i ][ j ].TileType_ = gaMAP_TILE_BLANK;
			}
			*/
			MapTiles_[ i ][ j ].Function_ = gaMAP_FUNCTION_NORMAL;
			MapTiles_[ i ][ j ].Param_ = 0;
		}
	}
}

//////////////////////////////////////////////////
// update
void GaMap::update()
{
	ScrollingA_ += BcVec2d( 0.0f, 190.0f ) * BcMain::pInstance()->tick();

	if( ScrollingA_.Y_ > 480.0f )
	{
		ScrollingA_.Y_ -= 480.0f;
	}

	ScrollingB_ += BcVec2d( 0.0f, 500.0f ) * BcMain::pInstance()->tick();

	if( ScrollingB_.Y_ > 480.0f )
	{
		ScrollingB_.Y_ -= 480.0f;
	}
}

//////////////////////////////////////////////////
// render
void GaMap::render( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom )
{
	// Scrolling BG
	pGameBGA_->draw( 0, ScrollingA_ - BcVec2d( 0.0f, 480.0f ) + Position );
	pGameBGA_->draw( 0, ScrollingA_ + Position );
	pGameBGB_->draw( 0, ScrollingB_ - BcVec2d( 0.0f, 480.0f ) + Position );
	pGameBGB_->draw( 0, ScrollingB_ + Position );

	// Main
	BcS32 iX = static_cast< BcS32 >( RenderFrom.X_ / 64.0f );
	BcS32 iY = static_cast< BcS32 >( RenderFrom.Y_ / 64.0f );

	BcS32 dX = static_cast< BcS32 >( Dimensions.X_ / 64.0f ) + 1;
	BcS32 dY = static_cast< BcS32 >( Dimensions.Y_ / 64.0f ) + 2;

	for( BcS32 i = iX; i < (iX + dX); ++i )
	{
		for( BcS32 j = iY; j < (iY + dY); ++j )
		{
			// Validate
			if( i >= 0 && j >= 0 && i < MAX_XSIZE && j < MAX_YSIZE )
			{
				BcU32 iSprite = MapTiles_[ i ][ j ].TileType_;
				BcVec2d SprPos = Position + BcVec2d( i * 64.0f, j * 64.0f ) - RenderFrom;

				if( iSprite > 0 )
				{
					BcU32 iSprite2 = (iSprite + ( i + j ) % 2) + 1;
					pSprites_->draw( iSprite2, SprPos );
					pSprites_->draw( iSprite, SprPos );
				}
			}
		}
	}
}

//////////////////////////////////////////////////
// renderField
void GaMap::renderField( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom )
{
	BcS32 iX = static_cast< BcS32 >( RenderFrom.X_ / 64.0f );
	BcS32 iY = static_cast< BcS32 >( RenderFrom.Y_ / 64.0f );

	BcS32 dX = static_cast< BcS32 >( Dimensions.X_ / 64.0f ) + 1;
	BcS32 dY = static_cast< BcS32 >( Dimensions.Y_ / 64.0f ) + 2;

	for( BcReal i = Position.X_; i < ( Position.X_ + Dimensions.X_ ); i += 16.0f )
	{
		for( BcReal j = Position.Y_; j < ( Position.Y_ + Dimensions.Y_ ); j += 16.0f )
		{ 
			BcVec2d LineCentre = BcVec2d( i, j );
			BcVec2d FieldDir = PhManager::pInstance()->fieldDir( ( LineCentre + RenderFrom ) - Position );
			FieldDir.normalise();
			FieldDir = FieldDir * 4.0f;

			BcVec2d PosA = LineCentre - FieldDir;
			BcVec2d PosB = LineCentre + FieldDir;

			lineRGBA( RcSprite::pScreenSurface_, LineCentre.X_, LineCentre.Y_, PosB.X_, PosB.Y_, 0, 0, 255, 255 );
			lineRGBA( RcSprite::pScreenSurface_, PosA.X_, PosA.Y_, LineCentre.X_, LineCentre.Y_, 255, 0, 0, 255 );
		}
	}
}

//////////////////////////////////////////////////
// renderEnemies
void GaMap::renderEnemies( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom )
{
	for( BcU32 i = 0; i < nEnemies_; ++i )
	{
		BcVec2d EnemyPos = Position + ( Enemies_[ i ] - RenderFrom );
		pEnemy_->drawCentered( 0, EnemyPos );
	}
}

//////////////////////////////////////////////////
// renderPortal
void GaMap::renderPortal( const BcVec2d& Position, const BcVec2d& Dimensions, const BcVec2d& RenderFrom )
{
	BcVec2d Pos = Position + ( Portal_ - RenderFrom );
	pPortal_->drawCentered( 0, Pos );
}


//////////////////////////////////////////////////
// testCircle
BcBool GaMap::testCircle( BcU32 TileType, const BcVec2d& Pos, BcReal Rad, BcVec2d* pNormal )
{
	BcVec2d Normal;
	BcBool bCol = BcFalse;

	BcS32 iX = static_cast< BcS32 >( ( Pos.X_ ) / 64.0f );
	BcS32 iY = static_cast< BcS32 >( ( Pos.Y_ ) / 64.0f );

	// Edge of map as walls.
	if( TileType == gaMAP_TILE_WALL )
	{
		if( iX < 0 )
		{
			bCol |= BcTrue;
			Normal += BcVec2d( 1.0f, 0.0f );
		}

		if( iX >= MAX_XSIZE )
		{
			bCol |= BcTrue;
			Normal += BcVec2d( -1.0f, 0.0f );
		}

		if( iY < 0 )
		{
			bCol |= BcTrue;
			Normal += BcVec2d( 0.0f, 1.0f );
		}

		if( iY >= MAX_YSIZE )
		{
			bCol |= BcTrue;
			Normal += BcVec2d( 0.0f, -1.0f );
		}
	}

	// Edge collision
	for( BcU32 i = 0; i < 9; ++i )
	{
		BcS32 iX = static_cast< BcS32 >( ( ( Pos.X_ ) - ( gEdgeOffsets[ i ].X_ * Rad ) ) / 64.0f );
		BcS32 iY = static_cast< BcS32 >( ( ( Pos.Y_ ) - ( gEdgeOffsets[ i ].Y_ * Rad ) ) / 64.0f );

		if( iX >= 0 && iY >= 0 && iX < MAX_XSIZE && iY < MAX_YSIZE )
		{
			BcU32 Tile = MapTiles_[ iX ][ iY ].TileType_;

			if( Tile == TileType )
			{
				bCol = BcTrue;
				Normal += gEdgeOffsets[ i ];
			}
		}
	}


	// Assign if need to.
	if( pNormal != NULL )
	{
		Normal.normalise();
		*pNormal = Normal;
	}

	return bCol;
}

//////////////////////////////////////////////////
// setTile
BcBool GaMap::setTile( BcU32 TileType, BcS32 iX, BcS32 iY )
{
	BcBool bRetVal = BcFalse;

	if( iX >= 0 && iY >= 0 && iX < MAX_XSIZE && iY < MAX_YSIZE )
	{
		if( TileType < gaMAP_TILE_MAX )
		{
			MapTiles_[ iX ][ iY ].TileType_ = TileType;
		}
	}

	return bRetVal;
}

//////////////////////////////////////////////////
// save
void GaMap::save( const BcChar* pFileName )
{
	FILE* pFile = NULL;

	pFile = fopen( pFileName, "wb+" );
	BcAssert( pFile != NULL );

	fwrite( &MapTiles_[ 0 ], sizeof( MapTiles_ ), 1, pFile );
	fwrite( &nEnemies_, sizeof( nEnemies_ ), 1, pFile );
	fwrite( &Enemies_[ 0 ], sizeof( Enemies_ ), 1, pFile );
	fwrite( &Portal_, sizeof( Portal_ ), 1, pFile );
	fclose( pFile );
}

//////////////////////////////////////////////////
// load
void GaMap::load( const BcChar* pFileName )
{
	FILE* pFile = NULL;

	pFile = fopen( pFileName, "rb+" );
	BcAssert( pFile != NULL );

	fread( &MapTiles_[ 0 ], sizeof( MapTiles_ ), 1, pFile );
	fread( &nEnemies_, sizeof( nEnemies_ ), 1, pFile );
	fread( &Enemies_[ 0 ], sizeof( Enemies_ ), 1, pFile );
	fread( &Portal_, sizeof( Portal_ ), 1, pFile );
	fclose( pFile );
}