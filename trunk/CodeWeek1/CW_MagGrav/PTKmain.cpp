/*				UN-NAMED
	Code Week #1: Magnets and Gravity

	Uses PTK lib located at http://www.phelios.com/ptk/
	http://www.flammablepenguins.com/	- Claire "Kimau" Blackshaw
*/

//--------------------------------
//	Includes
//--------------------------------
#include <list>
#include <ptk.h>
#include "gwVector.h"
#include "gwFlies.h"
#include "gwMagnet.h"
#include "gwMapObject.h"
#include "gwFileHelper.h"

//--------------------------------
//	Define
//--------------------------------
#define		FP_SCREENW		800
#define		FP_SCREENH		600
#define		FP_TITLE		"Flammable Penguins: Magnets and Gravity"

#define		GW_FILE_NOT_FOUND	-55
#define		GW_QUIT				-3
#define		GW_NOT_READY		-1
#define		GW_MENU				2
#define		GW_READY			3
#define		GW_RACE				5
#define		GW_COMPLETE			10

//--------------------------------
//	Predeclare
//--------------------------------
void Init();
void StartLevel();
void Shutdown();

gwMapObject* getObject(const gwVector& _pos);
gwMagnet* getMagnet(const gwVector& _pos);

void UpdateObjects();
void UpdateFlyers();

void CollideAllWalls(const gwVector& _oldPos, gwVector& _newPos);
int CollideWall(const gwVector& _oldPos, gwVector& _newPos, const gwSegment& _wall);

void RenderGame();
void RenderGUI();
void RenderObject(gwMapObject* _obj, short _x, short _y, float _zoom, float _blend);
void RenderMenu();

void SaveLevel(const char* _filename);
void LoadLevel(const char* _filename);

//--------------------------------
//	Globals
//--------------------------------
KWindow*	g_gameWin;			// PTK Window
KTrueText*	g_txtSmall;			// PTK Text Instance
KTrueText*	g_txtBig;			// PTK Text Instance
KGraphic*	g_gfxBack;			// Graphic Back
KGraphic*	g_gfxObj;			// Object Graphics
KGraphic*	g_gfxGui;			// GUI Graphics
KSound*		g_music;			// Music in Backgroound

// TRACKING VAR
unsigned long g_Delta = 0;
unsigned long g_Timer = 0;
int		g_State = -1;
unsigned int g_LevelNum = 0;

gwFlies*	s_Flies;
gwMagnet	g_Mouse;



// MAP EDITING
bool	g_drawing = false;
gwVector g_drawlast;

gwMapObject*	g_selObj = 0;
gwMapObject*	g_dragObj = 0;
gwMagnet*		g_dragMag = 0;

// LISTS
std::list<gwSegment> g_Walls;			typedef std::list<gwSegment>::iterator SegIter;
std::list<gwMapObject*> g_Objects;		typedef std::list<gwMapObject*>::iterator ObjIter;
std::list<gwMagnet*> g_Magnets;			typedef std::list<gwMagnet*>::iterator MagIter;

