#include "maze.h"
#include "cellstack.h"
#include <memory.h>

//Maze Constant Definitions (Unfortunately this has to be here to keep MSVC happy)
/* OLD:
const int Maze::dimensions.height = 10;
const int Maze::dimensions.width  = 10;
const int Maze::dimensions.depth  = 1;*/

const unsigned	Maze::N_FULL		= 127;
const unsigned	Maze::N_PASS		= 1;		// 0
const unsigned	Maze::N_DOOR		= 2;		// 1
const unsigned	Maze::N_OPEN		= 4;		// 2
const unsigned	Maze::N_BREAK		= 8;		// 3
const unsigned	Maze::N_HEALTH		= 16;		// 4

const unsigned	Maze::N_NUM1		= 0;
const unsigned	Maze::N_NUM2		= 32;		// 5
const unsigned	Maze::N_NUM3		= 64;		// 6
const unsigned	Maze::N_NUM4		= 96;		// 5+6

const unsigned	Maze::W_FULL		= 16256;
const unsigned	Maze::W_PASS		= 128;		// 7
const unsigned	Maze::W_DOOR		= 256;		// 8
const unsigned	Maze::W_OPEN		= 512;		// 9
const unsigned	Maze::W_BREAK		= 1024;		// 10
const unsigned	Maze::W_HEALTH		= 2048;		// 11

const unsigned	Maze::W_NUM1		= 0;
const unsigned	Maze::W_NUM2		= 4096;		// 12
const unsigned	Maze::W_NUM3		= 8192;		// 13
const unsigned	Maze::W_NUM4		= 12288;	// 12+13

const unsigned	Maze::U_FULL		= 2080768;
const unsigned	Maze::U_PASS		= 16384;	// 14
const unsigned	Maze::U_DOOR		= 32768;	// 15
const unsigned	Maze::U_OPEN		= 65536;	// 16
const unsigned	Maze::U_BREAK		= 131072;	// 17
const unsigned	Maze::U_HEALTH		= 262144;	// 18

const unsigned	Maze::U_NUM1		= 0;
const unsigned	Maze::U_NUM2		= 524288;	// 19
const unsigned	Maze::U_NUM3		= 1048576;	// 20
const unsigned	Maze::U_NUM4		= 1572864;	// 19 + 20

const unsigned  Maze::BREAK_WALL	= (N_BREAK + W_BREAK + U_BREAK);

const unsigned	Maze::NORTH_WALL	= 1;
const unsigned	Maze::EAST_WALL		= 2;
const unsigned	Maze::SOUTH_WALL	= 4;
const unsigned	Maze::WEST_WALL		= 8;
const unsigned	Maze::UP_WALL		= 16;
const unsigned	Maze::DOWN_WALL		= 32;

/*const int	Maze::NORTH				= 1;
const int	Maze::EAST				= 2;
const int	Maze::SOUTH				= 3;
const int	Maze::WEST				= 4;
const int	Maze::UP 				= 5;
const int	Maze::DOWN				= 6;*/


Maze* Maze::onlyInstance = 0;

Maze* Maze::getInstance()
{
	if(onlyInstance == 0)
		onlyInstance = new Maze();
	return onlyInstance;
}

Maze::Maze() : cells(0)
{}

/*
	Default Constructor
*/
bool Maze::Create(int height, int width, int depth)
{
	if(onlyInstance == 0)
		return false;

  dimensions.height = height;
  dimensions.width  = width;
  dimensions.depth  = depth;

  if(cells)
    delete cells;

  int mSize_ = width*height*depth;
  cells = new unsigned[mSize_];
  for(int c=0; c< mSize_; ++c)
  {
	  cells[c] = BREAK_WALL;
  }

	seed = rand()%(height + width + depth);
	CalcRooms();
  return true;
}

/*
	Constructor of Maze of X size
*/
bool Maze::Create(int height, int width, int depth, int seed_)
{
	if(onlyInstance == 0)
		return false;

  dimensions.height = height;
  dimensions.width  = width;
  dimensions.depth  = depth;

  if(cells)
    delete cells;

  int mSize_ = width*height*depth;
  cells = new unsigned[mSize_];
  for(int c=0; c< mSize_; ++c)
  {
	  cells[c] = BREAK_WALL;
  }

	seed = seed_;
	CalcRooms();
  return true;
}

