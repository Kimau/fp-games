#ifndef COMBAT_BASE_FOR_ICE_35642
#define COMBAT_BASE_FOR_ICE_35642

//---------------------------------
//	Includes
//---------------------------------
#include "..\gameBlock.h"
#include "..\Support\Singleton.h"
#include <vector>
#include "icObject.h"
#include "icMessages.h"
#include "icEltronic.h"

#include "..\GOB\GOB.h"
#include "..\GOB\gMatrix.h"
#include "..\GOB\gParticle.h"

//---------------------------------
//	Defines
//---------------------------------
#define IC_MSG				CSingletonT<icMessages>::Get()
#define IC_CDEG				0.2f
#define IC_CMOVE			500.0f
#define IC_PROJECT_SPEED	100.0f

//---------------------------------------
//	Ice Combat Class
//---------------------------------------
class iceBlock : public gameBlock
{
public:
	//-----------------------------------
	//	Contructors
	//-----------------------------------
	iceBlock();
	~iceBlock();
	//-----------------------------------
	//	Interface Functions
	//-----------------------------------
	int Open();
	int Step();
	int Input(UINT _iMsg, WPARAM wParam, LPARAM lParam);
	int Exit();
	//-----------------------------------
	//	Logic Functions
	//-----------------------------------
	void UpdateProjectiles();
	void UpdateGOBs();
	void ProcessMessages();
	//-----------------------------------
	//	Graphics Functions
	//-----------------------------------
	void Render();
	void RenderCur();
	void RenderProj(icProject& _proj);

private:
	//-----------------------
	//	Player
	//-----------------------
	float m_Health;		// Player Health
	float m_Shield;		// Player Shield
	float m_sOff;		// Offense Stat
	float m_sDef;		// Defense Stat
	float m_sHack;		// Hack Stat
	float m_sHide;		// Stealth Stat
	float m_sScan;		// Scan Stat

	float m_Alert;
	CVector3 m_Turret;

	//-----------------------
	//	Objects
	//-----------------------
	//icEltronic m_Ice;					// Intruder Counter-Measure Electronic
	std::vector<icProject>	m_Projecs;	// Projectiles
	std::vector<icObject*>	m_Objects;	// Object

	//-----------------------
	//	Graphics
	//-----------------------
	QCamera m_Camera;				// Camera Objects
	CVector2 m_Mouse;				// Mouse Pos
	GLuint m_TexMouse;				// Cursor Texture
	std::vector<GOB*> m_GOBs;		// Graphical Object	
};

#endif