#ifndef __GRAVITUX_HH__
# define __GRAVITUX_HH__


// Default velocity modifier (attacking)
# define DEFAULT_ATTACK_VMOD 0.0025f
// Default velocity modifier (friendly)
# define DEFAULT_FRIEND_VMOD 0.005f
// Target velocity modifier
# define TARGET_VMOD 0.005f
// Magnet radius
# define MAG_RADIUS 0.1f
// Magnet size
# define MAG_SIZE (MAG_RADIUS*2.3f)
// Radius to keep friends at
# define HAPPY_RADIUS 0.25f
// Max attraction radius
# define MAX_ATTRACT_RADIUS 1.0f
// Air resistance value
# define AIR_RES 0.98f
// Damping value for walls
# define WALL_DAMPING 0.9f
// Collision damping value
# define COLLISION_DAMPING 0.9f
// Max speed
# define MAX_SPEED 0.2f
// Starting HP
# define STARTING_HP 1
// Flee time band
# define FLEE_MIN_TIME 25
# define FLEE_MAX_TIME 75


// AI ---
// Minimum number of mags in play
# define AI_MIN_MAGS 10
// Number to be spawned after min reached
# define AI_LUMP_SPAWN 20
// Number of mags required before attack
# define AI_BATTLE_READY 15


// Grid size
# define GRID_W 30
# define GRID_H 30
# define CELL_SIZE MAX_ATTRACT_RADIUS
# define WALL_LEFT (MAG_RADIUS)
# define WALL_BOTTOM (MAG_RADIUS)
# define WALL_RIGHT ((GRID_W*CELL_SIZE)-MAG_RADIUS)
# define WALL_TOP ((GRID_H*CELL_SIZE)-MAG_RADIUS)


// Base radius
# define BASE_RADIUS 0.8f
// Base size
# define BASE_SIZE (BASE_RADIUS*2.3f)
// Symbol size
# define SYM_SIZE (BASE_SIZE*0.7f)
// Base starting HP
# define BASE_STARTING_HP 500
// Base spawn countdown
# define BASE_DEFAULT_SPAWN_COUNTDOWN 60
// Starting spawnable magnets
# define STARTING_MAGS 5

// Base positions
# define BASE_TUX_POS cVec2(CELL_SIZE*1.5f,CELL_SIZE*1.5f)
# define BASE_WIN_POS cVec2((GRID_W*CELL_SIZE)-(CELL_SIZE*1.5f),(GRID_H*CELL_SIZE)-(CELL_SIZE*1.5f))
# define BASE_MAC_POS cVec2((GRID_W*CELL_SIZE)-(CELL_SIZE*1.5f),CELL_SIZE*1.5f)
# define BASE_SOL_POS cVec2(CELL_SIZE*1.5f,(GRID_H*CELL_SIZE)-(CELL_SIZE*1.5f))


// Scroll speed
//# define SCROLL_CATCH 10
# define CAM_SPEED 0.02f
# define CAM_RES 0.9f

// Maximum number of magnets per team
# define MAX_TEAM_MAGS 250
// Maximum number of magnets
# define MAX_MAGS (MAX_TEAM_MAGS*4)


// Class pre-decs
class cMain;
class cControl;
class cMag;


// Magnet state
enum eMagState
{
	MAG_SEEK=0,
	MAG_FLEE
};


// Magnet teams
enum eMagTeam
{
	TEAM_TUX=0,
	TEAM_WIN,
	TEAM_MAC,
	TEAM_SOL,
	
	NUM_TEAMS
};


// Get camera zoom
float getZoom();


// Get screen modifier
cVec3 getScreenMod();


// Get team colour
inline cFCol getTeamCol(eMagTeam lTeam)
{
	if (lTeam==TEAM_TUX) return cFCol(0.2f,0.2f,0.2f,1.0f);
	if (lTeam==TEAM_WIN) return cFCol(0.2f,0.3f,0.8f,1.0f);
	if (lTeam==TEAM_MAC) return cFCol(0.8f,0.8f,0.8f,1.0f);
	if (lTeam==TEAM_SOL) return cFCol(0.8f,0,0,1.0f);
	return cFCol(1,1,1,1);
}


// Game grid cell
struct sCell
{
	// Magnet array
	cMag* mMags[MAX_MAGS];
	// Magnet cursor
	int mCurr;
	// Ctor
	sCell():mCurr(0){}
	// Add magnet to cell
	void addMag(cMag* lpMag);
	// Remove magnet from cell
	void removeMag(cMag* lpMag);
};


// Target struct
struct sTarget
{
	cVec2 mTarget;
	bool mTargetSet;
};


// Team position struct
struct sTeamPos
{
	cVec2 mPos,mBuff;
	bool mValid;
};


// Target process
class cTarget: public cProcess
{
private:
	float mAng;
	eMagTeam mTeam;
	
public:
	cTarget(eMagTeam lTeam);
	~cTarget();
	
	void updatePos();
	
	void update();
};


// Main game process
class cMain: public cProcess
{
private:
	cControl* mCtrls[NUM_TEAMS];
public:
	cMain();
	~cMain();
	
	// Main process update
	void update();
};


// Controller process
class cControl: public cProcess
{
private:
	cVec2 mCamPos,mCamVel;
	eMagTeam mTeam;
	cTarget* mpTarget;
	bool mAI,mLumpSpawn;
	
public:
	cControl(eMagTeam lTeam,bool lAI);
	~cControl();
	
	// Protect base
	void protectBase();
	
	// Controller process update
	void update();
};


// Magnet base symbol class
class cMagBaseSym: public cProcess
{
private:
public:
	cMagBaseSym(cVec2 lPos,eMagTeam lTeam,float lScale);
	~cMagBaseSym();
	
	void setPos(cVec2 lPos);
	void setVisible(bool lVisible){ mpMesh->setVisible(lVisible);}
};


// Magnet base class
class cMagBase: public cProcess
{
private:
	cMagBaseSym* mpSym;
	cMagBaseSym* mpMarker;
public:
	int mHP,mMaxHP,mSpawnCountDown,mSpawnable;
	cVec2 mPos;
	eMagTeam mTeam;
	bool mSpawnMe;
	
	cMagBase(cVec2 lPos,eMagTeam lTeam);
	~cMagBase();

	// Hitpoint management
	inline void heal(){ if (mHP<mMaxHP) ++mHP;}
	void damage();
	
	bool onScreen(){ return mpMesh->onScreen();}
	
	// Process update
	void update();
};


// Magnet class
class cMag: public cProcess
{
public:
	// Hitpoints
	int mHP,mMaxHP;
	
	// Position and velocity
	cVec2 mPos,mVel;
	// Velocity modifiers
	float mAVMod,mFVMod;
	// Magnet state
	eMagState mState;
	// Magnet team
	eMagTeam mTeam;
	
	// Pointer to cell we exist in
	sCell* mpCell;
	// Cursor location in cell
	int mLoc,mGridX,mGridY;
	
	int mFleeTime;
	
	// Magnet constructor
	cMag(cVec2 lPos,eMagTeam lTeam);
	// Magnet destructor
	~cMag();
	
	// Magnet update function
	void update();
	
	// Hitpoint management
	inline void heal(){ if (mHP<mMaxHP) ++mHP;}
	void damage();
	
	// Perform interactions
	void interact(cMag* lpMag);
	void interact(cMagBase* lpMag);
	inline void interactWith(sCell* lpCell);
};


// Perform interaction with all entities in cell
inline void cMag::interactWith(sCell* lpCell)
{
	int t;
	for(t=0; t<lpCell->mCurr; ++t) interact(lpCell->mMags[t]);
}


#endif
