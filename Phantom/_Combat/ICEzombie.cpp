#include "ICEzombie.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Creation Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================
//	Create Blank Zombie
//=========================================================================
CIZombie::CIZombie(comPData* _player, comICEdata _ice)
{
	m_Player = _player;
	m_ID = _ice.m_ID;
	m_Exposed = 1.0f;

	//----------------
	//	Start Points
	//----------------
	for(int c = 0; c < ZZ_POINTS; ++c)
	{
		ZombiePoint _point = {0,CVector3(),CVector3()};

		m_targeted = -1;
		_point.m_value = 100.0f;		
		//-------------------------
		//	Set Starting Point
		//-------------------------
		_point.m_Pos.x = (rand() % 1000 - 500) * 0.1f;
		_point.m_Pos.y = (rand() % 1000 - 500) * 0.1f;
		_point.m_Pos.z = (rand() % 1000 - 500) * 0.1f;
		_point.m_Pos.Normalize();
		_point.m_Pos *= ZZ_DIST;

		//-------------------------
		//	Set Rotation Axis
		//-------------------------
		do
		{
			_point.m_RotAxis.x = (rand() % 1000 - 500) * 0.1f;
			_point.m_RotAxis.y = (rand() % 1000 - 500) * 0.1f;
			_point.m_RotAxis.z = (rand() % 1000 - 500) * 0.1f;
			_point.m_RotAxis.Normalize();
		} while (_point.m_RotAxis.Angle(_point.m_Pos) < 0.3f);
		m_Points.push_back(_point);
	}
	//-----------------------------
	//	Build Model
	//-----------------------------
	m_list = glGenLists(1);
	BGLModel3ds _temp;
	_temp.Import3ds("Data/Zombie.3DS");
	glNewList(m_list,GL_COMPILE);
		_temp.RenderObjects();
	glEndList();

	PH_LOG->PrintS(LG_STATE_REPORT,"Zombie Made : %i",m_ID);

	m_State = 1;
}
//=========================================================================
//	Clean up Memory
//=========================================================================
CIZombie::~CIZombie()
{
	PH_LOG->PrintS(LG_STATE_REPORT,"Zombie Destroyed : %i",m_ID);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Interface Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================
//	Render the Pretty Ice
//=========================================================================
void CIZombie::Render()
{
	//---------------------------------
	//	Main Body
	//---------------------------------
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);
		glScalef(2.5f,2.5f,2.5f);
		glCallList(m_list);
	glPopMatrix();

	glColor3f(1.0f,1.0f,1.0f);
	//---------------------------------
	//	Points
	//---------------------------------
	for(int c = int(m_Points.size() - 1); c >= 0; --c)
	{
		ZombiePoint& _point = m_Points[c];
		glPushMatrix();
			glTranslatef(_point.m_Pos.x,_point.m_Pos.y,_point.m_Pos.z);

			CVector3 _long = _point.m_Pos.getLongitude() * ZZ_TARSIZE * m_Exposed;
			CVector3 _lat  = _point.m_Pos.getLatitude()  * ZZ_TARSIZE * m_Exposed;
			CVector3 _temp = _long + _lat;

			//-----------------------------------
			//	Draw Target
			//-----------------------------------
			glBegin(GL_QUADS);
			if(m_targeted == c)
			{
				glColor3f(1.0f,1.0f,1.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);		_temp -= (_lat * 2);
				glColor3f(0.0f,1.0f,1.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);		_temp -= (_long * 2);
				glColor3f(0.0f,0.0f,1.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);		_temp += (_lat * 2);
				glColor3f(1.0f,0.0f,1.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);
			}
			else
			{
				glColor3f(1.0f,1.0f,0.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);		_temp -= (_lat * 2);
				glColor3f(0.0f,1.0f,0.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);		_temp -= (_long * 2);
				glColor3f(0.0f,0.0f,0.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);		_temp += (_lat * 2);
				glColor3f(1.0f,0.0f,0.0f);	glVertex3f( _temp.x, _temp.y, _temp.z);
			}
			glEnd();

			//-----------------------------------
			//	Display Health Bar
			//-----------------------------------
			if(m_targeted == c)
			{
				glDisable(GL_DEPTH_TEST);
				BGLHelper::Billboard();


				glColor3f(0,1,0);
				float _temp = _point.m_value / 500.0f;
				glBegin(GL_QUADS);
					glVertex3f( _temp,-0.3f,0.0f);
					glVertex3f(-_temp,-0.3f,0.0f);
					glVertex3f(-_temp,-0.4f,0.0f);
					glVertex3f( _temp,-0.4f,0.0f);
				glEnd();
				glEnable(GL_DEPTH_TEST);
			}

			glColor3f(1.0f,1.0f,1.0f);

		glPopMatrix();
	}
}
//=========================================================================
//	Take a Step of Game Time
//=========================================================================
int CIZombie::Step(DWORD _delta)
{
	moveTargets(_delta * ZZ_DEG);

	return(!(m_Points.empty()));
}
//=========================================================================
//	Whenever the player attacks / comprimises or any other attack
//=========================================================================
void CIZombie::Zap(int _type, float _value, CVector3 _vector)
{
	//----------------------------
	//	Prefetch
	//----------------------------
	CVector3& _start = m_Player->m_Pos;

	//--------------------------------
	//	Collision check on each point
	//--------------------------------
	float _minDist = 9999999.9f;
	m_targeted = -1;
	for(int c = int(m_Points.size() - 1); c >= 0; --c)
	{
		ZombiePoint& _point = m_Points[c];
		float _dist = _vector.Distance(_point.m_Pos - _start);
		float _dot = _point.m_Pos.Dot(m_Player->m_Pos);	
		//---------------------------------
		//	Check if Distance is closer
		//---------------------------------
		if((_dist < _minDist) && (_dot > 0))
		{
			_minDist = _dist;
			m_targeted = c;
		}
	}

	//---------------------------------
	// Run Closest One
	// Check its within Bounds
	//---------------------------------
	if((m_targeted >= 0) && (_minDist < (ZZ_TARSIZE * 1.2f * m_Exposed)))
	{
		ZombiePoint& _point = m_Points[m_targeted];
		switch(_type)
		{
			//------------------------------
			//	Attack
			//------------------------------
		case 1:
			_point.m_value -= _value;
			break;
			//------------------------------
			//	Hack
			//------------------------------
		case 2:
			PH_LOG->PrintS(LG_ACTION_REPORT,"HACK: Point[%i], Dist[%f]",m_targeted,_minDist);
			m_Exposed += 0.1f;
			break;
		};
	}
	else
	{
		m_targeted = -1;
	}
	//--------------------------------
	//	Kill Points
	//--------------------------------
	m_Points.erase(std::remove_if(m_Points.begin(),m_Points.end(),ZombieHelper::isDead), m_Points.end());
}
//=========================================================================
//	Ice Dies, returns Black Box
//=========================================================================
comICEdata CIZombie::Dead()
{
	comICEdata _bbox = {m_ID, CI_ZOMBIE};
	return _bbox;
}

//=========================================================================
//	Move the targets
//=========================================================================
void CIZombie::moveTargets(float _degs)
{
	for(int c = int(m_Points.size() - 1); c >= 0; --c)
	{
		ZombiePoint& _point = m_Points[c];
		_point.m_Pos.Rotate(_degs,_point.m_RotAxis);
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//						ZOMBIE HELPERS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bool ZombieHelper::isDead(ZombiePoint& _point)
{
	return(_point.m_value < 0.0f);
}