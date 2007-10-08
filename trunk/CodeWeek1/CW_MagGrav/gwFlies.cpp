#include "gwFlies.h"
#include <string.h>

gwFlies* gwFlies::s_instance = 0;

//==========================================================================
// Constructor
//==========================================================================
gwFlies::gwFlies() : m_amount(0), m_state(-1), m_flies(0)
{
	memset(&m_nullFly,0,sizeof(gwFlyer));
}

//==========================================================================
// Deconstructor
//==========================================================================
gwFlies::~gwFlies() 
{
	if(m_state > 0)
	{
		Kill();
	}
}

//==========================================================================
// Generate Swarm
//==========================================================================
void gwFlies::Init(unsigned int _amount)
{
	if(m_state < 0)
	{
		m_state = 1;
		m_amount = _amount;
		m_flies = new gwFlyer[m_amount];
		memset(m_flies,0,sizeof(gwFlyer)*m_amount);
	}
}

//==========================================================================
// Kill Swarm
//==========================================================================
void gwFlies::Kill()
{
	if(m_state > 0)
	{
		delete[] m_flies;
		m_amount = 0;
		m_flies = 0;
		m_state = -1;
	}
}

//==========================================================================
// Get Amount
//==========================================================================
unsigned int gwFlies::Amount() const
{
	return m_amount;
}

//==========================================================================
// Get Flyer
//==========================================================================
gwFlyer& gwFlies::getFly(unsigned int _index)
{
	if((m_state > 0) && (_index < m_amount))
	{
		return m_flies[_index];
	}

	return m_nullFly;
}

//==========================================================================
// Get Pointer to Flyers
//==========================================================================
gwFlyer* const gwFlies::getPtr()
{
	return m_flies;
}