/*
	Complete Constructor of Maze
*/
bool Maze::Create(int height, int width, int depth, int seed_, int bigroom_, int medroom_, int smlroom_)
{
	if(onlyInstance == 0)
		return false;

  dimensions.height = height;
  dimensions.width  = width;
  dimensions.depth  = depth;

  if(cells)
    delete cells;

  int mSize_ = width*height*depth;
  cells = new unsigned[mSize_];
  for(int c=0; c< mSize_; ++c)
  {
	  cells[c] = BREAK_WALL;
  }

	seed = seed_;
	CalcRooms();
	// Check rooms aren't more than there is space for
	if ((bigroom_ + medroom_ + smlroom_) < (bigroom + medroom + smlroom))
	{
		bigroom = bigroom_;
		medroom = medroom_;
		smlroom = smlroom_;
	}
  return true;
}

/*
	Decontructor
*/
Maze::~Maze()
{
	delete cells;
	onlyInstance = 0;
}

/*
	Maze Generator Method
*/
bool Maze::Generate()
{
	if(onlyInstance == 0)
		return false;
	/*
		Generate a perfect one solution maze
	*/
	CellStack list_;												// create a CellStack (LIFO) to hold a list of cell locations
	int totalCells_ = dimensions.height*dimensions.width*dimensions.depth;		// set TotalCells = number of cells in grid

	srand(seed);														// choose a cell at random and call it CurrentCell
	int x_,y_,z_;
	x_ = rand()%dimensions.height;
	y_ = rand()%dimensions.width;
	z_ = rand()%dimensions.depth;
	pointXYZ currentCell_(x_,y_,z_);

	int vistedCells_ = 1;										// set VisitedCells = 1
	while (vistedCells_ < totalCells_)
	{
		if (vistedCells_ == 50)
		{
		  int quicknum_ = 1;
		  quicknum_++;
		}		
		switch(CheckWalls(x_,y_,z_))					//	 find all neighbors of CurrentCell with all walls intact
		{																			//	 knock down the wall between it and CurrentCell
			case NORTH:
				getCell(x_,y_,z_) += N_PASS;
				y_--;
				break;
			case EAST:
				getCell(x_+1,(y_),z_) += W_PASS;
				x_++;
				break;
			case SOUTH:
				getCell(x_,(y_+1),z_) += N_PASS;
				y_++;
				break;
			case WEST:
				getCell(x_,y_,z_) += W_PASS;
				x_--;
				break;
			case UP:
				getCell(x_,y_,z_) += U_PASS;
				z_++;
				break;
			case DOWN:
				getCell(x_,y_,(z_-1)) += U_PASS;
				z_--;
				break;
			case 0:															//   No valid directions found
				pointXYZ newCell_ = list_.pop();	//	 pop the most recent cell entry off the CellStack and make it the CurrentCell
          x_ = newCell_.getX();
          y_ = newCell_.getY();
          z_ = newCell_.getZ();
        if (isThere(x_,y_,z_))
        {
          currentCell_ = pointXYZ(x_,y_,z_);
        }
        else
        {
          return false;                   //  ERROR:: Empty or corpute stack detected
        }
				continue;
		}
		list_.push(currentCell_);							//	 push CurrentCell location on the CellStack
		currentCell_ = pointXYZ(x_,y_,z_);		//	 make the new cell CurrentCell
		vistedCells_++;												//	 add 1 to VisitedCells
	}
	/*
		Slap Rooms Down in Maze
	*/
   return MakeRoom(3);
   /*
	int bRooms_ = bigroom;
	int mRooms_ = medroom;
	int sRooms_ = smlroom;

	while(bRooms_ + mRooms_ + sRooms_)			//	While there are still rooms left
	{
		if(bRooms_ && MakeRoom(5))
			{bRooms_--;}
		if(mRooms_ && MakeRoom(4))
			{mRooms_--;}
		if(sRooms_ && MakeRoom(3))
			{sRooms_--;}
	}              */

  return true;
}

/*
	Check Walls to see if broken if its slightly intact then break a wall down
*/
int Maze::CheckWalls(int x_, int y_, int z_)
{
	int valid_ = 0;
	if (isIntact(x_,(y_-1),z_))															// North
		{
			valid_ = valid_ + NORTH_WALL;
		}
	if (isIntact((x_+1),y_,z_))															// East
		{
			valid_ = valid_ + EAST_WALL;
		}
	if (isIntact(x_,(y_+1),z_))															// South
		{
			valid_ = valid_ + SOUTH_WALL;
		}
	if (isIntact((x_-1),y_,z_))															// West
		{
			valid_ = valid_ + WEST_WALL;
		}
	if (isIntact(x_,y_,(z_+1)))															// Up
		{
			valid_ = valid_ + UP_WALL;
		}
	if (isIntact(x_,y_,(z_-1)))															// Down
		{
			valid_ = valid_ + DOWN_WALL;
		}

	if (valid_ == 0)																				// Did we find more than 3 walls intact
		return 0;

	int dir_ = rand() %6;
	if
		((static_cast<int>(pow(2.0,double(dir_)))) & valid_)
	{
		return (dir_+1);
	}
	else
	{
		while(!((static_cast<int>(pow(2.0,double(dir_)))) & valid_))
		{
			dir_ = ++dir_ % 6;
		}
		return (dir_+1);
	}
}

