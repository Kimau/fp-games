#include <iostream>
#include "Maze.h"

using namespace std;

int main()
{
  bool success_ = true;
	cout << "Welcome to the quick maze Test" << endl;
	int seed_;
	cout << "Seed : "; cin >> seed_ ; cout << endl;
	Maze* myMaze = Maze::Instance();

	cout << "Creating Maze..." << endl;
	if (myMaze->Create(seed_))
		cout << "OK!" << endl;
  else
  {
    cout << "FAILED!" << endl;
		success_ = false;
  }
  
	cout << "Generating Maze..." <<endl;
  
	if (myMaze->Generate())
		cout << "OK!" << endl;
	else
  {
    cout << "FAILED!" << endl;
		success_ = false;
  }
  cout << "--------------------------------------------------------------" << endl;
	for(int z=0; z < 1; z++)
	{
		for(int y=0; y < 81; y++)
		{
			for(int x=0; x < 30; x++)
			{
				if (!(y & 1))
				{
					// Is North Wall Passable
					if (myMaze->isPassable(x,(y/2),z,1))
					{
						// Is West Wall Passable
						/*if (myMaze->isPassable(x,(y/2),z,4))
						{ cout << "# ";}
						else*/
						{ cout << " |";}
					}
					else
					{
						// Is North a Wall
						if (myMaze->isDoor(x,(y/2),z,1))
							{cout << " $";}
						else
							{cout << "  ";}
					}
				}
				else
				{
					// Is West Wall Passable
					if (myMaze->isPassable(x,(y/2),z,4))
					{
						{ cout << "-";}
					}
					else
					{
						// Is West a Door
						if (myMaze->isDoor(x,(y/2),z,4))
							{cout << "$";}
						else
							{cout << " ";}
					}

					// Is Up or Down Exsisting
					if (((myMaze->isPassable(x,(y/2),z,5))) ||((myMaze->isPassable(x,(y/2),z,6))))
					{ cout << "0";}
					else
					{ cout << "X";}
				}
			}
			cout << endl;
  }
		cout << "--------------------------------------------------------------" << endl;
		//cout << "===========================================================" << endl;
		//cout << "==                   LEVEL "<< z << "                        ==" << endl;
		//cout << "===========================================================" << endl;
	}
	delete myMaze;
	cout << "GOODBYE" << endl;
  return success_;
}
