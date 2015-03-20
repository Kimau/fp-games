/**************************************************************************
*
* File:		ObjManager.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "ObjManager.h"

#include "PhObject.h"

//////////////////////////////////////////////////
// Ctor
ObjManager::ObjManager()
{

}

//////////////////////////////////////////////////
// Dtor
ObjManager::~ObjManager()
{

}

//////////////////////////////////////////////////
// addObject
void ObjManager::addObject( ObjBase* pObject )
{
	BcAssert( pObject != NULL );
	ObjectList_.push_back( pObject );
}

//////////////////////////////////////////////////
// delObject
void ObjManager::delObject( ObjBase* pObject )
{
	BcAssert( pObject != NULL );
	ObjBaseIterator Iter = ObjectList_.begin();
	ObjBaseIterator IterEnd = ObjectList_.end();

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
// update
void ObjManager::update()
{
	ObjBaseIterator Iter = ObjectList_.begin();
	ObjBaseIterator IterEnd = ObjectList_.end();

	while( Iter != IterEnd )
	{
		if( (*Iter)->bAlive_ )
		{
			(*Iter)->update();

			++Iter;
		}
		else
		{
			ObjBase* pDelete = (*Iter);
			Iter = ObjectList_.erase( Iter );
			delete pDelete;
		}
	}
}

//////////////////////////////////////////////////
// render
void ObjManager::render( const BcVec2d& WorldPos )
{
	ObjBaseIterator Iter = ObjectList_.begin();
	ObjBaseIterator IterEnd = ObjectList_.end();

	while( Iter != IterEnd )
	{
		if( (*Iter)->bAlive_ )
		{
			(*Iter)->render( WorldPos - BcVec2d( 192.0f, 0.0f ) );
		}

		++Iter;
	}
}

//////////////////////////////////////////////////
// testCollision
ObjBase* ObjManager::testCollision( ObjBase* pIgnore, const BcVec2d& Position, BcReal Radius, BcVec2d* pNormal )
{
	ObjBaseIterator Iter = ObjectList_.begin();
	ObjBaseIterator IterEnd = ObjectList_.end();

	while( Iter != IterEnd )
	{
		if( (*Iter) != pIgnore && (*Iter)->bAlive_ )
		{
			PhObject* pObject = dynamic_cast< PhObject* >( (*Iter) );

			// Collide physics objects
			if( pObject != NULL )
			{
				//
				BcReal Distance = ( Position - pObject->Pos_ ).magnitude();

				if( Distance < ( (*Iter)->Radius_ + Radius ) )
				{
					// Collision
					BcVec2d Normal = Position - pObject->Pos_; 
					Normal.normalise();

					if( pNormal )
					{
						*pNormal = Normal;
					}

					return (*Iter);
				}
			}
		}

		++Iter;
	}
}
