#include "controller.h"
#include "pointXYZ.h"
#include "misc.h"

bool Controller::getNewDir(Pawn *pawn)
{
/*
	I've hit something
	New direction
*/
	if (!(path.isEmpty()))
	{
	  pointXYZ curr_ = pawn->getGridPos();
	  pointXYZ dest_ = path.peak();

	  if (curr_ == dest_)
	  {
		  dest_ = path.pop();
	  }
	  return true;
	}
	execute(pawn);
  if(path.isEmpty())
    return false; //If path is still empty return false (TODO: this shouldn't happen? but it does sometimes?)
	return getNewDir(pawn);
}

/*
	Checks how long the corpse has been sitting there without doing anything
	If Corpse exceeded time it resets its posisition
	If its rotted before then it randomly moves it
*/
//DEL bool Controller::rottedCheck(Pawn *pawn, Time currentTime)
//DEL {
//DEL 	if( (lastPos[0] == pawn->getXi()) && (lastPos[1] == pawn->getYi()) )
//DEL 	{
//DEL 	if((currentTime - rottingTime) > 1)
//DEL 		{
//DEL 			rottingTime = currentTime;
//DEL 			// CORPSE?
//DEL 			if(isRotting)
//DEL 			{
//DEL 				isRotting = false;
//DEL 				if ((path.peak()).getZ() >= 0)
//DEL 				{
//DEL 					pawn->moveTo(path.pop());
//DEL 					return true;
//DEL 				}
//DEL 				// No path to follow so move to 0 pos
//DEL 				pawn->moveTo(pointXYZ(0,0,0));
//DEL 				return true;
//DEL 			}
//DEL 			else
//DEL 			{
//DEL 				pawn->moveTo(pawn->getGridPos());
//DEL 			}
//DEL 			isRotting = true;
//DEL 
//DEL 		}
//DEL 	}
//DEL 	rottingTime = currentTime;
//DEL 	return false;
//DEL }

void Controller::update(Pawn *pawn, Time currentTime) //prototype design pattern??
{
    if(!isInitialized)
      start(pawn, currentTime);
    Controller::currentTime = currentTime;
	/*
		Check if path is empty
		If so call execute function to make a choice
	*/
	if (path.isEmpty())
	{
		execute(pawn);
	}

	/*
		If path exsists then follow it
	*/
	if (!(path.isEmpty()))
	{
	  pointXYZ curr_;
	  pointXYZ dest_;

	  curr_ = pawn->getGridPos();
	  dest_ = path.peak();

	  if (curr_ == dest_)
	  {
		  dest_ = path.pop();
	  }

	  // Get Direction
	  if (curr_.getX() > dest_.getX())
	  {
		  pawn->moveDir(Maze::EAST);				  
	  }
	  if (curr_.getX() < dest_.getX())
	  {
		  pawn->moveDir(Maze::WEST);				  
	  }
	  if (curr_.getY() > dest_.getY())
	  {
		  pawn->moveDir(Maze::NORTH);				  
	  }
	  if (curr_.getY() < dest_.getY())
	  {
		  pawn->moveDir(Maze::SOUTH);				  
	  }
	}

    previousTime = currentTime;
}

