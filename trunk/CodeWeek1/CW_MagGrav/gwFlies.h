#pragma once

#include "gwVector.h"

struct gwFlyer
{
	bool m_alive;
	gwVector m_pos;
	gwVector m_vel;
};

/*-----------------------------------------
		Fly Manager
	A little class that managers the flies
-----------------------------------------*/
class gwFlies
{
private:
	gwFlies();			// Constructor
	static gwFlies* s_instance;

	int m_state;
	unsigned int m_amount;
	gwFlyer* m_flies;
	gwFlyer m_nullFly;

public:
	static gwFlies* Instance() 
	{
		if(s_instance == 0) 
			s_instance = new gwFlies();	
		return s_instance;
	}

	~gwFlies();							// Deconstructor

	void Init(unsigned int _amount);	// Generate Swarm
	void Kill();						// Kill Swarm

	unsigned int Amount() const;			// Get Amount
	gwFlyer& getFly(unsigned int _index);	// Get Flyer
	gwFlyer* const getPtr();				// Get Pointer
};