//========================================================================
//	Handels Events
//========================================================================
bool EventHandler(KEvent* _eventPtr)
{
	switch(_eventPtr->type)
	{
	//--------------------------------
	//	Key Up
	//--------------------------------
	case K_EVENT_KEYUP:
		{
			//---------------------------
			//	If Complete
			//---------------------------
			if(g_State == GW_COMPLETE)
			{
				Init();
				return true;
			}

			switch(_eventPtr->ptkKey)
			{
			case K_VK_LEFT:		g_LevelNum = max(0,g_LevelNum-1);		break;
			case K_VK_RIGHT:	g_LevelNum = min(99,g_LevelNum+1);		break;
			case K_VK_SPACE:	StartLevel();		break;
			case K_VK_ESCAPE:	g_State = GW_QUIT;	break;
				//---------------------------
				//	Load Level
				//---------------------------
			case K_VK_L:
			case K_VK_RETURN:	
				{
					char _temp[50] = {0};
					sprintf(_temp,"level%.2i.map",g_LevelNum);
					LoadLevel(_temp);	
				} break;
				//---------------------------
				//	Save Level
				//---------------------------
			case K_VK_S:
				{
					char _temp[50] = {0};
					sprintf(_temp,"level%.2i.map",g_LevelNum);
					SaveLevel(_temp);	
				} break;
				//---------------------------
				//	Turn Drawing On/Off
				//---------------------------
			case K_VK_W:
				{
					g_drawing = !g_drawing;
					g_drawlast = g_Mouse.m_pos;
				} break;
				//---------------------------
				//	Add Spawner
				//---------------------------
			case K_VK_1:
				{
					gwSpawner* _obj = new gwSpawner(g_Mouse.m_pos,20, 1000);
					g_Objects.push_back(_obj);
				} break;
				//---------------------------
				//	Add Home
				//---------------------------
			case K_VK_2:
				{
					gwHome* _obj = new gwHome(g_Mouse.m_pos,20);
					g_Objects.push_back(_obj);
				} break;
				//---------------------------
				//	Add + Magnet
				//---------------------------
			case K_VK_ADD:
				{
					gwMagnet* _mag = new gwMagnet(3.0f,g_Mouse.m_pos);
					g_Magnets.push_back(_mag);
				} break;
				//---------------------------
				//	Add - Magnet
				//---------------------------
			case K_VK_SUBTRACT:
				{
					gwMagnet* _mag = new gwMagnet(-3.0f,g_Mouse.m_pos);
					g_Magnets.push_back(_mag);
				} break;
				//---------------------------
				//	Del Object
				//---------------------------
			};

			return true;
		}
		break;
	//--------------------------------
	//	Process Mouse Move
	//--------------------------------
	case K_EVENT_MOUSEMOVE:
		{
			g_Mouse.m_pos.x = float(_eventPtr->mouseX);
			g_Mouse.m_pos.y = float(_eventPtr->mouseY);

			if(g_dragObj != 0)		g_dragObj->m_pos = g_Mouse.m_pos;
			if(g_dragMag != 0)		g_dragMag->m_pos = g_Mouse.m_pos;

			return true;
		}
		break;
	//--------------------------------
	//	Process Mouse Down
	//--------------------------------
	case K_EVENT_MOUSEDOWN:
		{
			g_Mouse.m_pos.x = float(_eventPtr->mouseX);
			g_Mouse.m_pos.y = float(_eventPtr->mouseY);

			switch(_eventPtr->buttonIndex)
			{
			case K_LBUTTON:			
				{
				}break;
				//------------------------
				//	Pick up and Drag
				//------------------------
			case K_RBUTTON:			
				{
					g_selObj = 0;
					g_dragObj = getObject(g_Mouse.m_pos);
					if(g_dragObj == 0)
					{
						g_dragMag = getMagnet(g_Mouse.m_pos);
					}

				}break;
			}


		}
		break;

	//--------------------------------
	//	Process Mouse Up
	//--------------------------------
	case K_EVENT_MOUSEUP:
		{
			g_Mouse.m_pos.x = float(_eventPtr->mouseX);
			g_Mouse.m_pos.y = float(_eventPtr->mouseY);

			switch(_eventPtr->buttonIndex)
			{
			case K_LBUTTON:			
				{
					//----------------------------
					//	Drawing Wall
					//----------------------------
					if(g_drawing == true)
					{
						g_Walls.push_back(gwSegment(g_drawlast,g_Mouse.m_pos));
						g_drawlast = g_Mouse.m_pos;
					}
					//----------------------------
					//	Select Object
					//----------------------------
					{
						g_selObj = getObject(g_Mouse.m_pos);
					}
					return true;
				}break;
				//------------------------
				// Release Dargged Object
				//------------------------
			case K_RBUTTON:			
				{
					if(g_dragObj != 0)
					{
						g_dragObj->m_pos = g_Mouse.m_pos;
						g_dragObj = 0;
					}
					if(g_dragMag != 0)
					{
						g_dragMag->m_pos = g_Mouse.m_pos;
						g_dragMag = 0;
					}
					return true;
				}
				break;
			}
		}
		break;
	}
	return true;
}
//========================================================================
//	WINDOWS MAIN FUNCTION
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	//-----------------------------------
	//	Init Window
	//-----------------------------------
	g_State = GW_NOT_READY;
	unsigned long _oldTime = KMiscTools::getMilliseconds();
	unsigned long _newTime = 0;
	srand(_oldTime);											// Seed Random

	//-----------------------------------
	//	Create Window
	//-----------------------------------
	g_gameWin = KPTK::createKWindow(K_OPENGL);													// Init Window
	g_gameWin->createGameWindow(FP_SCREENW,FP_SCREENH,32,true,FP_TITLE);						// Create Window
	g_gameWin->setPTKCallBack(&EventHandler);

	//-----------------------------------
	//	Graphic
	//-----------------------------------
	g_gfxBack = KPTK::createKGraphic("back.png");
	g_gfxObj = KPTK::createKGraphic("obj.png");
	g_gfxGui = KPTK::createKGraphic("gui.png");

	//-----------------------------------
	//	Get Font
	//-----------------------------------
	KTrueText _Smallfont("5by5.ttf");												// Load Font
	g_txtSmall = &_Smallfont;														// Link PTR
	g_txtSmall->setHeightPix(15);
	g_txtSmall->setColor(0.0f,0.0f,0.0f,1.0f);

	KTrueText _Bigfont("5by5.ttf");													// Load Font
	g_txtBig = &_Bigfont;															// Link PTR
	g_txtBig->setHeightPix(30);
	g_txtBig->setColor(1.0f,1.0f,1.0f,0.8f);

	//-----------------------------------
	//	Sound
	//-----------------------------------
	g_music = new KSound ;
	if(g_music->loadStream("music.ogg"))
	{
		g_music->setGlobalVolumes(20,10);
        g_music->playStream(true);
	}

	//-----------------------------------
	//	Init Game
	//-----------------------------------
	g_Magnets.push_back(&g_Mouse);
	s_Flies = gwFlies::Instance();

	Init();

	//-----------------------------------
	//	Game Loop
	//-----------------------------------
	while((g_State > 0) && (g_gameWin->isQuit() == false))
	{
		//-------------------------
		//	Update Delta
		//-------------------------
		{
			_newTime = KMiscTools::getMilliseconds();
			g_Delta = (_oldTime - _newTime) / 100000;
			_oldTime = _newTime;
		}
		//-------------------------
		//	Game Logic
		//-------------------------
		if(g_State == GW_RACE)
		{
			g_Timer += g_Delta / 10000;
			UpdateObjects();
			UpdateFlyers();
		}

		//-------------------------
		//	Render
		//-------------------------
		if(g_State == GW_MENU)
			RenderMenu();
		else
			RenderGame();

		//-------------------------
		// Buffer Flip and Process
		//-------------------------
		g_gameWin->flipBackBuffer();
		g_gameWin->processEvents();
	}

	//-----------------------------------
	//	De-Init and Shutdown
	//-----------------------------------
	Shutdown();
	g_music->stopStream();
}

