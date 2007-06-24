#include "KISSproto.h"

static kActionList s_Timeline;

//####################################
//####################################
//		Starters and Stopers
//####################################
//####################################
//==========================================================================
//	Constructor
//==========================================================================
KISSproto::KISSproto()
{
	memset(this,0,sizeof(KISSproto));
	m_Select = -1;
	m_State = GS_SETUP;
	m_Curr = s_Timeline.end();
}
//==========================================================================
//	Deconstructor
//==========================================================================
KISSproto::~KISSproto()
{
	if(m_Cast)
	{
		delete[] m_Cast;
	}
}
//==========================================================================
//	Setup Game
//==========================================================================
GameState KISSproto::Setup(unsigned int _castSize)
{
	//-----------------------------
	//	Setup Cast
	//-----------------------------
	m_CastSize = _castSize;	
	if(m_Cast)
	{
		delete[] m_Cast;
	}
	m_Cast = new kActor[_castSize];
	m_Select = getActorID(0);
	m_EventQ.post(OUTBOX, GEM_SETUPDONE, m_CastSize);
	
	//-----------------------------
	//	Setup Timeline
	//-----------------------------
	memset(&m_Next, 0, sizeof(kAction));
	s_Timeline.clear();
	postAction(0, KA_START, 5);
	m_Curr = s_Timeline.begin();
	m_TimeLeft = m_Curr->m_time*GAME_SPEED;

	m_State = GS_PLAY;
	return m_State;
}
//==========================================================================
//	Update Current Game State
//==========================================================================
GameState KISSproto::Update(unsigned long _delta)
{
	m_Cast[0].y = (m_Cast[0].y + 0.005f);	if(m_Cast[0].y > 1.0f)		m_Cast[0].y = 0.0f;		// HACK :: TESTING
	m_Cast[1].x = (m_Cast[1].x + 0.005f);	if(m_Cast[1].x > 1.4f)		m_Cast[1].x = -1.4f;	// HACK :: TESTING

	//------------------------------------------
	//	Process Events
	//------------------------------------------
	while(m_EventQ.empty(INBOX) == false)
	{
		ProcessEvent(m_EventQ.pop(INBOX));
	}

	//------------------------------------------
	//	Update Timeline
	//------------------------------------------
	UpdateTimeLine(_delta);

	return m_State;
}
//==========================================================================
//	Update Time Line
//==========================================================================
void KISSproto::UpdateTimeLine(unsigned long _delta)
{
	//----------------------------------
	//	Building DEAD time
	//----------------------------------
	if(m_Curr->m_Action == KA_NULL)
	{
		m_TimeLeft += _delta;
		m_Curr->m_time = m_TimeLeft / GAME_SPEED;
	}
	//----------------------------------
	//	Still doing Action
	//----------------------------------
	else if(_delta < m_TimeLeft)
	{
		m_TimeLeft -= _delta;
	}
	//----------------------------------
	//	Complete Action
	//----------------------------------
	else
	{
		++m_Curr;
		//----------------------------------
		//	End of Timeline (for now)
		//----------------------------------
		if(m_Curr == s_Timeline.end())
		{
			m_TimeLeft = _delta;
			postAction(0);			// POST NULL
		}
		//----------------------------------
		//	Move to Next Action
		//----------------------------------
		else
		{
			m_TimeLeft = (m_Curr->m_time * GAME_SPEED) - _delta + m_TimeLeft;
		}
	}
}
//==========================================================================
//	Process Event
//==========================================================================
void KISSproto::ProcessEvent(gEvent& _event)
{
	switch(_event.m_type)
	{
		//--------------------------------
		//	Set Cast Member
		//--------------------------------
	case GEM_SET_ACTOR:
		{
			setActor(
				_event.m_int[0],_event.m_int[1],_event.m_int[2],
				_event.m_flt[0],_event.m_flt[1],_event.m_flt[2],_event.m_flt[3]);
		}
		break;
		//--------------------------------
		//	Move Cast Member
		//--------------------------------
	case GEM_MOVE_ACTOR:
		{
			unsigned int _index = getActorIndex(_event.m_int[0]);
			if( _index < m_CastSize)
			{
				m_Cast[_index].x = _event.m_flt[0];
				m_Cast[_index].y = _event.m_flt[1];
			}
		}
		break;
		//--------------------------------
		//	Move Cast Member
		//--------------------------------
	case GEM_SELECT:
		{
			m_Select = _event.m_int[0];
		}
		break;
		//--------------------------------
		//	Select what Action to Perform
		//--------------------------------
	case GEM_PERFORM:
		{
			unsigned int _index = getActorID(_event.m_int[0]);
			if( _index < m_CastSize)
			{
				m_Next.m_id = _event.m_int[0];
			}
			else
			{
				m_Next.m_id = m_Select;
				_index = getActorIndex(m_Select);
			}
			m_Next.m_Action = KAType(_event.m_int[1]);
			m_Next.m_time = 4;
			m_Next.x = m_Cast[_index].x;
			m_Next.y = m_Cast[_index].y;
			postAction(m_Next);
		}
		break;
		//--------------------------------
		//	Commit Next Action to Script
		//--------------------------------
	case GEM_COMMIT:
		postAction(m_Next);
		break;
	};
}

