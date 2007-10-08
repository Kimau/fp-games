#include <math.h>
#include <SDL/SDL.h>
#include "./types.hh"
#include "./drawable.hh"
#include "./base.hh"
#include "./graviTux.hh"
#include "./effects.hh"


// Magnet texture
cMat* gpMagnet=0x0;

// Team symbols
cMat* gpTeamSym[NUM_TEAMS];


// Game grid
sCell gGrid[GRID_W][GRID_H];


// Team targets
sTarget gTarget[NUM_TEAMS];


// Team positions
sTeamPos gTeamPos[NUM_TEAMS];


// Team bases
cMagBase* gpMagBase[NUM_TEAMS];


// Team magnet counts
int gMags[NUM_TEAMS];


// Camera zoom
float gZoom=10;
float getZoom(){ return gZoom;}

// Screen dimention modifier
cVec3 gScreen;
// Get screen modifier
cVec3 getScreenMod()
{
	static bool lSet=false;
	if (!lSet){ getPickVec(gScreen,SCREEN_W,0); lSet=true;}
	return gScreen;
}


// Get grid location from float
inline int getGridLoc(float lPos)
{
	int lLoc=(int)(lPos/CELL_SIZE);
	if (lPos<0.0f) --lLoc;
	return lLoc;
}


// Find target and issue attack
inline void findTarget(eMagTeam lTeam)
{
	if (!gTeamPos[lTeam].mValid) return;
	cVec2 lPosA=gTeamPos[lTeam].mPos,lPosB;
	float lDist,lNearest=999999999.0f;

	gTarget[lTeam].mTargetSet=false;
	
	int t;
	for(t=0; t<NUM_TEAMS; ++t)
	{
		if (t!=lTeam)
		{
			if (gTeamPos[t].mValid) lPosB=gTeamPos[t].mPos;
			else if (gpMagBase[t]) lPosB=gpMagBase[t]->mPos;
			lDist=(lPosA-lPosB).mag();
			// Set target
			if (lDist<lNearest)
			{
				gTarget[lTeam].mTarget=lPosB;
				gTarget[lTeam].mTargetSet=true;
				lNearest=lDist;
			}
		}
	}
}


// Add magnet to cell
void sCell::addMag(cMag* lpMag)
{
	if (lpMag->mLoc>=0) return;
	mMags[mCurr]=lpMag;
	lpMag->mLoc=mCurr;
	lpMag->mpCell=this;
	++mCurr;
}


// Remove magnet from cell
void sCell::removeMag(cMag* lpMag)
{
	--mCurr;
	mMags[mCurr]->mLoc=lpMag->mLoc;
	mMags[lpMag->mLoc]=mMags[mCurr];
	lpMag->mLoc=-1;
	lpMag->mpCell=0x0;
}


// Target process constructor
cTarget::cTarget(eMagTeam lTeam):
	mAng(0.0f),mTeam(lTeam)
{
	cMat* lpTargetMask=loadTexture("./data/target.bmp",ALPHA_FULL,0x0);
	mpMat=loadTexture("./data/flat32.bmp",ALPHA_VALUE,lpTargetMask);
	mpMat->mask();
	mpMesh=makePlane(0.5f,0.5f);
	mpMesh->setCol(getTeamCol(lTeam));
	mpMesh->setVisible(false);
	delete lpTargetMask;
}


// Target process destructor
cTarget::~cTarget()
{
	delete mpMesh;
	delete mpMat;
}


// Update target pos
void cTarget::updatePos()
{
	mpMesh->setPos(gTarget[mTeam].mTarget.X,gTarget[mTeam].mTarget.Y,0);
	mpMesh->setVisible(true);
}


// Target update
void cTarget::update()
{
	if (msBtn(1))
	{
		// Get pick vector
		cVec3 lPos;
		getPickVec(lPos,mX(),mY());
		lPos=lPos*camGetDist();
		gTarget[mTeam].mTarget=cVec2(lPos.X+camGetX(),lPos.Y+camGetY());
		gTarget[mTeam].mTargetSet=true;
		updatePos();
	}
	// Rotate slowly
	mAng+=0.05f;
	if (mAng>PI*2.0f) mAng=mAng-(PI*2.0f);
	mpMesh->zRotate(mAng);
}


