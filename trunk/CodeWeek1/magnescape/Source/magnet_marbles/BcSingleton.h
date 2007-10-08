/**************************************************************************
*
* File:		BcSingleton.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __BCSINGLETON_H__
#define __BCSINGLETON_H__

#include "BcTypes.h"

//////////////////////////////////////////////////
// BcSingleton
template< class T >
class BcSingleton
{
public:
	virtual ~BcSingleton(){};

	static T* pInstance()
	{
		if( pInstance_ == NULL )
		{
			pInstance_ = new T();
		}

		return pInstance_;
	}

private:
	static T* pInstance_;
};

//////////////////////////////////////////////////
//
template< class T >
T* BcSingleton< T >::pInstance_ = NULL;

#endif