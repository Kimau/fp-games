#ifndef HEADER_KISS_SCRIPT_23432
#define HEADER_KISS_SCRIPT_23432

//#include <list>

enum KAType {KA_NULL = 0, KA_START, KA_JOKE, KA_ANGRY};

//------------------------
//	Event
//------------------------
struct kAction
{
	KAType m_Action; 
	float x;
	float y;
	unsigned int m_id;
	unsigned int m_time;
};
/*
class kScript
{
public:
	kScript();
	~kScript();
private:
	unsigned int m_castSize;
	std::list<kAction> m_timeline;
};*/

#endif