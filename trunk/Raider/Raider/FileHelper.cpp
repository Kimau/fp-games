/*****************************************************************************
  Title: File Helper to Load and Save
  File: D:\CODE\RAIDER\RAIDER\FILEHELPER.CPP
  Author: Claire Blackshaw
  Created: 2007/12/02
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/

#include "FileHelper.h"
#include "BigHead.h"

/*----------------------------------
	 File Helpers
 ----------------------------------*/
void SaveStaticShape(void* _obj, void* _data);
void SaveCircleShape(const cpCircleShape* _shape, FILE* _file, bool _static);
void SaveSegmentShape(const cpSegmentShape* _shape, FILE* _file, bool _static);
void SavePolyShape(const cpPolyShape* _shape, FILE* _file, bool _static);

void LoadStaticShapes(cpSpace* _space, FILE* _file);
void LoadCircleShape(cpSpace* _space, FILE* _file);
void LoadSegmentShape(cpSpace* _space, FILE* _file);
void LoadPolyShape(cpSpace* _space, FILE* _file);

/*----------------------------------
	 Globals
 ----------------------------------*/
static cpBody* s_staticbody = 0;

//============================================================================
//  Save Level
//============================================================================
void SaveSpace(cpSpace* _space, const char* _filename)
{
	/*----------------------------------
	Open Blank File
	----------------------------------*/
	FILE* _file = fopen(_filename,"w");
	if (_file == 0)
	{
		return;
	}
	/*----------------------------------
	Save Static Shapes
	----------------------------------*/
	fprintf(_file,"BEGIN STATIC \n");
	cpSpaceHashEach(_space->staticShapes,SaveStaticShape,_file);
	fprintf(_file,"END STATIC \n");

	/*----------------------------------
	Close File
	----------------------------------*/
	fclose(_file);
}
//============================================================================
//  Save Static Shapes
//============================================================================
void SaveStaticShape(void* _obj, void* _data)
{
	cpShape* _shape = (cpShape*)_obj;
	FILE* _file = (FILE*)_data;

	/*----------------------------------
	Save Object
	----------------------------------*/
	switch(_shape->type)
	{
		/*----------------------------------
		Circle Shape
		----------------------------------*/
	case CP_CIRCLE_SHAPE:	
		{
			fprintf(_file,"[CIRCLE ]");
			SaveCircleShape((cpCircleShape*) _shape, _file, true);
		}break;
		/*----------------------------------
		Segment Shape
		----------------------------------*/
	case CP_SEGMENT_SHAPE:
		{
			fprintf(_file,"[SEGMENT ]");
			SaveSegmentShape((cpSegmentShape*) _shape, _file, true);
		}break;
		/*----------------------------------
		Poly Shape
		----------------------------------*/
	case CP_POLY_SHAPE:
		{
			fprintf(_file,"[POLY ]");
			SavePolyShape((cpPolyShape*) _shape, _file, true);
		}break;
	}
	fputc('\n',_file);
}
//============================================================================
//  Save Helpers
//============================================================================
void SavecpInt(const int& _num, FILE* _file)			{fprintf(_file,"%d|",_num);}
void SavecpVect(const cpVect& _vect, FILE* _file)		{fprintf(_file,"%f,%f|",_vect.x,_vect.y);}
void SavecpFloat(const cpFloat& _float, FILE* _file)	{fprintf(_file,"%f|",_float);}
//============================================================================
//  Save Circle Shape to File
//============================================================================
void SaveCircleShape(const cpCircleShape* _shape, FILE* _file, bool _static)
{
	SavecpVect(_shape->c,_file);
	SavecpFloat(_shape->r,_file);
}
//============================================================================
//  Save Segment Shape to File
//============================================================================
void SaveSegmentShape(const cpSegmentShape* _shape, FILE* _file, bool _static)
{
	SavecpVect(_shape->a,_file);
	SavecpVect(_shape->b,_file);
}
//============================================================================
//  Save Polygon Shape to File
//============================================================================
void SavePolyShape(const cpPolyShape* _shape, FILE* _file, bool _static)
{
	int _vCount = _shape->numVerts;
	SavecpInt(_vCount,_file);
	for(int i = 0; i < _vCount; ++i)
	{
		SavecpVect(_shape->verts[i],_file);
	}
}