/*##################################################

			LOGIC SECTION

###################################################*/
//======================================================================================
//	Init Game
//======================================================================================
void Init()
{
	//------------------------------
	//	Setup Mouse Magnet
	//------------------------------
	g_Mouse.m_size = 10.0f;

	//------------------------------
	//	Level
	//------------------------------


	//------------------------------
	//	Set State Ready
	//------------------------------
	g_State = GW_MENU;
}

//======================================================================================
//	Starts a Level
//======================================================================================
void StartLevel()
{
	//------------------------------
	//	Start only when Ready
	//------------------------------
	if(g_State != GW_READY)
		return;

	unsigned int _flyCount = 0;
	//------------------------------
	//	Count Finishes and Exits
	//------------------------------
	for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
	{
		gwMapObject* _obj = (*_iter);
		_obj->Init();

		if(_obj->Type() == GW_OBJ_SPAWNER)
		{
			gwSpawner* _spawn = static_cast<gwSpawner*>(_obj);
			_flyCount+= _spawn->m_max;
		}
	}
	//------------------------------
	//	Setup Flyers
	//------------------------------
	s_Flies->Kill();
	s_Flies->Init(_flyCount);

	//------------------------------
	//	Set Timer and State
	//------------------------------
	g_Timer = 0;
	g_State = GW_RACE;
}

