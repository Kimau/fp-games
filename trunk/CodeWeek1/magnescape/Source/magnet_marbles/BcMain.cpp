/**************************************************************************
*
* File:		BcMain.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "BcMain.h"

#include "BcBaseState.h"

#include "SoManager.h"

//////////////////////////////////////////////////
// Ctor
BcMain::BcMain():
	pTopState_( NULL ),
	pScreenSurface_( NULL )
{

}

//////////////////////////////////////////////////
// Dtor
BcMain::~BcMain()
{

}

//////////////////////////////////////////////////
// initialise
void BcMain::initialise()
{
	SDL_Init( SDL_INIT_EVERYTHING );

	pScreenSurface_ = SDL_SetVideoMode( 640, 480, 32, SDL_DOUBLEBUF | SDL_HWSURFACE );
	BcAssert( pScreenSurface_ != NULL );

	SDL_WM_SetCaption( "Magnescape", NULL );

	PrevTicks_ = SDL_GetTicks();
	CurrTicks_ = SDL_GetTicks();

	for( BcU32 i = 0; i < 8; ++i )
	{
		bPrevMouseButton_[ i ] = BcFalse;
		bCurrMouseButton_[ i ] = BcFalse;
	}

	for( BcU32 i = 0; i < 512; ++i )
	{
		CurrKeyState_[ i ] = BcFalse;
		PrevKeyState_[ i ] = BcFalse;
	}

	SoManager::pInstance()->initialise();
}

//////////////////////////////////////////////////
// execute
void BcMain::execute()
{
	SDL_Event Event;
	while( 1 )
	{
		// do timing
		PrevTicks_ = CurrTicks_;
		CurrTicks_ = SDL_GetTicks();

		Tick_ = static_cast< BcReal >( CurrTicks_ - PrevTicks_ ) / 1000.0f;

		//
		for( BcU32 i = 0; i < 8; ++i )
		{
			bPrevMouseButton_[ i ] = bCurrMouseButton_[ i ];
		}

		for( BcU32 i = 0; i < 512; ++i )
		{
			PrevKeyState_[ i ] = CurrKeyState_[ i ];
		}

		// do events
		SDL_PollEvent( &Event );

		{
			switch( Event.type )
			{
			case SDL_QUIT:
				return;
				break;

			case SDL_MOUSEMOTION:
				CursorPos_.X_ = Event.motion.x;
				CursorPos_.Y_ = Event.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if( Event.button.button < 8 )
				{
					bCurrMouseButton_[ Event.button.button ] = BcTrue;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if( Event.button.button < 8 )
				{
					bCurrMouseButton_[ Event.button.button ] = BcFalse;
				}
				break;

			case SDL_KEYDOWN:
				CurrKeyState_[ Event.key.keysym.sym ] = BcTrue;
				break;

			case SDL_KEYUP:
				CurrKeyState_[ Event.key.keysym.sym ] = BcFalse;
				break;

			default:
				break;
			}

		}

		// do execute
		BcAssert( pTopState_ != NULL );
		pTopState_->processExecute();

		// do render
		SDL_FillRect( pScreenSurface_, NULL, 0x00000000 );
		pTopState_->processRender();
		SDL_Flip( pScreenSurface_ );

	}
}

//////////////////////////////////////////////////
// destroy
void BcMain::destroy()
{
	SoManager::pInstance()->destroy();
}

//////////////////////////////////////////////////
// main

int main( int argc, char** argv )
{
	BcMain::pInstance()->initialise();

	extern void externInit();
	externInit();

	BcMain::pInstance()->execute();

	BcMain::pInstance()->destroy();

	return 0;
}
