#include "combatBlock.h"

/*****************************************************************************************************
									BASIC FUNCS
/****************************************************************************************************/
//=================================================================
// Loads the base class, Still DEAD BLOCK
//=================================================================
combatBlock::combatBlock()
{
	m_State = -1;
	m_ice = 0;
}
//=================================================================
// Decon
//=================================================================
combatBlock::~combatBlock()
{
	if(m_ice) 
		delete m_ice;
}
/*****************************************************************************************************
								INTERFACE FUNCS
/****************************************************************************************************/
//=================================================================
// Opens the Block, Ready for Use. DORMANT BLOCK
//=================================================================
int combatBlock::Open()
{
	m_State = 0;

	//------------------------------------
	//  Ensure OpenGL is in right state
	//------------------------------------
	glEnable(GL_COLOR_MATERIAL);
	m_bitmap.Load("cur.bmp");
	m_font.Load(PH_ENGINE);
	m_MouseState = 0;

	PH_LOG->PrintS(LG_STATE_REPORT,"Combat Block Opened, %i", m_State);

	return m_State;
}
//=================================================================
// Starts the Block, Currently Running. ACTIVE BLOCK
//=================================================================
int combatBlock::Start(comPData _player, comICEdata _ice)
{
	if(m_State == 0)
	{
		m_State = 1;
		m_Player = _player;
		//------------------------------------
		//	Init ICE
		//------------------------------------
		switch(_ice.m_Type)
		{
		case CI_ZOMBIE:		m_ice = new CIZombie(&m_Player, _ice);		break;
		case 0:				m_ice = 0;									break;
		};

		//------------------------------------
		//	Ensure Vectors are Correct
		//------------------------------------
		m_Player.m_Move *= 0.0f;

		m_Player.m_Pos.Normalize();
		m_Player.m_Pos *= CMB_PLAYERDIST;		

		m_Player.m_Turret = m_Player.m_Pos.getNormalized() * (-CMB_TURRETDIST);

		PH_LOG->PrintS(LG_STATE_REPORT,"Combat Block Started, %i", m_State);
	}

	return m_State;
}
//=================================================================
// Runs Iteration of Block
//=================================================================
int combatBlock::Step()
{
	if(m_State > 0)
	{
		// RUN BLOCK
		//-----------------------------
		// Logic Calcs
		//-----------------------------
		m_ice->Zap(m_MouseState,x_delta / 15.0f,m_Player.m_Turret);
		
		
		if(m_ice->Step(x_delta) == 0)		// ICE CALC
		{
			Exit();
		}
		else
		{
			movePlayer();					// Move Player

			//-----------------------------
			// Render
			//-----------------------------
			Render();
		}
	}
	return m_State;
}
//=================================================================
// Parses Input
//=================================================================
int combatBlock::Input(UINT _iMsg, WPARAM wParam, LPARAM lParam)
{
	//-----------------------------
	//	Check State
	//-----------------------------
	if(m_State > 0)
	{
		switch(_iMsg)
		{
			//-----------------------------
			// Mouse Movement
			//-----------------------------
		case WM_MOUSEMOVE:
			{
				CVector2 _newPos(LOWORD(lParam),HIWORD(lParam));

				m_Player.m_Turret = (BGLHelper::UnProject(_newPos)) * CMB_TURRETDIST;
				return 1;
			}
		case WM_LBUTTONDOWN:
			{
				m_MouseState |= PH_LMOUSE;
				return 1;
			}
		case WM_LBUTTONUP:
			{
				m_MouseState &= !PH_LMOUSE;
				return 1;
			}
		case WM_RBUTTONDOWN:
			{
				m_MouseState |= PH_RMOUSE;
				return 1;
			}
		case WM_RBUTTONUP:
			{
				m_MouseState &= !PH_RMOUSE;
				return 1;
			}

			//-----------------------------
			//	Key Press
			//-----------------------------
			case WM_KEYUP:
			{
				switch(wParam)
				{
				case 'w':
				case 'W':
				case 's':
				case 'S':
					m_Player.m_Move.x = 0.0f;
					m_Player.m_Move.y = 0.0f;
					return 1;
				case 'a':
				case 'A':
				case 'd':
				case 'D':
					m_Player.m_Move.z = 0.0f;
					return 1;
				case VK_ESCAPE:
					return -1;
				};
				break;
			}
			case WM_KEYDOWN:
			{
				switch(wParam)
				{
					//-----------------------------------
					//	 Move Player
					//-----------------------------------
				case 'w':
				case 'W':
						m_Player.m_Move.x = -m_Player.m_Pos.y;
						m_Player.m_Move.y =  m_Player.m_Pos.x;
					return 1;
				case 's':
				case 'S':
						m_Player.m_Move.x =  m_Player.m_Pos.y;
						m_Player.m_Move.y = -m_Player.m_Pos.x;
					return 1;
				case 'a':
				case 'A':
						m_Player.m_Move.z = 0.5f;
					return 1;
				case 'd':
				case 'D':
						m_Player.m_Move.z =-0.5f;
					return 1;
				};
				break;
			}
		};
	}
	return 0;
}
//=================================================================
// Switches back to DORMANT BLOCK
//=================================================================
int combatBlock::Exit()
{
	if(m_State > 0)
	{
		// RUN BLOCK
		m_State = 0;
		if(m_ice)
			delete m_ice;
	}
	PostQuitMessage(m_State);
	return m_State;
}

