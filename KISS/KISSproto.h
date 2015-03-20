#ifndef HEADER_KISS_PROTO_HEADER_2343
#define HEADER_KISS_PROTO_HEADER_2343

//------------------------
//	Includes
//------------------------
#include "gEvents.h"
#include "KISSactor.h"
#include "KISSscript.h"
#include <list>

#define	GAME_SPEED	500		// Smaller Number is faster

typedef std::list<kAction> kActionList;

enum GameState {GS_NULL = 0, GS_SETUP, GS_PLAY, GS_GAMEOVER};

//------------------------
//	KEEP IT SIMPLE STUPID :: PROTO
//------------------------
class KISSproto
{
public:
	//-----------------------
	//	Starters and Stopers
	//-----------------------
	KISSproto();
	~KISSproto();
	GameState Setup(unsigned int _castSize);
	GameState Update(unsigned long _delta);

	//-----------------------
	//	Accesors
	//-----------------------
	GameState getState() const;
	unsigned int getCastSize() const;
	int getSelected() const;
	kActor& getActor(unsigned int _index) const;
	unsigned int getActorIndex(unsigned int _id) const;
	unsigned int getActorID(unsigned int _index) const;
	kActionList::const_iterator getStart() const;
	kActionList::const_iterator getCurr() const;
	kActionList::const_iterator getEnd() const;
	unsigned int getTimeLeft() const;

private:
	//-----------------------
	//	Update Functions
	//-----------------------
	void ProcessEvent(gEvent& _event);
	void UpdateTimeLine(unsigned long _delta);
	//-----------------------
	//	Timeline Functions
	//-----------------------
	void postAction(unsigned int _id, KAType _type = KA_NULL, unsigned int _time = 0, float _x = -1000.0f, float _y = -1000.0f);
	void postAction(kAction _action);
	//-----------------------
	//	Actor Functions
	//-----------------------	
	void setActor(unsigned int _id, int _energy, unsigned int _type, float _comedy, float _drama, float _horror, float _love);
	//-----------------------
	//	Varibles
	//-----------------------
	GameState m_State;
	gEventManager m_EventQ;
	unsigned int m_CastSize;
	int m_Select;
	kActor* m_Cast;
	kAction m_Next;
	kActionList::iterator m_Curr;
	unsigned long m_TimeLeft;
};

#endif