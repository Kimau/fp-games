/**************************************************************************
*
* File:		PhField.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "PhField.h"

//////////////////////////////////////////////////
// Ctor
PhField::PhField():
	Mag_( 1.0f )
{

}

//////////////////////////////////////////////////
// Dtor
PhField::~PhField()
{
	
}

//////////////////////////////////////////////////
// Ctor
BcVec2d PhField::fieldDir( const BcVec2d& Pos )
{
	// Vector to position
	BcVec2d Delta = Pos - Pos_;
	BcVec2d Vec = Delta;
	Vec.normalise();

	// 
	BcReal Mag = Vec.dot( Ori_ );

	//
	return ( ( Vec * Mag ) / Delta.magnitude() ) * Mag_;
}

