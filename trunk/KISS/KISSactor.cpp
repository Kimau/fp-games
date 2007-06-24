#include "KISSactor.h"

static s_id = 100;

//=========================================================
//	Constructor
//=========================================================
kActor::kActor()
{
	
	m_id = ++s_id;

	m_energy = 0;
	x = -2.0f;
	y = 0.0f;

	m_comedy = 0.0f;
	m_drama = 0.0f;
	m_horror = 0.0f;
	m_love = 0.0f;
}