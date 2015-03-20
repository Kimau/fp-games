/*****************************************************************************
  Title: Raider of the Block
  File: D:\CODE\RAIDER\RAIDER\PTKMAIN.CPP
  Author: Claire Blackshaw
  Created: 2007/11/30
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/

/*----------------------------------
	Includes
----------------------------------*/
#include "BigHead.h"
#include "DrawShapes.h"
#include "FileHelper.h"
#include <math.h>

/*----------------------------------
	Pre-declare
----------------------------------*/
void DrawObject(void *ptr, void *unused);
void RenderGame();
void startObjects();
void addStaticBorder();
int ColPlayerItem(cpShape* _player, cpShape* _item, cpContact* _contacts, int _numContacts, cpFloat _normalCoef, void* _data);
int ColPlayerWall(cpShape* a, cpShape* b, cpContact* _contacts, int _numContacts, cpFloat _normalCoef, void* _data);
void PlayerJump();
void PlayerStick( cpShape* _wall, bool _corner );
cpVect NearestPoint(const cpVect& _point, const cpVect& _start, const cpVect& _end);
void SlideAroundEdges(void* _obj, void* _data);
void UpdateGame();
void getIDforCollisionPointonWall(int& _id );

/*----------------------------------
	PTK Handles
----------------------------------*/
KWindow*	g_gameWin = 0;		// PTK Window
KTrueText*	g_font = 0;			// PTK Text Instance
KGraphic*	g_gfxBack = 0;		// Graphic Back
gpPen*		g_pen = 0;

/*----------------------------------
	 Chipmunk Handles
 ----------------------------------*/
cpSpace*	g_space = 0;
cpBody*		g_player = 0;
cpJoint*	g_groove = 0;
cpVect		g_grVect[4] = {cpvzero, cpvzero, cpvzero, cpvzero};
const cpFloat g_impulse = 100.0f;
cpSegmentShape*	g_collideWall = 0;
cpSegmentShape* g_pastWall = 0;
cpShape*	g_delMe = 0;
/*----------------------------------
	Flags and Globals
----------------------------------*/
gpAction g_action = GPA_NULL;	// Action player is currently doing
int		g_xDir = 0;
int		g_yDir = 0;
bool	g_draw = false;			// Are we in drawing mode
cpVect	g_last = cpvzero;		// Last Mouse click
cpVect	g_mouse = cpvzero;		// Current Mouse Position
unsigned long g_delta;			// Stores Game time
int		g_state = GPS_NULL;		// Stores game state
bool	g_loopFlag = false;