// Main process constructor
cMain::cMain()
{
	// Get screen mod
	getScreenMod();
	// Init stuff
	int t;
	for(t=0; t<NUM_TEAMS; ++t)
	{
		// Init target array
		gTarget[t].mTargetSet=false;
		// Init magnet count array
		gMags[t]=0;
		// Init base pointers
		gpMagBase[t]=0x0;
		// Init controls
		mCtrls[t]=0x0;
	}
	// Load magnet texture
	cMat* lpTmp;
	lpTmp=loadTexture("./data/magMask.bmp",ALPHA_FULL,0x0);
	gpMagnet=loadTexture("./data/mag.bmp",ALPHA_VALUE,lpTmp);
	gpMagnet->mask();
	delete lpTmp;
	// Load team symbols
	lpTmp=loadTexture("./data/tuxMask.bmp",ALPHA_FULL,0x0);
	gpTeamSym[TEAM_TUX]=loadTexture("./data/tux.bmp",ALPHA_VALUE,lpTmp);
	gpTeamSym[TEAM_TUX]->mask();
	delete lpTmp;
	lpTmp=loadTexture("./data/winblowsMask.bmp",ALPHA_FULL,0x0);
	gpTeamSym[TEAM_WIN]=loadTexture("./data/winblows.bmp",ALPHA_VALUE,lpTmp);
	gpTeamSym[TEAM_WIN]->mask();
	delete lpTmp;
	lpTmp=loadTexture("./data/appleMask.bmp",ALPHA_FULL,0x0);
	gpTeamSym[TEAM_MAC]=loadTexture("./data/apple.bmp",ALPHA_VALUE,lpTmp);
	gpTeamSym[TEAM_MAC]->mask();
	delete lpTmp;
	lpTmp=loadTexture("./data/solarisMask.bmp",ALPHA_FULL,0x0);
	gpTeamSym[TEAM_SOL]=loadTexture("./data/solaris.bmp",ALPHA_VALUE,lpTmp);
	gpTeamSym[TEAM_SOL]->mask();
	delete lpTmp;
	// Make bases
	new cMagBase(BASE_TUX_POS,TEAM_TUX);
	new cMagBase(BASE_WIN_POS,TEAM_WIN);
	new cMagBase(BASE_MAC_POS,TEAM_MAC);
	new cMagBase(BASE_SOL_POS,TEAM_SOL);
	// Make controllers
	mCtrls[TEAM_TUX]=new cControl(TEAM_TUX,false);
	mCtrls[TEAM_WIN]=new cControl(TEAM_WIN,true);
	mCtrls[TEAM_MAC]=new cControl(TEAM_MAC,true);
	mCtrls[TEAM_SOL]=new cControl(TEAM_SOL,true);
}


// Main process destructor
cMain::~cMain()
{
	// Delete magnet mask
	delete gpMagnet;
	// Delete team symbols
	delete gpTeamSym[TEAM_TUX];
	delete gpTeamSym[TEAM_WIN];
	delete gpTeamSym[TEAM_MAC];
	delete gpTeamSym[TEAM_SOL];
	// Delete bases
	if (gpMagBase[TEAM_TUX]) gpMagBase[TEAM_TUX]->kill();
	if (gpMagBase[TEAM_WIN]) gpMagBase[TEAM_WIN]->kill();
	if (gpMagBase[TEAM_MAC]) gpMagBase[TEAM_MAC]->kill();
	if (gpMagBase[TEAM_SOL]) gpMagBase[TEAM_SOL]->kill();
	// Delete controllers
	if (mCtrls[TEAM_TUX]) mCtrls[TEAM_TUX]->kill();
	if (mCtrls[TEAM_WIN]) mCtrls[TEAM_WIN]->kill();
	if (mCtrls[TEAM_MAC]) mCtrls[TEAM_MAC]->kill();
	if (mCtrls[TEAM_SOL]) mCtrls[TEAM_SOL]->kill();
}


