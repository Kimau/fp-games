#ifndef __EFFECTS_HH__
# define __EFFECTS_HH__


// Particle size
# define PTCL_SIZE 0.1f
// Particle time band
# define PTCL_MIN_TIME 60
# define PTCL_MAX_TIME 120
// Particle air resistance
# define PTCL_AIR_RES 0.975f

// Shockwave time band
# define SHOCK_MIN_TIME 5
# define SHOCK_MAX_TIME 10
// Shockwave size
# define SHOCK_SIZE 0.1f
// Shockwave size inc
# define SHOCK_SIZE_INC 0.4f

// Magnet explosion velocity
# define EXPL_MAG_VEL 0.03f
// Magnet explode particles count
# define EXPL_MAG_PTCLS 10

// Base explosion velocity
# define EXPL_BASE_VEL 0.1f
// Base explode particles count
# define EXPL_BASE_PTCLS 100

// Number of starfield stars
# define NUM_STARS 100
// Starfield depth
# define STAR_DEPTH 0
// Star size
# define STAR_SIZE 10.0f


// Magnet explosion
void magExplode(cVec2 lPos,eMagTeam lTeam);

// Base explosion
void baseExplode(cVec2 lPos,eMagTeam lTeam);


// Particle process
class cPtcl: public cProcess
{
private:
	cVec3 mPos,mVel;
	cFCol mCol;
	int mTime;
	float mFadeMod;
public:
	cPtcl(cVec3 lPos,cVec3 lVel,eMagTeam lTeam);
	~cPtcl();
	
	void update();
};


// Shockwave process
class cShock: public cProcess
{
private:
	cFCol mCol;
	int mTime;
	float mFadeMod,mSize;
public:
	cShock(cVec3 lPos,eMagTeam lTeam,int lTime);
	~cShock();
	
	void update();
};


// Starfield (not working)
class cStars: public cProcess
{
private:
	cVec3 mScreen,mStarPos[NUM_STARS];
	cVec2 mCamPos,mCamVel;
	float mZMod;
	sVertex* mpVtxs;
public:
	cStars();
	~cStars();
	
	void update();
};


#endif
