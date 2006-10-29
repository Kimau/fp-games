#include "CellStack.h"

class Cell
{
	public:
		Cell(pointXYZ input, Cell * _next) : next(_next)
		{
			x = input.getX();
			y = input.getY();
			z = input.getZ();
		}

	private:
		int x;
		int y;
		int z;
		Cell * next;

	friend class CellStack;
};

// Create the list
CellStack::CellStack()
: top(0)
{
}

// Delete the list
CellStack::~CellStack()
{
	while (top != 0)
	{
		pop();
	}
}

// Create new node that points to second newest node
void CellStack::push(pointXYZ cell)
{
	top = new Cell(cell, top);
}

// Overloaded version of Push
void CellStack::push(int x_, int y_, int z_)
{
	pointXYZ tmp(x_,y_,z_);
	push(tmp);
}

// Removes and returns newest node
pointXYZ CellStack::pop()
{
  if (top != 0)
  {
    Cell node_ = *top;
    pointXYZ tmp_(node_.x,node_.y,node_.z);
    top = node_.next;
    return tmp_;
  }
  return (pointXYZ (-1, -1, -1));  
}

// Look at the top of the stack
pointXYZ CellStack::peak() const
{
	if (top != 0)
	{
		pointXYZ tmp(top->x,top->y,top->z);
		return tmp;
	}
	return (pointXYZ(-1,-1,-1));
}

// Check if stack is empty
bool CellStack::isEmpty() const
{
	if ( (top == 0) || (top->z < 0) )
		return true;
	return false;
}