// Main process update
void cMain::update()
{
	// Set team positions
	int t;
	for(t=0; t<NUM_TEAMS; ++t)
	{
		if (gMags[t]>0)
		{
			gTeamPos[t].mPos=gTeamPos[t].mBuff/gMags[t];
			gTeamPos[t].mBuff=cVec2(0,0);
			gTeamPos[t].mValid=true;
		}
		else gTeamPos[t].mValid=false;
	}
	
/*	gTarget[TEAM_WIN].mTarget=gTeamPos[TEAM_TUX].mPos;
	gTarget[TEAM_WIN].mTargetSet=gTeamPos[TEAM_TUX].mValid&&key(SDLK_SPACE);
	
	if (key(SDLK_1)&&gpMagBase[TEAM_TUX]) gpMagBase[TEAM_TUX]->mSpawnMe=true;
	if (key(SDLK_2)&&gpMagBase[TEAM_WIN]) gpMagBase[TEAM_WIN]->mSpawnMe=true;
	if (key(SDLK_3)&&gpMagBase[TEAM_MAC]) gpMagBase[TEAM_MAC]->mSpawnMe=true;
	if (key(SDLK_4)&&gpMagBase[TEAM_SOL]) gpMagBase[TEAM_SOL]->mSpawnMe=true;*/
	
	// Check escape key
	if (key(SDLK_ESCAPE)) stop();
}


// Controller process constructor
cControl::cControl(eMagTeam lTeam,bool lAI):
	mTeam(lTeam),mpTarget(0x0),mAI(lAI),mLumpSpawn(false)
{
	if (!mAI) mpTarget=new cTarget(mTeam);
}


// Controller process destructor
cControl::~cControl()
{
	if (!mAI) mpTarget->kill();
}


// Protect base
void cControl::protectBase()
{
	gTarget[mTeam].mTarget=gpMagBase[mTeam]->mPos;
	gTarget[mTeam].mTargetSet=true;
}


// Controller process update
void cControl::update()
{
	if (mAI) // Computer controlled
	{
		// Base controls
		if (gpMagBase[mTeam])
		{
			if (gMags[mTeam]<AI_MIN_MAGS) mLumpSpawn=true;
			if (mLumpSpawn&&(gMags[mTeam]<AI_LUMP_SPAWN)&&(gpMagBase[mTeam]>0))
				{ protectBase(); gpMagBase[mTeam]->mSpawnMe=true;}
			else mLumpSpawn=false;
		}
		
		// Attack
		if ((!gpMagBase[mTeam])||(gMags[mTeam]>=AI_BATTLE_READY))
		{
			mLumpSpawn=false;
			findTarget(mTeam);
		}
	}
	else // Player controlled
	{
		// Scroll camera
		if (gTeamPos[mTeam].mValid) mCamVel=mCamVel+((gTeamPos[mTeam].mPos-mCamPos)*CAM_SPEED);
		// Smooth camera velocity and integrate
		mCamVel.X=mCamVel.X*CAM_RES; mCamVel.Y=mCamVel.Y*CAM_RES;
		mCamPos=mCamPos+mCamVel;
		// Catch map edges
		if (mCamPos.X<gScreen.X*gZoom) mCamPos.X=gScreen.X*gZoom;
		if (mCamPos.X>(GRID_W*CELL_SIZE)-(gScreen.X*gZoom)) mCamPos.X=(GRID_W*CELL_SIZE)-(gScreen.X*gZoom);
		if (mCamPos.Y<gScreen.Y*gZoom) mCamPos.Y=gScreen.Y*gZoom;
		if (mCamPos.Y>(GRID_H*CELL_SIZE)-(gScreen.Y*gZoom)) mCamPos.Y=(GRID_H*CELL_SIZE)-(gScreen.Y*gZoom);
		// Position camera
		camSetPos(mCamPos.X,mCamPos.Y,gZoom);
		
		// Base controls
		if (gpMagBase[mTeam])
		{
			// Protect base
			if (key(SDLK_SPACE)){ protectBase(); mpTarget->updatePos();}
			// Spawn magnets
			if (msBtn(3)) gpMagBase[mTeam]->mSpawnMe=true;
		}
	}
}


// Magnet base symbol class constructor
cMagBaseSym::cMagBaseSym(cVec2 lPos,eMagTeam lTeam,float lScale)
{
	mpMat=gpTeamSym[lTeam];
	mpMesh=makePlane(SYM_SIZE*lScale,SYM_SIZE*lScale);
	mpMesh->setPos(cVec3(lPos.X,lPos.Y,0));
}