//============================================================================
//  Handel Events
//============================================================================
bool EventHandler(KEvent* _eventPtr)
{

	/*----------------------------------
		 Determine Type of Action
	 ----------------------------------*/
	switch(_eventPtr->type)
	{
		/*----------------------------------
			 Key Press Up
		 ----------------------------------*/
	case K_EVENT_KEYUP:
		switch(_eventPtr->ptkKey)
		{
			/*----------------------------------
			Directional Controls
			----------------------------------*/
		case K_VK_NUM1:
		case K_VK_NUM2:
		case K_VK_NUM3:
		case K_VK_NUM4:
		case K_VK_NUM6:
		case K_VK_NUM7:
		case K_VK_NUM8:
		case K_VK_NUM9:		g_xDir = 0;	g_yDir = 0;	break;
			/*----------------------------------
				 ACTION KEY UP
			 ----------------------------------*/
		case K_VK_HOME:
		case K_VK_SPACE:
			{
				switch(g_action)
				{
					/*----------------------------------
						 Stop trying to Stick
					 ----------------------------------*/
				case GPA_FLOOR:
				case GPA_NULL:
				case GPA_GRABY:  g_action = GPA_NULL; break;
					/*----------------------------------
						 Trying to Jump
					 ----------------------------------*/
				case GPA_JUMP: 
				case GPA_STICKY: g_action = GPA_JUMP; break;
				}
			} break;
			/*----------------------------------
				 Add ITEM
			 ----------------------------------*/
		case K_VK_I:
			{
				addItem(g_space,g_mouse);
			}
			break;
			/*----------------------------------
				 Enter/Exit drawing Mode
			 ----------------------------------*/
		case K_VK_D:	
			{
				g_draw = !g_draw;
				g_last = cpv((cpFloat)(_eventPtr->mouseX),(cpFloat)(FP_WORLDH- _eventPtr->mouseY));
			} break;
			/*----------------------------------
				 Save map to File
			 ----------------------------------*/
		case K_VK_S:
			{
				SaveSpace(g_space,"test.map");
			} break;
			/*----------------------------------
				 Load map from File
			 ----------------------------------*/
		case K_VK_L:
			{
				cpSpaceFree(g_space);
				g_space = LoadSpace("test.map");
				g_state = GPS_READY;
				startObjects();
			} break;
			/*----------------------------------
				 Start/Pause Physics
			 ----------------------------------*/
		case K_VK_RETURN:
			{
				if((g_state == GPS_READY) || (g_state == GPS_PAUSE))
				{
					g_state = GPS_PLAYING;
				}
				else if (g_state == GPS_PLAYING)
				{
					g_state = GPS_PAUSE;
				}
			} break;
		} break;
		/*----------------------------------
			 Key Press In
		 ----------------------------------*/
	case K_EVENT_KEYDOWN:
		switch(_eventPtr->ptkKey)
		{
			/*----------------------------------
			Directional Controls
			----------------------------------*/
		case K_VK_NUM1:		g_xDir =-1;	g_yDir =-1;	break;
		case K_VK_NUM2:		g_xDir = 0;	g_yDir =-1;	break;
		case K_VK_NUM3:		g_xDir = 1;	g_yDir =-1;	break;

		case K_VK_NUM4:		g_xDir =-1;	g_yDir = 0;	break;
		case K_VK_NUM6:		g_xDir = 1;	g_yDir = 0;	break;

		case K_VK_NUM7:		g_xDir =-1;	g_yDir = 1;	break;
		case K_VK_NUM8:		g_xDir = 0;	g_yDir = 1;	break;
		case K_VK_NUM9:		g_xDir = 1;	g_yDir = 1;	break;
			/*----------------------------------
				ACTION KEY DOWN
			----------------------------------*/
		case K_VK_HOME:
		case K_VK_SPACE:
			{
				switch(g_action)
				{
				case GPA_FLOOR:
				case GPA_NULL:	g_action = GPA_GRABY; break;
				case GPA_GRABY: break;
				case GPA_JUMP: break;
				case GPA_STICKY: break;
				}
			} break;
		} break;
		/*----------------------------------
			 Update Mouse Cursor
		 ----------------------------------*/
	case K_EVENT_MOUSEMOVE:
		{
			g_mouse = cpv((cpFloat)(_eventPtr->mouseX),(cpFloat)(FP_WORLDH- _eventPtr->mouseY));
		} break;
		/*----------------------------------
			 Mouse Button Up
		 ----------------------------------*/
	case K_EVENT_MOUSEUP:
		{
			if (g_draw)
			{
				cpVect _new = cpv((cpFloat)(_eventPtr->mouseX),(cpFloat)(FP_WORLDH- _eventPtr->mouseY));
				addStaticSegment(g_last,_new,g_space);
				g_last = _new;
			}
		} break;
	}
	return true;
}

