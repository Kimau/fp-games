/**************************************************************************
*
* File:		PhObject.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __PHOBJECT_H__
#define __PHOBJECT_H__

#include "BcTypes.h"
#include "BcVec2d.h"

//////////////////////////////////////////////////
// PhObject
class PhObject
{
public:
	PhObject();
	virtual ~PhObject();

	void step( BcReal TimeStep );

	BcVec2d Pos_;
	BcVec2d OldPos_;
	BcVec2d Vel_;
	BcVec2d Acc_;


};

#endif
