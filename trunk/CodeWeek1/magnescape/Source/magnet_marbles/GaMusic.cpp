/**************************************************************************
*
* File:		GaMusic.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaMusic.h"
#include "BcMain.h"

#include "SoManager.h"

//////////////////////////////////////////////////
// Music!
GaMusicLine gMusicA[] =
{
	{ 1, 0, 0, 60, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 2, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
};

GaMusicLine gMusicAInto[] =
{
	{ 0, 0, 0, 60, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 2, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
};

GaMusicLine gMusicB[] =
{
	{ 1, 36,  0,  60, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 48,  0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 2, 36,  0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 48,  0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
};

GaMusicLine gMusicC1[] =
{
	{ 1, 36,  91,  60, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 48,  89,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 2, 36,  88,  0, 0 },
	{ 0, 0,   88,  0, 0 },
	{ 1, 48,  88,  0, 0 },
	{ 0, 0,   0,  0, 0 },
};

GaMusicLine gMusicC2[] =
{
	{ 1, 41,  96,  65, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 53,  94,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 2, 41,  93,  0, 0 },
	{ 0, 0,   93,  0, 0 },
	{ 1, 53,  93,  0, 0 },
	{ 0, 0,   0,  0, 0 },
};

GaMusicLine gMusicC3[] =
{
	{ 1, 36,  91,  60, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 48,  84,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 2, 36,  88,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 48,  0,  0, 0 },
	{ 0, 0,   89,  0, 0 },
};

GaMusicLine gMusicC4[] =
{
	{ 1, 41,  96,  65, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 53,  89,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 2, 41,  93,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 53,  0,  0, 0 },
	{ 0, 0,   94,  0, 0 },
};

GaMusicLine gMusicBreak1[] =
{
	{ 1, 0,  89,  0, 65 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
};

GaMusicLine gMusicBreak2[] =
{
	{ 1, 0,  87,  0, 63 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 0,  0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,  0,  0, 0 },
};
GaMusicLine gMusicBreak3[] =
{
	{ 1, 0,  86,  62, 62 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
};

GaMusicLine gMusicBreak4[] =
{
	{ 1, 48,  84,  60, 60 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 50,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 1, 51,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
	{ 0, 53,   0,  0, 0 },
	{ 0, 0,   0,  0, 0 },
};

GaMusicLine gMusicSilence[] =
{
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
};

GaMusicLine gMusicWin1[] =
{
	{ 1, 60, 60 ,0 ,72 },
	{ 0, 64, 64 ,0 ,76 },
	{ 0, 66, 66 ,0 ,78 },
	{ 0, 67, 67 ,0 ,79 },
	{ 0, 66, 66 ,0 ,78 },
	{ 0, 64, 64 ,0 ,76 },
	{ 1, 60, 60 ,0 ,72 },
	{ 0, 0, 0 ,0 ,0 },
};

GaMusicLine gMusicWin2[] =
{
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 0, 0, 0 ,0 ,0 },
	{ 4, 0, 0 ,0 ,0 },
};

//////////////////////////////////////////////////
// Ctor
GaMusic::GaMusic()
{

}

//////////////////////////////////////////////////
// Dtor
GaMusic::~GaMusic()
{

}

//////////////////////////////////////////////////
// initialise
void GaMusic::initialise()
{
	CurrentNote_ = 0;
	CurrentTrack_ = 0;

	pTracks_[ 0 ] = &gMusicA[0];
	pTracks_[ 1 ] = &gMusicAInto[0];
	pTracks_[ 2 ] = &gMusicA[0];
	pTracks_[ 3 ] = &gMusicAInto[0];

	pTracks_[ 4 ] = &gMusicB[0];
	pTracks_[ 5 ] = &gMusicB[0];
	pTracks_[ 6 ] = &gMusicB[0];
	pTracks_[ 7 ] = &gMusicB[0];

	pTracks_[ 8 ] = &gMusicC1[0];
	pTracks_[ 9 ] = &gMusicC1[0];
	pTracks_[ 10 ] = &gMusicC1[0];
	pTracks_[ 11 ] = &gMusicC1[0];

	pTracks_[ 12 ] = &gMusicC2[0];
	pTracks_[ 13 ] = &gMusicC2[0];
	pTracks_[ 14 ] = &gMusicC2[0];
	pTracks_[ 15 ] = &gMusicC2[0];

	pTracks_[ 16 ] = &gMusicC3[0];
	pTracks_[ 17 ] = &gMusicC3[0];
	pTracks_[ 18 ] = &gMusicC1[0];
	pTracks_[ 19 ] = &gMusicC1[0];

	pTracks_[ 20 ] = &gMusicC4[0];
	pTracks_[ 21 ] = &gMusicC4[0];
	pTracks_[ 22 ] = &gMusicC2[0];
	pTracks_[ 23 ] = &gMusicC2[0];

	pTracks_[ 24 ] = &gMusicBreak1[0];
	pTracks_[ 25 ] = &gMusicBreak2[0];
	pTracks_[ 26 ] = &gMusicBreak3[0];
	pTracks_[ 27 ] = &gMusicBreak4[0];

	pTracks_[ 28 ] = &gMusicC3[0];
	pTracks_[ 29 ] = &gMusicC1[0];
	pTracks_[ 30 ] = &gMusicC3[0];
	pTracks_[ 31 ] = &gMusicC1[0];

	pTracks_[ 32 ] = &gMusicC4[0];
	pTracks_[ 33 ] = &gMusicC4[0];
	pTracks_[ 34 ] = &gMusicC2[0];
	pTracks_[ 35 ] = &gMusicC2[0];
	
	pTracks_[ 36 ] = &gMusicBreak1[0];
	pTracks_[ 37 ] = &gMusicBreak2[0];
	pTracks_[ 38 ] = &gMusicBreak3[0];
	pTracks_[ 39 ] = &gMusicBreak4[0];

	pTracks_[ 40 ] = &gMusicB[0];
	pTracks_[ 41 ] = &gMusicB[0];
	pTracks_[ 42 ] = &gMusicB[0];
	pTracks_[ 43 ] = &gMusicB[0];

	pTracks_[ 44 ] = &gMusicB[0];
	pTracks_[ 45 ] = &gMusicB[0];
	pTracks_[ 46 ] = &gMusicB[0];
	pTracks_[ 47 ] = &gMusicB[0];

	pTracks_[ 48 ] = &gMusicSilence[0];
	pTracks_[ 49 ] = &gMusicSilence[0];

	pTracks_[ 50 ] = &gMusicWin1[0];
	pTracks_[ 51 ] = &gMusicWin2[0];

	pTracks_[ 52 ] = &gMusicB[0];
	pTracks_[ 53 ] = &gMusicB[0];


	StartTrack_ = 52;
	EndTrack_ = StartTrack_ + 2;

	NoteTick_ = 0.0f;
	CurrentNote_ = 0;
	CurrentTrack_ = StartTrack_;
}

//////////////////////////////////////////////////
// play
void GaMusic::play( BcU32 Start, BcU32 End )
{
	if( StartTrack_ != Start )
	{
		StartTrack_ = Start;
		EndTrack_ = End;

		//StartTrack_ = 50;
		//EndTrack_ = StartTrack_ + 2;

		NoteTick_ = 0.0f;
		CurrentNote_ = 0;
		CurrentTrack_ = StartTrack_;
	}
}

//////////////////////////////////////////////////
// update
void GaMusic::update()
{
	if( NoteTick_ > 0.10f )
	{
		NoteTick_ = 0.0f;

		playLine();
	}

	NoteTick_ += BcMain::pInstance()->tick();
}

//////////////////////////////////////////////////
// shutdown
void GaMusic::shutdown()
{

}

//////////////////////////////////////////////////
// playLine
void GaMusic::playLine()
{
	GaMusicLine* pMusicLine = &pTracks_[ CurrentTrack_ ][ CurrentNote_ ];//&gMusicC[ CurrentNote_ ];

	if( pMusicLine->Percussion_ != 0 )
	{
		switch( pMusicLine->Percussion_ )
		{
		case 1:	// Bass drum
			SoManager::pInstance()->pSynth()->play( soSYNTH_SINE, 100, 30, 0.5f, 10, 50, 100, 100 );
			break;

		case 2:	// Snare
			SoManager::pInstance()->pSynth()->play( soSYNTH_NOISE, 50, 30, 0.3f, 10, 50, 1000, 5000 );
			break;

		case 3:	// Cymbal + bass drum
			SoManager::pInstance()->pSynth()->play( soSYNTH_NOISE, 50, 30, 0.4f, 10, 50, 3000, 6000 );
			SoManager::pInstance()->pSynth()->play( soSYNTH_SINE, 50, 30, 0.3f, 10, 50, 100, 20 );
			break;
		}
	}

	// Bass
	if( pMusicLine->Track1_ != 0 )
	{
		BcReal Freq = SO_MIDIFREQ[ pMusicLine->Track1_ ];

		SoManager::pInstance()->pSynth()->play( soSYNTH_PULSE, Freq, Freq, 0.3f, 50, 200, 1000, 500 );

	}

	// Synth
	if( pMusicLine->Track2_ != 0 )
	{
		BcReal Freq = SO_MIDIFREQ[ pMusicLine->Track2_ ];

		SoManager::pInstance()->pSynth()->play( soSYNTH_PULSE, Freq, Freq, 0.5f, 50, 200, 100, 200 );
	}

	// Pad
	if( pMusicLine->Track3_ != 0 )
	{
		BcReal Freq = SO_MIDIFREQ[ pMusicLine->Track3_ ];

		SoManager::pInstance()->pSynth()->play( soSYNTH_SINE, Freq, Freq, 0.2f, 12000, 200, 1000, 5000 );
	}

	// Additional
	if( pMusicLine->Track4_ != 0 )
	{
		BcReal Freq = SO_MIDIFREQ[ pMusicLine->Track4_ ];

		SoManager::pInstance()->pSynth()->play( soSYNTH_PULSE, Freq, Freq, 0.15f, 50, 200, 1000, 200 );
	}

	CurrentNote_ = ( CurrentNote_ + 1 ) % 8;

	if ( pMusicLine->Percussion_ != 4 )
	{
		if( CurrentNote_ == 0 )
		{
			CurrentTrack_ = CurrentTrack_ + 1;

			if( CurrentTrack_ == EndTrack_ )
			{
				CurrentTrack_ = StartTrack_;
			}
		}
	}
	else
	{
		CurrentTrack_ = 48;
		StartTrack_ = 48;
		EndTrack_ = 49;
	}
}
