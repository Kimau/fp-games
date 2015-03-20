/**************************************************************************
*
* File:		SoManager.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __SOMANAGER_H__
#define __SOMANAGER_H__

#include "BcSingleton.h"

#include "portaudio.h"

//////////////////////////////////////////////////
// MIDI
const BcReal SO_MIDIFREQ[] =
{
	8.18f,
	8.66f,
	9.18f,
	9.72f,
	10.3f,
	10.91f,
	11.56f,
	12.25f,
	12.98f,
	13.75f,
	14.57f,
	15.43f,
	16.35f,
	17.32f,
	18.35f,
	19.45f,
	20.6f,
	21.83f,
	23.12f,
	24.5f,
	25.96f,
	27.5f,
	29.14f,
	30.87f,
	32.7f,
	34.65f,
	36.71f,
	38.89f,
	41.2f,
	43.65f,
	46.25f,
	49.0f,
	51.91f,
	55.0f,
	58.27f,
	61.74f,
	65.41f,
	69.3f,
	73.42f,
	77.78f,
	82.41f,
	87.31f,
	92.5f,
	98.0f,
	103.83f,
	110.0f,
	116.54f,
	123.47f,
	130.81f,
	138.59f,
	146.83f,
	155.56f,
	164.81f,
	174.61f,
	185.0f,
	196.0f,
	207.65f,
	220.0f,
	233.08f,
	246.94f,
	261.63f,
	277.18f,
	293.66f,
	311.13f,
	329.63f,
	349.23f,
	369.99f,
	392.0f,
	415.3f,
	440.0f,
	466.16f,
	493.88f,
	523.25f,
	554.37f,
	587.33f,
	622.25f,
	659.26f,
	698.46f,
	739.99f,
	783.99f,
	830.61f,
	880.0f,
	932.33f,
	987.77f,
	1046.5f,
	1108.73f,
	1174.66f,
	1244.51f,
	1318.51f,
	1396.91f,
	1479.98f,
	1567.98f,
	1661.22f,
	1760.0f,
	1864.66f,
	1975.53f,
	2093.0f,
	2217.46f,
	2349.32f,
	2489.02f,
	2637.02f,
	2793.83f,
	2959.96f,
	3135.96f,
	3322.44f,
	3520.0f,
	3729.31f,
	3951.07f,
	4186.01f,
	4434.92f,
	4698.64f,
	4978.03f,
	5274.04f,
	5587.65f,
	5919.91f,
	6271.93f,
	6644.88f,
	7040.0f,
	7458.62f,
	7902.13f,
	8372.02f,
	8869.84f,
	9397.27f,
	9956.06f,
	10548.08f,
	11175.3f,
	11839.82f,
	12543.85f
};

//////////////////////////////////////////////////
// Enums
enum eSynthType
{
	soSYNTH_SINE = 0,
	soSYNTH_PULSE,
	soSYNTH_NOISE
};

//////////////////////////////////////////////////
// Structs
struct SoSynthParam
{
	eSynthType Type_;
	BcReal StartFreq_;
	BcReal EndFreq_;
	BcReal Gain_;
	BcReal A_;
	BcReal D_;
	BcReal S_;
	BcReal R_;
};

//////////////////////////////////////////////////
// SoSynth
class SoSynth
{
public:
	SoSynth();
	~SoSynth();

	void play( const SoSynthParam& Params );
	void play( eSynthType Type, BcReal StartFreq, BcReal EndFreq, BcReal Gain, BcReal A, BcReal D, BcReal S, BcReal R );
	void process( BcU32 Frames, BcReal* pOutput );

	BcReal lerp( BcReal A, BcReal B, BcReal T );

	BcReal synthSine();
	BcReal synthPulse();
	BcReal synthNoise();

	BcReal adsr();

public:
	BcBool bActive_;
	BcReal Cursor_;
	
	eSynthType Type_;
	BcReal StartFreq_;
	BcReal EndFreq_;
	BcReal Gain_;

	BcReal StartCycle_;
	BcReal EndCycle_;
	BcReal Cycle_;
	BcReal Timer_;

	BcReal A_, D_, S_, R_;
	BcReal AEnd_, DEnd_, SEnd_, REnd_;

	BcReal TotalTime_;

};

//////////////////////////////////////////////////
// Inlines
inline SoSynth::SoSynth():
	bActive_( BcFalse )
{
	
}

inline SoSynth::~SoSynth()
{

}

inline void SoSynth::play( const SoSynthParam& Params )
{
	play( Params.Type_, Params.StartFreq_, Params.EndFreq_, Params.Gain_, Params.A_, Params.D_, Params.S_, Params.R_ );
}

inline void SoSynth::play( eSynthType Type, BcReal StartFreq, BcReal EndFreq, BcReal Gain, BcReal A, BcReal D, BcReal S, BcReal R )
{
	Type_ = Type;
	StartFreq_ = StartFreq;
	EndFreq_ = EndFreq;
	Gain_ = Gain;
	A_ = A;
	D_ = D;
	S_ = S;
	R_ = R;

	AEnd_ = A;
	DEnd_ = A + D;
	SEnd_ = A + D + S;
	REnd_ = A + D + S + R;

	TotalTime_ = A_ + D_ + S_ + R_;

	StartCycle_ = ( M_PI * 2.0f ) / ( 22050.0f / StartFreq_ );
	EndCycle_ = ( M_PI * 2.0f ) / ( 22050.0f / EndFreq_ );

	Cycle_ = 0.0f;
	Timer_ = 0.0f;

	bActive_ = BcTrue;
	Cursor_ = 0.0f;
}

inline BcReal SoSynth::lerp( BcReal A, BcReal B, BcReal T )
{
	return ( A * ( 1.0f - T ) ) + ( B * T );
}

inline BcReal SoSynth::synthSine()
{
	const BcReal Val = sinf( Timer_ );

	const BcReal LerpVal = ( Cursor_ / TotalTime_ );
	Cycle_ = lerp( StartCycle_, EndCycle_, LerpVal );

	Timer_ += Cycle_;

	if( Timer_ > ( M_PI * 2.0f ) )
	{
		Timer_ -= ( M_PI * 2.0f );
	}

	return Val;
}

inline BcReal SoSynth::synthPulse()
{
	const BcReal Val = sinf( Timer_ );

	const BcReal LerpVal = ( Cursor_ / TotalTime_ );
	Cycle_ = lerp( StartCycle_, EndCycle_, LerpVal );

	Timer_ += Cycle_;

	if( Timer_ > ( M_PI * 2.0f ) )
	{
		Timer_ -= ( M_PI * 2.0f );
	}

	return ( Val * Val * Val * Val * Val );
}

inline BcReal SoSynth::synthNoise()
{
	return BcRandReal();
}

inline BcReal SoSynth::adsr()
{
	BcReal Env = 0.0f;

	if( Cursor_ < AEnd_ )
	{
		Env = lerp( 0.0f, 1.0f, ( Cursor_ ) / A_ );
	}
	else if( Cursor_ < DEnd_ )
	{
		Env = lerp( 1.0f, 0.5f, ( Cursor_ - AEnd_ ) / D_ );
	}
	else if( Cursor_ < SEnd_ )
	{
		Env = 0.5f;
	}
	else if( Cursor_ < REnd_ )
	{
		Env = lerp( 0.5f, 0.0f, ( Cursor_ - SEnd_ ) / R_ );
	}

	return Env;
}

//////////////////////////////////////////////////
// SoManager
class SoManager:
	public BcSingleton< SoManager >
{
public:	
	SoManager();
	~SoManager();

	void initialise();
	void destroy();

	static int callback( void *pInpBuf, void *pOutBuf, unsigned int Frames, PaTimestamp OutTime, void *pUser );
	void process( BcU32 Frames, BcReal* pOutput );

	void addParams( const SoSynthParam& Params );
	void play( BcU32 iParam );

	SoSynth* pSynth();

private:
	PortAudioStream* pStream_;

	SoSynth Synths_[ 128 ];
	SoSynth EmptySynth_;

	SoSynthParam Params_[ 128 ];
	BcU32 nParams_;
};

//////////////////////////////////////////////////
// Inlines
inline void SoManager::addParams( const SoSynthParam& Params )
{
	if( nParams_ < 128 )
	{
		Params_[ nParams_++ ] = Params;
	}
}

inline void SoManager::play( BcU32 iParam )
{
	pSynth()->play( Params_[ iParam ] );
}


#endif
