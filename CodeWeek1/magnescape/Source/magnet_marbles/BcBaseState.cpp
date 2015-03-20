/**************************************************************************
*
* File:		BcBaseState.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "BcBaseState.h"

//////////////////////////////////////////////////
// Ctor
BcBaseState::BcBaseState():
	pChild_( NULL ),
	pParent_( NULL ),
	pPendingChild_( NULL ),
	bInhibit_( BcFalse )
{

}

//////////////////////////////////////////////////
// Dtor
BcBaseState::~BcBaseState()
{
	if( pChild_ != NULL )
	{
		delete pChild_;
		pChild_ = NULL;
	}
}

//////////////////////////////////////////////////
// processExecute
void BcBaseState::processExecute()
{
	if( pPendingChild_ != NULL )
	{
		delete pChild_;
		pChild_ = pPendingChild_;
		pPendingChild_ = NULL;
	}

	if( bInhibit_ == BcFalse )
	{
		execute();
	}

	if( pChild_ != NULL )
	{
		pChild_->processExecute();
	}
}

//////////////////////////////////////////////////
// processRender
void BcBaseState::processRender()
{
	render();

	if( pChild_ != NULL )
	{
		pChild_->processRender();
	}
}

//////////////////////////////////////////////////
// spawnState
void BcBaseState::spawnState( BcBaseState* pState, BcBool bInhibit )
{
	bInhibit_ = bInhibit;

	if( pPendingChild_ != NULL )
	{
		delete pPendingChild_;
		pPendingChild_ = NULL;
	}

	pPendingChild_ = pState;
	pPendingChild_->pParent_ = this;
}