//============================================================================
//  Load Space from File
//============================================================================
cpSpace* LoadSpace(const char* _filename )
{
	/*----------------------------------
	Open Blank File
	----------------------------------*/
	FILE* _file = fopen(_filename,"r");
	if (_file == 0)
	{
		return 0;
	}
	/*----------------------------------
		 Prep Space
	 ----------------------------------*/
	cpSpace* _space = cpSpaceNew();
	cpSpaceResizeStaticHash(_space,1.0,500);		// Set Resolution DIM is granularity and count is object max
	cpSpaceResizeActiveHash(_space,1.0,500);
	_space->gravity = cpv(0,-100);

	/*----------------------------------
	Load Static Shapes
	----------------------------------*/
	char _buffer[200] = "";		// Temp Buffer
	fgets(_buffer,200,_file);	// BEGIN STATIC
	LoadStaticShapes(_space, _file);
	fgets(_buffer,200,_file);	// END STATIC

	/*----------------------------------
	Close File
	----------------------------------*/
	fclose(_file);
	return _space;
}

//============================================================================
//  Load Static Shapes
//============================================================================
void LoadStaticShapes(cpSpace* _space, FILE* _file)
{
	char _buffer[200] = "";	// Temp Buffer
	/*----------------------------------
		 Get Type
	 ----------------------------------*/
	while(fscanf(_file,"[%s ]",_buffer) != EOF)
	{
		switch(_buffer[0])
		{
		case 'c':
		case 'C':
			LoadCircleShape(_space,_file);
			break;
		case 's':
		case 'S':
			LoadSegmentShape(_space,_file);
			break;
		case 'p':
		case 'P':
			LoadPolyShape(_space,_file);
			break;
		case 'e':
		case 'E':
			break;
		}
		/*----------------------------------
		Clean Out Line
		----------------------------------*/
		fgets(_buffer,200,_file);
	}
}
//============================================================================
//  Load Helpers
//============================================================================
void LoadcpInt(int& _num, FILE* _file)			{fscanf(_file,"%d|",&_num);}
void LoadcpVect(cpVect& _vect, FILE* _file)		{fscanf(_file,"%f,%f|",&(_vect.x),&(_vect.y));}
void LoadcpFloat(cpFloat& _float, FILE* _file)	{fscanf(_file,"%f|",&_float);}

//============================================================================
//  Load Circle Shape from File
//============================================================================
void LoadCircleShape(cpSpace* _space, FILE* _file)
{
	cpVect _offset;
	cpFloat _radius;

	LoadcpVect(_offset,_file);
	LoadcpFloat(_radius,_file);
	
	addStaticCircle(_radius,_offset,_space);
}
//============================================================================
//  Load segmented Shape from File
//============================================================================
void LoadSegmentShape(cpSpace* _space, FILE* _file)
{
	cpVect _start;
	cpVect _end;

	LoadcpVect(_start,_file);
	LoadcpVect(_end,_file);

	addStaticSegment(_start,_end,_space);
}
//============================================================================
//  Load Poly Shape from File
//============================================================================
void LoadPolyShape(cpSpace* _space, FILE* _file)
{
	/*----------------------------------
		 Get  Vert Count
	 ----------------------------------*/
	int _vCount = 0;
	LoadcpInt(_vCount,_file);
	cpVect* _verts = new cpVect[_vCount];

	/*----------------------------------
		 Get Verts
	 ----------------------------------*/
	for(int i = 0; i < _vCount; ++i)
	{
		LoadcpVect(_verts[i],_file);
	}

	addStaticPoly(_vCount,_verts, _space);
}

//============================================================================
//  Add Static Segment to Space
//============================================================================
void addStaticSegment(cpVect _start, cpVect _end, cpSpace* _space)
{
	/*----------------------------------
	Check static body 
	----------------------------------*/
	if(s_staticbody == 0)	
		s_staticbody = cpBodyNew(INFINITY, INFINITY);

	/*----------------------------------
		 Create and Add Segment
	 ----------------------------------*/
	cpShape* _segShape = cpSegmentShapeNew(s_staticbody, _start,_end,0);
	_segShape->e = 0.7f; 
	_segShape->u = 1.0f;
	_segShape->collision_type = GPC_STATIC;
	cpSpaceAddStaticShape(_space, _segShape);
}

