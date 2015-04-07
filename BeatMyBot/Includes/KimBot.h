/********************************************************************
	created:	11:11:2009   18:56
	file path:	c:\Dev\BeatMyBot\Includes\KimBot.h
	author:		Claire Blackshaw
	
	purpose:	Claire's Combat Robot
*********************************************************************/

#ifndef HEADER_INCLUDE_KIMBOT
#define HEADER_INCLUDE_KIMBOT

// Base Define
#include "IBot.h"

class KimTeam;

///-------------------------------------
/// KimBot
///
/// Brief: Basic Combat Bot
///
///-------------------------------------
class KimBot : public IBot
{
public:
	//----------------------------------------------------- Public Functions
					KimBot();
	virtual 		~KimBot();

	virtual void Update(GameState* pTheGameState);

	void UpdateMovement( GameState* pTheGameState );
	virtual void Initialise(MapData* pTheMapData, int teamNumber, int characterNumber);

	// Accessors
	bool IsDead() const						{ return m_pCharData->miHealth < 1; }

	virtual Vector2D GetVelocity() const	{ return m_moveDir; }
	virtual bool GetIfFiring() const;
	virtual bool GetIfAiming() const;
	virtual bool GetIfThrowing() const;
	virtual int GetTargetTeam() const;
	virtual int GetTargetNumber() const;

	// Unused Debug Functions (new interface seems not to use
	virtual Vector2D GetDebugPoint()		{ return Vector2D(0,0); }
	virtual Vector2D* GetDebugList()		{ return NULL; }
	virtual int GetDebugListLength()		{ return NULL; }
	virtual char* GetDebugData()			{ return NULL; }

private:
	//----------------------------------------------------- Private Enums
	enum CurrAction
	{
		CA_IDLE,
		CA_MOVING,
		CA_AIMING,
		CA_FIRING,
		CA_THROWING,
		NOOF_CURRENT_ACTIONS
	};

	static const int MAX_PATH_LENGTH = 20;

	//----------------------------------------------------- Private Functions
	void		NewPath(const Vector2D& destPos);
	void		AddPoint(const Vector2D& destPos);
	void		SetTarget(Character* pTarget, int iTargetID);

	//----------------------------------------------------- Private Variables
	
	int			m_iTeamNumber;
	int			m_iBotNumber;
	int			m_nPathLength;

	int			m_attraction;
	int			m_aggressive;

	bool		m_bDead;

	Vector2D	m_lastPos;
	Vector2D	m_moveDir;		// Current Direction of Motion
	CurrAction	m_eAction;		// Current Action

	int			m_TargetID;
	Character*	m_pTarget;		// Current Enemy Target

	Character*	m_pCharData;
	Vector2D	m_currPath[MAX_PATH_LENGTH];

	friend class KimTeam;
};

#endif // HEADER_INCLUDE_KIMBOT