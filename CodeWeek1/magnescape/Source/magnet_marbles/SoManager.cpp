/**************************************************************************
*
* File:		SoManager.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "SoManager.h"

//////////////////////////////////////////////////
// process
void SoSynth::process( BcU32 Frames, BcReal* pOutput )
{
	switch( Type_ )
	{
	case soSYNTH_SINE:
		for( BcU32 i = 0; i < Frames; ++i )
		{
			*pOutput += synthSine() * adsr() * Gain_;
			++pOutput;
			Cursor_ += 1.0f;
		}
		break;

	case soSYNTH_PULSE:
		for( BcU32 i = 0; i < Frames; ++i )
		{
			*pOutput += synthPulse() * adsr() * Gain_;
			++pOutput;
			Cursor_ += 1.0f;
		}
		break;

	case soSYNTH_NOISE:
		for( BcU32 i = 0; i < Frames; ++i )
		{
			*pOutput += synthNoise() * adsr() * Gain_;
			++pOutput;
			Cursor_ += 1.0f;
		}
		break;
	}

	if( Cursor_ > TotalTime_ )
	{
		bActive_ = BcFalse;
	}
}


//////////////////////////////////////////////////
// Ctor
SoManager::SoManager():
	nParams_( 0 )
{

}

//////////////////////////////////////////////////
// Dtor
SoManager::~SoManager()
{

}

//////////////////////////////////////////////////
// initialise
void SoManager::initialise()
{
	Pa_Initialize();

	// Setup audio stream
	PaError Err = Pa_OpenDefaultStream( &pStream_,
	                                    0,	
	                                    1,
	                                    paFloat32,
	                                    22050,
	                                    1024,		
	                                    2,		
	                                    callback,
	                                    this );

	if( Err == paNoError )
	{
		// Start her up.
		Pa_StartStream( pStream_ );
	}
}	

//////////////////////////////////////////////////
// destroy
void SoManager::destroy()
{
	Pa_StopStream( pStream_ );
}

//////////////////////////////////////////////////
// callback
int SoManager::callback( void *pInpBuf, void *pOutBuf, unsigned int Frames, PaTimestamp OutTime, void *pUser )
{
	SoManager* pThis = reinterpret_cast< SoManager* >( pUser );

	pThis->process( Frames, reinterpret_cast< BcReal* >( pOutBuf ) );

	return 0;
}

//////////////////////////////////////////////////
//	process
void SoManager::process( BcU32 Frames, BcReal* pOutput )
{
	memset( pOutput, 0, sizeof( BcReal ) * Frames );

	for( BcU32 i = 0; i < 128; ++i )
	{
		if( Synths_[ i ].bActive_ == BcTrue )
		{
			Synths_[ i ].process( Frames, pOutput );
		}
	}
}

//////////////////////////////////////////////////
//
SoSynth* SoManager::pSynth()
{
	for( BcU32 i = 0; i < 128; ++i )
	{
		if( Synths_[ i ].bActive_ == BcFalse )
		{
			return &Synths_[ i ];
		}
	}

	return &EmptySynth_;
}