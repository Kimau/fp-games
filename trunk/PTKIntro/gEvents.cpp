#include "gEvents.h"
#include <queue>

static std::queue<gEvent> s_EventQ;

//#################################
//#################################
//			CONSTRUCTOR
//#################################
//#################################
//=============================================================================
//	Constructor
//=============================================================================
gEventManager::gEventManager()
{
}
//=============================================================================
//	Deconstructor
//=============================================================================
gEventManager::~gEventManager()
{
}

//#################################
//#################################
//			FUNCTIONS
//#################################
//#################################
//=============================================================================
//	Post Event
//=============================================================================
bool gEventManager::empty()
{
	return s_EventQ.empty();
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::post(gEvent _event)
{
	s_EventQ.push(_event);
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::post(EventType _type, 
						 int _v1, int _v2, int _v3, int _v4, 
						 int _v5, int _v6, int _v7, int _v8)
{
	gEvent _event;
	_event.m_type = _type;
	_event.m_buffer[0] = _v1;
	_event.m_buffer[1] = _v2;
	_event.m_buffer[2] = _v3;
	_event.m_buffer[3] = _v4;
	_event.m_buffer[4] = _v5;
	_event.m_buffer[5] = _v6;
	_event.m_buffer[6] = _v7;
	_event.m_buffer[7] = _v8;
	s_EventQ.push(_event);
}
//=============================================================================
//	Peek at the next Event
//=============================================================================
gEvent gEventManager::peek() const
{
	if(s_EventQ.empty())
	{
		gEvent _event;
		_event.m_type = GEM_NULL;
		return _event;
	}

	return s_EventQ.front();
}
//=============================================================================
//	Pop off event
//=============================================================================
gEvent gEventManager::pop()
{
	if(s_EventQ.empty())
	{
		gEvent _event;
		_event.m_type = GEM_NULL;
		return _event;
	}

	gEvent _top = s_EventQ.front();
	s_EventQ.pop();
	return _top;
}