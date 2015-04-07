#pragma once

#include <windows.h>
#include "vector2d.h"

enum BlockType{SOLID=0, WATER=1, FOLIAGE=2, OTHER=3};

const int MAXTEAMS = 2;			// Max number of teams in game
const int MAXCHARACTERS = 6;
const int MAXPACKS = 5;			// Max number of ammo and health packs (of each type, so double this)
const int FOLIAGE_FACTOR = 5;	// Effect of foliage on shooting distance

class Block
{
public:
	RECT mArea;			// Area the block occupies. Note that .left and .top are inside the block, but .right and .bottom are not
	BlockType mType;	// The type of block, affecting movement and cover

	void Set(const RECT& position, BlockType type);
};

struct Situation
{
	int NumberOfTeams;
	int PlayersPerTeam;
	int PointsPerKill;
	int PointsPerFlag;
	int CharacterRespawnTime;
	int HealthPackRespawnTime;
	int AmmoPackRespawnTime;
	int TimeLimit;
	int ScoreLimit;
	double AITimeLimit;
	int DamagePerShot;
};

struct MapData
{
	static const int MAXBLOCKS = 50;
	Situation mTheSituation;			// Rules of the game
	Block mrgRectangles[MAXBLOCKS];		// Array of blocks
	int miNumBlocks;					// The number of blocks

	Vector2D mrgTeamPoints[MAXTEAMS];	// List of team points
	int miNumTeamPoints;

	Vector2D mrgFlagPoints[MAXTEAMS];	// List of flags points
	int miNumFlags;

	Vector2D mrgHealthPacks[MAXPACKS];	// Array of health pack points
	int miNumHealthPacks;				// The number of health pack points in the game

	Vector2D mrgAmmoPacks[MAXPACKS];	// Array of ammo pack points
	int miNumAmmoPacks;					// The number of ammo pack points in the game
	MapData();

	// Returns the effective shooting distance between two points (pythagorean) on the map
	// (foliage greatly increases the effective shooting distance)
	// Returns a negative number if there is no line of sight
	double EffectiveShootDistance(const Vector2D& start, const Vector2D& end) const;

	// Returns the distance to walk in a straight line between the two points,
	// adding a 3x penalty for walking through water.
	// If there is no line of walk between the two points, returns a negative number
	double EffectiveWalkDistance(const Vector2D& start, const Vector2D& end) const;

	// Returns true if there is a line of sight between the two points
	// False otherwise
	// (You can shoot through foliage and over water)
	bool LineOfSight(const Vector2D& start, const Vector2D& end) const;

	// Returns true if it is possible to walk in a straight line between the two points
	// False otherwise
	// (You can't walk through buildings or foliage)
	// assumes character has a zero size
	// Allows for the radius of the characters (8 units)
	// Might not spot objects smaller than 16 units
	bool LineOfWalk(const Vector2D& start, const Vector2D& end) const;

	// Returns true if the specified location can be walked on (not solid or foliage)
	// returns false otherwise
	bool CanWalk(const Vector2D& point) const;

	// Returns true if all of the specified RECT can be walked on (not solid or foliage)
	// returns false otherwise
	bool CanWalk(const RECT& area) const;

	// If the specified point is inside a solid or foliage block
	// returns a unit vector in the direction of the normal to the nearest wall.
	// Otherwise returns (0,0)
	Vector2D CollisionNormal(const Vector2D& point) const;
};


struct Character
{
	int miTeamNo;			// No of the team 0-3 (or more!)
	Vector2D mPosition;		// Position of the character
	Vector2D mDirection;	// Unit vector with the direction of facing
	Vector2D mVelocity;		// Velocity vector of the character
	int miTgtTeamNo;		// Team number of the current target (-ve if none)
	int miTgtNumber;		// Character number of the current target (-ve if none)
	double mdFireTime;		// Time since last fired
	int miHealth;			// Health state (0-100) dead if 0 or less
	int miAmmo;				// Number of shots remaining in weapon
	bool hasGrenade;		// True if this character has a grenade
	double mdSpawnTime;		// Time until respawn if dead
	double mdAccuracy;		// Probability of hitting with the current aimed shot 0.0-1.0 (0.0 if not aiming)
};

struct Team
{

	Character mrgMembers[MAXCHARACTERS];	// Pointer to the array of team members
	int miNumCharacters;					// Number of team members
	int miScore;							// Current score for this game

	Team();
};

struct HealthPack
{
	Vector2D mPosition;		// Position of the health pack
	double mdSpawnTime;		// Time until respawn (0 if ready)
};

struct AmmoPack
{
	Vector2D mPosition;
	double mdSpawnTime;		// Time until respawn (0 if ready)
};

struct Flag
{
	Vector2D mPosition;		// Current position
	int miTeamNo;			// Team member of carrying character (-ve if on floor)
	int miCharacterNo;		// Character number of carrying character (-ve if on floor)
};

struct GameState
{
	int miNumTeams;					// Number of teams (and also flags)
	Flag mrgFlags[MAXTEAMS];		// Array of flags
	Team mrgTeams[MAXTEAMS];		// Array of teams

	int miNumAmmoPacks;						// Number of ammo packs
	AmmoPack mrgAmmoPacks[MAXPACKS];		// Pointer to array of ammo packs

	int miNumHealthPacks;						// Number of health packs
	HealthPack mrgHealthPacks[MAXPACKS];		// Pointer to array of health packs
 
	double mdFrameTime;						// Time in seconds of the previous frame
	double mdGameTime;						// Time since the start of the game

	GameState();
};