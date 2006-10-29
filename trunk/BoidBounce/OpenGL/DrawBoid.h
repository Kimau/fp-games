#ifndef DRAW_THE_SMALL_LITTLE_AND_EXTREMLY_CUTE_BOIDS_555
#define DRAW_THE_SMALL_LITTLE_AND_EXTREMLY_CUTE_BOIDS_555

//===============================================================
//	Header Includes
//===============================================================
#include "BaseGl.h"
#include "..\Flock\CFlock.h"

//===============================================================
//	Draw Boids
//===============================================================
class BGLDrawBoid
{
	//----------------------------------
	//	Initialsation Functions
	//----------------------------------
public:
	BGLDrawBoid();
	BGLDrawBoid(const WinGLengine& _engine, const bFlock3& _flock);
	~BGLDrawBoid();
	//----------------------------------
	//	Render Functions
	//----------------------------------
	void Render();
	//----------------------------------
	//	Varibles
	//----------------------------------
private:
	const bFlock3* m_flock;
	GLUquadricObj* m_quadric;
};

#endif
