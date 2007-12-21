/*****************************************************************************
  Title: Draw Shapes
  File: D:\CODE\RAIDER\RAIDER\DRAWSHAPES.CPP
  Author: Claire Blackshaw
  Created: 2007/12/01
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#include "DrawShapes.h"
#include <math.h>

//============================================================================
//  Constructor
//============================================================================
gpPen::gpPen()
{
	m_pen = KPTK::createKGraphic("circle.png");
	m_player = KPTK::createKGraphic("player.png");
	m_land = KPTK::createKGraphic("land.png");
	m_item = KPTK::createKGraphic("item.png");
}

//============================================================================
//  Deconstructor
//============================================================================
gpPen::~gpPen()
{

}
//============================================================================
//  Draw Chipmunk Object
//============================================================================
void gpPen::DrawShape(cpShape* _shape)
{
	switch(_shape->type)
	{
		case CP_CIRCLE_SHAPE:
			switch(_shape->collision_type)
			{
			case GPC_ITEM: DrawItem((cpCircleShape*) _shape);	break;
			case GPC_PLAYER: DrawPlayer((cpCircleShape*) _shape);	break;
			default: DrawCircleShape((cpCircleShape*) _shape); break;
			};
			break;
		case CP_SEGMENT_SHAPE:
			DrawLand((cpSegmentShape*) _shape);
			//DrawSegmentShape((cpSegmentShape*) _shape);
			break;
		case CP_POLY_SHAPE:
			DrawPolyShape((cpPolyShape*) _shape);
			break;
		default:
			break;
	}
}
//============================================================================
//  Draw Circle
//============================================================================
void gpPen::DrawCircle( cpVect _pos, cpFloat _radius, cpFloat _angle)
{
	/*----------------------------------
		Draw Circle
	----------------------------------*/
	m_pen->blitAlphaRectFxF(0,0,128.0f,128.0f,
		(_pos.x)-64.0f,(FP_WORLDH - _pos.y)-64.0f,
		_angle*(float)(180.0 / K_PI) , _radius / 64.0f, 1.0f);
}

//============================================================================
//  Draw Rectangle
//============================================================================
void gpPen::DrawRect(cpVect _pos, cpVect _dim)
{
	m_pen->drawRect(
		_pos.x-(_dim.x / 2.0f),FP_WORLDH - (_pos.y-(_dim.y / 2.0f)),
		_pos.x+(_dim.x / 2.0f),FP_WORLDH - (_pos.y+(_dim.y / 2.0f)),
		0,0,0, 1.0f);
}

//============================================================================
//  Draw Circle Shape
//============================================================================
void gpPen::DrawCircleShape(cpCircleShape* _shape)
{
	cpBody* _body = _shape->shape.body;
	cpVect _centre = cpvadd(_body->p, cpvrotate(_shape->c, _body->rot));
	DrawCircle(_centre, _shape->r, _body->a);
}

//============================================================================
//  Draw Segment Shape : Disconnected Lines
//============================================================================
void gpPen::DrawSegmentShape(cpSegmentShape* _seg)
{
	/*----------------------------------
		 Get Segment Details
	 ----------------------------------*/
	cpBody* _body = _seg->shape.body;
	cpVect _start = cpvadd(_body->p, cpvrotate(_seg->a, _body->rot));
	cpVect _end = cpvadd(_body->p, cpvrotate(_seg->b, _body->rot));

	/*----------------------------------
		 Draw Line
	 ----------------------------------*/
	m_pen->drawLine(_start.x,FP_WORLDH - _start.y,_end.x,FP_WORLDH - _end.y, 0,0,0, 1.0f);
}

//============================================================================
//  Draw Poly Shape : Connected Lines
//============================================================================
void gpPen::DrawPolyShape(cpPolyShape* _poly)
{
	/*----------------------------------
		 Get Poly Details
	 ----------------------------------*/
	cpBody* _body = _poly->shape.body;

	int _vertCount = _poly->numVerts;
	cpVect* _verts = _poly->verts;

	/*----------------------------------
		 Draw Poly
	 ----------------------------------*/
	cpVect _start = cpvadd(_body->p, cpvrotate(_verts[0], _body->rot));
	cpVect _prev = _start;

	for(int i=1; i<_vertCount; ++i)
	{
		cpVect _next = cpvadd(_body->p, cpvrotate(_verts[i], _body->rot));
		m_pen->drawLine(_prev.x,FP_WORLDH - _prev.y,_next.x,FP_WORLDH - _next.y, 0,0,0, 1.0f);
		_prev = _next;
	}

	m_pen->drawLine(_prev.x,FP_WORLDH - _prev.y,_start.x,FP_WORLDH - _start.y, 0,0,0, 1.0f);
}

//============================================================================
//  Draw Poly Shape : Connected Lines
//============================================================================
void gpPen::DrawPlayer(cpCircleShape* _shape)
{
	cpBody* _body = _shape->shape.body;
	cpVect _centre = cpvadd(_body->p, cpvrotate(_shape->c, _body->rot));

	if (g_action == GPA_STICKY)
	{
		m_player->blitAlphaRectFxF(0,128.0f,128.0f,256.0f,
			(_centre.x)-64.0f,(FP_WORLDH - _centre.y)-64.0f,
			_body->a*(float)(180.0 / K_PI) , _shape->r / 64.0f, 1.0f);
	}
	else
	{
		m_player->blitAlphaRectFxF(0,0,128.0f,128.0f,
			(_centre.x)-64.0f,(FP_WORLDH - _centre.y)-64.0f,
			_body->a*(float)(180.0 / K_PI) , _shape->r / 64.0f, 1.0f);
	}
}

//============================================================================
//  Draw Poly Shape : Connected Lines
//============================================================================
void gpPen::DrawItem(cpCircleShape* _shape)
{
	cpBody* _body = _shape->shape.body;
	cpVect _centre = cpvadd(_body->p, cpvrotate(_shape->c, _body->rot));

	m_item->blitAlphaRectFxF(0,0,128.0f,128.0f,
		(_centre.x)-64.0f,(FP_WORLDH - _centre.y)-64.0f,
		_body->a*(float)(180.0 / K_PI) , _shape->r / 64.0f, 1.0f);
}

//============================================================================
//  Draw Land
//============================================================================
void gpPen::DrawLand(cpSegmentShape* _seg)
{
	/*----------------------------------
		Get Segment Details
	----------------------------------*/
	cpBody* _body = _seg->shape.body;
	cpVect _start = cpvadd(_body->p, cpvrotate(_seg->a, _body->rot));
	cpVect _end = cpvadd(_body->p, cpvrotate(_seg->b, _body->rot));

	/*----------------------------------
		Draw Line
	----------------------------------*/
	m_pen->drawLine(_start.x,FP_WORLDH - _start.y,_end.x,FP_WORLDH - _end.y, 0,0,0, 1.0f);
}