#ifndef ICE_OBJECT_BASE
#define ICE_OBJECT_BASE

//---------------------------------
//	Includes
//---------------------------------
#include "..\glHeader.h"

//---------------------------------
//	Structs
//---------------------------------
struct icProject
{
	int m_type;
	CVector3 m_Pos;
	CVector3 m_Move;
	CVector3 m_Accel;
};

//-----------------------------------
//	Base ICE Object
//-----------------------------------
class icObject
{
public:
	//------------------------
	//	Interface Functions
	//------------------------
	virtual bool Collide(icProject& _proj);
	virtual void Boom(icProject& _proj);
	virtual void Update();
	virtual void Render();

	//------------------------
	//	Varibels
	//------------------------
	int m_id;
	int m_type;
	CVector3 m_Size;
	CVector3 m_Pos;
	CVector3 m_Move;
	CVector3 m_Acc;
};

#endif