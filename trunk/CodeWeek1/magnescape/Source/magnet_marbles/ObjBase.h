/**************************************************************************
*
* File:		ObjBase.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __OBJBASE_H__
#define __OBJBASE_H__

//////////////////////////////////////////////////
// ObjBase
class ObjBase
{
public:
	ObjBase();
	virtual ~ObjBase();

	virtual void update() = 0;
	virtual void render( const BcVec2d& WorldPos ) = 0;

	virtual void damage(){};

	BcReal Radius_;
	BcBool bAlive_;

};

//////////////////////////////////////////////////
// Inlines
inline ObjBase::ObjBase():
	Radius_( 16.0f ),
	bAlive_( BcTrue )
{

}

inline ObjBase::~ObjBase()
{

}


#endif

