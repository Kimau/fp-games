/**************************************************************************
*
* File:		PhObject.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "PhObject.h"

//////////////////////////////////////////////////
// Ctor
PhObject::PhObject()
{

}

//////////////////////////////////////////////////
// Dtor
PhObject::~PhObject()
{

}

//////////////////////////////////////////////////
// step
void PhObject::step( BcReal TimeStep )
{
	OldPos_ = Pos_;

	Vel_ += Acc_ * TimeStep;
	Pos_ += Vel_ * TimeStep;
}
