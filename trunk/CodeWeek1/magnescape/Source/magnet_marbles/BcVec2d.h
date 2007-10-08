/**************************************************************************
*
* File:		BcVec2d.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __BCVEC2D_H__
#define __BCVEC2D_H__

#include "BcTypes.h"

#include <math.h>

//////////////////////////////////////////////////
// BcVec2d
class BcVec2d
{
public:
	BcVec2d();
	BcVec2d( BcReal X, BcReal Y );
	~BcVec2d();

	BcVec2d operator + ( const BcVec2d& Other ) const;
	BcVec2d operator - ( const BcVec2d& Other ) const;

	BcVec2d& operator += ( const BcVec2d& Other );
	BcVec2d& operator -= ( const BcVec2d& Other );

	BcVec2d operator * ( const BcVec2d& Other ) const;
	BcVec2d operator / ( const BcVec2d& Other ) const;

	BcVec2d operator * ( BcReal Other ) const;
	BcVec2d operator / ( BcReal Other ) const;

	BcVec2d operator - () const;

	BcReal dot( const BcVec2d& Other ) const;
	BcReal magnitude() const;
	void normalise();

	BcReal X_;
	BcReal Y_;
};

//////////////////////////////////////////////////
// Inlines
inline BcVec2d::BcVec2d():
	X_( 0.0f ),
	Y_( 0.0f )
{

}

inline BcVec2d::BcVec2d( BcReal X, BcReal Y ):
	X_( X ),
	Y_( Y )
{

}

inline BcVec2d::~BcVec2d()
{

}

inline BcVec2d BcVec2d::operator + ( const BcVec2d& Other ) const
{
	return BcVec2d( X_ + Other.X_, Y_ + Other.Y_ );
}

inline BcVec2d BcVec2d::operator - ( const BcVec2d& Other ) const
{
	return BcVec2d( X_ - Other.X_, Y_ - Other.Y_ );
}

inline BcVec2d& BcVec2d::operator += ( const BcVec2d& Other )
{
	X_ = X_ + Other.X_;
	Y_ = Y_ + Other.Y_;

	return (*this);
}

inline BcVec2d& BcVec2d::operator -= ( const BcVec2d& Other )
{
	X_ = X_ - Other.X_;
	Y_ = Y_ - Other.Y_;

	return (*this);}

inline BcVec2d BcVec2d::operator * ( const BcVec2d& Other ) const
{
	return BcVec2d( X_ * Other.X_, Y_ * Other.Y_ );
}

inline BcVec2d BcVec2d::operator / ( const BcVec2d& Other ) const
{
	return BcVec2d( X_ / Other.X_, Y_ / Other.Y_ );
}

inline BcVec2d BcVec2d::operator * ( BcReal Other ) const
{
	return BcVec2d( X_ * Other, Y_ * Other );
}

inline BcVec2d BcVec2d::operator / ( BcReal Other ) const
{
	return BcVec2d( X_ / Other, Y_ / Other );
}

inline BcVec2d BcVec2d::operator - () const
{
	return BcVec2d( -X_, -Y_ );
}

inline BcReal BcVec2d::dot( const BcVec2d& Other ) const
{
	return ( X_ * Other.X_ ) + ( Y_ * Other.Y_ );
}

inline BcReal BcVec2d::magnitude() const
{
	return sqrtf( ( X_ * X_ ) + ( Y_ * Y_ ) );
}

inline void BcVec2d::normalise()
{
	const BcReal Mag = magnitude();

	if( Mag > 0.0f )
	{
		const BcReal InvMag = 1.0f / Mag;
		X_ = X_ * InvMag;
		Y_ = Y_ * InvMag;
	}
}

#endif