//============================================================================
//  WINDOWS MAIN FUNCTION
//============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	/*----------------------------------
		 Init Window
	 ----------------------------------*/
	g_state = GPS_SETUP;
	srand(KMiscTools::getMilliseconds());									// Seed Random

	/*----------------------------------
		 Create Window
	 ----------------------------------*/
	g_gameWin = KPTK::createKWindow(K_OPENGL);													// Init Window
	g_gameWin->createGameWindow(FP_SCREENW,FP_SCREENH,32,true,FP_TITLE);						// Create Window
	g_gameWin->setPTKCallBack(&EventHandler);

	/*----------------------------------
		 Graphic
	 ----------------------------------*/
	g_gfxBack = KPTK::createKGraphic("back.png");
	gpPen _pen;
	g_pen = &_pen;

	/*----------------------------------
		 Get Font
	 ----------------------------------*/
	KTrueText _font("5by5.ttf");											// Load Font
	g_font = &_font;														// Link PTR
	g_font->setHeightPix(20);
	g_font->setColor(0.0f,0.0f,0.0f,1.0f);

	/*----------------------------------
		 Start Chipmunk Physics
	 ----------------------------------*/
	cpInitChipmunk();
	cpResetShapeIdCounter();

	g_space = LoadSpace("test.map");

	addStaticBorder();

	startObjects();


	/*----------------------------------
		 Init Game
	 ----------------------------------*/
	g_state = GPS_READY;

	/*----------------------------------
		 Game Loop
	 ----------------------------------*/
	while((g_state >= GPS_READY) && (g_gameWin->isQuit() == false))
	{
		/*----------------------------------
			Game Logic
		----------------------------------*/
		if(g_state == GPS_PLAYING)
		{
			UpdateGame();

		}
		else
		{
			g_delta = 0;
		}

		/*----------------------------------
			Render
		----------------------------------*/
		g_gameWin->setDefaultWorldView();
		RenderGame();


		/*----------------------------------
			 Buffer Flip and Process
		 ----------------------------------*/
		g_gameWin->flipBackBuffer();
		g_gameWin->processEvents();
	}

	/*----------------------------------
		 De-Init and Shutdown
	 ----------------------------------*/
	cpSpaceFree(g_space);
	return 0;
}

//============================================================================
//  Draw Object :: Callback function used by Chipmunk
//============================================================================
void DrawObject(void* _ptr, void* _unused)
{
	g_pen->DrawShape((cpShape *)_ptr);
}

//============================================================================
//  Render the Game
//============================================================================
void RenderGame()
{
	g_gfxBack->drawRect(0,0,FP_WORLDW,FP_WORLDH,1,1,1,1.0f);

	/*----------------------------------
		 Draw Physics Objects
	 ----------------------------------*/
	cpSpaceHashEach(g_space->activeShapes, &DrawObject, NULL);
	cpSpaceHashEach(g_space->staticShapes, &DrawObject, NULL);

	/*----------------------------------
		 Show guidelines
	 ----------------------------------*/
	if (g_draw)
	{
		g_gfxBack->drawLine(g_last.x,(FP_WORLDH - g_last.y),g_mouse.x,(FP_WORLDH - g_mouse.y), 1.0f,0.0f,0.0f, 1.0f);
	}
}

//============================================================================
//  Collision Between Player and Item
//============================================================================
int ColPlayerItem(cpShape* _player, cpShape* _item, cpContact* _contacts, int _numContacts, cpFloat _normalCoef, void* _data)
{
	_item->collision_type = GPC_DELETE;
	g_delMe = _item;
	return 0;
}
//============================================================================
//  Collision between Player and Walls
//============================================================================
int ColPlayerWall(cpShape* _player, cpShape* _wall, cpContact* _contacts, int _numContacts, cpFloat _normalCoef, void* _data)
{
	/*----------------------------------
		 Process Action
	 ----------------------------------*/
	switch(g_action)
	{
		/*----------------------------------
			 Running Normally
		 ----------------------------------*/
	case GPA_FLOOR:
	case GPA_NULL:
		{
			g_action = GPA_FLOOR;
		}
		break;
		/*----------------------------------
			 Jumping off Surface
		 ----------------------------------*/
	case GPA_JUMP:	// Jumping off Surface
		{
			PlayerJump();
		}
		break;
		/*----------------------------------
			Grab onto Surface
		 ----------------------------------*/
	case GPA_GRABY:
		{
			g_action = GPA_STICKY;
		}
		/*----------------------------------
			 Run in Groove
		 ----------------------------------*/
	case GPA_STICKY:
		{
			/*----------------------------------
				 Check Collision is not already Groove
			 ----------------------------------*/
			if (g_collideWall != (cpSegmentShape*) _wall)
			{
				/*----------------------------------
				Stick onto Wall
				----------------------------------*/
				PlayerStick(_wall, false);
			}
		} break;
	}

	return 1;
}

