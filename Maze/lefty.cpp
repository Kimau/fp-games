#include "lefty.h"
#include "misc.h"
#include "cellstack.h"
#include "vector.h"

/* 
	Lefty follows the left hand rule
 */
void Lefty::execute(Pawn* pawn)
{
	Vector<float,2> pawnPos_ = pawn->getWorldPos();
	float wPos_X = (pawnPos_[0] - int(pawnPos_[0]));
	float wPos_Y = ((0-pawnPos_[1]) - int(0-pawnPos_[1]));

	// If centerish
	if((wPos_X > 0.2) && (wPos_X < 0.8) && (wPos_Y > 0.2) && (wPos_Y < 0.8))
	{
		// Move Left at fist chance
		pointXYZ pos_ = pawn -> getGridPos();
		int lDir_ = Dir + 1;
		if (lDir_ > 4)
		{
			lDir_ = 1;
		}

		if (pawn->checkStep(pos_.getX(),pos_.getY(),pos_.getZ(),lDir_))
		{
			Dir = lDir_;
		}
	}

	pawn->moveDir(Dir);

}

bool Lefty::getNewDir(Pawn* pawn)
{
	++Dir;
	if (Dir > 4)
		Dir = 1;
	pawn->moveDir(Dir);
	return true;
}
/* 
	Lefty constructor
 */
Lefty::Lefty()
{
}


void Lefty::start(Pawn* pawn, Time startTime)
{
  //initialize with parent start method
  Controller::start(pawn, startTime);
  Dir = 1;
  oldTime = startTime;
}

