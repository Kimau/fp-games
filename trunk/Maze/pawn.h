#ifndef __PAWN_H__
#define __PAWN_H__

#include "pointXYZ.h"
#include "maze.h"
#include "types.h"
#include "misc.h"
#include "controller.h"
#include <memory>
#include <vector>
#include <cmath>
//#pragma once
/*
				Pawn
=============================================================================
Pawn, the base class of all things alive that can be controlled by players or AI.
Pawns are the physical representations of players and creatures in a level.
Pawns have a movement and collision engine.  Pawns can take damage, make sounds,
and hold weapons and other inventory.  In short, they are responsible for all
physical interaction between the player or AI and the world.
=============================================================================
*/


class Pawn
{
public:
	Pawn();
	Pawn(float posX, float posY, float posZ);
	~Pawn();
	
	virtual bool biteHim() = 0; // What to do if you hit the player
	virtual void draw() = 0;	// What do I loook Like
	bool isCenter();

	//acessors
	bool getMovable();
	bool getGhost();
	bool getGodMode();
	bool getPickup();
	bool getWounded();
	bool getAngry();
	bool getCollector();
	bool getBeserk();
	bool getCorrupt();

	int getXi();
	int getYi();

	int  getMaxHealth();
	int  getHealth();
	float getBaseSpeed();
	char* getName();

	int getX();
	int getY();
	int getZ();
	pointXYZ getGridPos();
	Vector<float, 2> getWorldPos();

	bool checkStep(int x_, int y_, int z_, int dir_);
  virtual void anihilateMe(){}

	Pawn* getLastSeen();
	pointXYZ getLastSeenPos();
	float getLastSeenTime();
    
  void setLastSeenPos(pointXYZ pos);

	//booleans
	//reportList reportQ;

	/* setController
	 *
	 * After a controller is assigned to a monster, the monster object manages the controller itself.
	 *  Thus the controller must not be deleted from outside and can only be assigned to one monster
	 */
	void setController(Controller *controller);

  /* getController
   */
  Controller* getController() const;	

  /* update 
   *
   * Calls all the update functions
   */
  void update(Time currentTime);

	//movers	
	bool moveDir(int);
	bool moveTo(pointXYZ);
	float getMoveTime(pointXYZ);  

protected:
	Vector<float, 2> direction;
	Controller* controller;
  
  /* updateController
	 *
	 * Executes the pawn's controller (commonly an AI controller) for this AI frame, using the current time point. 
	 * (NOTE: I refer to the time frame as the AI frame, because theoretically, with a few modifications,
	 * the AI could be run in a sepparate thread and thus have its own "frame rate" which would differ from
	 * the display frame rate)
   *
	 * currentTime - The frame's time. The AI controller uses this to determine how much time has passed etc.
	 */
	void updateController(Time currentTime);  

	//functions
	virtual bool takeStep(Time currentTime);

	//booleans
	bool isMovable;
	bool isGhost;
	bool isGodMode;
	bool isPickup;
	bool isWounded;
	bool isAngry;
	bool isCollector;
	bool isBeserk;
	bool isCorrupt;
	//int
	int maxHealth;
	int health;	
	//string
	char* name;
	//float
	float x;
	float y;
	float z;  
  float baseSpeed; //in seconds
	//others
	Pawn* lastSeen;
	pointXYZ lastSeenPos;
	float lastSeenTime;
  Time  lastUpdateTime;
	static Maze* maze;
};

#endif