/*
	Check if a cells walls are intact
*/
bool Maze::isIntact(int x_, int y_, int z_)
{
  if(isThere(x_,y_,z_))
	if ((y_==0)						|| !(getCell(x_,y_,z_)		& N_PASS))	// North
	if ((x_==dimensions.width-1)	|| !(getCell((x_+1),y_,z_)	& W_PASS)) 	// East
	if ((y_ == dimensions.height-1) || !(getCell(x_,(y_+1),z_)	& N_PASS))	// South
	if ((x_ == 0)					|| !(getCell(x_,y_,z_)		& W_PASS))	// West
	if ((z_ == dimensions.depth-1)	|| !(getCell(x_,y_,z_)		& U_PASS))  // Up
	if ((z_==0)						|| !(getCell(x_,y_,(z_-1))	& U_PASS))	// Down
		return true;

  	// Failed Check
	return false;
}

/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!			# #  #  ### # #			!
	!			### ### #   ## 			!
	!			# # # # ### # #			!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
int min(int one_,int other_)
{	if(one_ > other_)		return other_;	else		return one_;}
int max(int one_,int other_)
{	if(one_ < other_)		return other_;	else		return one_;}

/*
	Calculate Amount of Rooms
*/
void Maze::CalcRooms()
{
	if ((dimensions.width > 3) && (dimensions.height > 3))
		smlroom = static_cast<int>(((dimensions.height*dimensions.width)/50) * dimensions.depth);
			if ((dimensions.width > 4) && (dimensions.height > 4))
				medroom = static_cast<int>(((dimensions.height*dimensions.width)/75) * dimensions.depth);
					if ((dimensions.width > 5) && (dimensions.height > 5))
						bigroom = static_cast<int>(((dimensions.height*dimensions.width)/100) * dimensions.depth);

  smlroom = max(smlroom,0);
  medroom = max(medroom - (smlroom/2),0);
  bigroom = max(bigroom - (medroom/2),0);
}

/*
	Attempt to make a room of x Size
*/
bool Maze::MakeRoom(int sz_)
{
	/*
		Pick Random Cell
	*/
	int x_,y_,z_;
	x_ = min(rand()%dimensions.height,dimensions.height-sz_);
	y_ = min(rand()%dimensions.width,dimensions.width-sz_);
	z_ = rand()%dimensions.depth;

	/*
		Construct Room
		Take each wall in room area.
		If wall is bordering then make wall a door
		else Demolish wall
	*/
	for(int x=x_;x <= (x_ + sz_); x++)
	for(int y=y_;y <= (y_ + sz_); y++)
	{
		// X wall
    if(y < (y_ + sz_))
    {
    if((x==x_)||(x==(x_ + sz_)))
    {
      if(getCell(x,y,z_) & W_PASS)   // If passable turn into a door
      {
        getCell(x,y,z_) += W_DOOR;
        getCell(x,y,z_) -= W_PASS;
      }
		}
    else // Not an X Wall
    {
      getCell(x,y,z_) = (getCell(x,y,z_) & (N_FULL + U_FULL)) + W_PASS;} // Make simple passable
    }

		// Y wall
    if(x < (x_ + sz_))
    {
		if((y==y_)||(y==(y_ + sz_)))
		{
			if(getCell(x,y,z_) & N_PASS)   // If passable turn into a door
      {
        getCell(x,y,z_) += N_DOOR;
        getCell(x,y,z_) -= N_PASS;
      }
		}
    else // Not an Y Wall
    {getCell(x,y,z_) = (getCell(x,y,z_) & (W_FULL + U_FULL)) + N_PASS;} // Make simple passable
    }
	}
  return true;
}

/*
	Check that Cell Exsists
*/
bool Maze::isThere(int x_, int y_, int z_)
{
	if ((x_ < 0) || (y_ < 0) || (z_ < 0))
	 return false;

	if ((x_ >= dimensions.height) || (y_ >= dimensions.width) || (z_ >= dimensions.depth))
	 return false;

	return true;
}

