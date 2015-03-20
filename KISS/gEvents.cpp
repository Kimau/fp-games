#include "gEvents.h"
#include <queue>

static std::queue<gEvent> s_InboxQ;
static std::queue<gEvent> s_OutboxQ;

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
bool gEventManager::empty(bool _inbox) const
{
	if(_inbox)		return s_InboxQ.empty();		else		return s_OutboxQ.empty();
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::post(bool _inbox, gEvent _event)
{
	if(_inbox)		return s_InboxQ.push(_event);	else		return s_OutboxQ.push(_event);
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::post(bool _inbox, EventType _type, 
						 int _v1, int _v2, int _v3, int _v4, 
						 float _f1, float _f2, float _f3, float _f4)
{
	gEvent _event;
	_event.m_type = _type;
	_event.m_int[0] = _v1;
	_event.m_int[1] = _v2;
	_event.m_int[2] = _v3;
	_event.m_int[3] = _v4;
	_event.m_flt[0] = _f1;
	_event.m_flt[1] = _f2;
	_event.m_flt[2] = _f3;
	_event.m_flt[3] = _f4;

	if(_inbox)		return s_InboxQ.push(_event);	else		return s_OutboxQ.push(_event);
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::posti(bool _inbox, EventType _type, 
						 int _v1, int _v2, int _v3, int _v4)
{
	gEvent _event;
	_event.m_type = _type;
	_event.m_int[0] = _v1;
	_event.m_int[1] = _v2;
	_event.m_int[2] = _v3;
	_event.m_int[3] = _v4;
	_event.m_flt[0] = 0.0f;
	_event.m_flt[1] = 0.0f;
	_event.m_flt[2] = 0.0f;
	_event.m_flt[3] = 0.0f;

	if(_inbox)		return s_InboxQ.push(_event);	else		return s_OutboxQ.push(_event);
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::postf(bool _inbox, EventType _type, 
						 float _f1, float _f2, float _f3, float _f4)
{
	gEvent _event;
	_event.m_type = _type;
	_event.m_int[0] = 0;
	_event.m_int[1] = 0;
	_event.m_int[2] = 0;
	_event.m_int[3] = 0;
	_event.m_flt[0] = _f1;
	_event.m_flt[1] = _f2;
	_event.m_flt[2] = _f3;
	_event.m_flt[3] = _f4;

	if(_inbox)		return s_InboxQ.push(_event);	else		return s_OutboxQ.push(_event);
}
//=============================================================================
//	Post Event
//=============================================================================
void gEventManager::postif(bool _inbox, EventType _type, int _v1,
		float _f1, float _f2, float _f3, float _f4)
{
	gEvent _event;
	_event.m_type = _type;
	_event.m_int[0] = _v1;
	_event.m_int[1] = 0;
	_event.m_int[2] = 0;
	_event.m_int[3] = 0;
	_event.m_flt[0] = _f1;
	_event.m_flt[1] = _f2;
	_event.m_flt[2] = _f3;
	_event.m_flt[3] = _f4;

	if(_inbox)		return s_InboxQ.push(_event);	else		return s_OutboxQ.push(_event);
}

//=============================================================================
//	Peek at the next Event
//=============================================================================
gEvent gEventManager::peek(bool _inbox) const
{
	if(empty(_inbox))
	{
		gEvent _event;
		_event.m_type = GEM_NULL;
		return _event;
	}

	if(_inbox)		return s_InboxQ.front();	else		return s_OutboxQ.front();
}
//=============================================================================
//	Pop off event
//=============================================================================
gEvent gEventManager::pop(bool _inbox)
{
	if(empty(_inbox))
	{
		gEvent _event;
		_event.m_type = GEM_NULL;
		return _event;
	}

	if(_inbox)
	{
		gEvent _top = s_InboxQ.front();
		s_InboxQ.pop();
		return _top;
	}
	else
	{
		gEvent _top = s_OutboxQ.front();
		s_OutboxQ.pop();
		return _top;
	}
}