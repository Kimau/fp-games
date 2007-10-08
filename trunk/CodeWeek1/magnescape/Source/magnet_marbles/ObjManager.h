/**************************************************************************
*
* File:		ObjManager.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

#include "BcSingleton.h"
#include "BcVec2d.h"

#include "GaMap.h"
#include "ObjBase.h"

#include <list>

//////////////////////////////////////////////////
// ObjManager
class ObjManager:
	public BcSingleton< ObjManager >
{
public:
	ObjManager();
	~ObjManager();

	void addObject( ObjBase* pObject );
	void delObject( ObjBase* pObject );

	GaMap* pMap();
	void pMap( GaMap* pMap );

	void update();
	void render( const BcVec2d& WorldPos );

	ObjBase* testCollision( ObjBase* pIgnore, const BcVec2d& Position, BcReal Radius, BcVec2d* Normal );

	void reset();

private:
	typedef std::list< ObjBase* > ObjBaseList;
	typedef ObjBaseList::iterator ObjBaseIterator;

	ObjBaseList ObjectList_;

	GaMap* pMap_;
};

//////////////////////////////////////////////////
// Inlines
inline GaMap* ObjManager::pMap()
{
	return pMap_;
}

inline void ObjManager::pMap( GaMap* pMap )
{
	pMap_ = pMap;
}

inline void ObjManager::reset()
{
	ObjectList_.clear();
}

#endif