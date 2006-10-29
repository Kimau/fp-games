#ifndef BGL_HELPERS_OF_NOTE_2343
#define BGL_HELPERS_OF_NOTE_2343

//--------------------------------------
//	Header Includes
//--------------------------------------
#include "BaseGL.h"
#include "..\Math\CVector2.h"
#include "..\Math\CVector3.h"

namespace BGLHelper
{
	void Billboard();
	CVector3 UnProject(CVector2 _point);

	void DrawCube(float _size);
};


#endif