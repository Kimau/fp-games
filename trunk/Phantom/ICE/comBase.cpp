#include "comBase.h"

//###################################
//###################################
//			CONTRUCTORS
//###################################
//###################################
//========================================================================
//	Contructor
//========================================================================
iceBlock::iceBlock()
{
	m_State		= -1;
	m_Health	= 0.0f;
	m_Shield	= 0.0f;
	m_sOff		= 0.0f;
	m_sDef		= 0.0f;
	m_sHack		= 0.0f;
	m_sHide		= 0.0f;
	m_sScan		= 0.0f;
	m_Alert		= 0.0f;
}
//========================================================================
//	Decontructor
//========================================================================
iceBlock::~iceBlock()
{
	while(m_Objects.size() > 0)
	{
		delete m_Objects[0];
		m_Objects.erase(m_Objects.begin());
	}
	while(m_GOBs.size() > 0)
	{
		delete m_GOBs[0];
		m_GOBs.erase(m_GOBs.begin());
	}
}
//###################################
//###################################
//		INTERFACE FUNCTIONS
//###################################
//###################################
//========================================================================
//	Open Game Block
//========================================================================
int iceBlock::Open()
{
	if(m_State < 0)
	{
		m_State		= 1;
		m_Health	= 10.0f;
		m_Shield	= 10.0f;
		m_sOff		= 1.0f;
		m_sDef		= 1.0f;
		m_sHack		= 1.0f;
		m_sHide		= 1.0f;
		m_sScan		= 1.0f;

		m_Alert		= 0.0f;
		//-----------------------------
		//	TEMP
		//-----------------------------
		icObject* _obj = new icObject();
		_obj->m_Pos.x = 10.0f;
		_obj->m_Size = CVector3(0.3f,0.3f,0.3f);
		m_Objects.push_back(_obj);

		//-----------------------------
		// Texture
		//-----------------------------
		PH_TEX->Load("Data/Cur.bmp");
		m_TexMouse = PH_TEX->Load("Data/target.png");  
		//-----------------------------
		//	Camera
		//-----------------------------
		m_Camera.setCamera(5.0f,0.0f,0.0f,	0.0f,0.0f,0.0f,		0.0f,0.0f,1.0f);

		//-----------------------------
		//	GOBS
		//-----------------------------
		GOBMatrix* _matrix = new GOBMatrix();
		_matrix->m_Pos.z = -1.0f;
		m_GOBs.push_back(_matrix);
	}
	return m_State;
}
//========================================================================
//	Take a Step
//========================================================================
int iceBlock::Step()
{
	if(m_State > 0)
	{
		//------------------------------------------
		//	Process Messages
		//------------------------------------------
		ProcessMessages();
		//------------------------------------------
		//	DO LOGIC
		//------------------------------------------		
		UpdateProjectiles();
		UpdateGOBs();
		//------------------------------------------
		//	RENDER
		//------------------------------------------
		Render();
	}
	return m_State;
}
//========================================================================
//	Process Input
//========================================================================
int iceBlock::Input(UINT _iMsg, WPARAM wParam, LPARAM lParam)
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
				m_Mouse.x = LOWORD(lParam);
				m_Mouse.y = HIWORD(lParam);
				return 1;
			}
		case WM_LBUTTONDOWN:
			{
				return 1;
			}
		case WM_LBUTTONUP:
			{
				icProject _bullet = {1,CVector3(m_Camera.m_current[0],m_Camera.m_current[1],m_Camera.m_current[2]), m_Turret, CVector3(0.0f,0.0f,-0.01f)};
				m_Projecs.push_back(_bullet);
				return 1;
			}
		case WM_RBUTTONDOWN:
			{
				return 1;
			}
		case WM_RBUTTONUP:
			{
				return 1;
			}

			//-----------------------------
			//	Key Press
			//-----------------------------
			case WM_KEYUP:
			{
				switch(wParam)
				{
				case 'x':
				case 'X':
					{
					}
					return 1;

				case 'w':
				case 'W':
				case 's':
				case 'S':

					return 1;
				case 'a':
				case 'A':
				case 'd':
				case 'D':

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
				case 'z':
				case 'Z':
					{
					}
					return 1;
					//-----------------------------------
					//	 Move Player
					//-----------------------------------
				case 'w':
				case 'W':
					if(m_Camera.getView().SqrdMag() > 3.0f)
						m_Camera.Zoom(x_delta / IC_CMOVE);
					return 1;
				case 's':
				case 'S':
					if(m_Camera.getView().SqrdMag() < 350.0f)
						m_Camera.Zoom(x_delta / -IC_CMOVE);
					return 1;
				case 'a':
				case 'A':
					m_Camera.RotatePos(+IC_CDEG * x_delta, CVector3(m_Camera.m_current[6],m_Camera.m_current[7],m_Camera.m_current[8]));
					return 1;
				case 'd':
				case 'D':
					m_Camera.RotatePos(-IC_CDEG * x_delta, CVector3(m_Camera.m_current[6],m_Camera.m_current[7],m_Camera.m_current[8]));
					return 1;
				};
				break;
			}
		};
	}
	return 0;
}
//========================================================================
//	Exit Game Block
//========================================================================
int iceBlock::Exit()
{
	return m_State;
}
//###################################
//###################################
//		LOGIC FUNCTIONS
//###################################
//###################################
//========================================================================
//	Moves Projectiles and Calls Collisions
//========================================================================
void iceBlock::UpdateProjectiles()
{
	int c = int(m_Projecs.size() - 1);
	UPPROJSTARTOFLOOP:
	while((c >= 0) && (int(m_Projecs.size()) > c))
	{
		icProject& _proj = m_Projecs[c];
		_proj.m_Move += _proj.m_Accel * (x_delta / IC_PROJECT_SPEED);
		_proj.m_Pos  += _proj.m_Move * (x_delta / IC_PROJECT_SPEED);

		//------------------------------------
		//	Spawn GOBs
		//------------------------------------
		GOBParticle* _particle = new GOBParticle();
		_particle->m_Pos = _proj.m_Pos;
		_particle->m_Move.x = (rand() % 1000) / 1000.0f - 0.5f;
		_particle->m_Move.y = (rand() % 1000) / 1000.0f - 0.5f;;
		_particle->m_Move.z = (rand() % 500) / 500.0f;
		_particle->m_Move.Normalize();
		_particle->m_Acc.z = -1.0f;
		_particle->m_life = 300;
		m_GOBs.push_back(_particle);

		//------------------------------------
		//	Check Collision with Floor
		//------------------------------------
		if(_proj.m_Pos.z < (((GOBMatrix*)m_GOBs[0])->getHeight(_proj.m_Pos.x,_proj.m_Pos.y) + m_GOBs[0]->m_Pos.z))
		{
			float _mag = 1.5f;
			MatrixWeight _weight = 
			{
				0,					// Type
				_mag,				// Size
				0,					// Age
				_mag*1000,			// DWORD m_life;
				{_proj.m_Pos.x,_proj.m_Pos.y,_proj.m_Pos.z},	// float m_pos[3];
				{1.0f,0.0f,0.0f}	// float m_colour[3];
			};

			((GOBMatrix*)m_GOBs[0])->m_weights.push_back(_weight);

			m_Projecs.erase(m_Projecs.begin() + c);
			goto UPPROJSTARTOFLOOP;
		}

		//------------------------------------
		//	Check Collision with Objects
		//------------------------------------
		for(int i = int(m_Objects.size() - 1); i >= 0; --i)
		{
			if(m_Objects[i]->Collide(_proj))
			{
				m_Projecs.erase(m_Projecs.begin() + c);
				goto UPPROJSTARTOFLOOP;
			}
		}

		//------------------------------------
		//	Delete Any Far off Points
		//------------------------------------
		if(_proj.m_Pos.SqrdMag() > 10000.0f)
		{
			m_Projecs.erase(m_Projecs.begin() + c);
			goto UPPROJSTARTOFLOOP;
		}
		else
		{
			--c;
		}
	}
}
//========================================================================
//	Update Graphical Objects
//========================================================================
void iceBlock::UpdateGOBs()
{
	//------------------------------------------
	//	Update GOBS
	//------------------------------------------
	for(int c = int(m_GOBs.size() - 1); c >= 0; --c)
	{
		GOB* _gob = m_GOBs[c];
		_gob->Update();
		//------------------------------------------
		//	Ensure Player is on Floor
		//------------------------------------------
		if(_gob->Type() == GOBMATRIX_ID)
		{
			m_Camera.m_current[2] = ((GOBMatrix*)_gob)->getHeight(m_Camera.m_current[0],m_Camera.m_current[1]) + _gob->m_Pos.z + 1.0f;
		}
		if(_gob->Type() == GOBPARTICLE_ID)
		{
			if(((GOBParticle*)_gob)->m_life == 0)
				m_GOBs.erase(m_GOBs.begin() + c);
		}
	}
}
//========================================================================
//	Process Messages
//========================================================================
void iceBlock::ProcessMessages()
{
	while(IC_MSG->m_Msgs.size() > 0)
	{
		//----------------------------
		//	Fetch Top Message
		//----------------------------
		icMsg& _msg = IC_MSG->m_Msgs[0];

		//----------------------------
		//	Process Message
		//----------------------------
		switch(_msg.m_type)
		{
		case 0: break;
		default: break;
		};

		//----------------------------
		//	Delete Top Message
		//----------------------------
		IC_MSG->m_Msgs.erase(IC_MSG->m_Msgs.begin());
	}
}
//###################################
//###################################
//		RENDER FUNCTIONS
//###################################
//###################################
//========================================================================
//	Main Render Function
//========================================================================
void iceBlock::Render()
{
	//-----------------------------------------------------
	//		RESET WORLD
	//-----------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//---------------------------------
	//	Update Camera
	//	Update Mouse
	//---------------------------------
	m_Camera.LookAt();
	m_Turret = (BGLHelper::UnProject(m_Mouse));

	glColor3f(1.0f,1.0f,1.0f);

	//--------------------------------
	//	Draw ICE
	//--------------------------------
	glEnable(GL_TEXTURE_2D);
	PH_TEX->BindTexture(0);
	BGLHelper::DrawCube(1.0f);
	glDisable(GL_TEXTURE_2D);

	//---------------------------------
	//	Render Projectiles
	//---------------------------------
	for(int c = int(m_Projecs.size() - 1); c >= 0; --c)
		RenderProj(m_Projecs[c]);
	//---------------------------------
	//	Render Object
	//---------------------------------
	for(int c = int(m_Objects.size() - 1); c >= 0; --c)
		m_Objects[c]->Render();
	//---------------------------------
	//	Render GOBS
	//---------------------------------
	m_GOBs[0]->Render();		// FIX for Depth Sorting
	for(int c = int(m_GOBs.size() - 1); c > 0; --c)
		m_GOBs[c]->Render();
	//---------------------------------
	//	Render Mouse
	//---------------------------------
	RenderCur();

	//-----------------------------------------------------------------------------------------------------
	//	SWAP BUFFERS
	//-----------------------------------------------------------------------------------------------------
	PH_ENGINE->SwapGL_Buffers();
}
//========================================================================
//	Render Mouse Cursor
//========================================================================
void iceBlock::RenderCur()
{
	glPushMatrix();
		//----------------------------------
		//	Move and Billboard
		//----------------------------------
		glTranslatef(	m_Camera.m_current[0] + m_Turret.x,
						m_Camera.m_current[1] + m_Turret.y,
						m_Camera.m_current[2] + m_Turret.z);
		BGLHelper::Billboard();

		//----------------------------------
		//	Draw Quad
		//----------------------------------
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
		PH_TEX->BindTexture(m_TexMouse);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f,0.0f);	glVertex3f( 0.02f, 0.02f, 0.0f);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(-0.02f, 0.02f, 0.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(-0.02f,-0.02f, 0.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f( 0.02f,-0.02f, 0.0f);
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
//========================================================================
//	Render Projectiles
//========================================================================
void iceBlock::RenderProj(icProject& _proj)
{
	glPushMatrix();
		glColor3f(0.0f,0.0f,1.0f);
		glTranslatef(_proj.m_Pos.x,_proj.m_Pos.y,_proj.m_Pos.z);
		BGLHelper::DrawCube(0.1f);
	glPopMatrix();
}