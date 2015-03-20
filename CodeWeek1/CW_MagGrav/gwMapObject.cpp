#include "gwMapObject.h"
#include "gwFlies.h"
#include <stdlib.h>

//======================================================================
//	Init Spawner
//======================================================================
void gwSpawner::Init()
{
	m_count = 0;
	m_spawned = 0;
	m_alive = true;
}

//======================================================================
//	Init Hose
//======================================================================
void gwHome::Init()
{
	m_housed = 0;
	m_alive = true;
}


//======================================================================
//	Spawn Flies
//======================================================================
void gwSpawner::Update(unsigned long _delta) 
{
	//------------------------------------
	//	Spawn till Limit Reached
	//------------------------------------
	if(m_spawned < m_max)
	{
		m_count += _delta / 1000;
		//------------------------------------
		//	Spawn till Quota reach for Peroid
		//------------------------------------
		while(m_count > m_delay)
		{
			gwFlies* s_Flies = gwFlies::Instance();
			for(unsigned int _flyID = 0; _flyID < s_Flies->Amount(); ++_flyID)
			{
				//--------------------------------------
				//	Get Fly check Alive
				//--------------------------------------
				gwFlyer& _fly = s_Flies->getFly(_flyID);
				if(_fly.m_alive == false)
				{
					_fly.m_alive = true;
					_fly.m_pos.x = m_pos.x + float((rand() % 100) - 50) * 0.1f;			// Add a little margin to avoid overflow
					_fly.m_pos.y = m_pos.y + float((rand() % 100) - 50) * 0.1f;

					++m_spawned;		// Increment Spawn Count
					break;
				}
			}
			m_count -= m_delay;
		}
	}
	else
	{
		m_alive = false;
	}
}

//-------------------------------------
//	Houses Flyers
//-------------------------------------
void gwHome::Update(unsigned long _delta)
{
	//------------------------------------
	//	Collect Tenats till Full
	//------------------------------------
	if(m_housed < m_max)
	{
		gwFlies* s_Flies = gwFlies::Instance();
		for(unsigned int _flyID = 0; _flyID < s_Flies->Amount(); ++_flyID)
		{
			//--------------------------------------
			//	Get Fly check Alive
			//--------------------------------------
			gwFlyer& _fly = s_Flies->getFly(_flyID);
			if(_fly.m_alive == true)
			{
				//--------------------------------------
				//	Check if in Home Range
				//	Size could be positive or neg
				//--------------------------------------
				if((_fly.m_pos - m_pos).SqrLength() < (m_size*m_size))
				{
					_fly.m_alive = false;
					++m_housed;
				}
			}
		}
	}
	else
	{
		m_alive = false;
	}
}