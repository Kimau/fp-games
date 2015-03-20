#ifndef HEADER_KISS_ACTOR_454354
#define HEADER_KISS_ACTOR_454354

//------------------------
//	Actor
//------------------------
class kActor
{
public:
	kActor();
	~kActor()	{}
	unsigned int ID() const { return m_id ;}

	int m_energy;			// Energy Level
	unsigned int m_type;	// Actor Type (Mostly Graphical)
	float x,y;				// Stage is [-1:1]
	
	// -- Skills --
	float m_comedy;
	float m_drama;
	float m_horror;
	float m_love;

private:
	unsigned int m_id;
};

#endif