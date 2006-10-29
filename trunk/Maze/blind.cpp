#include "blind.h"
#include "player.h"

/* Blind execute
 
 */
void Blind::execute(Pawn* pawn)
{
	CellStack cellList;
	pointXYZ currentPos = pawn->getGridPos();
	int z = currentPos.getZ();
	for(int c=0; c<10; ++c)
	{
		switch(rand()%4 + 1)
		{
		case 1:
			if (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::EAST))
			{
				cellList.push(currentPos);
				currentPos = pointXYZ(currentPos.getX()+1,currentPos.getY(),z);
				break;
			}
		case 2:
			if (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::WEST))
			{
				cellList.push(currentPos);
				currentPos = pointXYZ(currentPos.getX()-1,currentPos.getY(),z);
				break;
			}
		case 3:
			if (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::SOUTH))
			{
				cellList.push(currentPos);
				currentPos = pointXYZ(currentPos.getX(),currentPos.getY()+1,z);
				break;
			}
		case 4:
			if (pawn->checkStep(currentPos.getX(),currentPos.getY(),z,Maze::NORTH))
			{
				cellList.push(currentPos);
				currentPos = pointXYZ(currentPos.getX(),currentPos.getY()-1,z);
				break;
			}
		}
	}

	path.~CellStack(); //clear the cell stack

	while(!cellList.isEmpty())
	{
	  path.push(cellList.pop());
	}
	return;
}


/* Blind constructor
 */
Blind::Blind()
{
}


void Blind::start(Pawn* pawn, Time startTime)
{
  //initialize with parent start method
  Controller::start(pawn, startTime);
}