//======================================================================================
//	Closes down game
//======================================================================================
void Shutdown()
{
	//------------------------------
	//	Set State Dead
	//------------------------------
	g_State = GW_NOT_READY;

	//------------------------------
	//	Delete Walls
	//------------------------------
	g_Walls.clear();

	//------------------------------
	//	Delete Objects
	//------------------------------
	for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
	{
		if(*_iter)
		{
			delete (*_iter);
		}
	}
	g_Objects.clear();

	//------------------------------
	//	Kill Flyers
	//------------------------------
	s_Flies->Kill();
}

//======================================================================================
//	Get Map Object at Pos
//======================================================================================
gwMapObject* getObject(const gwVector& _pos)
{
	//------------------------------
	//	Loop Through Objects
	//------------------------------
	for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
	{
		gwMapObject& _obj = *(*_iter);

		//------------------------------
		//	Check
		//------------------------------
		if((_obj.m_pos - _pos).SqrLength() < (256.0f))
		{
			return (*_iter);
		}
	}

	return 0;
}
//======================================================================================
//	Get Magnet at Position
//======================================================================================
gwMagnet* getMagnet(const gwVector& _pos)
{
	//------------------------------
	//	Loop Through Objects
	//------------------------------
	for(MagIter _iter = g_Magnets.begin(); _iter !=  g_Magnets.end(); ++_iter)
	{
		gwMagnet& _obj = *(*_iter);

		//------------------------------
		//	Check
		//------------------------------
		if((_obj.m_pos - _pos).SqrLength() < (256.0f))
		{
			return (*_iter);
		}
	}

	return 0;
}
//======================================================================================
//	Update Map Objects
//======================================================================================
void UpdateObjects()
{
	bool _done = true;
	//------------------------------
	//	Loop Through Objects
	//------------------------------
	for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
	{
		gwMapObject& _obj = *(*_iter);
		
		//------------------------------
		//	If Alive Update
		//------------------------------
		if(_obj.Alive() == true)
		{
			_obj.Update(g_Delta);

			//------------------------------
			//	Checks for Completion
			//------------------------------
			if(_obj.Type() == GW_OBJ_HOME)
				_done = false;
		}
	}

	if(_done == true)
	{
		g_State = GW_COMPLETE; 
	}
}
//======================================================================================
//	Update Flyers
//======================================================================================
void UpdateFlyers()
{
	//--------------------------------------
	//	Iterate through Flies
	//--------------------------------------
	for(unsigned int _flyID = 0; _flyID < s_Flies->Amount(); ++_flyID)
	{
		//--------------------------------------
		//	Get Fly check Alive
		//--------------------------------------
		gwFlyer& _fly = s_Flies->getFly(_flyID);
		if(_fly.m_alive == false)
 			continue;

		//--------------------------------------
		//	Work Out Magnets
		//--------------------------------------
		gwVector _acl;

		for(MagIter _iter = g_Magnets.begin(); _iter !=  g_Magnets.end(); ++_iter)
		{
			gwMagnet* _mag = (*_iter);
			_acl += _mag->Force(_fly.m_pos);
		}

		//---------------------------------
		//	Avoid Objects
		//---------------------------------
		for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
		{
			gwMapObject* _obj = (*_iter);

			if(_obj->Alive())
			{
				_acl += Avoid(_fly.m_pos, _obj->m_pos, _obj->m_size);
			}
		}
		//---------------------------------
		//	Avoid Other Flyers
		//---------------------------------
		for(unsigned int _otherID = 0; _otherID < s_Flies->Amount(); ++_otherID)
		{
			if(_flyID != _otherID)
			{
				//--------------------------------------
				//	Get Fly check Alive
				//--------------------------------------
				gwFlyer& _other = s_Flies->getFly(_otherID);
				if(_other.m_alive == false)
					continue;

				_acl += Avoid(_fly.m_pos,_other.m_pos, 5.0f);
			}
		}/**/

		//---------------------------------
		//	Update Flyer
		//---------------------------------
		_fly.m_vel += _acl * float(g_Delta) / 60000.0f;
		_fly.m_vel.Cap(1.5f);

		gwVector _newPos = _fly.m_pos + (_fly.m_vel * float(g_Delta) / 30000.0f);

		//---------------------------------
		//	Avoid Walls
		//---------------------------------
		CollideAllWalls(_fly.m_pos,_newPos);

		_fly.m_pos = _newPos;
	}
}
//======================================================================================
//	Check vector against Walls
//======================================================================================
void CollideAllWalls(const gwVector& _oldPos, gwVector& _newPos)
{
	for(SegIter _iter = g_Walls.begin(); _iter !=  g_Walls.end(); ++_iter)
	{
		gwSegment& _wall = (*_iter);
		if(CollideWall(_oldPos,_newPos, _wall) < 0)
			return;
	}
}
//======================================================================================
//	Collision Check and Reaction
//======================================================================================
int CollideWall(const gwVector& _oldPos, gwVector& _newPos, const gwSegment& _wall)
{
	//-----------------------------
	//	Collision Check
	//-----------------------------
	gwVector _result;
	if(gwSegment(_oldPos,_newPos).Intersect(_wall, _result) != GW_CROSS)
	{
		return 0;	// No Collision
	}

	//-----------------------------
	//	Recursion Safe Guard
	//	Stop Stack overflow
	//-----------------------------
	static int _recurCount = 0;
	++_recurCount;

	if(_recurCount > 2)
	{
		--_recurCount;
		_newPos = _oldPos;
		return -1;
	}

	//-----------------------------
	//	Calculate Slide
	//-----------------------------
	gwVector _bounce = (_oldPos - _newPos) - ((_oldPos - _wall.NearestPoint(_newPos)) * 2.0f);
	_newPos = _oldPos + _bounce;

    //-----------------------------
	//	Recursive check to see if 
	//	new Vector doesn't collide
	//	Removing will speed up but
	//	occasional leaks will happen
	//-----------------------------
	CollideAllWalls(_oldPos,_newPos);
	--_recurCount;

	return 1;
}

