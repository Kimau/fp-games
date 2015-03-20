#ifndef COMMAND_QUEUE_WHERE_THINGS_HANG_OUT_FOR_A_WHILE
#define COMMAND_QUEUE_WHERE_THINGS_HANG_OUT_FOR_A_WHILE

#include "Structs.h"

class JWL_CmdQ
{
public:
	//-----------------------------------
	// System Functions
	//-----------------------------------
	JWL_CmdQ();
	~JWL_CmdQ();
	void Flush();
	bool isEmpty();
	//-----------------------------------
	// Command Handling
	//-----------------------------------
public:
	void addCMD(UINT _TeamID, UINT _UnitID, UINT _CmdID, UINT _x, UINT _y, UINT _z, UINT _var);
	void addCMD(JWL_CMD _cmd);
	JWL_CMD popCMD();
private:
	Q_CMD* m_Oldest;		// Oldest Command in Queue
	Q_CMD* m_Newest;		// Newest Command in Queue
};

#endif