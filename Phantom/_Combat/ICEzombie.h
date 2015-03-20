#ifndef ZOMBIE_ICE_OF_DECAYING_FLESH_234
#define ZOMBIE_ICE_OF_DECAYING_FLESH_234

//---------------------------------------
//	Includes
//---------------------------------------
#include "ICEbase.h"
#include "..\OpenGL\Model3DS.h"
#include <vector>
#include <algorithm>

#define CI_ZOMBIE	5001
#define ZZ_DIST		4.0f
#define ZZ_POINTS	10
#define ZZ_DEG		0.02f
#define ZZ_TARSIZE	0.2f
#define ZZ_DMGDIV	50.0f

//------------------------------------------
//	Structs
//------------------------------------------
struct ZombiePoint
{
	float m_value;
	CVector3 m_Pos;
	CVector3 m_RotAxis;
};

//------------------------------------------
//	Helper Functions
//------------------------------------------
namespace ZombieHelper
{
	bool isDead(ZombiePoint& _point);
}

//------------------------------------------
//	Zombie ICE
// Render : Basic Graphic
// 3 Points : Non-Offensive
//------------------------------------------
class CIZombie : public comICE
{
public:
	//------------------------
	// Creation Functions
	//------------------------
	CIZombie(comPData* _player, comICEdata _ice);
	~CIZombie();
	//------------------------
	// Interface Functions
	//------------------------
	void Render();
	int Step(DWORD _delta);
	void Zap(int _type, float _value, CVector3 _vector);
	comICEdata Dead();

private:
	//------------------------
	//	Support Functions
	//------------------------
	void moveTargets(float _degs);
	//------------------------
	// Varibles
	//------------------------
	std::vector<ZombiePoint> m_Points;
	float m_Exposed;
	int m_targeted;
	GLuint m_list;
};

#endif