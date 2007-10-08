/**************************************************************************
*
* File:		PhManager.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "PhManager.h"

//////////////////////////////////////////////////
// Ctor
PhManager::PhManager()
{

}

//////////////////////////////////////////////////
// Dtor
PhManager::~PhManager()
{

}

//////////////////////////////////////////////////
// addObject
void PhManager::addObject( PhObject* pObject )
{
	BcAssert( pObject != NULL );
	ObjectList_.push_back( pObject );
}

//////////////////////////////////////////////////
// delObject
void PhManager::delObject( PhObject* pObject )
{
	BcAssert( pObject != NULL );

	PhObjectIterator Iter = ObjectList_.begin();
	PhObjectIterator IterEnd = ObjectList_.end();

	while( Iter != IterEnd )
	{
		if( (*Iter) == pObject )
		{
			Iter = ObjectList_.erase( Iter );
			break;
		}

		++Iter;
	}
}

//////////////////////////////////////////////////
// addField
void PhManager::addField( PhField* pField )
{
	BcAssert( pField != NULL );
	FieldList_.push_back( pField );
}

//////////////////////////////////////////////////
// delObject
void PhManager::delField( PhField* pField )
{
	BcAssert( pField != NULL );

	PhFieldIterator Iter = FieldList_.begin();
	PhFieldIterator IterEnd = FieldList_.end();

	while( Iter != IterEnd )
	{
		if( (*Iter) == pField )
		{
			Iter = FieldList_.erase( Iter );
			break;
		}

		++Iter;
	}
}

//////////////////////////////////////////////////
// delObject
void PhManager::step( BcReal TimeStep )
{
	PhObjectIterator Iter = ObjectList_.begin();
	PhObjectIterator IterEnd = ObjectList_.end();

	while( Iter != IterEnd )
	{
		(*Iter)->step( TimeStep );
		++Iter;
	}
}

//////////////////////////////////////////////////
// fieldDir
BcVec2d PhManager::fieldDir( const BcVec2d& Pos )
{
	PhFieldIterator Iter = FieldList_.begin();
	PhFieldIterator IterEnd = FieldList_.end();
	BcVec2d FieldDir;

	while( Iter != IterEnd )
	{
		FieldDir += (*Iter)->fieldDir( Pos );
		++Iter;
	}

	return FieldDir;
}