// Magnet base symbol class destructor
cMagBaseSym::~cMagBaseSym()
{
	delete mpMesh;
}


// Magnet base symbol position
void cMagBaseSym::setPos(cVec2 lPos)
{
	mpMesh->setPos(lPos.X,lPos.Y,0.1f);
}


// Magnet base class constructor
cMagBase::cMagBase(cVec2 lPos,eMagTeam lTeam):
	mHP(BASE_STARTING_HP),mMaxHP(BASE_STARTING_HP),mSpawnCountDown(BASE_DEFAULT_SPAWN_COUNTDOWN),
	mSpawnable(STARTING_MAGS),mPos(lPos),mTeam(lTeam),mSpawnMe(false)
{
	mpMat=gpMagnet;
	mpMesh=makePlane(BASE_SIZE,BASE_SIZE);
	mpMesh->setPos(cVec3(lPos.X,lPos.Y,0));

	mpSym=new cMagBaseSym(lPos,lTeam,1.0f);
	mpMarker=new cMagBaseSym(lPos,lTeam,0.25f);

	gpMagBase[lTeam]=this;
}


// Magnet base class destructor
cMagBase::~cMagBase()
{
	gpMagBase[mTeam]=0x0;
	// Delete mesh
	delete mpMesh;
	// Delete symbol and marker
	mpSym->kill();
	mpMarker->kill();
}


// Magnet base damage
void cMagBase::damage()
{
	if (mHP>0) --mHP; else{ baseExplode(mPos,mTeam); mKill=true;}
}


// Magnet base update function
void cMagBase::update()
{
	static int lTick=mSpawnCountDown;
	if (lTick>0) --lTick;
	else
	{
		// Add magnet ready to spawn
		if (gMags[mTeam]+mSpawnable<MAX_TEAM_MAGS) ++mSpawnable;
		lTick=mSpawnCountDown;
	}
	// Spawn request
	if (mSpawnMe)
	{
		if (mSpawnable>0)
		{
			// Spawn new magnet
			float lAng=rnd(0,PI*200)*0.01f;
			new cMag(cVec2(mPos.X+(sinf(lAng)*(BASE_RADIUS+MAG_RADIUS)),
				mPos.Y+(cosf(lAng)*(BASE_RADIUS+MAG_RADIUS))),mTeam);
			--mSpawnable;
		}
		mSpawnMe=false;
	}
	// Show marker
	if (gpMagBase[mTeam])
	{
		if (gpMagBase[mTeam]->onScreen()) mpMarker->setVisible(false);
		else
		{
			mpMarker->setVisible(true);
			cVec2 lDir=cVec2(gpMagBase[mTeam]->mPos.X-camGetX(),gpMagBase[mTeam]->mPos.Y-camGetY());
			float lDist=lDir.mag();
			lDir=lDir/lDist;
			mpMarker->setPos(cVec2(camGetX()+(lDir.X*3.5f),camGetY()+(lDir.Y*3.5f)));
		}
	}
	else mpMarker->setVisible(false);
}


// Magnet constructor
cMag::cMag(cVec2 lPos,eMagTeam lTeam):
	mHP(STARTING_HP),mMaxHP(STARTING_HP),mPos(lPos),
	mAVMod(DEFAULT_ATTACK_VMOD),mFVMod(DEFAULT_FRIEND_VMOD),
	mState(MAG_SEEK),mTeam(lTeam),
	mpCell(0x0),mLoc(-1),mGridX(0),mGridY(0)
{
	mpMat=gpMagnet;
	mpMesh=makePlane(MAG_SIZE,MAG_SIZE);
	mpMesh->setCol(getTeamCol(lTeam));
	++gMags[mTeam];
}


// Magnet destructor
cMag::~cMag()
{
	if (mpCell) mpCell->removeMag(this);
	delete mpMesh;
	--gMags[mTeam];
}