//============================================================================
//  Start Level with Adding Objects
//============================================================================
void startObjects()
{
	/*----------------------------------
		Add Player
	----------------------------------*/
	g_player = addPlayer(g_space);
	//g_player->p = cpv(400,550);

	/*----------------------------------
		 Add Collision Pair
	 ----------------------------------*/
	cpSpaceAddCollisionPairFunc(g_space,GPC_PLAYER,GPC_STATIC,ColPlayerWall,0);
	cpSpaceAddCollisionPairFunc(g_space,GPC_PLAYER,GPC_ITEM,ColPlayerItem,0);
}

//============================================================================
//  Add Static Borders around Map
//============================================================================
void addStaticBorder()
{
	/*----------------------------------
		Draw Border
	----------------------------------*/
	addStaticSegment(cpvzero,			cpv(FP_WORLDW,0),			g_space);
	addStaticSegment(cpv(0,FP_WORLDH),	cpv(FP_WORLDW,FP_WORLDH),	g_space);
	addStaticSegment(cpvzero,			cpv(0,FP_WORLDH),			g_space);
	addStaticSegment(cpv(FP_WORLDW,0),	cpv(FP_WORLDW,FP_WORLDH),	g_space);
}

//============================================================================
//  Player Jumps
//============================================================================
void PlayerJump()
{
	/*----------------------------------
		Remove Joint
	----------------------------------*/
	if(g_groove != 0)
	{
		cpSpaceRemoveJoint(g_space,g_groove);
		cpJointFree(g_groove);
		g_groove = 0;
	}
	g_collideWall = 0;
	/*----------------------------------
		Jump
	----------------------------------*/
	cpBodyResetForces(g_player);
	cpBodyApplyImpulse(g_player,cpvmult(g_grVect[2],150.0f), cpvzero);
	g_action = GPA_NULL;
}

//============================================================================
//  Player Sticks onto a Wall
//============================================================================
void PlayerStick(cpShape* _wall, bool _corner)
{
	/*----------------------------------
		 Parameters
	 ----------------------------------*/
	const cpFloat _offset = 13.5f;
	/*----------------------------------
		Check static body 
	----------------------------------*/
	static cpBody* s_staticbody = 0;
	if(s_staticbody == 0)	
		s_staticbody = cpBodyNew(INFINITY, INFINITY);
	/*----------------------------------
		Check for Previous Groove
		Create a Replacement
	----------------------------------*/
	if(g_groove != 0)
	{
		/*----------------------------------
			 Delete Groove
		 ----------------------------------*/
		cpSpaceRemoveJoint(g_space,g_groove);
		cpJointFree(g_groove);
		g_groove = 0;
	}

	/*----------------------------------
		 Turning a Corner
	 ----------------------------------*/
	if(_corner)
	{
		/*----------------------------------
			Create Poly for Extrusion
		 ----------------------------------*/
		g_grVect[0] = ((cpSegmentShape*)_wall)->ta;
		g_grVect[1] = ((cpSegmentShape*)_wall)->tb;
		
		int _id = 0;
		getIDforCollisionPointonWall(_id);

		/*----------------------------------
			Get Normal and Perpendicular
		 ----------------------------------*/
		cpVect _diamondPoint = g_grVect[2];

		g_grVect[3] = cpvnormalize(cpvadd(g_grVect[0],g_grVect[1]));
		g_grVect[2] = cpvmult(cpvperp(g_grVect[3]),_offset);

		cpVect _nearPoint = NearestPoint(g_player->p,g_grVect[0],g_grVect[1]);
		_nearPoint = cpvsub(g_player->p,_nearPoint);

		/*----------------------------------
			Flip around offset if wrong 
		 ----------------------------------*/
		if((_nearPoint.y < 0) != (g_grVect[2].y < 0))
		{
			g_grVect[2] = cpvneg(g_grVect[2]);
		}

		_diamondPoint = cpvadd(_diamondPoint, g_grVect[2]);	// Add two offsets together
		_diamondPoint = cpvmult(_diamondPoint, 0.6f);		// Reduce by 80% (Half causes glitch)

		g_grVect[_id] = cpvadd(g_grVect[_id],_diamondPoint);

		_id = 1 - _id;	// Flip ID between 0/1

	
		g_grVect[_id] = cpvadd(g_grVect[_id],g_grVect[2]);
	}
	/*----------------------------------
		 Already Hit New Surface
	 ----------------------------------*/
	else
	{
		/*----------------------------------
			 Create a Fresh Groove
		 ----------------------------------*/
		g_grVect[0] = ((cpSegmentShape*)_wall)->ta;
		g_grVect[1] = ((cpSegmentShape*)_wall)->tb;
		g_grVect[2] = cpvsub(g_player->p,NearestPoint(g_player->p,g_grVect[0],g_grVect[1]));
		g_grVect[2] = cpvmult(cpvnormalize(g_grVect[2]),_offset);
		g_grVect[0] = cpvadd(g_grVect[0],g_grVect[2]);
		g_grVect[1] = cpvadd(g_grVect[1],g_grVect[2]);
		g_grVect[3] = cpvnormalize(cpvadd(g_grVect[0],g_grVect[1]));
	}

	/*----------------------------------
		 Add Joint
	 ----------------------------------*/
	g_groove = cpGrooveJointNew(s_staticbody, g_player, g_grVect[0], g_grVect[1], cpvzero);
	cpSpaceAddJoint(g_space,g_groove);
	
	/*----------------------------------
		 Update Collision Wall
		 Disable Old Wall to Avoid Stutter
	 ----------------------------------*/
	if(g_pastWall != 0)
	{
		((cpWallData*)(g_pastWall->shape.data))->m_deadTime = 0;
	}
	g_pastWall = g_collideWall;
	g_collideWall = (cpSegmentShape*) _wall;

	g_collideWall->shape.data = new cpWallData;
	((cpWallData*)g_collideWall->shape.data)->m_deadTime = 2; // TIME OUT

	/*----------------------------------
		 Eliminate Gravity
	 ----------------------------------*/
	cpBodyResetForces(g_player);
	cpBodyApplyForce(g_player,cpv(0,1500),cpvzero);
}

