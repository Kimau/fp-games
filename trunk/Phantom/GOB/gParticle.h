#ifndef GOB_PARTICLE_OBJECT_3454
#define GOB_PARTICLE_OBJECT_3454
//----------------------------------
//	Includes
//----------------------------------
#include "GOB.h"

//----------------------------------
//	Defines
//----------------------------------
#define GOBPARTICLE_ID		2
#define GOBPARTICLE_SPEED	200.0f

//------------------------------------------
//	GOB Matrix
//------------------------------------------
class GOBParticle : public GOB
{
public:
	GOBParticle();
	~GOBParticle();

	void Render();
	void Update();
	int  Type();

	unsigned long m_id;
	unsigned long m_life;
};

#endif