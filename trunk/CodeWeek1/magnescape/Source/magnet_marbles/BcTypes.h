/**************************************************************************
*
* File:		BcTypes.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __BCTYPES_H__
#define __BCTYPES_H__

#define _CRT_SECURE_NO_DEPRECATE

#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "SDL_rotozoom.h"
#include "SDL_gfxPrimitives.h"

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////
// Normal type defines
typedef unsigned int		BcU32;
typedef int					BcS32;

typedef unsigned short		BcU16;
typedef short				BcS16;

typedef unsigned char		BcU8;
typedef char				BcS8;

typedef char				BcChar;

typedef float				BcReal;

typedef bool				BcBool;

#define BcTrue				BcBool( true )
#define BcFalse				BcBool( false )
#define BcErrorCode			0xffffffff
#define NULL				0

#define BcAssert( a )		if( !(a) ){ __asm{ int 3 };}

//////////////////////////////////////////////////
// Util
inline BcReal BcRandReal()
{
	const BcReal Number = static_cast< BcReal >( rand()  ) / static_cast< BcReal >( RAND_MAX );
	return ( Number - 0.5f ) * 2.0f;
}

#endif