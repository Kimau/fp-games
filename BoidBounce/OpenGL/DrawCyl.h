#ifndef DRAW_THE_TUBE_OR_CYLINDER_OF_DOOM_I_CANT_SPELL_3454
#define DRAW_THE_TUBE_OR_CYLINDER_OF_DOOM_I_CANT_SPELL_3454

//---------------------------------------------
//	Headers
//---------------------------------------------
#include "..\Math\CVector3.h"
#include "Basegl.h"
//---------------------------------------------
//	Uses Quadratics to draw a Tube
//---------------------------------------------
class BGLDrawTube
{
	//--------------------------------
	//	Initialisation
	//--------------------------------
public:
	BGLDrawTube(CVector3 _start = CVector3(0,0,0), CVector3 _end = CVector3(0,0,0), float _width = 1.0f);
	~BGLDrawTube();
	//--------------------------------
	//	Functions
	//--------------------------------
	void Render();
	void RenderSimple();
	void setStart(CVector3 _start);
	void setEnd(CVector3 _end);

	//--------------------------------
	//	Varibles
	//--------------------------------
private:
	float m_width;
	CVector3 m_start;
	CVector3 m_end;
	GLUquadricObj* m_quadric;
};

#endif