//============================================================================
//	Nearest point on this Segment
//============================================================================
cpVect NearestPoint(const cpVect& _point, const cpVect& _start, const cpVect& _end)
{
	cpVect _offset = cpvsub(_point,_start);
	cpVect _dir = cpvsub(_end,_start);
	float _oLen = cpvlength(_offset);
	float _dLen = cpvlength(_dir);

	/* SIMPLER VERSION */
	cpFloat _temp = cpvdot(_offset,_dir);
	_temp = ( _oLen * (_temp / (_oLen * _dLen))) / _dLen;
	_dir.x = _dir.x * _temp;
	_dir.y = _dir.y * _temp;

	return cpvadd(_start,_dir);
}

//============================================================================
//  Check for Slide Around Corners
//============================================================================
void SlideAroundEdges(void* _obj, void* _data)
{
	/*----------------------------------
		 Only do this once
	 ----------------------------------*/
	if(g_loopFlag)
	{
		/*----------------------------------
			 Ensure not same Wall
		 ----------------------------------*/
		cpSegmentShape* _shape = (cpSegmentShape*)_obj;
		if (_shape == g_collideWall)
		{
			return;
		}
		/*----------------------------------
			 Valid Circuit
		 ----------------------------------*/
		if((g_pastWall != 0) && (((cpWallData*)(g_pastWall->shape.data))->m_deadTime > 0))
		{
			return;
		}
		/*----------------------------------
			Check Intersect
		 ----------------------------------*/
		cpVect _inPoint = NearestPoint(g_player->p,_shape->ta,_shape->tb);
		cpFloat _length = cpvlength(cpvsub(g_player->p,_inPoint));

		/*----------------------------------
			 Check for Distance
		 ----------------------------------*/
		if(_length > 13.2f)
		{
			return;
		}

		/*----------------------------------
			Check Angle
		----------------------------------*/
		cpFloat _angle = acos(cpvdot(cpvnormalize(cpvsub(_shape->ta,_shape->tb)),g_grVect[3]));
		if(_angle > 2.0f)
		{
			return;
		}

		/*----------------------------------
			 Passed All Checks
		 ----------------------------------*/
		PlayerStick((cpShape*)_shape,true);
		g_loopFlag = false;
	}
	else
	{
		return;
	}
}