/*
	Checks Cell exsists in direction
*/
bool Maze::isThere(int x_, int y_, int z_, int dir_)
{
	switch (dir_)
	{
	  case NORTH:
		return(isThere(x_,(y_-1),z_));
	  case EAST:
		return(isThere((x_+1),y_,z_));
	  case SOUTH:
		return(isThere(x_,(y_+1),z_));
	  case WEST:
		return(isThere((x_-1),y_,z_));
	  case UP:
		return(isThere(x_,y_,(z_+1)));
	  case DOWN:
		return(isThere(x_,y_,(z_-1)));
	}
  return false;
}

/*
	Checks if a wall is passable
*/
bool Maze::isPassable(int x_, int y_, int z_, int dir_)
{
	if ( ((x_==0) && (dir_ == WEST)) || ((y_==0) && (dir_ == NORTH)))
	{
		return false;
	}
	if (!isThere(x_,y_,z_))
		return false;
	switch (dir_)
	{
	  case NORTH:
		return ((getCell(x_,y_,z_) & N_PASS) != 0);
	  case EAST:
		return(isPassable((x_+1),y_,z_,WEST));
	  case SOUTH:
		return(isPassable(x_,(y_+1),z_,NORTH));
	  case WEST:
		return ((getCell(x_,y_,z_) & W_PASS) != 0);
	  case UP:
		return ((getCell(x_,y_,z_) & U_PASS) != 0);
	  case DOWN:
		return(isPassable(x_,y_,(z_-1),UP));
	}
  return false;
}

/*
	Checks if its a wall
*/
bool Maze::isDoor(int x_, int y_, int z_, int dir_)
{
	if ( ((x_==0) && (dir_ == WEST)) || ((y_==0) && (dir_ == NORTH)))
	{
		return false;
	}
	if (!isThere(x_,y_,z_))
		return false;
	switch(dir_)
	{
	  case NORTH:
		return ((getCell(x_,y_,z_) & N_DOOR) != 0);
	  case EAST:
		return(isDoor((x_+1),y_,z_,WEST));
	  case SOUTH:
		return(isDoor(x_,(y_+1),z_,NORTH));
	  case WEST:
		return ((getCell(x_,y_,z_) & W_DOOR) != 0);
	  case UP:
		return ((getCell(x_,y_,z_) & U_DOOR) != 0);
	  case DOWN:
		return(isDoor(x_,y_,(z_-1),SOUTH));
	}
  return false;
}

/*
	Checks if a wall is Breakable
*/
bool Maze::isBreakable(int x_, int y_, int z_, int dir_)
{
	if ( ((x_==0) && (dir_ == WEST)) || ((y_==0) && (dir_ == NORTH)))
	{
		return false;
	}
	if (!isThere(x_,y_,z_))
		return false;
	switch(dir_)
	{
	  case NORTH:
		return((getCell(x_,y_,z_) & N_BREAK) == N_BREAK);
	  case EAST:
		return(isBreakable((x_+1),y_,z_,WEST));
	  case SOUTH:
		return(isBreakable(x_,(y_+1),z_,NORTH));
	  case WEST:
		return((getCell(x_,y_,z_) & W_BREAK) == W_BREAK);
	  case UP:
		return((getCell(x_,y_,z_) & U_BREAK) == U_BREAK);
	  case DOWN:
		return(isBreakable(x_,y_,(z_-1),SOUTH));
	}
  return false;
}

/*
	Checks if a door is open
*/
bool Maze::isOpen(int x_, int y_, int z_, int dir_)
{
	if (!isDoor(x_,y_,z_,dir_))
		return false;
	switch(dir_)
	{
	  case NORTH:
		return ((getCell(x_,y_,z_) & N_OPEN) != 0);
	  case EAST:
		return(isOpen((x_+1),y_,z_,WEST));
	  case SOUTH:
		return(isOpen(x_,(y_+1),z_,NORTH));
	  case WEST:
		return ((getCell(x_,y_,z_) & W_OPEN) != 0);
	  case UP:
		return ((getCell(x_,y_,z_) & U_OPEN) != 0);
	  case DOWN:
		return(isOpen(x_,y_,(z_-1),SOUTH));
	}
  return false;
}

