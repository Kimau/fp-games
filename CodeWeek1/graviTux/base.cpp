// Include system headers
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <SDL/SDL.h>

// Include game headers
#include "./types.hh"
#include "./drawable.hh"
#include "./base.hh"


// Tick the system and get time mod (gSync)
#define TICK gettimeofday(&gSyncTimeVal,NULL);\
	gSyncCurr=gSyncTimeVal.tv_sec;\
	gSyncCurrf=gSyncTimeVal.tv_usec*0.000001f;\
	gSync=(gSyncCurrf-gSyncLastf)+((float)(gSyncCurr-gSyncLast));\
	gSyncLast=gSyncCurr;\
	gSyncLastf=gSyncCurrf;\


// Synchroniser values
float gSync=0.0f;
timeval gSyncTimeVal;
int gSyncCurr=0,gSyncLast=0;
float gSyncCurrf=0.0f,gSyncLastf=0.0f;


// Buffer event
SDL_Event gEvn;
// Key array
bool gKeys[SDLK_LAST];
// Mouse data
int gMsX,gMsY,gMsZ,gMsVX,gMsVY;
float gMsAcc;
bool gMsBtns[255];


// Render arrays
cProcess* gRender[NUM_RENDER_STATES][RENDER_LIMIT];
int gRCursor[NUM_RENDER_STATES];


// Process list pointers
cProcess* gpFirstProc;
cProcess* gpLastProc;


// Game running
bool gRun;


// Process constructor
cProcess::cProcess():
	mpMesh(0x0),mpMat(0x0),mKill(false)
{
	// Add this process to list
	if (!gpFirstProc) gpFirstProc=this;
	mpPrev=gpLastProc;
	mpNext=0x0;
	if (gpLastProc) gpLastProc->setNext(this);
	gpLastProc=this;
}


// Process destructor
cProcess::~cProcess()
{
	// Remove this process from list
	if (mpPrev) mpPrev->setNext(mpNext);
	else gpFirstProc=mpNext;
	if (mpNext) mpNext->setPrev(mpPrev);
	else gpLastProc=mpPrev;
}


// Process update
void cProcess::update()
{
}


// Process draw
void cProcess::draw()
{
	// Set material states
	mpMat->setGLStates();
	// Draw mesh object
	mpMesh->draw();
}


// Submit object for rendering
void cProcess::submit()
{
	// Process must contain mesh and material, must be on-screen and there must be space for it!
	if (!mpMesh) return;
	if (!mpMat) return;
	if (!mpMesh->onScreen()) return;
	// Submit to locked set
	if (mpMesh->getLocked())
	{
		if (gRCursor[RENDER_LOCKED]>=RENDER_LIMIT) return;
		gRender[RENDER_LOCKED][gRCursor[RENDER_LOCKED]]=this;
		++gRCursor[RENDER_LOCKED];
		return;
	}
	// Submit to blended set
	if (mpMat->getBlended())
	{
		if (gRCursor[RENDER_BLENDED]>=RENDER_LIMIT) return;
		gRender[RENDER_BLENDED][gRCursor[RENDER_BLENDED]]=this;
		++gRCursor[RENDER_BLENDED];
		return;
	}
	// Submit to basic solid set
	if (gRCursor[RENDER_SOLID]>=RENDER_LIMIT) return;
	gRender[RENDER_SOLID][gRCursor[RENDER_SOLID]]=this;
	++gRCursor[RENDER_SOLID];
}


// Kill all processes
void killProcs()
{
	while(gpFirstProc) delete gpFirstProc;
}


// Startup stuff
void init()
{
	SDL_Surface* lpSurf;

	// Init SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,true);
	lpSurf=SDL_SetVideoMode(SCREEN_W,SCREEN_H,32,SDL_OPENGL|SDL_FULLSCREEN);
//	lpSurf=SDL_SetVideoMode(SCREEN_W,SCREEN_H,32,SDL_OPENGL);
	// Catch error
	if (!lpSurf)
		{ printf("Could not start rendering context: %s\n",SDL_GetError()); SDL_Quit(); exit(1);}
	
	// Init OpenGL
	initGL();
	
	// Init input data
	int t;
	gMsX=0; gMsY=0; gMsZ=0; gMsVX=0; gMsVY=0;
	gMsAcc=0.0f;
	for(t=0; t<SDLK_LAST; ++t) gKeys[t]=false;
	for(t=0; t<255; ++t) gMsBtns[t]=false;
	
	// Init render sets
	gRCursor[RENDER_SOLID]=0;
	gRCursor[RENDER_BLENDED]=0;
	gRCursor[RENDER_LOCKED]=0;
	
	// Init other game stuff
	camSetPos(0,0,10.0f);
	setColKey(0,0,0);
	gpFirstProc=0x0;
	gpLastProc=0x0;
	gRun=true;
}
/*	for(lPass=0; lPass<4; ++lPass)
	{
		// Initialise counts
		for(t=0; t<256; ++t) mSortCounts[t]=0;
		// Get counts
		for(t=0; t<lCount; ++t) mSortCounts[255-lpArray[t]->getRadix()[lPass]]++;
		// Build offset table
		mSortOffsets[0]=0;
		for(t=1; t<256; ++t) mSortOffsets[t]=mSortOffsets[t-1]+mSortCounts[t-1];
		// Make sorted array
		for(t=0; t<lCount; ++t) lpSorted[mSortOffsets[255-lpArray[t]->getRadix()[lPass]]++]=lpArray[t];
		for(t=0; t<lCount; ++t) lpArray[t]=lpSorted[t];
	}*/


