#include <math.h>
#include <stdio.h>
#include "./types.hh"
#include "./drawable.hh"
#include "./base.hh"
#include "./graviTux.hh"
#include "./effects.hh"


// Particle material
cMat* gpPtclMat;

// Shockwave material
cMat* gpShockMat;


// Magnet explosion
void magExplode(cVec2 lPos,eMagTeam lTeam)
{
	cVec3 lEVel;
	int t;
	new cShock(cVec3(lPos.X,lPos.Y,0),lTeam,rnd(SHOCK_MIN_TIME,SHOCK_MAX_TIME));
	for(t=0; t<EXPL_MAG_PTCLS; ++t)
	{
		lEVel=cVec3(rnd(-EXPL_MAG_VEL*100,EXPL_MAG_VEL*100)*0.01f,
			rnd(-EXPL_MAG_VEL*100,EXPL_MAG_VEL*100)*0.01f,
			rnd(-EXPL_MAG_VEL*100,EXPL_MAG_VEL*100)*0.01f);
		new cPtcl(cVec3(lPos.X,lPos.Y,0),lEVel,lTeam);
	}
}


// Base explosion
void baseExplode(cVec2 lPos,eMagTeam lTeam)
{
	cVec3 lEVel;
	int t;
	new cShock(cVec3(lPos.X,lPos.Y,0),lTeam,rnd(SHOCK_MIN_TIME*4,SHOCK_MAX_TIME*4));
	for(t=0; t<EXPL_BASE_PTCLS; ++t)
	{
		lEVel=cVec3(rnd(-EXPL_BASE_VEL*100,EXPL_BASE_VEL*100)*0.01f,
			rnd(-EXPL_BASE_VEL*100,EXPL_BASE_VEL*100)*0.01f,
			rnd(-EXPL_BASE_VEL*100,EXPL_BASE_VEL*100)*0.01f);
		new cPtcl(cVec3(lPos.X,lPos.Y,0),lEVel,lTeam);
	}
}


// Particle constructor
cPtcl::cPtcl(cVec3 lPos,cVec3 lVel,eMagTeam lTeam):
	mPos(lPos),mVel(lVel)
{
	if (!gpPtclMat)
	{
		cMat* lpTmp=loadTexture("./data/particle.bmp",ALPHA_FULL,0x0);
		gpPtclMat=loadTexture("./data/flat32.bmp",ALPHA_VALUE,lpTmp);
		gpPtclMat->mask();
		delete lpTmp;
	}
	mTime=rnd(PTCL_MIN_TIME,PTCL_MAX_TIME);
	mFadeMod=1.0f/mTime;
	mCol=getTeamCol(lTeam);
	mpMesh=makePlane(PTCL_SIZE,PTCL_SIZE);
	mpMat=gpPtclMat;
}


// Particle destructor
cPtcl::~cPtcl()
{
	delete mpMesh;
}


// Particle update
void cPtcl::update()
{
	--mTime;
	if (mTime<0) mKill=true;
	
	// Update velocity and position
	mPos=mPos+mVel;
	mVel=mVel*PTCL_AIR_RES;
	
	// Fade out
	mCol.A=mTime*mFadeMod;
	
	// Update renderable
	mpMesh->setCol(mCol);
	mpMesh->setPos(mPos);
}


// Shockwave constructor
cShock::cShock(cVec3 lPos,eMagTeam lTeam,int lTime):
	mTime(lTime)
{
	if (!gpShockMat)
	{
		cMat* lpTmp=loadTexture("./data/splode.bmp",ALPHA_FULL,0x0);
		gpShockMat=loadTexture("./data/flat64.bmp",ALPHA_VALUE,lpTmp);
		gpShockMat->mask();
		delete lpTmp;
	}
	mFadeMod=1.0f/mTime;
	mCol=getTeamCol(lTeam);
	mSize=SHOCK_SIZE*0.5f;
	mpMesh=makePlane(mSize,mSize);
	mpMat=gpShockMat;
	mpMesh->setPos(lPos);
}


// Shockwave destructor
cShock::~cShock()
{
	delete mpMesh;
}


// Shockwave update
void cShock::update()
{
	--mTime;
	if (mTime<0) mKill=true;
	
	// Fade out
	mCol.A=mTime*mFadeMod;
	
	// Update renderable
	mpMesh->setCol(mCol);
	
	// Scale up
	mSize+=SHOCK_SIZE_INC;
	mpMesh->getVtxs()[0].mPos=cVec3(-mSize,mSize,0);
	mpMesh->getVtxs()[1].mPos=cVec3(-mSize,-mSize,0);
	mpMesh->getVtxs()[2].mPos=cVec3(mSize,-mSize,0);
	mpMesh->getVtxs()[3].mPos=cVec3(-mSize,mSize,0);
	mpMesh->getVtxs()[4].mPos=cVec3(mSize,-mSize,0);
	mpMesh->getVtxs()[5].mPos=cVec3(mSize,mSize,0);
}


