#include "ptkGameRender.h"
#include <list>
#include <map>

//--------------------------------
//	Define
//--------------------------------
#define		PTK_TITLE		"Flammable Penguins: KISS"
#define		KSTAGE_TOP		288
#define		KSTAGE_BOTTOM	400
#define		KSTAGE_ILEFT	130
#define		KSTAGE_IRIGHT	660
#define		KSTAGE_OLEFT	40
#define		KSTAGE_ORIGHT	760
#define		KTIME_X1		680
#define		KTIME_Y1		0
#define		KTIME_WIDTH		118
#define		KTIME_HEIGHT	300

#define		KMOUSE_PROT		900
#define		KMOUSE_MENU		1200

struct KEventClip
{
	gEvent	m_event;
	bool	m_button;	// Left = 0 , Right = 1
	int 	m_clip[4];
	float	m_depth;
};
static std::map<unsigned int,KEventClip> s_MouseMap;
typedef std::map<unsigned int,KEventClip>::iterator KECiter;

//========================================================================
//	Constructor
//========================================================================
ptkGameRender::ptkGameRender()
{
	m_gameWin = 0;
	m_gameGFX = 0;
	m_gameTxt = 0;
}
//========================================================================
//	Deconstructor
//========================================================================
ptkGameRender::~ptkGameRender()
{
	Cleanup();
}
//========================================================================
//	Setup Game API
//========================================================================
KWindow* ptkGameRender::Setup(KISSproto* _game)
{
	//-----------------------------------
	//	Clear Lists
	//-----------------------------------
	s_MouseMap.clear();

	//-----------------------------------
	//	Create Window
	//-----------------------------------
	m_gamePTR = _game;
	m_gameWin = KPTK::createKWindow(K_OPENGL);										// Init Window
	m_gameWin->createGameWindow(PTK_SCREENW,PTK_SCREENH,32,true,PTK_TITLE);			// Create Window

	m_gfxActor = m_Images.AssignKey("Data/actor.png", 255, 5, 170);
	m_gfxBack  = m_Images.Assign("Data/stage_back.png");
	m_gfxFront = m_Images.AssignKey("Data/stage_front.png", 255, 5, 170);
	m_gameGFX = m_gfxBack;

	//-----------------------------------
	//	Create Text
	//-----------------------------------
	m_gameTxt = new KTrueText("Data/5by5.ttf");
	m_gameTxt->loadFontFile("Data/5by5.ttf");
	m_gameTxt->setHeightPix(20);
	m_gameTxt->setColor(0.6f,0.6f,1.0f,1.0);

	return m_gameWin;
}
//========================================================================
//	Clean Up
//========================================================================
void ptkGameRender::Cleanup()
{
	m_Images.Purge();
}

//========================================================================
//	Update Render
//========================================================================
void ptkGameRender::Update(unsigned long _delta)
{
	gEvent _event = m_EventQ.pop(OUTBOX);
	switch(_event.m_type)
	{
	case GEM_NULL:
		break;
	//----------------------------------------
	//	Setup Done
	//----------------------------------------
	case GEM_SETUPDONE:
		{
			s_MouseMap.clear();
			m_marker.x = -10.0f;
			m_marker.y = -10.0f;
			//-------------------------------------
			//	Setup Menu
			//-------------------------------------
			int c = 0;

			memset(m_menu,0,sizeof(MenuItem)*8);

			m_menu[0].m_trigger.m_int[1] = int(KA_JOKE);			
			m_menu[1].m_trigger.m_int[1] = int(KA_ANGRY);

			for(int x = 10; x < 700; x += 200)
			for(int y = 480; y < 550; y += 60)
			{
				m_menu[c].m_trigger.m_type = GEM_PERFORM;
				setMouseClip(KMOUSE_MENU + c,m_menu[c].m_trigger ,x + 0, y + 0, x + 180, y + 40,1.5f,false);
				strncpy(m_menu[c].m_name, "-", 3);
				++c;
			}

			strncpy(m_menu[0].m_name, "Joke", 19);
			strncpy(m_menu[1].m_name, "Angry", 19);
			//-------------------------------------
			//	Setup Actor Seclection
			//-------------------------------------			
			gEvent _trigger;
			_trigger.m_type = GEM_SELECT;

			for(int c = _event.m_int[0] - 1; c >= 0; --c)
			{
				unsigned int _id = m_gamePTR->getActorID(c);
				_trigger.m_int[0] = _id;

				//----------------------------
				//	Add Mose Clips
				//----------------------------				
				setMouseClip(_id, _trigger, 0,0,0,0, 0, false);		//	Actor
				setMouseClip(_id + KMOUSE_PROT, _trigger, 10,10+(c*50),52,52+(c*50), 1.5f, false);	//	Portrait
			}
		}
		break;
	};
}