/*
	Get the Health of a wall
*/
int Maze::getHealth(int x_, int y_, int z_, int dir_)
{
	if (!isThere(x_,y_,z_))
		return false;
	switch(dir_)
	{
	  case NORTH:
		return (getCell(x_,y_,z_) & N_HEALTH);
	  case EAST:
		return(getHealth((x_+1),y_,z_,WEST));
	  case SOUTH:
		return(getHealth(x_,(y_+1),z_,NORTH));
	  case WEST:
		return (getCell(x_,y_,z_) & W_HEALTH);
	  case UP:
		return (getCell(x_,y_,z_) & U_HEALTH);
	  case DOWN:
		return(getHealth(x_,y_,(z_-1),SOUTH));
	}
  return false;
}

/*
	Opens a Door :: Pawn is only user
*/
bool Maze::openDoor(int x_, int y_, int z_, int dir_)
{
	if ( (!(isPassable(x_,y_,z_,dir_))) && (!(isOpen(x_,y_,z_,dir_))) )
	{
	switch(dir_)
	{
	  case NORTH:
		getCell(x_,y_,z_) += (N_OPEN + N_PASS);
		return true;
	  case EAST:
		return openDoor((x_+1),y_,z_,WEST);
	  case SOUTH:
		return openDoor(x_,(y_+1),z_,NORTH);
	  case WEST:
		getCell(x_,y_,z_) += (W_OPEN + W_PASS);
		return true;
	  case UP:
		getCell(x_,y_,z_) += (U_OPEN + U_PASS);
		return true;
	  case DOWN:
		return openDoor(x_,y_,(z_-1),UP);
	}
	}
	return false;
}

/*
	Closes a Door :: Pawn is only user
*/
bool Maze::closeDoor(int x_, int y_, int z_, int dir_)
{
	if ( (isPassable(x_,y_,z_,dir_)) && (isOpen(x_,y_,z_,dir_)) )
	{
	switch(dir_)
	{
	  case NORTH:
		getCell(x_,y_,z_) -= (N_OPEN + N_PASS);
		return true;
	  case EAST:
		return closeDoor((x_+1),y_,z_,WEST);
	  case SOUTH:
		return closeDoor(x_,(y_+1),z_,NORTH);
	  case WEST:
		getCell(x_,y_,z_) -= (W_OPEN + W_PASS);
		return true;
	  case UP:
		getCell(x_,y_,z_) -= (U_OPEN + U_PASS);
		return true;
	  case DOWN:
		return closeDoor(x_,y_,(z_-1),UP);
	}
	}
	return false;
}

/*
	Breaks a Door :: Pawn is only user
*/
bool Maze::breakWall(int x_, int y_, int z_, int dir_)
{
	if(isBreakable(x_,y_,z_,dir_))
	{
	switch(dir_)
	{
	  case NORTH:
		getCell(x_,y_,z_) = N_PASS;
		return true;
	  case EAST:
		return breakWall((x_+1),y_,z_,WEST);
	  case SOUTH:
		return breakWall(x_,(y_+1),z_,NORTH);
	  case WEST:
		getCell(x_,y_,z_) = W_PASS;
		return true;
	  case UP:
		getCell(x_,y_,z_) = U_PASS;
		return true;
	  case DOWN:
		return breakWall(x_,y_,(z_-1),UP);
	}
	}
	return false;
}

/*
	Get the Type of wall
*/
int Maze::getType(int x_, int y_, int z_, int dir_)
{
	if (!isThere(x_,y_,z_))
		return false;
	switch(dir_)
	{
	  case NORTH:
		if (getCell(x_,y_,z_) & N_NUM1)
			return 1;
		if (getCell(x_,y_,z_) & N_NUM2)
			return 2;
		if (getCell(x_,y_,z_) & N_NUM3)
			return 3;
		if (getCell(x_,y_,z_) & N_NUM4)
			return 4;
	  	break;
	  case EAST:
		return(getType((x_+1),y_,z_,WEST));
	  case SOUTH:
		return(getType(x_,(y_+1),z_,NORTH));
	  case WEST:
		if (getCell(x_,y_,z_) & W_NUM1)
			return 1;
		if (getCell(x_,y_,z_) & W_NUM2)
			return 2;
		if (getCell(x_,y_,z_) & W_NUM3)
			return 3;
		if (getCell(x_,y_,z_) & W_NUM4)
			return 4;
	  	break;
	  case UP:
		if (getCell(x_,y_,z_) & U_NUM1)
			return 1;
		if (getCell(x_,y_,z_) & U_NUM2)
			return 2;
		if (getCell(x_,y_,z_) & U_NUM3)
			return 3;
		if (getCell(x_,y_,z_) & U_NUM4)
			return 4;
	  	break;
	  case DOWN:
		return(getType(x_,y_,(z_-1),SOUTH));
	}
  return false;
}

