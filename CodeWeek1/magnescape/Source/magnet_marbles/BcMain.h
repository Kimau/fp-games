/**************************************************************************
*
* File:		BcMain.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __BCMAIN_H__
#define __BCMAIN_H__

#include "BcSingleton.h"

#include "BcVec2d.h"

//////////////////////////////////////////////////
// Forward Declarations
class BcBaseState;

//////////////////////////////////////////////////
// BcMain
class BcMain:
	public BcSingleton< BcMain >
{
public:
	BcMain();
	virtual ~BcMain();

	void initialise();
	void execute();
	void destroy();

	//
	void topState( BcBaseState* pTopState );

	//
	void loadSprite( const BcChar* pFilename );

	//
	SDL_Surface* pScreenSurface();

	// timing
	BcReal tick() const;

	// input
	BcVec2d cursorPos() const;
	BcBool mouseDown( BcU32 iButton ) const;
	BcBool mousePressed( BcU32 iButton ) const;
	BcBool mouseReleased( BcU32 iButton ) const;

	BcBool keyDown( BcU32 iKey ) const;
	BcBool keyPressed( BcU32 iKey ) const;
	BcBool keyReleased( BcU32 iKey ) const;

private:
	// 
	BcBaseState* pTopState_;

	//
	SDL_Surface* pScreenSurface_;

	//
	BcU32 CurrTicks_;
	BcU32 PrevTicks_;
	BcReal Tick_;

	//
	BcVec2d CursorPos_;
	BcBool bPrevMouseButton_[ 8 ];
	BcBool bCurrMouseButton_[ 8 ];

	BcBool CurrKeyState_[ 512 ];
	BcBool PrevKeyState_[ 512 ];

};

//////////////////////////////////////////////////
// Inlines
inline void BcMain::topState( BcBaseState* pTopState )
{
	pTopState_ = pTopState;

}

inline SDL_Surface* BcMain::pScreenSurface()
{
	return pScreenSurface_;
}

inline BcReal BcMain::tick() const
{
	return Tick_;
}

inline 	BcVec2d BcMain::cursorPos() const
{
	return CursorPos_;
}

inline BcBool BcMain::mouseDown( BcU32 iButton ) const
{
	return bCurrMouseButton_[ iButton ];
}

inline BcBool BcMain::mousePressed( BcU32 iButton ) const
{
	return ( bCurrMouseButton_[ iButton ] && !bPrevMouseButton_[ iButton ] );
}

inline BcBool BcMain::mouseReleased( BcU32 iButton ) const
{
	return ( !bCurrMouseButton_[ iButton ] && bPrevMouseButton_[ iButton ] );
}

inline BcBool BcMain::keyDown( BcU32 iKey ) const
{
	return CurrKeyState_[ iKey ];
}

inline BcBool BcMain::keyPressed( BcU32 iKey ) const
{
	return ( CurrKeyState_[ iKey ] && !PrevKeyState_[ iKey ] );
}

inline BcBool BcMain::keyReleased( BcU32 iKey ) const
{
	return ( !CurrKeyState_[ iKey ] && PrevKeyState_[ iKey ] );
}


#endif

