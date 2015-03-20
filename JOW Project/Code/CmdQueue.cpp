#include "CmdQueue.h"

//####################################################################################################
//####################################################################################################
//									System Functions
//####################################################################################################
//####################################################################################################
//===================================================================
//		Constructor
//===================================================================
JWL_CmdQ::JWL_CmdQ()
{
	m_Newest = 0;
	m_Oldest = 0;
}
//===================================================================
//		Deconstructor
//===================================================================
JWL_CmdQ::~JWL_CmdQ()
{
	Flush();
}
//===================================================================
//			Flush Out Queue
//===================================================================
void JWL_CmdQ::Flush()
{
	if(m_Oldest == 0)
		return;

	// Walk Until no More place to walk
	while(m_Oldest->m_next)
	{
		// Delete as you go
		Q_CMD* _old = m_Oldest;
		m_Oldest = _old->m_next;
		delete _old;
	}
	// Delete Final Step
	delete m_Oldest;
	m_Oldest = 0;
}
//===================================================================
//    Is the Queue Empty
//===================================================================
bool JWL_CmdQ::isEmpty()
{
	return((m_Oldest == 0) && (m_Newest == 0));
}
//####################################################################################################
//####################################################################################################
//										Command Handling
//####################################################################################################
//####################################################################################################
//===================================================================
//    Add A command to the list
//===================================================================
void JWL_CmdQ::addCMD(UINT _TeamID, UINT _UnitID, UINT _CmdID, UINT _x, UINT _y, UINT _z, UINT _var)
{
	JWL_CMD _new = {_TeamID, _UnitID, _CmdID, _x, _y, _z, _var};
	addCMD(_new);
}
//===================================================================
//    Add a Command to the list
//===================================================================
void JWL_CmdQ::addCMD(JWL_CMD _cmd)
{
	// Setup New Qcommand
	Q_CMD* _new = new Q_CMD;
	_new->m_next = 0;
	_new->m_cmd = _cmd;

	// Check for Empty List
	if(m_Newest == 0)
	{
		m_Newest = _new;
		m_Oldest = _new;
	}
	else
	{
		// Add to List
		m_Newest->m_next = _new;
		m_Newest = _new;
	}
}
//===================================================================
//   Pop (Fetch and Remove) a command out of the list
//===================================================================
JWL_CMD JWL_CmdQ::popCMD()
{
	if(m_Oldest == 0)
	{
		JWL_CMD _temp = {0};
		return _temp;
	}
	// Get Command
	JWL_CMD _temp = m_Oldest->m_cmd;

	// Move Queue Along
	if(m_Oldest == m_Newest)
	{
		delete m_Oldest;
		m_Oldest = 0;
		m_Newest = 0;
	}
	else
	{
		Q_CMD* _old = m_Oldest;
		m_Oldest = _old->m_next;
		delete _old;
	}

	// Return Command
	return _temp;
}