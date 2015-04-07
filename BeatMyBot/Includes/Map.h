#include "world.h"
#include <windows.h>
#include "order.h"
#include "gametimer.h"
#include "viewport.h"
#include "mysoundengine.h"
#include "IBot.h"

#pragma once

extern Viewport gTheViewport;

extern "C"
{
typedef bool (*GETINTERFACE)(IBot ** pIBot);
typedef bool (*FREEINTERFACE)(IBot ** pIBot);
}

struct TeamBrain
{
	
	string DLLFileName;
	HMODULE DLLHandle;

	MapData DummyMapData;

	GETINTERFACE DLLGetInterfacePointer;
	FREEINTERFACE DLLFreeInterfacePointer;

	IBot* pBots[MAXCHARACTERS];
	int iNumBots;

	bool Connected;
	double mdSmoothedEvaluationTime;
	double mdAITimeLimit;

	TeamBrain();
	bool Connect(const MapData& theMapData, int teamNumber, double timeLimit);
	void Disconnect();
	Order Evaluate(GameState& theGameState, int TeamNo, int CharacterNo);
};

class Tracers;
class Blood;

class DebugData
{
public:
	int miDebugCharacter;		// Index of the debug character
	int miDebugTeam;			// Index of the debug team (-1 for none)
	bool mbShowDebugNodes;		// If true, debug nodes will be shown
	string mDebugString;		// String of data from the debug character
	Vector2D mDebugNode;		// Debug node from the debug character
	bool mbLockToCurrentCharacter;	// If true, will lock viewport on the dbug character
	void Draw(const Character& character, TeamBrain& theDebugBrain)const;				// Draws all the debug data
	DebugData();
};

class Map
{
private:


public:
	MapData mMapData;
	GameState mGameState;
	TeamBrain theBrains[MAXTEAMS];
	GameTimer theTimer;
	Tracers* mpTheTracers;
	Blood* mpTheBlood;
	DebugData mTheDebugData;

	Map();

	Map(const Map& other);

	~Map();

	// Draws the blocks, spawns, health packs, flags and ammo packs
	void Draw();

	// Loads the map from the specified filename. 
	// Returns true if the file loaded with no errors. False otherwise
	bool Load(string MapFilename, string SituationFilename);	

	// Saves the map to the specified filename. 
	// Returns true if the file loaded with no errors. False otherwise
	void Save(string filename);	

	// Runs the game 
	void Move();

	// Connects the brains
	void ConnectBrains(LoadConsole& writer);

	// Disonnects the brains
	void DisconnectBrains();

	// Initialised teams, packs, ammo, flags
	// and sets all to start
	void SetStartPositions(LoadConsole& writer);

	bool CollidedWithBlocks(const Vector2D& pos, double radius=0) const;

	bool InWater(const Vector2D& pos) const;
};

// *************************************************
// Tracers *****************************************
// *************************************************
class Tracers
{
	static const int MAXTRACERS = 10;
	static const double MAXTIME;
	Segment2D mrgTheTracers[MAXTRACERS];
	double mrgdTracerTime[MAXTRACERS];
	bool mrgbActive[MAXTRACERS];

public:

	int miNextTracer;

	Tracers();

	void AddTracer(const Vector2D& start, const Vector2D& end, const Map& theMap);

	void DrawTracers(const Map& theMap);
};

// *************************************************
// Blood *****************************************
// *************************************************
class Blood
{
	static const int MAXBLOOD = 50;
	static const double MAXSPEED;
	static const double MAXTIME;
	Vector2D mrgTheDots[MAXBLOOD];
	Vector2D mrgTheVelocities[MAXBLOOD];
	double mrgdBloodTime[MAXBLOOD];
	bool mrgbActive[MAXBLOOD];

public:

	int miNextBlood;

	Blood();

	void AddBlood(const Vector2D& position, int number, const Map& theMap, bool stationary = false);
	void AddBloodSpray(const Vector2D& position, int number, const Map& theMap, Vector2D velocity);

	void DrawBlood(const Map& theMap);
};