//==========================================================================
//	Add Actor
//==========================================================================
void KISSproto::setActor(unsigned int _id, int _energy = 0, unsigned int _type = 0, float _comedy = 1.0f, float _drama = 1.0f, float _horror = 1.0f, float _love = 1.0f)
{
	unsigned int _index = getActorIndex(_id);

	if( _index < m_CastSize)
	{
		m_Cast[_index].m_energy = _energy;
		m_Cast[_index].m_type = _type;
		m_Cast[_index].m_comedy = _comedy;
		m_Cast[_index].m_drama = _drama;
		m_Cast[_index].m_horror = _horror;
		m_Cast[_index].m_love = _love;
	}
}

//####################################
//####################################
//			Time Line
//####################################
//####################################
//==========================================================================
//	Post Action
//==========================================================================
void KISSproto::postAction(unsigned int _id, KAType _type, unsigned int _time, float _x, float _y)
{
	kAction _action;
	_action.m_id = _id;
	_action.m_Action = _type; 
	_action.m_time = _time;
	_action.x = _x;
	_action.y = _y;

	postAction(_action);
}
//==========================================================================
//	Post Action
//==========================================================================
void KISSproto::postAction(kAction _action)
{
	s_Timeline.push_back(_action);
	//------------------------------
	//	Currently in Deadtime
	//------------------------------
	if((m_Curr == s_Timeline.end()) || (m_Curr->m_Action == KA_NULL))
	{
		m_Curr = s_Timeline.end();	// could be NULL spot
		--m_Curr;
		m_TimeLeft = m_Curr->m_time * GAME_SPEED;
	}	
}

//####################################
//####################################
//			Accesors
//####################################
//####################################
//==========================================================================
//	Return State
//==========================================================================
GameState KISSproto::getState() const	{ return m_State; }
//==========================================================================
//	Return Cast Size
//==========================================================================
unsigned int KISSproto::getCastSize() const { return m_CastSize; }
//==========================================================================
//	Return Selected ID
//==========================================================================
int KISSproto::getSelected() const { return m_Select; }
//==========================================================================
//	Return Start of Script
//==========================================================================
kActionList::const_iterator KISSproto::getStart() const { return s_Timeline.begin(); }
//==========================================================================
//	Return Current Marker in Script
//==========================================================================
kActionList::const_iterator KISSproto::getCurr() const { return m_Curr; }
//==========================================================================
//	Return Current End of Script
//==========================================================================
kActionList::const_iterator KISSproto::getEnd() const  { return s_Timeline.end(); }
//==========================================================================
//	Return Time Left of Current Action
//==========================================================================
unsigned int KISSproto::getTimeLeft() const { return m_TimeLeft; }
//==========================================================================
//	Return Actor
//==========================================================================
kActor& KISSproto::getActor(unsigned int _index) const
{
	if(_index < m_CastSize)
		return m_Cast[_index];
	else
		return m_Cast[0];
}
//==========================================================================
//	Get Actor INDEX from ID
//==========================================================================
unsigned int KISSproto::getActorIndex(unsigned int _id) const
{
	for(unsigned int c = 0; c < m_CastSize; ++c)
	{
		if(m_Cast[c].ID() == _id)
			return c;
	}
	return m_CastSize + 10000;
}
//==========================================================================
//	Get Actor ID from INDEX
//==========================================================================
unsigned int KISSproto::getActorID(unsigned int _index) const
{
	if( _index < m_CastSize)
		return m_Cast[_index].ID();
	else
		return 0;
}