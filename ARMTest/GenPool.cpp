#include <stdlib.h>
#include <string.h>
#ifdef _DEBUG
#include <algorithm> 
#endif
#include "GenPool.h"


GenPool::GenPool()
{
	poolSize = 0;
	itemStride = 0;
	poolData = nullptr;
	nextFreeObj = nullptr;
}

/* Create a pool */
void GenPool::CreatePool(size_t newPoolSize, size_t objSize)
{
	if (poolSize > 0)
		DestroyPool();

	poolData = malloc(newPoolSize*objSize);
	maxPtr = (char*)poolData + newPoolSize*objSize;
	itemStride = objSize;
	poolSize = newPoolSize;
	
#ifdef _DEBUG
	{
		for (int* c = (int*)poolData; c < maxPtr; c += 1)
			*c = BAD_MEM_VALUE;
	}
#endif

	// Setup Next Pointers
	size_t voidStep = itemStride / sizeof(void*)+((itemStride % sizeof(void*))>0?1:0);
	void* prevPt = nullptr;
	void** iterPt = (void**)poolData;
	for (size_t i = 0; i < poolSize; i++)
	{
		*iterPt = prevPt;	// Point to next Free Pointer

		prevPt = iterPt;
		iterPt = iterPt + voidStep;
	}

	nextFreeObj = prevPt;
}

/* Allocate an object from this pool */
void* GenPool::CreateObject()
{
	if (nextFreeObj == nullptr)
		return nullptr;

	void* newObj = nextFreeObj;
	void** jumpToNextFree = (void**)nextFreeObj;
	nextFreeObj = *jumpToNextFree;
	
	return newObj;
}

/* Deallocate an object from this pool */
void GenPool::FreeObject(void* obj)
{
	if ((obj == nullptr) || (obj < poolData) || (obj > maxPtr))
		return;

#ifdef _DEBUG
	for (int* c = (int*)poolData; c < maxPtr; c += 1)
		*c = BAD_MEM_VALUE;
#endif

	void** objPtr = (void**)obj;
	*objPtr = nextFreeObj;
	nextFreeObj = objPtr;
}

/* Destroy this pool */
void GenPool::DestroyPool()
{
	if (poolSize < 1)
		return;

	poolSize = 0;
	free(poolData);
}