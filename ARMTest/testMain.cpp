#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Example.h"
#include "GenPool.h"

using namespace std;

void TestPoints()
{
	GenPool testPool;

	cout << "Creating Pool" << endl;
	const int poolSize = 1000;
	testPool.CreatePool(poolSize, sizeof(Point));

	Point** pointList = new Point*[poolSize];

	// Fill Pool
	{		
		cout << "Filling Pool" << endl;
		for (size_t i = 0; i < poolSize; i++)
		{
			pointList[i] = new(testPool)Point;
		}

		cout << "Pushing Beyond Limits" << endl;
		Point* badAssign = new(testPool)Point;
	}

	// Free Pool
	{
		cout << "Freeing some Pools" << endl;
		size_t minPoint = poolSize / 3;
		size_t maxPoint = poolSize - (poolSize / 5);
		for (size_t i = minPoint; i < maxPoint; i++)
		{
			operator delete(pointList[i], testPool);
			// OR testPool.FreeObject(pointList[i]);
			// Annoying bit of C++
		}

		cout << "Freeing Bad Data" << endl;
		operator delete(nullptr, testPool);
	}

	cout << "Destroying Pool" << endl;
	testPool.DestroyPool();
	delete[] pointList;
}

void TestNoDefaultConstructor()
{
	GenPool testPool;

	cout << "Creating Pool" << endl;
	const int poolSize = 1000;
	testPool.CreatePool(poolSize, sizeof(NoDefaultConstructor));

	NoDefaultConstructor** pointList = new NoDefaultConstructor*[poolSize];

	// Fill Pool
	{
		cout << "Filling Pool" << endl;
		for (size_t i = 0; i < poolSize; i++)
		{
			pointList[i] = new(testPool)NoDefaultConstructor(i);
		}

		cout << "Pushing Beyond Limits" << endl;
		NoDefaultConstructor* badAssign = new(testPool)NoDefaultConstructor(1000000);
	}

	// Free Pool
	{
		cout << "Freeing some Pools" << endl;
		size_t minPoint = poolSize / 3;
		size_t maxPoint = poolSize - (poolSize / 5);
		for (size_t i = minPoint; i < maxPoint; i++)
		{
			operator delete(pointList[i], testPool);
			// OR testPool.FreeObject(pointList[i]);
			// Annoying bit of C++
		}

		cout << "Freeing Bad Data" << endl;
		operator delete(nullptr, testPool);
	}

	cout << "Destroying Pool" << endl;
	testPool.DestroyPool();
	delete[] pointList;
}

void TestDerived()
{
	GenPool testPool;

	cout << "Creating Pool" << endl;
	const int poolSize = 1000;
	testPool.CreatePool(poolSize, sizeof(Derived));

	Derived** pointList = new Derived*[poolSize];

	// Fill Pool
	{
		cout << "Filling Pool" << endl;
		for (size_t i = 0; i < poolSize; i++)
		{
			pointList[i] = new(testPool)Derived();
		}

		cout << "Pushing Beyond Limits" << endl;
		Derived* badAssign = new(testPool)Derived();
	}

	// Free Pool
	{
		cout << "Freeing some Pools" << endl;
		size_t minPoint = poolSize / 3;
		size_t maxPoint = poolSize - (poolSize / 5);
		for (size_t i = minPoint; i < maxPoint; i++)
		{
			operator delete(pointList[i], testPool);
			// OR testPool.FreeObject(pointList[i]);
			// Annoying bit of C++
		}

		cout << "Freeing Bad Data" << endl;
		operator delete(nullptr, testPool);
	}

	cout << "Destroying Pool" << endl;
	testPool.DestroyPool();
	delete[] pointList;
}

// Entry
int main(int argc, char **argv)
{
	cout << endl << "Testing Points" << endl;
	TestPoints();

	cout << endl << "Testing No Default Constructor" << endl;
	TestNoDefaultConstructor();

	cout << endl << "Testing Derived" << endl;
	TestDerived();

	cout << "Finished Tests" << endl;
	char pauseChar;
	cin.get(pauseChar);
}