// Magnet update
void cMag::update()
{
	if (mState==MAG_FLEE)
	{
		--mFleeTime;
		if (mFleeTime<=0) mState=MAG_SEEK;
	}
	
	// Interact with surrounding magnets
	if ((mGridX>=0)&&(mGridY>=0)&&(mGridX<GRID_W)&&(mGridY<GRID_H))
	{
		// Interact with bases
		if (gpMagBase[TEAM_TUX]) interact(gpMagBase[TEAM_TUX]);
		if (gpMagBase[TEAM_WIN]) interact(gpMagBase[TEAM_WIN]);
		if (gpMagBase[TEAM_MAC]) interact(gpMagBase[TEAM_MAC]);
		if (gpMagBase[TEAM_SOL]) interact(gpMagBase[TEAM_SOL]);
		// Top left cell
		if ((mGridX>0)&&(mGridY<GRID_H-1)) interactWith(&gGrid[mGridX-1][mGridY+1]);
		// Left cell
		if (mGridX>0) interactWith(&gGrid[mGridX-1][mGridY]);
		// Bottom left cell
		if ((mGridX>0)&&(mGridY>0)) interactWith(&gGrid[mGridX-1][mGridY-1]);
		// Bottom cell
		if (mGridY>0) interactWith(&gGrid[mGridX][mGridY-1]);
		// Bottom right cell
		if ((mGridX<GRID_W-1)&&(mGridY>0)) interactWith(&gGrid[mGridX+1][mGridY-1]);
		// Right cell
		if (mGridX<GRID_W-1) interactWith(&gGrid[mGridX+1][mGridY]);
		// Top right cell
		if ((mGridX<GRID_W-1)&&(mGridY<GRID_H-1)) interactWith(&gGrid[mGridX+1][mGridY+1]);
		// Top cell
		if (mGridY<GRID_H-1) interactWith(&gGrid[mGridX][mGridY+1]);
		// This cell
		interactWith(&gGrid[mGridX][mGridY]);
	}
	
	// Move toward target
	if (gTarget[mTeam].mTargetSet)
	{
		cVec2 lDir=gTarget[mTeam].mTarget-mPos;
		mVel=mVel+(lDir*TARGET_VMOD);
	}
	
	// Clamp speed
	if (mVel.X>MAX_SPEED) mVel.X=MAX_SPEED;
	if (mVel.X<-MAX_SPEED) mVel.X=-MAX_SPEED;
	if (mVel.Y>MAX_SPEED) mVel.Y=MAX_SPEED;
	if (mVel.Y<-MAX_SPEED) mVel.Y=-MAX_SPEED;
	
	// Update position
	mPos=mPos+mVel;
	
	// Bounce off walls
	if ((mPos.X<WALL_LEFT)&&(mVel.X<0.0f)){ mVel.X=-mVel.X*WALL_DAMPING; mPos.X=WALL_LEFT;}
	if ((mPos.Y<WALL_BOTTOM)&&(mVel.Y<0.0f)){ mVel.Y=-mVel.Y*WALL_DAMPING; mPos.Y=WALL_BOTTOM;}
	if ((mPos.X>WALL_RIGHT)&&(mVel.X>0.0f)){ mVel.X=-mVel.X*WALL_DAMPING; mPos.X=WALL_RIGHT;}
	if ((mPos.Y>WALL_TOP)&&(mVel.Y>0.0f)){ mVel.Y=-mVel.Y*WALL_DAMPING; mPos.Y=WALL_TOP;}
	
	// Relocate if required
	mGridX=getGridLoc(mPos.X);
	mGridY=getGridLoc(mPos.Y);
	if ((mGridX>=0)&&(mGridY>=0)&&(mGridX<GRID_W)&&(mGridY<GRID_H))
	{
		// Cell hasn't changed
		if (&gGrid[mGridX][mGridY]!=mpCell)
		{
			if (mpCell) mpCell->removeMag(this);
			gGrid[mGridX][mGridY].addMag(this);
		}
	}
	
	// Increment team positon
	gTeamPos[mTeam].mBuff=gTeamPos[mTeam].mBuff+mPos;

	// Set renderable position
	mpMesh->setPos(mPos.X,mPos.Y,0.0f);
	
	// Apply air resistance
	mVel=mVel*AIR_RES;
}