//========================================================================
//	Render Game
//========================================================================
void ptkGameRender::Render()
{
	//-------------------------
	//	Clean
	//-------------------------
	m_gameWin->setDefaultWorldView();
	m_gameWin->setClearColor(0,0,0,1);

	//-------------------------
	//	Draw Blank
	//-------------------------
	m_gfxBack->blitRect(0,0,800,600, 0, 0);

	m_gameTxt->drawStringCentered(__TIMESTAMP__,0,800,100);

	//-------------------------
	//	Draw Stage
	//-------------------------
	RenderStage();

	//-------------------------
	//	Draw HUD
	//-------------------------
	RenderPortrait();
	RenderTimeline();
	RenderStageMenu();

	//-------------------------
	// Draw Menu
	//-------------------------
	int _xStep = 800 / 4;
	int _yStep = 800 / 4;

	//RenderMouseMap();
	//-------------------------
	// Buffer Flip
	//-------------------------
	m_gameWin->flipBackBuffer();
}


//========================================================================
//	Render Stage Menu :: Bottom of Screen during play
//========================================================================
void ptkGameRender::RenderStageMenu()
{
	int c = 0;
	for(int x = 10; x < 700; x += 200)
	for(int y = 480; y < 550; y += 60)
	{
		m_gameTxt->drawStringCentered(m_menu[c].m_name,x, x + 180,y+10);
		c++;
	}
}
//========================================================================
//	Render TimeLine
//========================================================================
void ptkGameRender::RenderTimeline()
{
	//------------------------------------------
	//	Retrieve Timeline
	//------------------------------------------
	kActionList::const_iterator _start = m_gamePTR->getStart();
	kActionList::const_iterator _mark = m_gamePTR->getCurr();
	kActionList::const_iterator _end = m_gamePTR->getEnd();
	kActionList::const_iterator _curr = _end;

	//------------------------------------------
	// Work from back
	//------------------------------------------
	bool _done = false;
	int _Ymark = KTIME_HEIGHT - KTIME_Y1;

	while((_Ymark > KTIME_Y1) && (_curr != _start))
	{
		--_curr;

		//------------------------------------------
		//	Is the Action be Acted yet
		//------------------------------------------
		bool _isCurrent = false;
		if(_curr == _mark)
		{
			_isCurrent = true;
		}

		//------------------------------------------
		//	Select Action
		//------------------------------------------
		char _action[20] = {0};
		switch(_curr->m_Action)
		{
		case KA_NULL:
			strncpy(_action, "NULL", 19);
			break;
		case KA_START:
			strncpy(_action, "START", 19);
			break;
		case KA_JOKE:
			strncpy(_action, "JOKE", 19);
			break;
		case KA_ANGRY:
			strncpy(_action, "ANGRY", 19);
			break;
		};

		//------------------------------------------
		//	Print Action
		//------------------------------------------
		char _buffer[30];
		sprintf(_buffer,"%i : %s", _curr->m_id, _action);

		if(_isCurrent)
		{
			_done = true;
			float _per = float(m_gamePTR->getTimeLeft()) / float(_curr->m_time * GAME_SPEED);
			m_gameGFX->drawRect(float(KTIME_X1),(_Ymark - _curr->m_time*5.0f),float(KTIME_X1 + KTIME_WIDTH),float(_Ymark),0,0,0,1.0f);
			m_gameGFX->drawRect(float(KTIME_X1),(_Ymark - _curr->m_time*5.0f),(KTIME_X1 + KTIME_WIDTH*_per),float(_Ymark),0,0,0.5f,1.0f);
		}
		else if(_done)
		{
			m_gameGFX->drawRect(float(KTIME_X1),(_Ymark - _curr->m_time*5.0f),float(KTIME_X1 + KTIME_WIDTH),float(_Ymark),0,0,0,1.0f);
		}
		else
		{
			m_gameGFX->drawRect(float(KTIME_X1),(_Ymark - _curr->m_time*5.0f),float(KTIME_X1 + KTIME_WIDTH),float(_Ymark),0,0,0,0.3f);
		}
		m_gameTxt->drawStringFromLeft(_buffer,KTIME_X1,_Ymark-20);

		m_gameGFX->drawLine(float(KTIME_X1),float(_Ymark),float(KTIME_X1 + KTIME_WIDTH),float(_Ymark),0.8f,0.8f,1.0f,1.0f);

		_Ymark -= _curr->m_time*5;
	}
}
//========================================================================
//	Render Portraits
//========================================================================
void ptkGameRender::RenderPortrait()
{
	//-------------------------
	//	Fetch Actors
	//-------------------------
	for(int c = m_gamePTR->getCastSize() - 1; c >= 0; --c)
	{
		const kActor& _actor = m_gamePTR->getActor(c);

		//---------------------------------
		// Portrait
		//---------------------------------
		if(m_gamePTR->getSelected() == _actor.ID())
			m_gfxActor->blitAlphaRect(80,42,122,84,10,10+(c*50));
		else
			m_gfxActor->blitAlphaRect(80,0,122,42,10,10+(c*50));
		m_gameGFX->drawRect(float(31 - _actor.m_energy),float(52+(c*50)),float(31 + _actor.m_energy),float(58+(c*50)),
							1.0f, 1.0f, 1.0f, 0.8f);
	}
}
//========================================================================
//	Render Stage
//========================================================================
struct wrapKActor{
	const kActor& m_ref; 
	wrapKActor(const kActor& _ref) : m_ref(_ref) {} 
}; 

