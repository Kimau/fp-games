#ifndef ICE_MESSAGE_SYSTEM
#define ICE_MESSAGE_SYSTEM

//--------------------------
//	Includes
//--------------------------
#include <vector>
#include "icObject.h"

//-----------------------------------
//	Define
//-----------------------------------
#define ICEM_DELETE_PROJ		23

//-----------------------------------
//	Base Message
//-----------------------------------
struct icMsg
{
	int m_type;
	float m_value;
	icObject* _source;
	icObject* _dest;
};

//--------------------------------
//	Message Class
//--------------------------------
class icMessages
{
public:
	icMessages() {}
	~icMessages() {}

	std::vector<icMsg> m_Msgs;
};

#endif