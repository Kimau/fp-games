#ifndef RAIDER_HEADER_DRAW_SHAPES
#define RAIDER_HEADER_DRAW_SHAPES

/*----------------------------------
	 Headers
 ----------------------------------*/
#include "BigHead.h"

class gpPen
{
public:
	gpPen();
	~gpPen();

	/*----------------------------------
		 Draw Primitives
	 ----------------------------------*/
	void DrawCircle(cpVect _pos, cpFloat _radius, cpFloat _angle);
	void DrawRect(cpVect _pos, cpVect _dim);

	/*----------------------------------
		Draw Shapes
	----------------------------------*/
	void DrawShape(cpShape* _shape);
	void DrawCircleShape(cpCircleShape* _shape);
	void DrawSegmentShape(cpSegmentShape* _shape);
	void DrawPolyShape(cpPolyShape* _shape);

	/*----------------------------------
		 Draw Actors
	 ----------------------------------*/
	void DrawPlayer(cpCircleShape* _shape);
	void DrawItem(cpCircleShape* _shape);
	void DrawLand(cpSegmentShape* _seg);

private:
	KGraphic* m_pen;
	KGraphic* m_player;
	KGraphic* m_land;
	KGraphic* m_item;
};

#endif