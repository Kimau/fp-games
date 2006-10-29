#include "seeker.h"
#include "misc.h"
#include "cellstack.h"
#include "player.h"

/* Seeker constants
 */
const float Seeker::MINIMUM_DEVIATION_TIME = 0.1f;
const int Seeker::RADIUS = 7;
const int Seeker::GRID_SIZE = RADIUS*RADIUS;

/* Seeker execute
 * 
 */
void Seeker::execute(Pawn* pawn)
{
  /*
    ALGORITHM:

		Loop Until you visit all locations
			OR
		you find footsteps
			OR
		you get back to starting point and have nowhere to go
	*/	
  int visitedCells = 1;
  int z = pawn->getZ();
  pointXYZ currentPos = pawn->getGridPos();
  pointXYZ gridPos((RADIUS/2),(RADIUS/2),z);
  pointXYZ destPos;
  bool grid[GRID_SIZE];
  
  CellStack cellList;
  memset(grid,1,GRID_SIZE*sizeof(bool));
  
  grid[currentPos.getX()+currentPos.getY()*RADIUS] = false;

  while(visitedCells < GRID_SIZE)
  {		

	/*
		Sniff
		1. Check current pos for player footsteps
			Looping through prints
		2. If found prints then retrace steps and remember
	*/
    for(int c = 0; c < 10; c++) //see if one of the player's footsteps crosses the monster's current cellList
	{
		pointXYZ footP = Player::getInstance()->getFootsteps()->getStep(c);
		if(footP == currentPos)
		{
			path.~CellStack(); //clear the cell stack

			while(!cellList.isEmpty())
			{
			  path.push(cellList.pop());
			}
			pawn->setLastSeenPos(currentPos);
			return;
		}
	}
		
    /*
		Search
		1. Set Dest
		2. Check that Dest hasn't been visted
		3. Check that you can move that way
		4. If Checks Succeeded then 
			Push current pos onto list
			Move to new pos
			Mark new pos
	*/
	destPos = pointXYZ(-1,-1,-1);

	//=========		NORTH	========
	int x_ = gridPos.getX();
	int y_ = gridPos.getY()-1;
	if (((x_> 0) && (x_ < RADIUS) && (y_ > 0) && (y_ < RADIUS)) && (grid[x_ + (y_*RADIUS)]) && (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::NORTH)))
		{
			destPos	= pointXYZ(currentPos.getX(),		(currentPos.getY()-1),	z);
			gridPos = pointXYZ(gridPos.getX(),			(gridPos.getY()-1),		z);
			goto moveMe;
		}
	//=========		EAST	========
	x_ = gridPos.getX()+1;
	y_ = gridPos.getY();
    if (((x_> 0) && (x_ < RADIUS) && (y_ > 0) && (y_ < RADIUS)) && (grid[x_ + (y_*RADIUS)]) && (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::EAST)))
		{
			destPos	= pointXYZ((currentPos.getX()+1),	currentPos.getY(),	z);
			gridPos = pointXYZ((gridPos.getX()+1),		gridPos.getY(),		z);
			goto moveMe;
		}
	//=========		WEST	========
	x_ = gridPos.getX()-1;
	y_ = gridPos.getY();
	if (((x_> 0) && (x_ < RADIUS) && (y_ > 0) && (y_ < RADIUS)) && (grid[x_ + (y_*RADIUS)]) && (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::WEST)))
		{
			destPos	= pointXYZ((currentPos.getX()-1),	currentPos.getY(),		z);
			gridPos = pointXYZ((gridPos.getX()-1),		gridPos.getY(),			z);
			goto moveMe;
		}
	//=========		SOUTH	========
	x_ = gridPos.getX();
	y_ = gridPos.getY()+1;
	if (((x_> 0) && (x_ < RADIUS) && (y_ > 0) && (y_ < RADIUS)) && (grid[x_ + (y_*RADIUS)]) && (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::SOUTH)))
		{
			destPos	= pointXYZ((currentPos.getX()),	(currentPos.getY()+1),	z);
			gridPos = pointXYZ(gridPos.getX(),		(gridPos.getY()+1),		z);
			goto moveMe;
		}

	if (false)
	{
		moveMe:
		cellList.push(currentPos);
		currentPos = destPos;
		visitedCells++;
		grid[currentPos.getX()+currentPos.getY()*RADIUS] = false;
		continue;
	}

	/*
		DeadEnd

	*/
		if (cellList.isEmpty())
		{
			break;
		}
		currentPos = cellList.pop();
	} // END WHILE
}


/* Seeker constructor
 */
Seeker::Seeker()
{
}


void Seeker::start(Pawn* pawn, Time startTime)
{
  //initialize with parent start method
  Controller::start(pawn, startTime);
  
  oldTime = startTime;
}

