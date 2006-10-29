#ifndef GRAPHICAL_OBJECT_4353
#define GRAPHICAL_OBJECT_4353

#include "../GLHeader.h"
//----------------------------------
//	Graphical Objcet
//----------------------------------
class GOB
{
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual int Type() = 0;

	//----------------------
	//	Varibles
	//----------------------
	CVector3 m_Pos;		// Position
	CVector3 m_Move;	// Direction Vector
	CVector3 m_Acc;		// Acceleration Vector
};

#endif