/*##################################################

			RENDER SECTION

###################################################*/
//======================================================================================
//	Main Render
//======================================================================================
void RenderGame()
{
	g_gfxBack->blitRect(0,0,FP_SCREENW,FP_SCREENH,0,0);
	//g_gfxBack->drawRect(0,0,FP_SCREENW,FP_SCREENH,1,1,1,1.0f);

	//---------------------------------
	//	Draw Walls
	//---------------------------------
	for(SegIter _iter = g_Walls.begin(); _iter !=  g_Walls.end(); ++_iter)
	{
		g_gfxBack->drawLine(_iter->sx,_iter->sy,_iter->ex,_iter->ey, 0.0f, 0.6f, 0.1f, 1.0f);
	}

	if(g_drawing == true)
	{
		g_gfxBack->drawLine(g_drawlast.x,g_drawlast.y,g_Mouse.m_pos.x,g_Mouse.m_pos.y, 1.0f, 0.2f, 0.2f, 1.0f);
	}

	//---------------------------------
	//	Draw Objects
	//---------------------------------
	for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
	{
		gwMapObject* _obj = (*_iter);
		RenderObject(_obj, short(_obj->m_pos.x), short(_obj->m_pos.y), 0.5f, 1.0f);
	}

	//---------------------------------
	//	Draw Flyers
	//---------------------------------
	for(unsigned int _flyID = 0; _flyID < s_Flies->Amount(); ++_flyID)
	{
		//--------------------------------------
		//	Get Fly check Alive
		//--------------------------------------
		gwFlyer& _fly = s_Flies->getFly(_flyID);
		if(_fly.m_alive == false)
			continue;

		//--------------------------------------
		//	Draw Fly
		//--------------------------------------
		g_gfxBack->drawRect(_fly.m_pos.x-2,_fly.m_pos.y-2,_fly.m_pos.x+2,_fly.m_pos.y+2, 1.0f, 0.0f, 0.0f, 1.0f);
	}

	//---------------------------------
	//	Draw GUI
	//---------------------------------
	if(g_State == GW_READY)
		RenderGUI();

	//---------------------------------
	//	Draw Timer
	//---------------------------------
	{
		int _mins	= (g_Timer /100) / 60;
		int _seconds= (g_Timer /100) % 60;
		int _mili	= g_Timer % 100;
		char _buffer[25] = {0};
		sprintf(_buffer,"%i:%.2i:%.2i",_mins,_seconds,_mili);

		g_txtBig->drawStringFromRight(_buffer,FP_SCREENW,FP_SCREENH-40);
	}
}
//======================================================================================
//	Render GUI
//======================================================================================
#define	RGI_PICTURE	40,40,1.0f,0.8f
#define RGI_HEADER	100,200,0
#define RGS_R1L1	100,28
#define RGS_R1L2	100,38

