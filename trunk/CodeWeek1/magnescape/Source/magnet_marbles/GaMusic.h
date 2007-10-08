/**************************************************************************
*
* File:		GaMusic.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GAMUSIC_H__
#define __GAMUSIC_H__

#include "BcSingleton.h"
#include "SoManager.h"

//////////////////////////////////////////////////
// Note
struct GaMusicLine
{
	BcU32 Percussion_;
	BcU32 Track1_;
	BcU32 Track2_;
	BcU32 Track3_;
	BcU32 Track4_;
};

//////////////////////////////////////////////////
// GaMusic
class GaMusic:
	public BcSingleton< GaMusic >
{
public:
	GaMusic();
	virtual ~GaMusic();

	void initialise();
	void update();
	void shutdown();

	void play( BcU32 Start, BcU32 End );
	void playLine();

private:
	BcBool bPlaying_;
	BcU32 CurrentNote_;
	BcU32 CurrentTrack_;

	GaMusicLine* pTracks_[ 128 ];

	BcReal NoteTick_;

	BcU32 StartTrack_;
	BcU32 EndTrack_;
};


#endif
