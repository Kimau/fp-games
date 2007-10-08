/**************************************************************************
*
* File:		PhManager.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __PHMANAGER_H__
#define __PHMANAGER_H__

#include "BcSingleton.h"
#include "PhObject.h"
#include "PhField.h"

#include <list>

///////////////////////////////////////////////////
// PhManager
class PhManager:
	public BcSingleton< PhManager >
{
public:
	PhManager();
	virtual ~PhManager();

	void addObject( PhObject* pObject );
	void delObject( PhObject* pObject );
	
	void addField( PhField* pField );
	void delField( PhField* pField );
	
	void step( BcReal TimeStep );

	BcVec2d fieldDir( const BcVec2d& Pos );

	void reset();

private:
	typedef std::list< PhObject* > PhObjectList;
	typedef PhObjectList::iterator PhObjectIterator;

	typedef std::list< PhField* > PhFieldList;
	typedef PhFieldList::iterator PhFieldIterator;

	PhObjectList	ObjectList_;
	PhFieldList		FieldList_;
};

//////////////////////////////////////////////////
// Inlines
inline void PhManager::reset()
{
	ObjectList_.clear();
	FieldList_.clear();
}

#endif
