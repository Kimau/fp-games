/**************************************************************************
*
* File:		PhField.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __PHFIELD_H__
#define __PHFIELD_H__

#include "BcVec2d.h"

//////////////////////////////////////////////////
// PhField
class PhField
{
public:
	PhField();
	~PhField();

	BcVec2d fieldDir( const BcVec2d& Pos );

	BcReal Mag_;
	BcVec2d Ori_;
	BcVec2d Pos_;
};


#endif

