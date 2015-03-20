#ifndef INTRUDER_COUNTER_MEASURE_BASIC_FRAME_23
#define INTRUDER_COUNTER_MEASURE_BASIC_FRAME_23

//------------------------------------------
//	Includes
//------------------------------------------
#include "..\GLHeader.h"

//------------------------------------------
//	Structs
//------------------------------------------
struct comPData
{
	float m_Offense;	// The ability to deal damage
	float m_Convert;	// The ability to comprimise
	float m_Defense;	// The ability to prevent offensive damage
	float m_Stealth;	// The ability to conceal
	float m_Scan;		// The ability to detect

	float m_Health;		// Player Health
	float m_Shield;		// Player Shield

	CVector3 m_Pos;		// Player Position
	CVector3 m_Move;	// Player Movement
	CVector3 m_Turret;	// Turret Direction
};

struct comICEdata
{
	int m_ID;			// A unique Identifer
	int m_Type;			// The TYPE of Ice
};

//------------------------------------------
//	Base Abstract Class
//------------------------------------------
class comICE abstract
{
public:
	// Interface Functions
	virtual void Render() = 0;
	virtual int  Step(DWORD _delta) = 0;
	virtual void Zap(int _type, float _value, CVector3 _vector) = 0;
	virtual comICEdata Dead() = 0;
	// Inherited Varibles
protected:
	comPData* m_Player;			// Player Information (Ref)
	int m_State;				// Ice State
	int m_ID;					// Ice ID
};


#endif