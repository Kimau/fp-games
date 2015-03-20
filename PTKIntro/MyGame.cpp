#include "MyGame.h"
#include <math.h>

static GObjects s_Objects;
static GFX_Events s_GFXs;
//#################################
//#################################
//		STARTERS AND STOPERS
//#################################
//#################################
//=====================================================================
//	Constructor
//=====================================================================
MyGame::MyGame()
{
	m_Score = 0;
	m_State = GS_NULL;
}
//=====================================================================
//	Decontructor
//=====================================================================
MyGame::~MyGame()
{
}
//=====================================================================
//	Constructor
//=====================================================================
GameState MyGame::Setup()
{
	//-------------------------
	//	Zero
	//-------------------------
	m_Score = 0;
	m_State = GS_PLAY;
	//-------------------------
	//	Create Blocks of Doom
	//-------------------------
	for(int i = 0; i < MG_BLOCK_COUNT; ++i)
	{
		GObject* _block = new GObject();
		GenerateBlock(_block);
		s_Objects.push_back(_block);
	}

	//--------------------------
	//	Update and Return State
	//--------------------------
	return m_State;
}
//=====================================================================
//	Update the Game
//=====================================================================
GameState MyGame::Update(unsigned long _delta)
{
	//----------------------------
	//	Process Events
	//----------------------------
	while(m_EventQ.empty() == false)
	{
		ProcessEvent(m_EventQ.pop());
	}
	//----------------------------
	//	Update Objects
	//----------------------------
	for(GObjects::iterator _iter = s_Objects.begin(); _iter != s_Objects.end(); ++_iter)
	{
		(*_iter)->Update(_delta / 10);

		if(((*_iter)->m_pos[1] < 0) ||  ((*_iter)->m_pos[0] < 0) ||  ((*_iter)->m_pos[0] > 800))
		{
			GenerateBlock(*_iter);
		}
	}

	return m_State;
}

//=====================================================================
//	Generate a New Block
//=====================================================================
void MyGame::GenerateBlock(GObject* _block)
{
	_block->setMoment(0,650.0f, 0.0f,0,0,0,0,-0.1f,0);
	_block->m_size[0] = 10.0f;
	_block->m_size[1] = 10.0f;

	_block->m_pos[0] = (float)(rand() % 800);
	_block->m_vel[0] = (float)(rand() % 1000 - 500) / 500.0f;
	_block->m_acl[1] = (float)(rand() % 1500 - 2000) / 15000.0f;
}

//=====================================================================
//	Process Event
//=====================================================================
void MyGame::ProcessEvent(gEvent& _event)
{
	switch(_event.m_type)
	{
	case GEM_NULL:
		// DO Nothing
		break;
	case GEM_SHOOT:
		Shoot(_event.m_buffer[0], _event.m_buffer[1]);
		break;
	};
}

//=====================================================================
//	Shoot Bock
//=====================================================================
void MyGame::Shoot(int x, int y)
{
	PostGFX(GFX_EXPLO,float(x),float(y));

	for(GObjects::iterator _iter = s_Objects.begin(); _iter != s_Objects.end(); )
	{
		GObject& _obj = *(*_iter);
		float _deltaX = abs(_obj.m_pos[0] - x);
		float _deltaY = abs(_obj.m_pos[1] - y);

		if((_deltaX < 50) && (_deltaY < 50))
		{
			++m_Score;
			s_Objects.erase(_iter++);
			PostGFX(GFX_DEBRIS, 
					_obj.m_pos[0], _obj.m_pos[1],
					_obj.m_vel[0], _obj.m_vel[1],
					_obj.m_acl[0], _obj.m_acl[1]);
		}
		else
		{
			++_iter;
		}
	}
}

//=====================================================================
//	Post GFX Event
//=====================================================================
void MyGame::PostGFX(GfxEvent _type, 
				 float _d1, float _d2, float _d3, float _d4,
				 float _d5, float _d6, float _d7, float _d8)
{
	gfxEvent _event;
	_event.m_type = _type;
	_event.m_d1 = _d1;
	_event.m_d2 = _d2;
	_event.m_d3 = _d3;
	_event.m_d4 = _d4;
	_event.m_d5 = _d5;
	_event.m_d6 = _d6;
	_event.m_d7 = _d7;
	_event.m_d8 = _d8;

	s_GFXs.push_back(_event);
}

//=====================================================================
//	Post GFX Event
//=====================================================================
gfxEvent MyGame::popGFX()
{
	if(s_GFXs.empty())
	{
		gfxEvent _null;
		_null.m_type = GFX_NULL;
		return _null;
	}

	gfxEvent _event = s_GFXs.front();
	s_GFXs.pop_front();
	return _event;
}
//#################################
//#################################
//			ACCESORS
//#################################
//#################################
//=====================================================================
//	Get State of Game
//=====================================================================
GameState MyGame::getState() const
{
	return m_State;
}
//=====================================================================
//	Get Blocks
//=====================================================================
GObjects::const_iterator MyGame::getBlocksBegin() const	{return s_Objects.begin();}
GObjects::const_iterator MyGame::getBlocksEnd() const	{return s_Objects.end();}

int MyGame::getScore() const { return m_Score; }