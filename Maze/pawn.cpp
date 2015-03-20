#include "pawn.h"
#include "player.h"

Maze* Pawn::maze = Maze::getInstance();

Pawn::Pawn()
{
	maze = Maze::getInstance();
	x = 0.5;
	y = 0.5;
	z = 0.5;
	controller = 0;
	lastUpdateTime = -1.0;
  
  //Initialize stats to default values
	isMovable	  =	true;
	isGhost		  =	false;
	isGodMode	  =	false;
	isPickup	  =	false;
	isWounded	  =	false;
	isAngry		  =	false;
	isCollector	=	false;
	isBeserk	  =	false;
	isCorrupt	  =	false;
	maxHealth	  =	10;
	health		  =	10;
	baseSpeed	  =	0.1f;
	name		    =	"";
  
}

Pawn::Pawn(float posX, float posY, float posZ)
{
	maze = Maze::getInstance();
	x = posX;
	y = posY;
	z = posZ;
	controller = 0;
	lastUpdateTime = -1.0;

  //Initialize stats to default values
	isMovable	  =	true;
	isGhost		  =	false;
	isGodMode	  =	false;
	isPickup	  =	false;
	isWounded	  =	false;
	isAngry		  =	false;
	isCollector	=	false;
	isBeserk	  =	false;
	isCorrupt	  =	false;
	maxHealth	  =	10;
	health		  =	10;
	baseSpeed	  =	0.1f;
	name		    =	"";
}

Pawn::~Pawn()
{	
	delete controller;
}

void Pawn::setController(Controller* controller)
{
  if(Pawn::controller)
    delete Pawn::controller;
  Pawn::controller = controller;
}
void Pawn::setLastSeenPos(pointXYZ pos)
{
  lastSeenPos = pos;
}

Controller* Pawn::getController() const
{
  return controller;
}

void Pawn::updateController(Time currentTime)
{  
  if(controller)
    controller->update(this, currentTime);
}

void Pawn::update(Time currentTime)
{
  if(lastUpdateTime > 0.0f)  //if it has been initialized
  {
    //updatePosition(currentTime);
    if (!takeStep(currentTime))
	{
		if (name != "hero")
			controller->getNewDir(this);
	}
  }
  updateController(currentTime);  
  lastUpdateTime = currentTime;
}

//Accesors
bool Pawn::getMovable()
{ return isMovable; }
bool Pawn::getGhost()
{ return isGhost; }
bool Pawn::getGodMode()
{ return isGodMode; }
bool Pawn::getPickup()
{ return isPickup; }
bool Pawn::getWounded()
{ return isWounded; }
bool Pawn::getAngry()
{ return isAngry; }
bool Pawn::getCollector()
{ return isCollector; }
bool Pawn::getBeserk()
{ return isBeserk; }
bool Pawn::getCorrupt()
{ return isCorrupt; }

int Pawn::getXi()
{return int(direction[0]*1.5);}
int Pawn::getYi()
{return int(direction[1]*1.5);}

int  Pawn::getMaxHealth()
{ return maxHealth; }
int  Pawn::getHealth()
{ return health; }
float Pawn::getBaseSpeed()
{ return baseSpeed; }
char* Pawn::getName()
{ return name; }

int Pawn::getX()
{ return int(x); }
int Pawn::getY()
{ return int(y); }
int Pawn::getZ()
{ return int(z); }
pointXYZ Pawn::getGridPos()
{ return pointXYZ((int)x,(int)y,(int)z); }
Vector<float, 2> Pawn::getWorldPos()
{ return Vector<float, 2>(x,-y); }

Pawn* Pawn::getLastSeen()
{ return lastSeen; }
pointXYZ Pawn::getLastSeenPos()
{ return lastSeenPos; }
float Pawn::getLastSeenTime()
{ return lastSeenTime; }

//movers
bool Pawn::moveDir(int dir_)
{
	/* Check if collision WILL happen
	pointXYZ destPos(int(x + baseSpeed*direction[0]),int(y + baseSpeed*direction[1]), int(z));
	(destPos == getGridPos())
	*/


	{
		switch(dir_)
		{
		case Maze::NORTH:
				direction.set(0.0f,-1.0f);
				return true;
		case Maze::EAST:
				direction.set(1.0f,0.0f);
				return true;
		case Maze::SOUTH:
				direction.set(0.0f,1.0f);
				return true;
		case Maze::WEST:
				direction.set(-1.0f,0.0f);
				return true;
		case Maze::UP:
				return true;
		case Maze::DOWN:
				return true;
		}
	}
	return false;
}

