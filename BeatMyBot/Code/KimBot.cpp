/********************************************************************
	created:	11:11:2009   18:56
	file path:	c:\Dev\BeatMyBot\Code\KimBot.cpp
	author:		Claire Blackshaw
	
	purpose:	
*********************************************************************/

//////////////////////////////////////////////////////////////////////////
///	Game Includes
#include "KimBot.h"
#include "KimTeam.h"

static bool s_bReval = FALSE;

//////////////////////////////////////////////////////////////////////////
///	[] KimBot::KimBot
KimBot::KimBot()
{
	m_iBotNumber = NULL;
	m_iTeamNumber = NULL;
	m_pCharData = NULL;
	m_nPathLength = NULL;
	m_TargetID = NULL;

	m_moveDir.set(0,0);
	m_eAction = CA_IDLE;
	m_pTarget = NULL;
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::~KimBot
KimBot::~KimBot()
{

}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::Update
void KimBot::Update( GameState* pTheGameState )
{
	// First Pass
	if(m_pCharData == NULL)
	{
		m_pCharData = &(pTheGameState->mrgTeams[m_iTeamNumber].mrgMembers[m_iBotNumber]);
		m_pCharData->mPosition.XValue += m_iBotNumber * 5;
	}
	// If First Team Member then update Team State
	else if(m_iBotNumber == NULL)
	{
		KimTeam::s_inst.Update( pTheGameState );

		if(s_bReval == TRUE)
		{
			s_bReval = FALSE;
			KimTeam::s_inst.EvalTeamTactic();
		}
	}

	if(IsDead() == TRUE)
	{
		if(m_bDead == FALSE)
		{
			// JUST DIED!
		}
	}
	else
	{
		if(m_bDead == TRUE)
		{
			// RESPAWN!
			s_bReval = TRUE;
		}
	}

	while(m_nPathLength > 1)
	{
		if(KimTeam::s_inst.ValidPath(m_pCharData->mPosition, m_currPath[m_nPathLength-2]) == TRUE)
		{
			m_nPathLength -= 1;
		}
		else
		{
			break;
		}
	}

	UpdateMovement(pTheGameState);

	if(m_pTarget != NULL)
	{
		if(m_pTarget->miHealth < 1)
		{
			m_pTarget = NULL;
		}
		else if(m_pCharData->mdAccuracy > 0.5)
		{
			double effShootDist = KimTeam::s_inst.GetMap()->EffectiveShootDistance(m_pCharData->mPosition, pTheGameState->mrgTeams[!m_iTeamNumber].mrgMembers[m_iBotNumber].mPosition);
			//pTheGameState->mrgTeams[!m_iTeamNumber].mrgMembers[m_iBotNumber].miHealth = 0;
		}
	}

	m_lastPos = m_pCharData->mPosition;
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::Initialise
void KimBot::Initialise( MapData* pTheMapData, int teamNumber, int characterNumber )
{
	m_iTeamNumber = teamNumber;
	m_iBotNumber = characterNumber;
	m_pCharData = NULL;

	KimTeam::s_inst.InitTeam( pTheMapData, teamNumber );
	KimTeam::s_inst.AddRobot(this, m_iBotNumber );
}

//////////////////////////////////////////////////////////////////////////
///	[] KimBot::NewPath
void KimBot::NewPath( const Vector2D& destPos )
{
	m_currPath[0] = destPos;
	m_nPathLength = 1;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimBot::AddPoint
void KimBot::AddPoint( const Vector2D& destPos )
{
	if(m_nPathLength < MAX_PATH_LENGTH)
	{
		m_currPath[m_nPathLength] = destPos;
		m_nPathLength += 1;
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimBot::UpdateMovement
void KimBot::UpdateMovement( GameState* pTheGameState )
{
	if(m_nPathLength > 0)
	{
		m_moveDir = m_currPath[m_nPathLength-1] - m_pCharData->mPosition;
		if(KimTeam::ManhattanDist(m_moveDir) > 10)
		{
			m_moveDir *= 100000;
		}
		else
		{
			for(int iFlag = 0; iFlag < 2; ++iFlag)
			{
				if(pTheGameState->mrgFlags[iFlag].miTeamNo < 0)
				{
					Vector2D pos = (pTheGameState->mrgFlags[iFlag].mPosition - m_pCharData->mPosition);

					if(pos.magnitude() < 5)
					{
						pTheGameState->mrgFlags[iFlag].miCharacterNo = m_iBotNumber;
						pTheGameState->mrgFlags[iFlag].miTeamNo = m_iTeamNumber;
						pTheGameState->mrgTeams[m_iTeamNumber].miScore += 5;
						KimTeam::s_inst.EvalTeamTactic();
						return;
					}
				}
			}
		}

		// Check for Collision
		Vector2D newPoint = m_pCharData->mPosition + (m_moveDir.unitVector() * 5.0f);
		Vector2D colPoint = KimTeam::s_inst.GetMap()->CollisionNormal(newPoint);
		if(KimTeam::ManhattanDist(colPoint) > 0)
		{
			m_moveDir = (colPoint.unitVector() * 20.0f) + (m_moveDir.unitVector() * 80.0f);
			return;
		}

	}
	else
	{
		m_moveDir.set(0,0);
	}

	m_attraction = 0;

	if(m_attraction != 0)
	{
		if(m_attraction < 0)
		{
			for(int iBot = 0; iBot < pTheGameState->mrgTeams[m_iTeamNumber].miNumCharacters; ++iBot)
			{
				if(iBot != m_iBotNumber)
				{
					Character& rChar = pTheGameState->mrgTeams[m_iTeamNumber].mrgMembers[m_iBotNumber];

					Vector2D dirVer = (rChar.mPosition - m_pCharData->mPosition);

					if(dirVer.magnitude() < 50)
					{
						m_moveDir = (m_moveDir.unitVector() + dirVer.unitVector()) * 1000.0f;
					}
				}
			}			
		}
		else
		{
			Vector2D avoidDir;

			for(int iBot = 0; iBot < pTheGameState->mrgTeams[m_iTeamNumber].miNumCharacters; ++iBot)
			{
				if(iBot != m_iBotNumber)
				{
					Character& rChar = pTheGameState->mrgTeams[m_iTeamNumber].mrgMembers[m_iBotNumber];
					Vector2D dirVer = (rChar.mPosition - m_pCharData->mPosition);

					if(KimTeam::ManhattanDist(dirVer) < 0.1)
					{
						Vector2D unit(1,0);
						avoidDir += unit.rotatedBy(1.0 * m_iBotNumber);
					}
					else if(KimTeam::ManhattanDist(dirVer) < 40)
					{
						avoidDir += (m_moveDir.perpendicularVector() * (-1.0f * (m_iBotNumber %2))).unitVector();
					}
				}
			}

			m_moveDir = (avoidDir.unitVector() * 20.0f) + (m_moveDir.unitVector() * 80.0f);
		}
	}

	if(KimTeam::ManhattanDist(m_moveDir) > 0.1)
	{

		// Check for Collision
		Vector2D newPoint = m_pCharData->mPosition + (m_moveDir.unitVector() * 5.0f);
		Vector2D colPoint = KimTeam::s_inst.GetMap()->CollisionNormal(newPoint);
		if(KimTeam::ManhattanDist(colPoint) > 0)
		{
			m_moveDir = (colPoint.unitVector() * 20.0f) + (m_moveDir.unitVector() * 80.0f);
			return;
		}
	}

	Vector2D dir = m_lastPos - m_pCharData->mPosition;
	if(KimTeam::ManhattanDist(dir) < 1)
	{
		// Sticky Test
		Vector2D whiskers[8] =
		{
			(m_pCharData->mPosition + Vector2D(+20.0f, + 0.0f)),
			(m_pCharData->mPosition + Vector2D(-20.0f, + 0.0f)),
			(m_pCharData->mPosition + Vector2D(+ 0.0f, -20.0f)),
			(m_pCharData->mPosition + Vector2D(+ 0.0f, +20.0f)),
			(m_pCharData->mPosition + Vector2D(+11.5f, +11.5f)),
			(m_pCharData->mPosition + Vector2D(+11.5f, -11.5f)),
			(m_pCharData->mPosition + Vector2D(-11.5f, +11.5f)),
			(m_pCharData->mPosition + Vector2D(-11.5f, -11.5f))
		};

		Vector2D avoidDir(0,0);

		for(int iPoint = 0; iPoint < 8; ++iPoint)
		{
			avoidDir += KimTeam::s_inst.GetMap()->CollisionNormal(whiskers[iPoint]);
		}

		if(KimTeam::ManhattanDist(avoidDir) > 0)
		{
			m_moveDir = (avoidDir.unitVector() * 20.0f) + (m_moveDir.unitVector() * 80.0f);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimBot::SetTarget
void KimBot::SetTarget( Character* pTarget, int iTargetID )
{
	m_pTarget = pTarget;
	m_TargetID = iTargetID;
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::GetIfFiring
bool KimBot::GetIfFiring() const
{
	if(m_pTarget != NULL) 
	{
		return TRUE; 
	} 
	else 
	{
		return FALSE; 
	}
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::GetIfAiming
bool KimBot::GetIfAiming() const
{
	if(m_pTarget != NULL) 
	{
		return TRUE; 
	} 
	else 
	{
		return FALSE; 
	}
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::GetIfThrowing
bool KimBot::GetIfThrowing() const
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::GetTargetTeam
int KimBot::GetTargetTeam() const
{
	if(m_pTarget != NULL) 
	{
		return ((m_iTeamNumber+1) % 2);
	}
	else
	{
		return -1;
	}
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimBot::GetTargetNumber
int KimBot::GetTargetNumber() const
{
	if(m_pTarget != NULL) 
	{
		return m_TargetID;
	}
	else
	{
		return -1;
	}
}