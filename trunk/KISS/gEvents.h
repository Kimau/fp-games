#ifndef HEADER_GAME_EVENT_MANAGER_2345643
#define HEADER_GAME_EVENT_MANAGER_2345643

enum EventType{GEM_NULL, GEM_SET_ACTOR, GEM_MOVE_ACTOR, GEM_SELECT, GEM_COMMIT, GEM_SETUPDONE, GEM_PERFORM};
#define INBOX true
#define OUTBOX false

struct gEvent
{
	EventType m_type;
	int m_int[4];
	float m_flt[4];
};

/*-------------------------------------------
	Handles Game Events
-------------------------------------------*/
class gEventManager
{
public:
	//-----------------------------
	//	Constructor
	//-----------------------------
	gEventManager();
	~gEventManager();

	//-----------------------------
	//	Functions
	//-----------------------------
	bool empty(bool _inbox) const;
	void post(bool _inbox, gEvent _event);
	void post(bool _inbox, EventType _type, 
		int _v1 = 0, int _v2 = 0, int _v3 = 0, int _v4 = 0,
		float _f1 = 0, float _f2 = 0, float _f3 = 0, float _f4 = 0);
	void posti(bool _inbox, EventType _type, 
		int _v1 = 0, int _v2 = 0, int _v3 = 0, int _v4 = 0);
	void postf(bool _inbox, EventType _type, 
		float _f1 = 0, float _f2 = 0, float _f3 = 0, float _f4 = 0);
	void postif(bool _inbox, EventType _type, int _v1 = 0,
		float _f1 = 0, float _f2 = 0, float _f3 = 0, float _f4 = 0);
	gEvent peek(bool _inbox) const;
	gEvent pop(bool _inbox);
};

#endif