/*****************************************************************************************************
									SUPPORT FUNCS
/****************************************************************************************************/
//=================================================================
// Moves the Player
//=================================================================
void combatBlock::movePlayer()
{
	if(m_Player.m_Move.SqrdMag() < 0.1f)
		return;

	//--------------------------------
	// For Fast Refrece
	//--------------------------------
	CVector3& _playPos = m_Player.m_Pos;
	CVector3 _old = _playPos;

	_playPos.Rotate(CMB_PLAYERMOVE * x_delta, m_Player.m_Move);

	//--------------------------------
	// Check you haven't fallen
	// through floor or gotten locked
	//--------------------------------
	if((_playPos.z < -1.2f) || (_playPos.z > (CMB_PLAYERDIST - 0.1f)))
	{
		_playPos = _old;
	}
	else
	//---------------------
	// Move Turret With
	//---------------------
	{
		m_Player.m_Turret.Rotate(CMB_PLAYERMOVE * x_delta, m_Player.m_Move);
	}
}
//=================================================================
// Renders the Game
//=================================================================
void combatBlock::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt( m_Player.m_Pos.x, m_Player.m_Pos.y, m_Player.m_Pos.z, 0,0,0, 0,0,1);

	//-----------------------------
	//	Draw Floor
	//-----------------------------
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	for(float x = -60.0f; x < 60.0f; x += CMB_BLOCKSIZE)
	{
		for(float y = -60.0f; y < 60.0f; y += CMB_BLOCKSIZE)
		{
			float _amount = 1.0f - (sqrtf(powf(x,2) + powf(y,2)) / 50.0f);
			glColor3f(0.0f,_amount,0.0f);
			glVertex3f(x, y, 0.0f);
		}
	}
	glEnd();

	//-----------------------------
	//	Draw Ice
	//-----------------------------
	if(m_ice)
		m_ice->Render();


	//-----------------------------
	// Draw Cursor / Turret
	//-----------------------------
	glPushMatrix();
		glTranslatef(m_Player.m_Pos.x,m_Player.m_Pos.y,m_Player.m_Pos.z);		
		glTranslatef(m_Player.m_Turret.x,m_Player.m_Turret.y,m_Player.m_Turret.z);

		BGLHelper::Billboard();

		//---------------------------------------
		//	Render
		//---------------------------------------
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		m_bitmap.BindTexture();	
		glBlendFunc(GL_ONE,GL_ONE);		
		
		glBegin(GL_QUADS);
			glTexCoord2i(1,1);	glVertex3f( 0.1f, 0.1f, 0.0f);
			glTexCoord2i(0,1);	glVertex3f(-0.1f, 0.1f, 0.0f);
			glTexCoord2i(0,0);	glVertex3f(-0.1f,-0.1f, 0.0f);
			glTexCoord2i(1,0);	glVertex3f( 0.1f,-0.1f, 0.0f);
		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	//-----------------------------
	// Draw Log
	//-----------------------------
	PH_LOG->RenderFlat();

	//-----------------------------
	//	Draw Bars
	//-----------------------------
	glPushMatrix();
		BGLHelper::Billboard();
		glDisable(GL_DEPTH_TEST);

		//-------------------------------------
		//	UI Dimenson
		//	(-20:20) | (-15:15)
		//-------------------------------------
		glTranslatef(0,0,-22.0f);

		float _value = 0;
		//-------------------------------------
		//	Draw Player Health
		//-------------------------------------
		glPushMatrix();
			glTranslatef(-13.0f,-14.5f,0);			

			_value = m_Player.m_Health / 10.0f;
			glBegin(GL_QUADS);
				glColor3f(0.0f,0.0f,1.0f);
				glVertex3f(0.0f,  1.0f,0.0f);				
				glVertex3f(0.0f,  0.5f,0.0f);
				glVertex3f(_value,0.5f,0.0f);
				glVertex3f(_value,1.0f,0.0f);

				glColor3f(0.0f,1.0f,0.0f);
				glVertex3f(_value,0.5f,0.0f);				
				glVertex3f(0.0f,  0.5f,0.0f);
				glVertex3f(0.0f,  0.0f,0.0f);
				glVertex3f(_value,0.0f,0.0f);
			glEnd();
		glPopMatrix();
		//-------------------------------------
		//	Draw Player Stats
		//-------------------------------------
		glPushMatrix();
			glTranslatef(-19.5f,-14.5f,0.0f);

			glBegin(GL_QUADS);
				_value = m_Player.m_Offense / 2.0f;
				glColor3f(1.0f,0.0f,0.0f);
				glVertex3f(0.5f,_value,0.0f);
				glVertex3f(0.0f,_value,0.0f);
				glColor3f(0.1f,0.0f,0.0f);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.5f,0.0f,0.0f);
				
				_value = m_Player.m_Defense / 2.0f;
				glColor3f(0.0f,1.0f,0.0f);
				glVertex3f(1.5f,_value,0.0f);
				glVertex3f(1.0f,_value,0.0f);
				glColor3f(0.0f,0.1f,0.0f);
				glVertex3f(1.0f,0.0f,0.0f);
				glVertex3f(1.5f,0.0f,0.0f);

				_value = m_Player.m_Convert / 2.0f;
				glColor3f(0.0f,0.0f,1.0f);
				glVertex3f(2.5f,_value,0.0f);
				glVertex3f(2.0f,_value,0.0f);
				glColor3f(0.0f,0.0f,0.1f);
				glVertex3f(2.0f,0.0f,0.0f);
				glVertex3f(2.5f,0.0f,0.0f);

				_value = m_Player.m_Stealth / 2.0f;
				glColor3f(1.0f,0.0f,1.0f);
				glVertex3f(3.5f,_value,0.0f);
				glVertex3f(3.0f,_value,0.0f);
				glColor3f(0.1f,0.0f,0.1f);
				glVertex3f(3.0f,0.0f,0.0f);
				glVertex3f(3.5f,0.0f,0.0f);

				_value = m_Player.m_Scan / 2.0f;
				glColor3f(1.0f,1.0f,0.0f);
				glVertex3f(4.5f,_value,0.0f);
				glVertex3f(4.0f,_value,0.0f);
				glColor3f(0.1f,0.1f,0.0f);
				glVertex3f(4.0f,0.0f,0.0f);
				glVertex3f(4.5f,0.0f,0.0f);
			glEnd();
		glPopMatrix();

		//-------------------------------------
		//	Performance Bar
		//-------------------------------------
		glPushMatrix();
			glTranslatef(-20.0f,0.0f,0.0f);

			glBegin(GL_QUADS);
				_value = float(x_delta) / 25.0f;
				glColor3f(1.0f,1.0f,0.0f);
				glVertex3f(0.5f,_value,0.0f);
				glVertex3f(0.0f,_value,0.0f);
				glColor3f(0.1f,0.1f,0.0f);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.5f,0.0f,0.0f);
			glEnd();
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
	glPopMatrix();

	//-----------------------------------------------------------------------------------------------------
	//	SWAP BUFFERS
	//-----------------------------------------------------------------------------------------------------
	PH_ENGINE->SwapGL_Buffers();
}