#ifndef MAZE
#define MAZE

#include <cstdlib>
#include <cmath>
#include "pointXYZ.h"

class Maze //Utilizes Singleton
{
	//Predefinitions
	friend class Pawn;
  friend class Player;
  

	public:
    //Constants
		/*
			Directions
		*/
    enum Direction
    {
      NORTH				= 1,
      EAST				= 2,
      SOUTH				= 3,
      WEST				= 4,
      UP 				= 5,
      DOWN				= 6
    };

		// Creation Methods
		static Maze* getInstance();
		bool Create(int height, int width, int depth);
		bool Create(int height, int width, int depth, int seed_);
		bool Create(int height, int width, int depth, int seed_, int bigroom_, int medroom_, int smlroom_);
		bool Generate();
		~Maze();

		// Check Methods
		bool isThere	(int x_, int y_, int z_);
		bool isThere	(int x_, int y_, int z_, int dir_);
		bool isPassable	(int x_, int y_, int z_, int dir_);
		bool isDoor		(int x_, int y_, int z_,   int dir_);
		bool isBreakable(int x_, int y_, int z_, int dir_);
		bool isOpen		(int x_, int y_, int z_, int dir_);
		int getHealth	(int x_, int y_, int z_, int dir_);
		int getType		(int x_, int y_, int z_, int dir_);
	protected:
		Maze();
		Maze(const Maze&);
		Maze& operator=(const Maze&);

		// Pawn Methods
		bool openDoor	(int x_, int y_, int z_, int dir_);
		bool closeDoor	(int x_, int y_, int z_, int dir_);
		bool breakWall	(int x_, int y_, int z_, int dir_);		
	private:
		int  CheckWalls	(int, int, int);
		bool isIntact	(int, int, int);
		void CalcRooms	();
		bool MakeRoom	(int);

		// Variables

    /* dimensions
     *
     * TODO: This struct (and access functions bellow) will have to be extended if an n'th dimensional maze is used
     */
    struct
    {
      int height;
		  int width;
		  int depth;
    } dimensions;
 
  public:
    int getHeight() const { return dimensions.height; }
    int getWidth()  const { return dimensions.width; }
    int getDepth()  const { return dimensions.depth; }
  private:
		static Maze* onlyInstance;
    unsigned* cells;
    int  seed;
		int  bigroom;
		int  medroom;
		int  smlroom;

    __inline unsigned& getCell(int x, int y, int z)
    {
      return cells[x + y*dimensions.width + z*dimensions.width*dimensions.height];
    }


		// FLAGS
		/*
		Using a flag system we make each cell an int which holds the wall to its west, north and above.
		Each wall is stored as follows. North, West, Up
		*/
		static const unsigned 	N_FULL;
		static const unsigned 	N_PASS;
		static const unsigned	N_DOOR;
		static const unsigned	N_OPEN;
		static const unsigned	N_BREAK;
		static const unsigned	N_HEALTH;

		static const unsigned	N_NUM1;
		static const unsigned	N_NUM2;
		static const unsigned	N_NUM3;
		static const unsigned	N_NUM4;

		static const unsigned	W_FULL;
		static const unsigned	W_PASS;
		static const unsigned	W_DOOR;
		static const unsigned	W_OPEN;
		static const unsigned	W_BREAK;
		static const unsigned	W_HEALTH;

		static const unsigned	W_NUM1;
		static const unsigned	W_NUM2;
		static const unsigned	W_NUM3;
		static const unsigned	W_NUM4;

		static const unsigned	U_FULL;
		static const unsigned	U_PASS;
		static const unsigned	U_DOOR;
		static const unsigned	U_OPEN;
		static const unsigned	U_BREAK;
		static const unsigned	U_HEALTH;

		static const unsigned	U_NUM1;
		static const unsigned	U_NUM2;
		static const unsigned	U_NUM3;
		static const unsigned	U_NUM4;

		static const unsigned	BREAK_WALL;

		static const unsigned	NORTH_WALL;
		static const unsigned	EAST_WALL;
		static const unsigned	SOUTH_WALL;
		static const unsigned	WEST_WALL;
		static const unsigned	UP_WALL;
		static const unsigned	DOWN_WALL;

};

#endif