void RenderGUI()
{
	g_gfxBack->drawRect(0,0,FP_SCREENW,100, 0.3f, 0.3f, 0.3f, 0.2f);

	if(g_selObj != 0)
	{
		RenderObject(g_selObj, RGI_PICTURE);
		g_gfxGui->blitAlphaRectFx(0,0,64,64,short(g_selObj->m_pos.x) - 32,short(g_selObj->m_pos.y) - 32,0,0.8f,0.6f);

		switch(g_selObj->Type())
		{
		case GW_OBJ_SPAWNER:
			g_txtBig->drawStringCentered("Worm Hole",RGI_HEADER);
			g_txtSmall->drawStringFromLeft("Your planes will arrive",		RGS_R1L1);
			g_txtSmall->drawStringFromLeft("through this one at a time.",	RGS_R1L2);
			// SPEED UPGRADE
			// AMOUNT
			break;
		case GW_OBJ_HOME:
			g_txtBig->drawStringCentered("Hanger",RGI_HEADER);
			g_txtSmall->drawStringFromLeft("Land your planes here",			RGS_R1L1);
			g_txtSmall->drawStringFromLeft("for safety.",					RGS_R1L2);
			// CAPACITY
			break;
		};
	}
}

//======================================================================================
//	Render Object
//======================================================================================
void RenderObject(gwMapObject* _obj, short _x, short _y, float _zoom, float _blend)
{
	switch(_obj->Type())
	{
		//-------------------------------
		//	Base Object
		//	Should not be used
		//-------------------------------
	case GW_OBJECT:
		g_gfxBack->drawRect(float(_x - 8), float(_y - 8), float(_x + 8), float(_y + 8), 0.3f, 0.3f, 0.3f, _blend);
		break;
		//-------------------------------
		//	Spawner
		//-------------------------------
	case GW_OBJ_SPAWNER:
		{
			if(_obj->Alive())
			{
				g_gfxObj->blitAlphaRectFx(0,0,64,64,_x-32,_y-32,0, _zoom, _blend);

				gwSpawner* _spawn = static_cast<gwSpawner*>(_obj);
				char _buffer[5] = {0};
				itoa((_spawn->m_max - _spawn->m_spawned),_buffer,10);

				g_txtSmall->drawStringCentered(_buffer, _x - 10, _x + 10, _y+32);
			}
			else
			{
				g_gfxObj->blitAlphaRectFx(64,0,128,64,_x-32,_y-32,0, _zoom, _blend);
			}
		}
		break;
		//-------------------------------
		//	Home
		//-------------------------------
	case GW_OBJ_HOME:
		{
			if(_obj->Alive())
			{
				g_gfxObj->blitAlphaRectFx(0,64,64,128,_x-32,_y-32,0, _zoom, _blend);

				gwHome* _home = static_cast<gwHome*>(_obj);
				char _buffer[5] = {0};
				itoa((_home->m_max - _home->m_housed),_buffer,10);

				g_txtSmall->drawStringCentered(_buffer, _x - 10, _x + 10, _y+32);
			}
			else
			{
				g_gfxObj->blitAlphaRectFx(64,64,128,128,_x-32,_y-32,0, _zoom, _blend);
			}
		}
		break;
	};
}