//============================================================================
//  Add Static Circle to Space
//============================================================================
void addStaticCircle( float _radius, cpVect _offset, cpSpace* _space )
{
	/*----------------------------------
		 Check static body 
	 ----------------------------------*/
	if(s_staticbody == 0)	
		s_staticbody = cpBodyNew(INFINITY, INFINITY);

	/*----------------------------------
		 Create and Add Static Circle
	 ----------------------------------*/
	cpShape* _cirShape = cpCircleShapeNew(s_staticbody,_radius,_offset);
	_cirShape->e = 0.7f; 
	_cirShape->u = 1.0f;
	_cirShape->collision_type = GPC_ITEM;
	cpSpaceAddStaticShape(_space, _cirShape);
}

//============================================================================
//  Add Static Poly to Space
//============================================================================
void addStaticPoly(int _vCount, cpVect* _verts, cpSpace* _space)
{
	/*----------------------------------
	Check static body 
	----------------------------------*/
	if(s_staticbody == 0)	
		s_staticbody = cpBodyNew(INFINITY, INFINITY);

	/*----------------------------------
		 Create and Add Static Poly
	 ----------------------------------*/
	cpShape* _polyShape = cpPolyShapeNew(s_staticbody,_vCount,_verts,cpvzero);
	_polyShape->e = 0.7f; 
	_polyShape->u = 1.0f;
	_polyShape->collision_type = GPC_STATIC;
	cpSpaceAddStaticShape(_space, _polyShape);
}

//============================================================================
//  Add Poly to Space
//============================================================================
cpShape* addPoly(gpMultiBody& _poly, cpVect _pos, cpSpace* _space)
{
	cpBody* _body = cpBodyNew(_poly.m_Mass,_poly.m_Inertia) ;
	cpShape* _shape = cpPolyShapeNew(_body,_poly.m_vCount,_poly.m_Verts,cpvzero);
	_body->p = _pos;
	_shape->e = _poly.m_Elastic;
	_shape->u = _poly.m_Friction;
	_shape->collision_type = _poly.m_Colgroup;

	cpSpaceAddBody(_space,_body);
	cpSpaceAddShape(_space,_shape);

	return _shape;
}

//============================================================================
//  Draw Item
//============================================================================
void addItem(cpSpace* _space, cpVect _pos)
{
	const cpFloat _radius = 8.0f;

	cpShape* _shape = cpCircleShapeNew(s_staticbody,_radius,_pos);
	_shape->collision_type = GPC_ITEM;

	cpSpaceAddStaticShape(_space,_shape);
}

//============================================================================
//  Add Player to Space
//============================================================================
cpBody* addPlayer(cpSpace* _space)
{
	const cpFloat _mass = 15.0f;
	const cpFloat _elastic = 0.1f;
	const cpFloat _friction = 1.5f;
	const cpFloat _radius = 13.0f;
	const cpFloat _verts[] = 
	{
		 0, 16,	 
		 4, 13,	 
		 6, 10,	 
		 6,-10,
		 4,-13,	 
		 0,-16,	
		-4,-13,	
		-6,-10,
		-6, 10,	
		-4, 13
	};
	/*
	const int _vCount = 10;
	const cpFloat _inertia = cpMomentForPoly(_mass,_vCount,(cpVect*)_verts,cpv(0,-10)) * 1000.0f;*/
	const cpFloat _inertia = cpMomentForCircle(_mass, 0, _radius,cpvzero);
	cpBody* _body = cpBodyNew(_mass,_inertia);
	_body->p = cpv(400,550);
	
	//cpShape* _shape = cpPolyShapeNew(_body,_vCount,(cpVect*)_verts,cpv(0,10));
	cpShape* _shape = cpCircleShapeNew(_body,_radius,cpvzero);
	_shape->e = _elastic;
	_shape->u = _friction;
	_shape->collision_type = GPC_PLAYER;
	
	
	cpSpaceAddBody(_space,_body);
	cpSpaceAddShape(_space,_shape);

	return _body;
}