// Starfield constructor
cStars::cStars()
{
	mScreen=getScreenMod();
	mpMat=loadTexture("./data/tux.bmp",ALPHA_FULL,0x0);// gpPtclMat;
	mpMesh=new cMesh;
	mpMesh->make(NUM_STARS*6);
	mpMesh->setIndCurr(NUM_STARS*6);
	mpMesh->setLocked(true);
	mpVtxs=mpMesh->getVtxs();
	// Populate random starfield
	int t;
	for(t=0; t<NUM_STARS; ++t)
	{
		mStarPos[t].Z=rnd(-STAR_DEPTH*100,0)*0.01f;
		mZMod=getZoom()-mStarPos[t].Z;
		mStarPos[t].X=rnd((int)(-100*mScreen.X*mZMod),(int)(100*mScreen.X*mZMod))*0.01f;
		mStarPos[t].Y=rnd((int)(-100*mScreen.Y*mZMod),(int)(100*mScreen.Y*mZMod))*0.01f;
		mpVtxs[(t*6)+0].mUV=cVec2(0,1);
		mpVtxs[(t*6)+1].mUV=cVec2(0,0);
		mpVtxs[(t*6)+2].mUV=cVec2(1,0);
		mpVtxs[(t*6)+3].mUV=cVec2(0,1);
		mpVtxs[(t*6)+4].mUV=cVec2(1,0);
		mpVtxs[(t*6)+5].mUV=cVec2(1,1);
	}
}


// Starfield destructor
cStars::~cStars()
{
	delete mpMesh;
}


void cStars::update()
{
	mCamVel=cVec2(camGetX()-mCamPos.X,camGetY()-mCamPos.Y);
	mCamPos=cVec2(camGetX(),camGetY());
	
	mpMesh->setPos(mCamPos.X,mCamPos.Y,-10);
	if (!mpMesh->onScreen()) printf("mesh not on screen! Why?!?!\n");
	int t;
	for(t=0; t<NUM_STARS; ++t)
	{
		// Update position
//		mStarPos[t].X-=mCamVel.X;
//		mStarPos[t].Y-=mCamVel.Y;
		// Get zoom mod
		mZMod=getZoom()-mStarPos[t].Z;
		// Check against edges
/*		if (mStarPos[t].X<-mScreen.X*mZMod)
		{
			mStarPos[t].X=mScreen.X*mZMod;
			mStarPos[t].Y=rnd((int)(-100*mScreen.Y*mZMod),(int)(100*mScreen.Y*mZMod))*0.01f;
		}
		if (mStarPos[t].X>mScreen.X*mZMod)
		{
			mStarPos[t].X=-mScreen.X*mZMod;
			mStarPos[t].Y=rnd((int)(-100*mScreen.Y*mZMod),(int)(100*mScreen.Y*mZMod))*0.01f;
		}
		if (mStarPos[t].Y<-mScreen.Y*mZMod)
		{
			mStarPos[t].Y=mScreen.Y*mZMod;
			mStarPos[t].X=rnd((int)(-100*mScreen.X*mZMod),(int)(100*mScreen.X*mZMod))*0.01f;
		}
		if (mStarPos[t].Y>mScreen.Y*mZMod)
		{
			mStarPos[t].Y=-mScreen.Y*mZMod;
			mStarPos[t].X=rnd((int)(-100*mScreen.X*mZMod),(int)(100*mScreen.X*mZMod))*0.01f;
		}*/
		// Update mesh
//		mpVtxs[(t*6)+0].mPos=cVec3(mStarPos[t].X-STAR_SIZE,mStarPos[t].Y+STAR_SIZE,mStarPos[t].Z);
//		mpVtxs[(t*6)+1].mPos=cVec3(mStarPos[t].X-STAR_SIZE,mStarPos[t].Y-STAR_SIZE,mStarPos[t].Z);
//		mpVtxs[(t*6)+2].mPos=cVec3(mStarPos[t].X+STAR_SIZE,mStarPos[t].Y-STAR_SIZE,mStarPos[t].Z);
//		mpVtxs[(t*6)+3].mPos=cVec3(mStarPos[t].X-STAR_SIZE,mStarPos[t].Y+STAR_SIZE,mStarPos[t].Z);
//		mpVtxs[(t*6)+4].mPos=cVec3(mStarPos[t].X+STAR_SIZE,mStarPos[t].Y-STAR_SIZE,mStarPos[t].Z);
//		mpVtxs[(t*6)+5].mPos=cVec3(mStarPos[t].X+STAR_SIZE,mStarPos[t].Y+STAR_SIZE,mStarPos[t].Z);
	}
}