//======================================================================================
//	Save Level
//======================================================================================
void RenderMenu()
{
	g_gfxBack->blitRect(0,0,FP_SCREENW,FP_SCREENH,0,0);

	g_txtBig->drawStringCentered("Maggilz",0,FP_SCREENW,0);
	g_txtSmall->drawStringCentered("A Swarm/Magnet game made in one weekend",0,FP_SCREENW,30);


	g_txtSmall->drawStringCentered("Level",0,FP_SCREENW,100);
	g_txtBig->setColor(1.0f,0.2f,0.2f,1.0f);
	char _temp[10] = {0};
	itoa(g_LevelNum,_temp,10);
	g_txtBig->drawStringCentered(_temp,0,FP_SCREENW,118);


	g_txtSmall->drawStringCentered("I AM OUT OF TIME!!",0,FP_SCREENW,300);
	g_txtSmall->drawStringCentered("Press Enter to Play and see the Readme!",0,FP_SCREENW,320);


	g_txtBig->setColor(1,1,1,0.8f);
	g_txtSmall->setColor(0,0,0,1.0f);
}

/*##################################################

			FILE SECTION

###################################################*/
//======================================================================================
//	Save Level
//======================================================================================
void SaveLevel(const char* _filename)
{
	//-------------------------------
	//	Open File
	//-------------------------------
	FILE* _file = fopen(_filename,"wb");

	//-------------------------------
	//	Save File Header
	//	MAGICNUM	: 666	Shows that right file type
	//	VERSION		: X		Ensure Right Version
	//	WALLCOUNT	: X		Amount of Walls
	//	OBJCOUNT	: X		Amount of Objects
	//-------------------------------
	fputc(666,_file);
	fputc(2,_file);
	fputc(int(g_Walls.size()),_file);
	fputc(int(g_Objects.size()),_file);

	//-------------------------------
	//	Save Walls
	//-------------------------------
	for(SegIter _iter = g_Walls.begin(); _iter !=  g_Walls.end(); ++_iter)
	{
		GW_File::save((*_iter),_file);
	}

	//-------------------------------
	//	Save Objects
	//	TYPE		: Type of Object
	//	OBJECT		: Object
	//-------------------------------
	for(ObjIter _iter = g_Objects.begin(); _iter !=  g_Objects.end(); ++_iter)
	{
		GW_File::save((*_iter), _file);
	}

	//-------------------------------
	//	Close File
	//-------------------------------
	fclose(_file);
}

//======================================================================================
//	Load Level
//======================================================================================
void LoadLevel(const char* _filename)
{
	//---------------------------
	//	Close Level
	//---------------------------
	Shutdown();
	
	//---------------------------
	//	Check File Exsists
	//---------------------------
	if(_filename != 0)
	{
		FILE* _file = fopen(_filename,"rb");
		if (_file == NULL)
		{
			g_State = GW_FILE_NOT_FOUND;
			//return;
		}
		else
		{
			//-------------------------------
			//	Load File Header
			//	MAGICNUM	: 666	Shows that right file type
			//	VERSION		: 1		Ensure Right Version
			//	WALLCOUNT	: X		Amount of Walls
			//	OBJCOUNT	: X		Amount of Objects
			//-------------------------------
			int _magicNum = fgetc(_file);
			int _version = fgetc(_file);
			int _wallCount = fgetc(_file);
			int _objCount = fgetc(_file);

			//-------------------------------
			//	Load Walls
			//-------------------------------
			for(int c = 0; c < _wallCount; ++c)
			{
				gwSegment _temp;
				GW_File::load(_temp, _file);
				g_Walls.push_back(_temp);
			}

			//-------------------------------
			//	Load Objects
			//	TYPE		: Type of Object
			//	OBJECT		: Object
			//-------------------------------
			for(int c = 0; c < _objCount; ++c)
			{
				gwMapObject* _obj = GW_File::loadObject(_file);		
				if(_obj != 0)
				{
					g_Objects.push_back(_obj);
				}
			}

			//-------------------------------
			//	Close File
			//-------------------------------
			fclose(_file);
		}
	}
	//-------------------------------
	//	Init Game
	//-------------------------------
	Init();
	g_State = GW_READY;
}