//============================================================================
//  Update the Game Tick
//============================================================================
void UpdateGame()
{
	static unsigned long _oldTime = KMiscTools::getMilliseconds();
	unsigned long _newTime = KMiscTools::getMilliseconds();
	g_delta += (_oldTime - _newTime) / 100000;
	_oldTime = _newTime;
	g_delta = min(35000,g_delta);
	/*----------------------------------
		Update Physics
	----------------------------------*/
	while(g_delta > 10000)
	{
		cpSpaceStep(g_space,0.01f);
		g_delta -= 10000;
		/*----------------------------------
			Jump Needs to be Done
		----------------------------------*/
		if(g_action == GPA_JUMP)
		{
			PlayerJump();
		}
		/*----------------------------------
			Move Player
		----------------------------------*/
		switch(g_action)
		{
		case GPA_NULL:
		case GPA_GRABY:
			cpBodyApplyImpulse(g_player,cpv(g_xDir*20.0f,0), cpvzero);
			break;
		case GPA_FLOOR:
			g_action = GPA_NULL;
		case GPA_STICKY:
		case GPA_JUMP:
			cpBodyApplyImpulse(g_player,cpv(g_xDir*20.0f,g_yDir*20.0f), cpvzero);
			break;			
		}
		/*----------------------------------
			Check if Near End of Joint
		----------------------------------*/
		if(false) //g_action == GPA_STICKY)
		{
			/*----------------------------------
				Check if Near end of Joint
			----------------------------------*/
			if( (cpvlength(cpvsub(g_grVect[0],g_player->p)) < 1.0f) ||
				(cpvlength(cpvsub(g_grVect[1],g_player->p)) < 1.0f))
			{
				g_loopFlag = true;
				cpSpaceHashEach(g_space->staticShapes,SlideAroundEdges,0);
			}
		}
		/*----------------------------------
			 Countdown
		 ----------------------------------*/
		if(g_pastWall != 0)
		{
			cpWallData* _data = (cpWallData*)(g_pastWall->shape.data);
			_data->m_deadTime = max(0,_data->m_deadTime-1);
		}

		/*----------------------------------
			 Delete Object
		 ----------------------------------*/
		if(g_delMe != 0)
		{
			cpSpaceRemoveStaticShape(g_space, g_delMe);
			g_delMe = 0;
		}
	}
}

//============================================================================
//  Get ID for the Collision Point to use when going around a Corner
//============================================================================
void getIDforCollisionPointonWall(int& _id )
{

	cpFloat _a0Len = cpvlength(cpvsub(g_grVect[0],g_collideWall->ta));
	cpFloat _a1Len = cpvlength(cpvsub(g_grVect[1],g_collideWall->ta));
	cpFloat _b0Len = cpvlength(cpvsub(g_grVect[0],g_collideWall->tb));
	cpFloat _b1Len = cpvlength(cpvsub(g_grVect[1],g_collideWall->tb));

	/*----------------------------------
	Find Closest Point
	Save Point into id
	Save other collide wall point
	HACK :: Horrible Code I know
	----------------------------------*/
	if(_a0Len < _a1Len)
	{
		if(_a0Len < _b0Len)
		{
			if(_a0Len < _b1Len)
			{
				_id = 0;
			}
			else
			{
				_id = 1;
			}
		} 
		else
		{
			if(_b0Len < _b1Len)
			{
				_id = 0;
			}
			else
			{
				_id = 1;
			}
		}
	}
	else
	{
		if(_a1Len < _b0Len)
		{
			if(_a1Len < _b1Len)
			{
				_id = 1;
			}
			else
			{
				_id = 1;
			}
		} 
		else
		{
			if(_b0Len < _b1Len)
			{
				_id = 0;
			}
			else
			{
				_id = 1;
			}
		}
	}
}