bool Pawn::moveTo(pointXYZ dest_)
{
	x = dest_.getX() + 0.5f;
	y = dest_.getY() + 0.5f;
	z = dest_.getZ() + 0.5f;
	return true;
}

float Pawn::getMoveTime(pointXYZ dest_)
{
	return 1;
}

bool Pawn::isCenter()
{
  /*
    Anti Lotering Code
	Checks if your in the O
	
	0:		1&3:NORTH	2&4:EAST
			and SOUTH	and WEST
	*XX*	*OO*		*XX*
	XOOX	XOOX		OOOO
	XOOX	XOOX		OOOO
	*XX*	*OO*		*XX*

	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  */

	int x_ = (int(x*4)) %4;
	int y_ = (int(x*4)) %4;

	return (((x_ != 0) && (x_ != 3)) && ((y_ != 0) && (y_ != 3)));
}

bool Pawn::takeStep(Time currentTime)
{
	float bar_ = 0.26f;
	float iX = (double)direction[0] * (double)baseSpeed * (currentTime-lastUpdateTime);
	float iY = (double)direction[1] * (double)baseSpeed * (currentTime-lastUpdateTime);

	if (iX > 1) iX = 1;
	if (iY > 1) iY = 1;

	float nX = x + iX;
	float nY = y + iY;
	int Xdir_ = 0;
	int Ydir_ = 0;

	/*
		Border Checking
	*/
	if (!(checkStep(int(nX),int(nY),int(z),Maze::NORTH)) )
	{
		int y_ = (int((nY-int(nY)) * 4));
		if (y_ == 0)
		{
			/*if (nY < y)
			{
				y = int(nY) + 0.3;
				return false;
			}*/
			nY = int(nY) + bar_;
		}
	}

	if (!(checkStep(int(nX),int(nY),int(z),Maze::SOUTH)) )
	{
		int y_ = (int((nY-int(nY)) * 4));
		if (y_ == 3)
		{
			/*if (nY > y)
			{
				y = int(nY) + 0.7;
				return false;
			}*/
			nY = int(nY) + 1 - bar_;
		}
	}

	if (!(checkStep(int(nX),int(nY),int(z),Maze::EAST)) )
	{
		int x_ = (int((nX-int(nX)) * 4));
		if (x_ == 3)
		{
			/*if (nX > x)
			{
				x = int(nX) + 0.7;
				return false;
			}*/
			nX = int(nX) + 1 - bar_;
		}
	}

	if (!(checkStep(int(nX),int(nY),int(z),Maze::WEST)) )
	{
		int x_ = (int((nX-int(nX)) * 4));
		if (x_ == 0)
			/*if (nX < x)
			{
				x = int(nX) + 0.3;
				return false;
			}*/
			nX = int(nX) + bar_;
	}


	// X movement check
	if((int)floor(nX+ bar_) > (int)floor(x))
		Xdir_ = Maze::EAST;
	if((int)floor(nX- bar_) < (int)floor(x))
		Xdir_ = Maze::WEST;
	if (checkStep(int(x),int(y),int(z),Xdir_))
	{
		// Y movement check
		if((int)floor(nY+ bar_) > (int)floor(y))
			Ydir_ = Maze::SOUTH;
		if((int)floor(nY- bar_) < (int)floor(y))
			Ydir_ = Maze::NORTH;
		if (checkStep(int(x),int(y),int(z),Ydir_))
		{
			if(isBeserk)
			{
				if (Xdir_ != 0)	maze->breakWall(int(x),int(y),int(z),Xdir_);
				if (Ydir_ != 0)	maze->breakWall(int(x),int(y),int(z),Ydir_);
			}        
			x = nX;
			y = nY;
			/*
				Check Collisions
			*/
			if (Player::getInstance()->checkCollide(*this))
			{
				return (biteHim());
			}
			return true;
		}
	}
  
	return false;
}

bool Pawn::checkStep(int x_, int y_, int z_, int dir_)
{
	if (dir_ == 0)
		return true;

	// If it can move AND the destination exsists
	if ( (isMovable) && (maze->isThere(x_,y_,z_,dir_)) )
	{
		// If it is beserk AND wall/door is breakable
		if ((isBeserk) && (maze->isBreakable(x_,y_,z_,dir_)) )
		{
			return true;
		}

		// If it is ghost OR wall is passable
		if ((isGhost) || (maze->isPassable(x_,y_,z_,dir_)) )
		{
			return true;
		}
	}
	return false;
}

