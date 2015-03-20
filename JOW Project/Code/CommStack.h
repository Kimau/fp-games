#ifndef COMMAND_STACK_WHERE_THINGS_HANG_OUT_FOR_A_WHILE
#define COMMAND_STACK_WHERE_THINGS_HANG_OUT_FOR_A_WHILE

struct JWL_CMD
{
	UINT m_Team;	// ID specifiying Team
	UINT m_Unit;	// ID specifiying Unit (100 Refers to Team Commands)
	UINT m_Cmd;		// ID specifying Command Type
	UINT x;			// X Position (optional)
	UINT y;			// Y Position (optional)
	UINT z;			// Z Position (optional)
	UINT var;		// Holder for any variable Information (optional)
};

struct STACK_CMD
{

	JWL_CMD* m_next;	// Next Command in Chain
};

class JWL_CMDStack
{
public:
	JWL_CMDStack();
	~JWL_CMDStack();
private:
};

#endif