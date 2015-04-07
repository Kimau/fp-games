#ifndef GENERAL_MEMORY_POOL_20140602_CB
#define GENERAL_MEMORY_POOL_20140602_CB

class GenPool
{
public:
	GenPool();

	/* Create a pool */
	void CreatePool(size_t newPoolSize, size_t objSize);

	/* Allocate an object from this pool */ 
	void* CreateObject();

	/* Deallocate an object from this pool */ 
	void FreeObject(void* obj);

	/* Destroy this pool */
	void DestroyPool();

private:
	size_t poolSize;
	void* nextFreeObj;
	void* poolData;
	size_t itemStride;

	void* maxPtr;

#ifdef _DEBUG
	const int BAD_MEM_VALUE = 0xDEADF00D;
#endif
};

inline void* operator new(std::size_t size, GenPool& pool)
{
	return pool.CreateObject();
}

inline void operator delete(void* p, GenPool& pool)
{
	pool.FreeObject(p);
}

#endif