// Goog ol' radix sort
void sortBlended()
{
	int t,lPass;
	static int lCounts[256],lOffsets[256],lCount;
	static cProcess** lArray;
	static cProcess* lSorted[RENDER_LIMIT];
	lArray=gRender[RENDER_BLENDED];
	lCount=gRCursor[RENDER_BLENDED];
	for(lPass=0; lPass<4; ++lPass)
	{
		for(t=0; t<256; ++t) lCounts[t]=0;
		// Get counts
		for(t=0; t<lCount; ++t) ++lCounts[lArray[t]->getMesh()->getRadix(lPass)];
		// Get offsets
		lOffsets[0]=0;
		for(t=1; t<256; ++t) lOffsets[t]=lOffsets[t-1]+lCounts[t-1];
		// Make sorted array and copy into real array
		for(t=0; t<lCount; ++t) lSorted[lOffsets[lArray[t]->getMesh()->getRadix(lPass)]++]=lArray[t];
		for(t=0; t<lCount; ++t) lArray[t]=lSorted[t];
	}
}


// Run the game
void run()
{
	int t;
	cProcess* lpProc;

	// Tick a few times to ensure first frame is stable
	TICK;
	TICK;
	
	// While game running
	while(gRun)
	{
		// Reset counts
		gRCursor[RENDER_SOLID]=0;
		gRCursor[RENDER_BLENDED]=0;
		gRCursor[RENDER_LOCKED]=0;
		// Tick the system
		TICK;
		// Force 120 FPS
		sleep(16-(gSync*1000));
		// Tick the system once more
		TICK;
		// Get user input
		gatherInputs();
		// Loop through all processes
		lpProc=gpFirstProc;
		while(lpProc)
		{
			if (lpProc->killed()){ cProcess* lpTmp=lpProc->getNext(); delete lpProc; lpProc=lpTmp;}
			else
			{
				// Update and submit to renderer
				lpProc->update();
				lpProc->submit();
				// Get next process
				lpProc=lpProc->getNext();
			}
		}
		// Flush the screen
		flushScreen();
		// Translate the scene
		camTranslate();
		// Render solid objects
		for(t=0; t<gRCursor[RENDER_SOLID]; ++t) gRender[RENDER_SOLID][t]->draw();
		// Sort blended here
		sortBlended();
		// Render blended objects
		for(t=0; t<gRCursor[RENDER_BLENDED]; ++t) gRender[RENDER_BLENDED][t]->draw();
		// Untranslate the scene
		camUntranslate();
		// Render screen locked objects
		for(t=0; t<gRCursor[RENDER_LOCKED]; ++t) gRender[RENDER_LOCKED][t]->draw();
		// Stop if no more processes to run
		if (!gpFirstProc) stop();
	}
}


// Stop game running
void stop()
{
	gRun=false;
}


// Shutdown stuff (hard stop)
void close()
{
	// Kill processes
	killProcs();
	
	// Shutdown SDL
	SDL_Quit();
	
	// Force exit
	exit(0);
}


// Random
int rnd(int lMin,int lMax)
{
	return (rand()%((lMax-lMin)+1))+lMin;
}


// Get user input
void gatherInputs()
{
	// Mouse training
	if (gMsAcc>=0.1f){ gMsVX=0; gMsVY=0; gMsAcc=0;}
	else gMsAcc+=gSync;

	// Check events
	while(SDL_PollEvent(&gEvn))
	{
		// Key board events
		if (gEvn.type==SDL_KEYDOWN) gKeys[gEvn.key.keysym.sym]=true;
		else if (gEvn.type==SDL_KEYUP) gKeys[gEvn.key.keysym.sym]=false;
		// Mouse events
		else if (gEvn.type==SDL_MOUSEBUTTONDOWN)
		{
			gMsBtns[gEvn.button.button]=true;
			// Mousewheel
			if (gMsBtns[4]) ++gMsZ;
			if (gMsBtns[5]) --gMsZ;
		}
		else if (gEvn.type==SDL_MOUSEBUTTONUP)
		{
			gMsBtns[gEvn.button.button]=false;
		}
		else if (gEvn.type==SDL_MOUSEMOTION)
		{
			if ((abs(gEvn.motion.x-gMsX)>0)||(abs(gEvn.motion.y-gMsY)>0))
				{ gMsVX=gEvn.motion.x-gMsX; gMsVY=gEvn.motion.y-gMsY;}
			gMsX=gEvn.motion.x;
			gMsY=gEvn.motion.y;
			gMsAcc=0;
		}
		// Sys events
		else if (gEvn.type==SDL_QUIT) stop();
	}
}


// Keyboard input
bool key(int lKey)
{
	return gKeys[lKey];
}


// Mouse X
int mX()
{
	return gMsX;
}


// Mouse Y
int mY()
{
	return gMsY;
}


// Mouse Z
int mZ()
{
	return gMsZ;
}


// Mouse velocity X
int mVX()
{
	return gMsVX;
}


// Mouse velocity Y
int mVY()
{
	return gMsVY;
}


// Mouse button
bool msBtn(int lBtn)
{
	return gMsBtns[lBtn];
}


// Sleep wrapper function, 'cause sdl functions are ugly
void sleep(int lMS)
{
	if (lMS<0) return;
	SDL_Delay(lMS);
}
