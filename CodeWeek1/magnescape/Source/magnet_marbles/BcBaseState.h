/**************************************************************************
*
* File:		BcBaseState.h
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __BCBASESTATE_H__
#define __BCBASESTATE_H__

#include "BcTypes.h"

//////////////////////////////////////////////////
// BcBaseState
class BcBaseState
{
public:
	BcBaseState();
	virtual ~BcBaseState();

	void processExecute();
	void processRender();

	void spawnState( BcBaseState* pState, BcBool bInhibit );

public:
	BcBaseState* pChild();
	BcBaseState* pParent();

	virtual void execute() = 0;
	virtual void render() = 0;

private:
	BcBaseState* pChild_;
	BcBaseState* pParent_;

	BcBaseState* pPendingChild_;

	BcBool bInhibit_;
};

//////////////////////////////////////////////////
// Inlines
inline BcBaseState* BcBaseState::pChild()
{
	return pChild_;
}

inline BcBaseState* BcBaseState::pParent()
{
	return pParent_;
}


#endif