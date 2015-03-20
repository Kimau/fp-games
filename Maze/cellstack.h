#ifndef CELLSTACK
#define CELLSTACK

#include "pointXYZ.h"

class Cell;

class CellStack
{
	public:
		CellStack ();
		virtual ~CellStack ();

		void push(pointXYZ);												// add integer to front of list
		void push(int, int, int);								// add integer to front of list
		pointXYZ pop();															// remove integer from front of list
		pointXYZ peak() const;											// observe integer at front of list
		bool isEmpty() const;										// is the list empty?

	private:
		Cell* top;
};

#endif

