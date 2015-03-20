#ifndef HEADER_GAME_EVENT_MANAGER_2345643
#define HEADER_GAME_EVENT_MANAGER_2345643

enum EventType{ GEM_NULL, GEM_SHOOT};

struct gEvent
{
	EventType m_type;
	int m_buffer[8];
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
	bool empty();
	void post(gEvent _event);
	void post(EventType _type, 
		int _v1 = 0, int _v2 = 0, int _v3 = 0, int _v4 = 0,
		int _v5 = 0, int _v6 = 0, int _v7 = 0, int _v8 = 0);
	gEvent peek() const;
	gEvent pop();
};

#endif