// Magnet damage
void cMag::damage()
{
	if (mHP>0) --mHP; else{ magExplode(mPos,mTeam); mKill=true;}
}


// Perform interactions with another magnet
void cMag::interact(cMag* lpMag)
{
	if (lpMag==this) return;
	cVec2 lDir;
	float lDist;
	// Get distance and normalised direction vector
	lDir=lpMag->mPos-mPos; lDist=lDir.mag();
	if (lDist>MAX_ATTRACT_RADIUS) return;
	lDir.X/=lDist; lDir.Y/=lDist;
	
	// Same team 
	if (lpMag->mTeam==mTeam)
	{
		if (lDist>HAPPY_RADIUS*1.5f) return;
		if (lDist>HAPPY_RADIUS) mVel=mVel+(lDir*(lDist-HAPPY_RADIUS)*mFVMod);
		else mVel=mVel+(lDir*(lDist-HAPPY_RADIUS)*mFVMod);
	}
	// Seeking enemy
	else if (mState==MAG_SEEK) mVel=mVel+((lDir*mAVMod)/lDist);
	// Fleeing enemy
	else if (mState==MAG_FLEE) mVel=mVel-((lDir*mAVMod)/lDist);
	
	// Check collision
	if (lDist<MAG_RADIUS*2.0f)
	{
		// Get relative velocity
		float lRelVel=((mVel-lpMag->mVel)^lDir)*COLLISION_DAMPING;
		if (lRelVel>0.0f)
		{
			// Collision response
			mVel=mVel-(lDir*lRelVel);
			lpMag->mVel=lpMag->mVel+(lDir*lRelVel);
			// Deal damage/take damage
			if (lpMag->mTeam!=mTeam)
			{
				if (abs(mVel^lDir)>abs(lpMag->mVel^lDir)){ heal(); lpMag->damage();}
				else{ damage(); lpMag->heal();}
				// Make this magnet flee
				mState=MAG_FLEE;
				mFleeTime=rnd(FLEE_MIN_TIME,FLEE_MAX_TIME);
				// Make other magnet flee
				lpMag->mState=MAG_FLEE;
				lpMag->mFleeTime=rnd(FLEE_MIN_TIME,FLEE_MAX_TIME);
			}
		}
		// Force penetration correction
		float lP=(lDist-(MAG_RADIUS*2.0f))*0.25f;
		mPos=mPos+(lDir*lP);
		lpMag->mPos=lpMag->mPos-(lDir*lP);
	}
}


// Perform interactions with magnet base
void cMag::interact(cMagBase* lpMag)
{
	cVec2 lDir;
	float lDist;
	// Get distance and normalised direction vector
	lDir=lpMag->mPos-mPos; lDist=lDir.mag();
	if (lDist>MAX_ATTRACT_RADIUS*2.0f) return;
	lDir.X/=lDist; lDir.Y/=lDist;
	if (mTeam!=lpMag->mTeam)
	{
		// Seeking enemy
		if (mState==MAG_SEEK) mVel=mVel+((lDir*mAVMod)/lDist);
		// Fleeing enemy
		else if (mState==MAG_FLEE) mVel=mVel-((lDir*mAVMod)/lDist);
	}
	
	// Check collision
	if (lDist<MAG_RADIUS+BASE_RADIUS)
	{
		// Get relative velocity
		float lRelVel=mVel^lDir;
		if (lRelVel>0.0f)
		{
			// Collision response
			mVel=mVel-(lDir*lRelVel);
			// Deal damage to base and flee
			if (lpMag->mTeam!=mTeam)
				{ lpMag->damage(); mState=MAG_FLEE; mFleeTime=2*rnd(FLEE_MIN_TIME,FLEE_MAX_TIME);}
		}
		// Force penetration correction
		float lP=(MAG_RADIUS+BASE_RADIUS)-lDist;
		mPos=mPos-(lDir*lP);
	}
}


// Entry point
int main(int lC,char** lppA)
{
	// Init stuff
	init();
	
	setClearColour(0.6f,0.6f,0.8f);
	
	// Create main process
	new cMain;
	
	// Run the game
	run();
	
	// Shutdown stuff
	close();

	// Warning dodger
	lC=lC;
	lppA=lppA;
	return 0;
}
