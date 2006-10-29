#include "loop.h"
#include "misc.h"
#include "cellstack.h"

/* Seeker execute
 * 
 */
void Loop::execute(Pawn* pawn)
{
	pawn->moveDir(Dir);
}

bool Loop::getNewDir(Pawn* pawn)
{
	++Dir;
	if (Dir == 5)
		Dir=1;
	pawn->moveDir(Dir);
	return true;
}
/* 
	Loop constructor
 */
Loop::Loop()
{
}


void Loop::start(Pawn* pawn, Time startTime)
{
  //initialize with parent start method
  Controller::start(pawn, startTime);
  Dir = 1;
  oldTime = startTime;
}