bool compare_actor(wrapKActor _first, wrapKActor _second){return _first.m_ref.y < _second.m_ref.y;}
void ptkGameRender::RenderStage()
{
	//-------------------------
	//	Fetch Actors
	//-------------------------
	std::list<wrapKActor> _cast;
	for(int c = m_gamePTR->getCastSize() - 1; c >= 0; --c)
	{
		const kActor& _actor = m_gamePTR->getActor(c);
		wrapKActor _temp(_actor);
		_cast.push_back(_temp);
	}

	//-------------------------
	//	Sort by Depth
	//-------------------------
	_cast.sort(compare_actor); // 0.3f

	//-------------------------
	//	Draw Marker
	//-------------------------
	if((m_marker.x > -1.51f) && (m_marker.x < 1.51f) && (m_marker.y > -0.05f) && (m_marker.y < 1.05f))
	{
		float _width = (31.5f + (10.5f * m_marker.y)) * 0.5f;	// Half Width 
		float _height = (80.0f + (30.0f * m_marker.y)) * 0.5f;	// Half Height of ACTOR

		pointI _centre = Project(m_marker.x,m_marker.y);
		m_gfxActor->blitAlphaRectFx(79,108,121,118, _centre.x-21, short(_centre.y + _height), 0.0f, _width / 21.0f, 1.0f);
	}

	//-------------------------
	//	Draw Actors
	//-------------------------
	bool _frontNotDrawn = true;
	while(_cast.empty() == false)
	{
		const kActor& _actor = _cast.front().m_ref;
		float _ax = _actor.x;
		float _ay = _actor.y;
		
		//-------------------------------------
		//	Draw Curtains if past Mark
		//-------------------------------------
		if((_frontNotDrawn) && (_ay > 0.3f))
		{
			_frontNotDrawn = false;
			m_gfxFront->blitAlphaRect2d(0,0,800,600, 0, 0);
		}
		//---------------------------------
		//	Draw if on Stage
		//---------------------------------
		if((_ax > -1.51f) && (_ax < 1.51f) && (_ay > -0.05f) && (_ay < 1.05f))
		{
			float _width = (60.0f + (20.0f * _ay)) * 0.5f;	// Half Width 
			float _height = (90.0f + (30.0f * _ay)) * 0.5f;	// Hal Height

			pointI _centre = Project(_ax,_ay);
			moveMouseClip(_actor.ID(), short(_centre.x-_width), short(_centre.y-_height), short(_centre.x+_width), short(_centre.y+_height), _ay);
			m_gfxActor->blitAlphaRectFx(0,0,80,120, (_centre.x - 40), (_centre.y - 60), 0.0f, _width / 40.0f, 1.0f);
		}
		_cast.pop_front();
	}

	//-------------------------------------
	//	Draw Curtains
	// Only called if all actors behind
	//-------------------------------------
	if(_frontNotDrawn)
	{
		m_gfxFront->blitAlphaRect2d(0,0,800,600, 0, 0);
	}
}
//========================================================================
//	Render Mouse Clips
//========================================================================
void ptkGameRender::RenderMouseMap()
{
	unsigned int _temp = rand();

	for(KECiter _iter = s_MouseMap.begin(); _iter != s_MouseMap.end(); ++_iter)
	{
		srand(_iter->second.m_clip[0] + _iter->second.m_clip[1] * 10 + _iter->second.m_clip[2] * 100 + _iter->second.m_clip[3] * 1000);
		m_gfxActor->drawRect(float(_iter->second.m_clip[0]),float(_iter->second.m_clip[1]),float(_iter->second.m_clip[2]),float(_iter->second.m_clip[3]),
							 (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, 0.5f);
	}
	srand(_temp);
}

//========================================================================
//	Set Mouse Click Area
//========================================================================
void ptkGameRender::setMouseClip(unsigned int _id, gEvent _event, int _tx, int _ty, int _bx, int _by, float _depth, bool _button)
{
	KEventClip _clip;
	_clip.m_event = _event;
	_clip.m_button = _button;
	_clip.m_clip[0] = _tx;
	_clip.m_clip[1] = _ty;
	_clip.m_clip[2] = _bx;
	_clip.m_clip[3] = _by;
	_clip.m_depth = _depth;

	s_MouseMap[_id] = _clip;
}
//========================================================================
//	Move Mouse Click Area
//========================================================================
void ptkGameRender::moveMouseClip(unsigned int _id, int _tx, int _ty, int _bx, int _by, float _depth)
{
	KEventClip& _clip = s_MouseMap[_id];
	_clip.m_clip[0] = _tx;
	_clip.m_clip[1] = _ty;
	_clip.m_clip[2] = _bx;
	_clip.m_clip[3] = _by;
}
//========================================================================
//	Get Mouse Click
//========================================================================
void ptkGameRender::MouseClick(int x, int y, bool _button)
{
	if(_button == false)
	{
		gEvent _trigger;
		float _depth = -1000.0f;
		//-------------------------------
		//	Check Mouse Map
		//-------------------------------
		for(KECiter _iter = s_MouseMap.begin(); _iter != s_MouseMap.end(); ++_iter)
		{
			//-------------------------------
			//	Check if this Clip Matches
			//-------------------------------
			if(	(_iter->second.m_button == _button) && 
				(x > _iter->second.m_clip[0]) && (y > _iter->second.m_clip[1]) && 
				(x < _iter->second.m_clip[2]) && (y < _iter->second.m_clip[3]) &&
				(_depth < _iter->second.m_depth))
			{
				_depth = _iter->second.m_depth;
				_trigger = _iter->second.m_event;
			}
		}
		//-------------------------------
		// Matched Clip
		//-------------------------------
		if(_depth > -1.0f)
			m_EventQ.post(INBOX, _trigger);
	}
	else
	{
		m_marker = Unproject(x,y);
		m_EventQ.postif(INBOX, GEM_MOVE_ACTOR,m_gamePTR->getSelected(),m_marker.x, m_marker.y);
	}
}
//========================================================================
//	Translate from Picture (Trapese) to Game (Rect)
//========================================================================
pointF ptkGameRender::Unproject(int x, int y)
{
	y -= 55; // HACK :: People tend to click where feet are

	pointF _result;
	_result.y = float(y - KSTAGE_TOP) / float(KSTAGE_BOTTOM - KSTAGE_TOP);

	float _stageWidth = (KSTAGE_IRIGHT - KSTAGE_ILEFT) + (((KSTAGE_ORIGHT - KSTAGE_OLEFT) - (KSTAGE_IRIGHT - KSTAGE_ILEFT)) * _result.y);	// Stage Width = Min + (Max-Min * %)
	float _offset = KSTAGE_OLEFT + ((KSTAGE_ILEFT-KSTAGE_OLEFT) * (1.0f - _result.y));														// Stage Offset = OL + (IL-OL * (1 - %))

	_result.x = ((x - _offset) / _stageWidth)*2.0f - 1.0f;
	return _result;
}
//========================================================================
//	Translate from Game (Rect) to Picture (Trapese)
//========================================================================
pointI ptkGameRender::Project(float x, float y)
{
	pointI _result;

	float _stageWidth = (KSTAGE_IRIGHT - KSTAGE_ILEFT) + (((KSTAGE_ORIGHT - KSTAGE_OLEFT) - (KSTAGE_IRIGHT - KSTAGE_ILEFT)) * y);	// Stage Width = Min + (Max-Min * %)
	float _offset = KSTAGE_OLEFT + ((KSTAGE_ILEFT-KSTAGE_OLEFT) * (1.0f - y));														// Stage Offset = OL + (IL-OL * (1 - %))

	_result.x = int(_offset + _stageWidth * ((x + 1.0f) / 2.0f));
	_result.y = int(KSTAGE_TOP + (y * (KSTAGE_BOTTOM - KSTAGE_TOP)));
	return _result;
}