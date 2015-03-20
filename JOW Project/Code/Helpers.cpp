#include "Helpers.h"
//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       SYSTEM FUNCTIONS			                                                 ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//===================================================================
//		Construct
//===================================================================
JWL_Helpers::JWL_Helpers(JWL_Scenario& _scence, WinGLengine& _gui) : m_Scence(_scence), m_Gui(_gui), m_Map(_scence.Map())
{
	m_Msmooth = 0;
}

//===================================================================
//		Construct
//===================================================================
JWL_Helpers::~JWL_Helpers()
{
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##						PATH FUNCTIONS                                           ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//===================================================================
//	Plot a Path
//===================================================================
MAP_PATH* JWL_Helpers::PlotPath(MAP_POS _pos, MAP_POS _dest, UINT _depth)
{
	if (_depth == 0)
		return 0;
	//----------------------------------------
	//	Create Base Path 
	//----------------------------------------
	MAP_PATH* _base = new MAP_PATH;
	_base->m_next = 0;
	_base->m_pos = _pos;
	//----------------------------------------
	//	Get Distance check for Match
	//----------------------------------------
	double _dist = Distance(_pos,_dest);
	if(_dist == 0)
	{
		return _base;
	}

	//----------------------------------------
	//	Drop onto floor
	//----------------------------------------
	if(m_Map.getLogic(_pos))
	{
		_pos.z += 6;
		while(m_Map.canMove(_pos,MAP_MOVE_DOWN))
		{
			_pos.z -= 2;
		}
	}
	else
	{
		UINT _newZ = m_Map.getHeight(_pos.x,_pos.y);
		if(_newZ < (_pos.z+5))
			_pos.z = m_Map.getHeight(_pos.x,_pos.y);
		else
			return 0;
	}

	//----------------------------------------
	//	Plot Paths Recursivly
	//----------------------------------------
	MAP_POS _delta = {0};
	if(_pos.x == _dest.x)	_delta.x = 1;	else if(_pos.x > _dest.x)	_delta.x = 0;	else	_delta.x = 2;
	if(_pos.y == _dest.y)	_delta.y = 1;	else if(_pos.y > _dest.y)	_delta.y = 0;	else	_delta.y = 2;

	MAP_POS _dir[3] = 
	{
		{_pos.x + _delta.x - 1,_pos.y				,_pos.z},
		{_pos.x				  ,_pos.y + _delta.y - 1,_pos.z},
		{_pos.x + _delta.x - 1,_pos.y + _delta.y - 1,_pos.z}
	};

	MAP_PATH* _path[3] = {0};

	//----------------------------------------
	//	Plot straight Path, if that fails
	// Only then divert
	//----------------------------------------
	_path[2] = PlotPath(_dir[2],_dest,_depth-1);

	if(_path[2] == 0)
	{
		_path[0] = PlotPath(_dir[0],_dest,(_depth-1)/2);
		_path[1] = PlotPath(_dir[1],_dest,(_depth-1)/2);
		//----------------------------------------
		//	Measure Paths
		//----------------------------------------
		UINT _shortest = 1000;
		UINT _shortID = 1000;
		for(UINT c = 0; c < 2; ++c)
		{
			if(_path[c])
			{
				if(Distance(_path[c]) < _shortest)
				{
					if(_shortID < 3)
					{
						KillPath(_path[_shortID]);
					}
					_shortID = c;
				}
				else
				{
					KillPath(_path[c]);
				}
			}
		}
		//----------------------------------------
		//	Return Shortest or NULL
		//----------------------------------------
		if(_shortID < 3)
		{
			_base->m_next = _path[_shortID];
			return _base;
		}
		else
		{
			KillPath(_base);
			return 0;
		}
	}
	else
	{
		//----------------------------------------
		//	Return Most Direct
		//----------------------------------------
		_base->m_next = _path[2];
		return _base;
	}
}
//========================================================================
//	Kills a dead path
//========================================================================
void JWL_Helpers::KillPath(MAP_PATH* _path)
{
	while(_path)
	{
		MAP_PATH* _old = _path;
		_path = _path->m_next;
		delete _old;		
	}
}
//===================================================================
//	Measure Distance of Path
//===================================================================
UINT JWL_Helpers::Distance(const MAP_PATH* _path) const
{
	const MAP_PATH* _curr = _path;
	UINT _length = 0;

	while(_curr)
	{
		++_length;
		_curr = _curr->m_next;
	}

	return _length;
}
//===================================================================
//	Measure Distance of Between two points
//	Ignores Z
//===================================================================
double JWL_Helpers::Distance(const MAP_POS _pos, const MAP_POS _dest) const
{
	double _disSqrX = pow(((double)_pos.x - (double)_dest.x),2.0);
	double _disSqrY = pow(((double)_pos.y - (double)_dest.y),2.0);
	return sqrt(_disSqrX + _disSqrY);
}
//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##					MOVE FROM SCENCE TO GUI                                      ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//======================================================================
//	Transfer Objects from Map into Graphics Part
//======================================================================
void JWL_Helpers::ObjectTransfer()
{
	UINT _oCount = m_Map.getObjectCount();
	UINT _palSize = m_Map.getPalleteSize();

	//---------------------------------------------
	// Add Pallete to OpenGL list
	// Note pallete counts from 1 up as 0 is
	// the map itself
	//---------------------------------------------
	for(UINT c = 0; c < _palSize; ++c)
	{
		MAP_O_PAL _pal = m_Map.getMapPallete(c);
		if(m_Gui.AddMesh(_pal.m_mesh)   == INVALID_PARAM)		return;
		if(m_Gui.AddTexture(_pal.m_tex) == INVALID_PARAM)		return;
	}
	//---------------------------------------------
	// Add Objects to Map
	//---------------------------------------------
	for(UINT c = 0; c < _oCount; ++c)
	{
		MAP_OBJ _obj = m_Map.getMapObject(c);
		CVector3 _pos(_obj.x,_obj.y,_obj.z*0.2);
		CVector3 _up(0,0,1);
		m_Gui.AddObject(_obj.id,_obj.id,_pos,_up,(float)(_obj.r) * 0.25f);
	}
}
//======================================================================
//	Load Units from File into GUI
//======================================================================
void JWL_Helpers::AddUnitMeshes()
{
	UINT _count = m_Scence.getUnitCount();
	for(UINT c = 0; c < _count; ++c)
	{
		const JWL_Unit& _unit = m_Scence.Unit(c);
		UINT _mesh = m_Gui.AddMesh(_unit.getMesh());
		UINT _tex = m_Gui.AddTexture(_unit.getTex());
		UINT _team = _unit.getController();
		MAP_POS _pos = _unit.getPosition();

		UINT _gui = m_Gui.AddUnit(
				_mesh,
				_tex,
				_team,
				CVector3((_pos.x) + 0.5,(_pos.y) + 0.5,(_pos.z*0.2)),
				CVector3(0,0,1),
				(float)(_unit.getFacing()) * 0.25f);

		m_Scence.setUnitGui(c,_gui);
	}
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##				UPDATE AND PROCESS FUNCTIONS                                     ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//======================================================================
//	Pass on the Parse
//======================================================================
LRESULT JWL_Helpers::Parse(HWND _hWnd, UINT _iMsg, WPARAM wParam, LPARAM lParam)
{
	return m_Scence.Parse(_hWnd, _iMsg, wParam, lParam);
}
//===================================================================
//	Process Commands
//===================================================================
void JWL_Helpers::ProcessCommands()
{
	m_Gui.selUnit(m_Scence.getSelUnitID());
	while(1)
	{
		JWL_CMD _cmd = m_Scence.processStack();
		switch(_cmd.m_CmdID)
		{
		case 0:
			return;
		case JC_SYS_PLOT_PATH:
			{ 
				//------------------------------------------
				//	Get Vars
				//------------------------------------------
				MAP_POS _dest = {_cmd.x,_cmd.y,_cmd.z};
				MAP_POS _pos  = m_Scence.Unit(m_Scence.getSelUnitID()).getPosition();

				//------------------------------------------
				//	Plot Path
				//------------------------------------------
				MAP_PATH* _path = PlotPath(_pos,_dest,20);

				//------------------------------------------
				//	Update
				//------------------------------------------
				m_Gui.setPath(_path);
				m_Scence.setPath(_path);
				
				return;
			}
		case JC_SYS_UPDATEUNIT:
			m_Gui.setPath(0);
			m_Scence.setPath(0);
			UpdateGui(_cmd);
		case JC_SYS_STRING:
			strcpy(m_Gui.m_string,m_Scence.LCon().getString());
			return;
		case JC_SYS_LOOK_SELECTION:
			{
				MAP_POS _pos = m_Scence.Unit(_cmd.m_UnitID).getPosition();
				CVector3 _Cpos = m_Gui.getCamPos();
				CVector3 _Cview(_pos.x,_pos.y,(_pos.z*0.2));

				//-----------------------------
				// Camera Never Allowed to be
				// under unit
				//-----------------------------
				if(_Cpos.z < _Cview.z)
					_Cpos.z = (_Cview.z + 5);

				m_Gui.CamLook(_Cpos,_Cview,CVector3(0,0,1));
				return;
			}
		case JC_SYS_CAM_FORWARD:
			{
				CVector3 _Cpos = m_Gui.getCamPos();

				CVector3 _dir = _Cpos - m_Gui.getCamView();
				_dir.z = 0;
				if(_dir.Magnitude() > 2.5)
				{
					_dir.Normalize();
					m_Gui.CamLook(_Cpos - (_dir * 2.5f));
				}
				return;
			}
		case JC_SYS_CAM_RIGHT:
			{
				CVector3 _Cpos = m_Gui.getCamPos();

				CVector3 _dir = _Cpos - m_Gui.getCamView();
				_dir.z = 0;
				_dir = _dir * CVector3(0,0,1);				
				_dir.Normalize();

				m_Gui.CamLook(_Cpos - (_dir * 2.5f));
				return;
			}
		case JC_SYS_CAM_BACKW:
			{
				CVector3 _Cpos = m_Gui.getCamPos();

				CVector3 _dir = _Cpos - m_Gui.getCamView();
				_dir.z = 0;
				_dir.Normalize();

				m_Gui.CamLook(_Cpos + (_dir * 2.5f));
				return;
			}
		case JC_SYS_CAM_LEFT:
			{
				CVector3 _Cpos = m_Gui.getCamPos();

				CVector3 _dir = _Cpos - m_Gui.getCamView();
				_dir.z = 0;
				_dir = _dir * CVector3(0,0,1);				
				_dir.Normalize();

				m_Gui.CamLook(_Cpos + (_dir * 2.5f));
				return;
			}
		};
	}
}


//======================================================================
//	Update GUI
//======================================================================
void JWL_Helpers::UpdateGui(JWL_CMD _cmd)
{
	switch(_cmd.m_CmdID)
	{
	case JC_SYS_UPDATEUNIT:
		{
			const JWL_Unit& _unit = m_Scence.Unit(_cmd.m_UnitID);
			MAP_POS _pos = _unit.getPosition();

			m_Gui.MoveUnit(
				_unit.getGui(),
				CVector3((_pos.x) + 0.5,(_pos.y) + 0.5,(_pos.z*0.2)),
				CVector3(0,0,1),
				(float)(_unit.getFacing()) * 0.25f);
		}
		break;
	};
	
}
//===================================================================
//		Convert Mouse CO-Ords Into World Vector
//===================================================================
void JWL_Helpers::ConvertMousePos(UINT_POINT _curPos)
{
	/*=========================================
	*	Set Up storage Locations
	*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	*	The reason we are using doubles is
	*	that the UnProject has to work with
	*	doubles and this function has to be
	*	fast so we want to cut down conversion
	*	time.
	*=========================================*/
	GLdouble _proj[16];
	GLdouble _model[16];
	GLint _view[4];

	CVector3 _near(0,0,0);
	CVector3 _far(0,0,0);

	/*=========================================
	*	Get Matrixs
	*=========================================*/
	glGetDoublev(GL_PROJECTION_MATRIX,_proj);
	glGetDoublev(GL_MODELVIEW_MATRIX,_model);
	glGetIntegerv(GL_VIEWPORT,_view);

	/*=========================================
	*	Unproject
	*=========================================*/
	gluUnProject(_curPos.x,_view[3]-_curPos.y,0,_model,_proj,_view,&(_near.x),&(_near.y),&(_near.z));
	gluUnProject(_curPos.x,_view[3]-_curPos.y,1,_model,_proj,_view,&(_far.x),&(_far.y),&(_far.z));

	/*=========================================
	*	Update varibles
	*=========================================*/
	CVector3 _dir = _far - _near;

	//----------------------------------------
	// Given a cube and a line there will be
	// two surface intersections or none
	// Find them
	//----------------------------------------
	MAP_SIZE _size = m_Map.getMapSize();
	CVector3 _start = m_Gui.getCamPos();
	CVector3 _max(_size.x,_size.y,1000.0);

    //----------------------------------------
	// Setup Planes
	//----------------------------------------
	CPlane _MapCube[6] =
	{
		CPlane(CVector3(-1.0, 0.0, 0.0),_max.x - 0.0001),		// EAST
		CPlane(CVector3( 1.0, 0.0, 0.0),0),						// WEST
		CPlane(CVector3( 0.0,-1.0, 0.0),_max.y - 0.0001),		// NORTH
		CPlane(CVector3( 0.0, 1.0, 0.0),0),						// SOUTH
		CPlane(CVector3( 0.0, 0.0, 1.0),0),						// BOTTOM
		CPlane(CVector3( 0.0, 0.0,-1.0),_max.z - 0.0001)		// TOP
	};
	//----------------------------------------
	// Ray
	//----------------------------------------
	CVector3 _line[3] = {CVector3(-1,-1,-1),CVector3(-1,-1,-1),CVector3(-1,-1,-1)};
	UINT _offset = 0;
	//----------------------------------------
	// If starting point is in cube
	// then we just need the end
	//----------------------------------------
	bool _cib = false;	// Is Cam in Box
	if((
		(_start.x > 0) && (_start.x < _max.x) &&
		(_start.y > 0) && (_start.y < _max.y) &&
		(_start.z > 0) && (_start.z < _max.z)
		))
	{
		_line[0] = _start;
		_cib = true;
		++_offset;
	}
	//----------------------------------------
	// Check for Collisions
	//----------------------------------------
	for(UINT c = 0; c < 6 ;++c)
	{
		CVector3 _point = _MapCube[c].Intersect(_start,_dir);
		if(!(
			(_point.x < 0) || (_point.x > _max.x) ||
			(_point.y < 0) || (_point.y > _max.y) ||
			(_point.z < 0) || (_point.z > _max.z)
			))
		{
			if((_offset == 3) || ((c == 6) && (_offset == 1)))
				return; // We ignore all hits involving ceiling and one other point
			if((_point != _start) && (c != 6))
				_line[_offset++] = _point;
		}
	}
	//----------------------------------------
	// Check for 'Dud'
	//----------------------------------------
	if(_offset == 1)
		return;
	//----------------------------------------
	// Find _start and _end
	// If start == line[0] then
	//  Find point closest to camera and set
	//  it as end
	// Else
	//  Find which one is closer to the start
	// that is the start and the other is the
	// end of the ray
	//
	// NB We use CIB because float == fucks
	// out on occasion
	//----------------------------------------
	CVector3 _end(0,0,0);
	if(_cib)
	{
		if(_line[1].z < _line[2].z)
			_end = _line[1];
		else
			_end = _line[2];
	}
	else
	{
		if(_line[1].z < _line[2].z)
		{			_start = _line[1];			_end = _line[0];		}
		else
		{			_start = _line[0];			_end = _line[1];		}
	}
	//----------------------------------------
	// Move Until Collision or until
	// End and Start are equal
	//----------------------------------------
	CVector3 _delta = _end - _start;
	UINT _maxSteps = ((UINT)abs(_delta.Magnitude()) + 1);
	_delta.Normalize();

	for(UINT _step = 0; _step < _maxSteps; ++_step)
	{
		if(m_Map.canMove(_start + (_delta * (_step)), _start + (_delta * (_step + 1))))
		{

		}
		else
		{
			_end =  _start + (_delta * (_step));
			break;
		}
	}
	//----------------------------------------
	//	Check for whacked out values
	//----------------------------------------
	if((_end.x < 0) || (_end.y < 0) || (_end.z < 0))
		return;

	CVector3 _diff = _end - m_Gui.getMapPos();
	if((m_Msmooth < 2) && (_diff.Magnitude() > 2))
	{
		++m_Msmooth;
		return;
	}
	m_Msmooth = 0;
	//----------------------------------------
	// Update Cursor
	//----------------------------------------
	m_Gui.setMouse(_curPos);
	m_Scence.selectPos((UINT)_end.x,(UINT)_end.y);

	// Flatten Cursor if on Floor
	_end.z = floor(_end.z + 0.2);

	m_Gui.MoveCursor(CVector3(floor(_end.x),floor(_end.y),(_end.z)));
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##				 BUILDING FUNCTIONS                                              ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//=============================================================================
//	Calculate The texture Cube needed at this position
//=============================================================================
float* JWL_Helpers::mapTexTile(float x, float y) const
{
	bool _baseX = ((floorf(x) + 0.1f) > x);
	bool _baseY = ((floorf(y) + 0.1f) > y);
	//--------------------------------
	//	Fetch Tiles
	//--------------------------------
	UINT _ux = (UINT)x;
	UINT _uy = (UINT)y;
	UINT _tiles[3] = {m_Map.getTex(_ux,_uy),INVALID_PARAM,INVALID_PARAM};
	if(_tiles[0] == INVALID_PARAM)
		return 0;

	if(_baseX)
		_tiles[1] = m_Map.getTex(_ux-1,_uy);
	else
		_tiles[1] = m_Map.getTex(_ux+1,_uy);
	if(_tiles[1] == INVALID_PARAM)
		_tiles[1] = _tiles[0];

	if(_baseY)
		_tiles[2] = m_Map.getTex(_ux,_uy-1);
	else
		_tiles[2] = m_Map.getTex(_ux,_uy+1);
	if(_tiles[2] == INVALID_PARAM)
		_tiles[2] = _tiles[0];

	//--------------------------------
	//	Error Checking
	//--------------------------------
	for(UINT c = 0; c < 3; ++c)
	{
		if((_tiles[c] == 0) || (_tiles[c] > 4))
			_tiles[c] = 1;
	}
	//--------------------------------
	//	Compute Needed Tile
	//--------------------------------
	UINT _row = 0;
	UINT _col = 0;
	UINT _sit = 0;
	UINT _rot = 0;

	//------------------------------------
	//	What Situation are you in
	//------------------------------------
	if((_tiles[0] > _tiles[1]) && (_tiles[0] > _tiles[2]) && (_tiles[1] == _tiles[2]))
	{
		_sit = 1;
	}
	else if(_tiles[0] > _tiles[1])
	{
		_sit = 2;
	}
	else if(_tiles[0] > _tiles[2])
	{
		_sit = 3;
	}
	//------------------------------------
	//	Choose	Tile
	//------------------------------------
	if(_sit)
	{
		if(_sit == 3)
			_row = (2*_tiles[2] - 1)*(_tiles[2] != 1) + (_tiles[0] - _tiles[2] - 1);
		else
			_row = (2*_tiles[1] - 1)*(_tiles[1] != 1) + (_tiles[0] - _tiles[1] - 1);

		_col = (1 - _row / 3);
		if(_sit > 1)
			_col += 2;

		_row = 3 - (_row % 3);
	}
	else
	{
		_row = 0;
		_col = 4 - _tiles[0];
	}

	//------------------------------------
	//	Setup Texture
	//------------------------------------
	float* _tex1 = new float[8];
	memset(_tex1,0,sizeof(float)*8);

	_tex1[0] = _tex1[6] = (float)(_col    ) * 0.25f;		
	_tex1[4] = _tex1[2] = (float)(_col + 1) * 0.25f;		

	_tex1[1] = _tex1[3] = (float)(_row    ) * 0.25f;		
	_tex1[5] = _tex1[7] = (float)(_row + 1) * 0.25f;

	float* _tex2 = new float[8];
	memcpy(_tex2,_tex1,sizeof(float)*8);
	//------------------------------------
	//	Rotate Tile
	//------------------------------------
	switch(_sit)
	{
	case 1:		//	Corner Tile
		_rot = _baseX*2 + _baseY;
		break;

	case 2:		//	Vertical Edge
		_rot = 3*(_baseX);
		break;
	case 3:		//	Horizontal Edge (BaseY)
		_rot = 1 + !(_baseY);
		break;
	}
	//-----------------------------------------
	//	Rotate Tiles
	//-----------------------------------------
	switch(_rot)
	{
		case 0:
			break;
		case 1:
			//-----------------------------------------
			//	Rotate Counter Clockwise
			//-----------------------------------------
			{
				_tex2[0] = _tex1[2];
				_tex2[1] = _tex1[3];
				_tex2[2] = _tex1[4];
				_tex2[3] = _tex1[5];
				_tex2[4] = _tex1[6];
				_tex2[5] = _tex1[7];
				_tex2[6] = _tex1[0];
				_tex2[7] = _tex1[1];
			}
			break;
		case 2:
			//-----------------------------------------
			//	Rotate 180 Degrees
			//-----------------------------------------
			{
				_tex2[0] = _tex1[6];
				_tex2[1] = _tex1[7];
				_tex2[2] = _tex1[0];					
				_tex2[3] = _tex1[1];
				_tex2[4] = _tex1[2];
				_tex2[5] = _tex1[3];
				_tex2[6] = _tex1[4];
				_tex2[7] = _tex1[5];
			}
			break;
			//-----------------------------------------
			//	Rotate Clockwise
			//-----------------------------------------
		case 3:
			{
				_tex2[0] = _tex1[4];
				_tex2[1] = _tex1[5];
				_tex2[2] = _tex1[6];
				_tex2[3] = _tex1[7];
				_tex2[4] = _tex1[0];
				_tex2[5] = _tex1[1];
				_tex2[6] = _tex1[2];
				_tex2[7] = _tex1[3];
			}
			break;
	};
	//-----------------------------------------
	//	Return Values
	//-----------------------------------------
	delete[] _tex1;
	return _tex2;
}

//=============================================================================
//	Samples Point and Generates an Averaged height
//=============================================================================
float JWL_Helpers::SamplePoint(float x, float y) const
{
	//-------------------------------
	//	Get Point height
	//-------------------------------
	const MAP_SIZE _size = m_Map.getMapSize();
	float _points = 1.0f;
	float _height = m_Map.getHeightf(x,y);
	if(_height < 0.0001)
		_points = 0.0f;

	//-------------------------------
	//	Sample Surrounding Points
	//-------------------------------
	if(((UINT)x < _size.x) && ((UINT)(y+0.3f) < _size.y))
	{
		_height += m_Map.getHeightf(x,y+0.3f);
		_points += 1.0f;
	}
	if(((UINT)x < _size.x) && ((UINT)(y-0.3f) < _size.y))
	{
		_height += m_Map.getHeightf(x,y-0.3f);
		_points += 1.0f;
	}
	if(((UINT)(x+0.3f) < _size.x) && ((UINT)y < _size.y))
	{
		_height += m_Map.getHeightf(x+0.3f,y);
		_points += 1.0f;
	}
	if(((UINT)(x-0.3f) < _size.x) && ((UINT)y < _size.y))
	{
		_height += m_Map.getHeightf((x-0.3f),y);
		_points += 1.0f;
	}
	//-------------------------------
	//	Divide Height by Points
	//-------------------------------
	_height /= _points;
	return _height;
}
//=================================================================================
//	Builds Grid of Vertex/Normal
//=================================================================================
void JWL_Helpers::BuildVNGrid(JWL_Mesh& _mesh, CNorm* _tNorm)
{
	const MAP_SIZE _size = m_Map.getMapSize();
	const UINT _lwidth = (_size.x * 4) + 2;
	//----------------------------------
	//	Loop through Grid and Set
	//	Heights of Grid
	//----------------------------------
	{
		UINT _offset = 0;
		for(float sy = 0.0f; sy < _size.y; sy += 0.5f)
		{
			UINT _rowStrat = _offset;
			//--------------------------------------------------
			//	Render Strip
			//--------------------------------------------------
			for(float x = 0.0f; x < (_size.x + 0.1f); x += 0.5f)
			{
				for(float y = sy; y < (sy + 0.51f); y += 0.5f)
				{
					//-----------------------
					// Set Vertex
					//-----------------------
					_mesh.setVf(_offset,x,y,SamplePoint(x,y));
					++_offset; // Increment
				}
				//----------------------------------
				//	Calculate Face Normals
				//----------------------------------
				_tNorm[_offset - 2] =
					(_mesh.getV(_offset-4) - _mesh.getV(_offset-2))*
					(_mesh.getV(_offset-4) - _mesh.getV(_offset-3));
				_tNorm[_offset - 1] =
					(_mesh.getV(_offset-1) - _mesh.getV(_offset-3))*
					(_mesh.getV(_offset-1) - _mesh.getV(_offset-2));
			}
			//----------------------------------
			//	Fix First Faces on Row
			//----------------------------------
			_tNorm[_rowStrat   ] = _tNorm[_rowStrat +2];
			_tNorm[_rowStrat +1] = _tNorm[_rowStrat +2];
		}
	}
	//-----------------------------------------------------------
	//		Calculate Vertex Normals
	//-----------------------------------------------------------
	for(UINT _strip = 0; _strip < (_size.y *2); ++_strip)
	{
		//-----------------------------------
		//	Traverse Coloumns
		//-----------------------------------
		for(UINT _xOff = 0; _xOff < (_lwidth - 2); _xOff+= 2)
		{
			UINT _Botoff = (_strip - 1)*_lwidth + _xOff;
			UINT _Midoff = (_strip    )*_lwidth + _xOff;
			UINT _Topoff = (_strip + 1)*_lwidth + _xOff;
			CVector3 _norm(0,0,0);
			//-----------------------------------
			//	Top Strip
			//-----------------------------------
			if((_strip + 1) == (_size.y *2))
			{
				_norm = 
					(_tNorm[_Midoff + 1] * 2)
					+  _tNorm[_Midoff + 2]
					+  _tNorm[_Midoff + 3];
			}
			else
			{
				_norm = 
					(_tNorm[_Midoff + 1] * 2)
					+  _tNorm[_Midoff + 2]
					+  _tNorm[_Midoff + 3]
					+  _tNorm[_Topoff    ]
					+  _tNorm[_Topoff + 1]
					+ (_tNorm[_Topoff + 2] * 2);
			}
			_norm.Normalize();
			_mesh.setN(_Midoff + 1, _norm);
			//-----------------------------------
			//	Bottom Strip
			//-----------------------------------
			if(_strip == 0)
			{
				_norm = 
					_tNorm[_Midoff    ]
					+  _tNorm[_Midoff + 1]
					+ (_tNorm[_Midoff + 2] * 2);
			}
			else
			{
				_norm = 
					_tNorm[_Midoff    ]
					+  _tNorm[_Midoff + 1]
					+ (_tNorm[_Midoff + 2] * 2)
					+ (_tNorm[_Botoff + 1] * 2)
					+  _tNorm[_Botoff + 2]
					+  _tNorm[_Botoff + 3];
			}
			_norm.Normalize();
			_mesh.setN(_Midoff, _norm);
		}
		//-----------------------------------
		//	Last Column (Special Case)
		//-----------------------------------
		{
			UINT _Botoff = (_strip - 1)*_lwidth + (_lwidth - 2);
			UINT _Midoff = (_strip    )*_lwidth + (_lwidth - 2);
			UINT _Topoff = (_strip + 1)*_lwidth + (_lwidth - 2);
			CVector3 _norm(0,0,0);
			//-----------------------------------
			//	Top Strip
			//-----------------------------------
			if((_strip + 1) == (_size.y *2))
			{
				_norm = (_tNorm[_Midoff + 1] * 2);
			}
			else
			{
				_norm = 
					(_tNorm[_Midoff + 1] * 2)
					+  _tNorm[_Topoff    ]
					+  _tNorm[_Topoff + 1];
			}
			_norm.Normalize();
			_mesh.setN(_Midoff + 1, _norm);
			//-----------------------------------
			//	Bottom Strip
			//-----------------------------------
			if(_strip == 0)
			{
				_norm = _tNorm[_Midoff] + _tNorm[_Midoff + 1];
			}
			else
			{
				_norm = 
					_tNorm[_Midoff    ]
					+  _tNorm[_Midoff + 1]
					+ (_tNorm[_Botoff + 1] * 2);
			}
			_norm.Normalize();
			_mesh.setN(_Midoff, _norm);
		}
	}
}
//======================================================================
//	Build Strip Map Mesh
//======================================================================
void JWL_Helpers::BuildStripMesh()
{
	//---------------------------------
	//	Allocate and Set Memory
	//---------------------------------
	const MAP_SIZE _size = m_Map.getMapSize();
	const UINT _lwidth = (_size.x * 4) + 2;
	const UINT _VCount =  _lwidth * (_size.y * 2);

	//---------------------------------
	//	Base Mesh for Floor
	//	Allocate and Set Memory
	//  Later Copied into Final
	//---------------------------------
	JWL_Mesh _mesh(_VCount,GL_TRIANGLE_STRIP);

	//---------------------------------
	//	Tri-Face Normals
	//	Allocate and Set Memory
	// Later Copied into WinGL
	//---------------------------------
	CNorm* _tNorm = new CNorm[_VCount];
	memset(_tNorm,0,sizeof(CNorm)*_VCount);

	//---------------------------------
	//	Calculate a Grid of
	//	Vertex and Normal values
	//---------------------------------
	BuildVNGrid(_mesh,_tNorm);

	//-----------------------------------------------------------
	//             Make the List
	//-----------------------------------------------------------
	UINT _mID = glGenLists(1);
	glNewList(_mID,GL_COMPILE);
	glPushMatrix();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		// Enable Clients to Recieve Data	
		glInterleavedArrays(GL_T2F_N3F_V3F,0,_mesh.m_array);

		for(UINT _strip = 0; _strip < (_size.y *2); ++_strip)
			glDrawArrays(_mesh.m_mode , _strip*(_lwidth), _lwidth);

		glCullFace(GL_BACK);
	glPopMatrix();
	glEndList();

	//--------------------------------------------
	//	Update Gui
	//--------------------------------------------
	m_Gui.setMapNorms(_tNorm);
	m_Gui.AddMap(_mID,"map.jpg");	
}
//======================================================================
//	Builds Map Mesh from Quads
//======================================================================
void JWL_Helpers::BuildQuadMesh()
{
	//---------------------------------
	//	Allocate and Set Memory
	//---------------------------------
	const MAP_SIZE _size = m_Map.getMapSize();
	const UINT _lwidth = (_size.x * 4) + 2;
	const UINT _VCount =  _size.x *_size.y * 16;

	//---------------------------------
	//	Base Mesh for Floor
	//	Allocate and Set Memory
	//---------------------------------
	JWL_Mesh _mesh((_lwidth * (_size.y * 2)),GL_TRIANGLE_STRIP);
	JWL_Mesh _final(_VCount,GL_QUADS);

	//---------------------------------
	//	Tri-Face Normals
	//	Allocate and Set Memory
	// Later Copied into WinGL
	//---------------------------------
	CNorm* _tNorm = new CNorm[_VCount];
	memset(_tNorm,0,sizeof(CNorm)*_VCount);

	//---------------------------------
	//	Calculate a Grid of
	//	Vertex and Normal values
	//---------------------------------
	BuildVNGrid(_mesh,_tNorm);

	//---------------------------------
	//	Copy Data Into Final
	//---------------------------------
	{
		UINT _offset = 0;
		for(UINT _strip = 0; _strip < (_size.y *2); ++_strip)
		{
			for(UINT x = 0; x < (_size.x *2); ++x)
			{
				UINT _mark = (_strip * _lwidth) + (x*2);
				_final.setP(_offset++,CTexCoord(0,0),_mesh.getN(_mark   ),_mesh.getV(_mark   ));
				_final.setP(_offset++,CTexCoord(0,0),_mesh.getN(_mark +2),_mesh.getV(_mark +2));
				_final.setP(_offset++,CTexCoord(0,0),_mesh.getN(_mark +3),_mesh.getV(_mark +3));
				_final.setP(_offset++,CTexCoord(0,0),_mesh.getN(_mark +1),_mesh.getV(_mark +1));
			}
		}
	}
	//---------------------------------
	//	Calculate Texture
	//---------------------------------
	{
		UINT _offset = 0;
		for(float y = 0.0f; y < (_size.y +0.1f); y += 0.5f)
		{
			for(float x = 0.0f; x < (_size.x +0.1f); x += 0.5f)
			{
				float* _map = mapTexTile(x,y);
				if(_map)
				{
					_final.setTf(_offset++,_map[0],_map[1]);
					_final.setTf(_offset++,_map[2],_map[3]);
					_final.setTf(_offset++,_map[4],_map[5]);
					_final.setTf(_offset++,_map[6],_map[7]);
					delete[] _map;
				}
			}
		}
	}
	//-----------------------------------------------------------
	//             Make the List
	//-----------------------------------------------------------
	UINT _mID = glGenLists(1);
	glNewList(_mID,GL_COMPILE);
	glPushMatrix();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable Clients to Recieve Data	
		glInterleavedArrays(GL_T2F_N3F_V3F,0,_final.m_array);

		glDrawArrays(_final.m_mode , 0, _VCount);
	glPopMatrix();
	glEndList();

	//--------------------------------------------
	//	Update Gui
	//--------------------------------------------
	m_Gui.setMapNorms(_tNorm);
	m_Gui.AddMap(_mID,"proto.jpg");	
}
//======================================================================
//	Build Map Mesh
//	HACK :: Incorrect Winding Cull Front
//======================================================================
void JWL_Helpers::BuildMapMesh()
{
	#define _XYZV(X,Y,Z,V)		_mesh.setV(((x*_size.y + y)*16 + (V)),(X),(Y),(Z));
	#define _GETV(V)			_mesh.getV((x*_size.y + y)*16 + (V))
	#define _XYZN(X,Y,Z,V)		_mesh.setN(((x*_size.y + y)*16 + (V)),(X),(Y),(Z));
	#define _XYZT(X,Y,V)		_mesh.setT(((x*_size.y + y)*16 + (V)),(X),(Y));
	#define _GET_Z(X,Y,V)		_mesh.m_array[(((X)*_size.y + (Y))*16 + (V))].vz

	#define _OFFV(X,Y,Z,V)		_final.setV((_VCount + _offset + (V)),(X),(Y),(Z));
	#define _OFFN(X,Y,Z,V)		_final.setN((_VCount + _offset + (V)),(X),(Y),(Z));
	#define _OFFT(X,Y,V)		_final.setT((_VCount + _offset + (V)),(X),(Y));

	#define _HEIGHT(X,Y)		_grid[(X)*_size.y + (Y)].m_height
	#define _TEXTURE(X,Y)		_grid[(X)*_size.y + (Y)].m_tl

	#define _TEX_LEFT		 0.755
	#define _TEX_MID		 0.875
	#define _TEX_RIGHT		 0.995

	#define _CLIFF(X,Y)			_cliff[(X)*_size.y + (Y)]
	#define _CLIFF_N			  1
	#define _CLIFF_E			  2
	#define _CLIFF_S			  4
	#define _CLIFF_W			  8
	#define _CLIFF_OFFSET		  4
	//---------------------------------
	//	Allocate and Set Memory
	//---------------------------------
	const MAP_SIZE _size = m_Map.getMapSize();
	const UINT _totalCells = _size.x * _size.y;
	const UINT _VCount = _totalCells * 16;

	//---------------------------------
	//	Base Mesh for Floor
	//	Allocate and Set Memory
	//  Later Copied into Final
	//---------------------------------
	JWL_Mesh _mesh(_VCount,GL_QUADS);
	const MAP_CELL* _grid = m_Map.getGridPTR();

	//---------------------------------
	//	Tri-Face Normals
	//	Allocate and Set Memory
	// Later Copied into WinGL
	//---------------------------------
	CNorm* _tNorm = new CNorm[_totalCells*8];
	memset(_tNorm,0,sizeof(CNorm)*_totalCells);

	//---------------------------------
	//	Cliff Data
	//	Allocate and Set Memory
	//  Deleted at End
	//---------------------------------
	UINT* _cliff = new UINT[_totalCells];
	memset(_cliff,0,sizeof(UINT)*_totalCells);
	UINT _CliffCount = 0;

	//---------------------------------
	//	Construct Basic Floor
	//	Detect Cliffs
	//---------------------------------
	for(UINT x = 0; x < _size.x; ++x)
	{
		for(UINT y = 0; y < _size.y; ++y)
		{
			UINT _tileHeight = _HEIGHT(x,y);
			double _vertHeight = _tileHeight*0.2;
			UINT& _cliffRef = _CLIFF(x,y);
			UINT _vOff = (x*_size.y + y)*16;
			//-------------------------------------------
			// Initial Textures
			//-------------------------------------------
			double _tx = (_TEXTURE(x,y) % 4) * 0.250;
			double _ty = (_TEXTURE(x,y) / 4) * 0.250;
			
			_mesh.setT(_vOff +  0, _tx + 0.002,_ty + 0.002);
			_mesh.setT(_vOff +  1, _tx + 0.002,_ty + 0.123);
			_mesh.setT(_vOff +  2, _tx + 0.123,_ty + 0.123);
			_mesh.setT(_vOff +  3, _tx + 0.123,_ty + 0.002);
			_mesh.setT(_vOff +  4, _tx + 0.002,_ty + 0.123);
			_mesh.setT(_vOff +  5, _tx + 0.002,_ty + 0.250);
			_mesh.setT(_vOff +  6, _tx + 0.123,_ty + 0.250);
			_mesh.setT(_vOff +  7, _tx + 0.123,_ty + 0.123);
			_mesh.setT(_vOff +  8, _tx + 0.123,_ty + 0.250);
			_mesh.setT(_vOff +  9, _tx + 0.250,_ty + 0.250);
			_mesh.setT(_vOff + 10, _tx + 0.250,_ty + 0.123);
			_mesh.setT(_vOff + 11, _tx + 0.123,_ty + 0.123);
			_mesh.setT(_vOff + 12, _tx + 0.250,_ty + 0.123);
			_mesh.setT(_vOff + 13, _tx + 0.250,_ty + 0.002);
			_mesh.setT(_vOff + 14, _tx + 0.123,_ty + 0.002);
			_mesh.setT(_vOff + 15, _tx + 0.123,_ty + 0.123);

			//-------------------------------------------
			// Detect Cliffs
			//-------------------------------------------			
			{
				if((y == (_size.y -1)) || (abs((int)_HEIGHT(x  ,y+1) - (int)_tileHeight) > _CLIFF_OFFSET))
				{
					_cliffRef |= _CLIFF_N;
					++_CliffCount;
				}

				if((y == 0) || (abs((int)_HEIGHT(x  ,y-1) - (int)_tileHeight) > _CLIFF_OFFSET))
				{
					_cliffRef |= _CLIFF_S;
					if(y == 0) ++_CliffCount;
				}

				if((x == (_size.x -1)) || (abs((int)_HEIGHT(x+1,y  ) - (int)_tileHeight) > _CLIFF_OFFSET))
				{
					_cliffRef |= _CLIFF_E;
					++_CliffCount;
				}

				if((x == 0) || (abs((int)_HEIGHT(x-1,y  ) - (int)_tileHeight) > _CLIFF_OFFSET))
				{
					_cliffRef |= _CLIFF_W;
					if(x == 0) ++_CliffCount;
				}
			}
			//-------------------------------------------
			// Logic Maze from HELL!!!
			// Notice NO CORNERS
			//-------------------------------------------
			{
				// 2,7,11,15
				_XYZV(x+0.5,y+0.5,_vertHeight, 2);
				_XYZV(x+0.5,y+0.5,_vertHeight, 7);
				_XYZV(x+0.5,y+0.5,_vertHeight,11);
				_XYZV(x+0.5,y+0.5,_vertHeight,15);
				if(_cliffRef & _CLIFF_N)
				{
					// 6,8
					_XYZV(x+0.5,y+1.0,_vertHeight, 6);
					_XYZV(x+0.5,y+1.0,_vertHeight, 8);
					if(_cliffRef & _CLIFF_E)
					{
						// 10, 12
						_XYZV(x+1.0,y+0.5,_vertHeight,10);
						_XYZV(x+1.0,y+0.5,_vertHeight,12);
					}
					else
					{
						// 10, 12
						double _smooth = (_HEIGHT(x+1,y  ) + _tileHeight) * 0.1;
						_XYZV(x+1.0,y+0.5,_smooth,10);
						_XYZV(x+1.0,y+0.5,_smooth,12);
					}
					if(_cliffRef & _CLIFF_W)
					{
						// 1, 4
						_XYZV(x+0.0,y+0.5,_vertHeight, 1);
						_XYZV(x+0.0,y+0.5,_vertHeight, 4);
					}
					else
					{
						// 1, 4
						double _smooth = (_HEIGHT(x-1,y  ) + _tileHeight) * 0.1;
						_XYZV(x+0.0,y+0.5,_smooth, 1);
						_XYZV(x+0.0,y+0.5,_smooth, 4);
					}

				}
				else // SMOOTH NORTH
				{
					// 6,8
					double _smooth = (_HEIGHT(x  ,y+1) + _tileHeight) * 0.1;
					_XYZV(x+0.5,y+1.0,_smooth, 6);
					_XYZV(x+0.5,y+1.0,_smooth, 8);
					if(_cliffRef & _CLIFF_E)
					{
						// 10, 12
						_XYZV(x+1.0,y+0.5,_vertHeight,10);
						_XYZV(x+1.0,y+0.5,_vertHeight,12);
					}
					else
					{
						// 10, 12
						double _smooth = (_HEIGHT(x+1,y  ) + _tileHeight) * 0.1;
						_XYZV(x+1.0,y+0.5,_smooth,10);
						_XYZV(x+1.0,y+0.5,_smooth,12);
					}
					if(_cliffRef & _CLIFF_W)
					{
						// 1, 4
						_XYZV(x+0.0,y+0.5,_vertHeight, 1);
						_XYZV(x+0.0,y+0.5,_vertHeight, 4);
					}
					else
					{
						// 1, 4
						double _smooth = (_HEIGHT(x-1,y) + _tileHeight) * 0.1;
						_XYZV(x+0.0,y+0.5,_smooth, 1);
						_XYZV(x+0.0,y+0.5,_smooth, 4);
					}
				}
				if(_cliffRef & _CLIFF_S)
				{
					// 3, 14
					_XYZV(x+0.5,y+0.0,_vertHeight, 3);
					_XYZV(x+0.5,y+0.0,_vertHeight,14);
				}
				else
				{
					// 3, 14
					double _smooth = (_HEIGHT(x  ,y-1) + _tileHeight) * 0.1;
					_XYZV(x+0.5,y+0.0,_smooth, 3);
					_XYZV(x+0.5,y+0.0,_smooth,14);
				}
			}
		}

	}

	//--------------------------------------------------------
	// Calculate the fucking uncooperative corners
	//--------------------------------------------------------
	for(UINT x = 0; x < _size.x; ++x)
	{
		for(UINT y = 0; y < _size.y; ++y)
		{
			//--------------------------------------------------------
			// Calculate the fucking uncooperative corners
			//--------------------------------------------------------
			{
				UINT _tileHeight = _HEIGHT(x,y);
				double _vertHeight = _tileHeight*0.2;
				UINT& _cliffRef = _CLIFF(x,y);

				_XYZV(x+0.0,y+0.0,_vertHeight, 0);
				_XYZV(x+0.0,y+1.0,_vertHeight, 5);
				_XYZV(x+1.0,y+1.0,_vertHeight, 9);
				_XYZV(x+1.0,y+0.0,_vertHeight,13);
				//---------------------------------------------------------
				//   South West Corner
				//---------------------------------------------------------
				{
					if((x == 0) || (y == 0))
					{
						// Border Situation
						if(((_cliffRef & _CLIFF_S) == 0) || ((_cliffRef & _CLIFF_W) == 0))
						{
							if(_cliffRef & _CLIFF_S)
							{
								double _smooth = (_HEIGHT(x-1,y  ) + _tileHeight) * 0.1;
								_XYZV(x+0.0,y+0.0,_smooth, 0);
							}
							else if(_cliffRef & _CLIFF_W)
							{
								double _smooth = (_HEIGHT(x  ,y-1) + _tileHeight) * 0.1;
								_XYZV(x+0.0,y+0.0,_smooth, 0);
							}
						}
						else
						{
							// Four Cliffs Needs to Smooth
							_XYZV(x+0.0,y+0.0,_vertHeight, 0);
						}
					}
					else
					{
						switch(
							((_cliffRef & _CLIFF_S) != 0) +
							((_cliffRef & _CLIFF_W) != 0) + 
							((_CLIFF(x-1,y-1) & _CLIFF_N) != 0) +
							((_CLIFF(x-1,y-1) & _CLIFF_E) != 0)
							)
						{
						case 0:
						case 1:
							{
								double _smooth = (_HEIGHT(x-1,y-1) + _HEIGHT(x-1,y  ) + _HEIGHT(x  ,y-1) + _tileHeight) * 0.05;
								_XYZV(x+0.0,y+0.0,_smooth, 0);
								break;
							}
						case 2:
							// Check if both cliffs belong to you
							{
								if((_cliffRef & _CLIFF_S) && (_cliffRef & _CLIFF_W))
								{
									// Nothing to smooth
									_XYZV(x+0.0,y+0.0,_vertHeight, 0);
									break;
								}
								else if(!(
									((_cliffRef & _CLIFF_S) && (_CLIFF(x-1,y-1) & _CLIFF_N)) ||
									((_cliffRef & _CLIFF_W) && (_CLIFF(x-1,y-1) & _CLIFF_E))))
								{
									double _smooth = 0.0;
									if(_cliffRef & _CLIFF_S)
										_smooth = (_HEIGHT(x-1,y-1) + _HEIGHT(x-1,y  ) + _tileHeight) * (0.2 / 3.0);
									else if(_cliffRef & _CLIFF_W)
										_smooth = (_HEIGHT(x-1,y-1) + _HEIGHT(x  ,y-1) + _tileHeight) * (0.2 / 3.0);
									else
										_smooth = (_HEIGHT(x-1,y  ) + _HEIGHT(x  ,y-1) + _tileHeight) * (0.2 / 3.0);
									_XYZV(x+0.0,y+0.0,_smooth, 0);
									break;
								}
							} // Fall into FULL smoothing function
						case 3:
							// Check if one of the smooth sides belongs to you
							if(((_cliffRef & _CLIFF_S) == 0) || ((_cliffRef & _CLIFF_W) == 0))
							{
								if(_cliffRef & _CLIFF_S)
								{
									double _smooth = (_HEIGHT(x-1,y  ) + _tileHeight) * 0.1;
									_XYZV(x+0.0,y+0.0,_smooth, 0);
									break;
								}
								else if(_cliffRef & _CLIFF_W)
								{
									double _smooth = (_HEIGHT(x  ,y-1) + _tileHeight) * 0.1;
									_XYZV(x+0.0,y+0.0,_smooth, 0);
									break;
								}
							}
							// Else fall into four cliff scenario
						case 4:
							{
								// Four Cliffs Needs to Smooth
								_XYZV(x+0.0,y+0.0,_vertHeight, 0);
								break;
							}
						};
					}
				}

				//---------------------------------------------------------
				//   North West Corner
				//---------------------------------------------------------
				{
					if((x == 0) || (y == (_size.y - 1)))
					{
						// Border Situation
						if(((_cliffRef & _CLIFF_N) == 0) || ((_cliffRef & _CLIFF_W) == 0))
						{
							if(_cliffRef & _CLIFF_N)
							{
								double _smooth = (_HEIGHT(x-1,y  ) + _tileHeight) * 0.1;
								_XYZV(x+0.0,y+1.0,_smooth, 5);
							}
							else if(_cliffRef & _CLIFF_W)
							{
								double _smooth = (_HEIGHT(x  ,y+1) + _tileHeight) * 0.1;
								_XYZV(x+0.0,y+1.0,_smooth, 5);
							}
						}
						else
						{
							// Four Cliffs Needs to Smooth
							_XYZV(x+0.0,y+1.0,_vertHeight, 5);
						}
					}
					else
					{
						switch(
							((_cliffRef & _CLIFF_N) != 0) +
							((_cliffRef & _CLIFF_W) != 0) + 
							((_CLIFF(x-1,y+1) & _CLIFF_S) != 0) +
							((_CLIFF(x-1,y+1) & _CLIFF_E) != 0)
							)
						{
						case 0:
						case 1:
							{
								double _smooth = (_HEIGHT(x-1,y+1) + _HEIGHT(x-1,y  ) + _HEIGHT(x  ,y+1) + _tileHeight) * 0.05;
								_XYZV(x+0.0,y+1.0,_smooth, 5);
								break;
							}	
						case 2:
							// Check if both cliffs belong to you
							{
								if((_cliffRef & _CLIFF_N) && (_cliffRef & _CLIFF_W))
								{
									// Nothing to smooth
									_XYZV(x+0.0,y+1.0,_vertHeight, 5);
									break;
								}
								else if(!(
									((_cliffRef & _CLIFF_N) && (_CLIFF(x-1,y+1) & _CLIFF_S)) ||
									((_cliffRef & _CLIFF_W) && (_CLIFF(x-1,y+1) & _CLIFF_E))))
								{
									double _smooth = 0.0;
									if(_cliffRef & _CLIFF_N)
										_smooth = (_HEIGHT(x-1,y+1) + _HEIGHT(x-1,y  ) + _tileHeight) * (0.2 / 3.0);
									else if(_cliffRef & _CLIFF_W)
										_smooth = (_HEIGHT(x-1,y+1) + _HEIGHT(x  ,y+1) + _tileHeight) * (0.2 / 3.0);
									else
										_smooth = (_HEIGHT(x-1,y  ) + _HEIGHT(x  ,y+1) + _tileHeight) * (0.2 / 3.0);
									_XYZV(x+0.0,y+1.0,_smooth, 5);
									break;
								}
							}
						case 3:
							// Check if one of the smooth sides belongs to you
							if(((_cliffRef & _CLIFF_N) == 0) || ((_cliffRef & _CLIFF_W) == 0))
							{
								if(_cliffRef & _CLIFF_N)
								{
									double _smooth = (_HEIGHT(x-1,y  ) + _tileHeight) * 0.1;
									_XYZV(x+0.0,y+1.0,_smooth, 5);
									break;
								}
								else if(_cliffRef & _CLIFF_W)
								{
									double _smooth = (_HEIGHT(x  ,y+1) + _tileHeight) * 0.1;
									_XYZV(x+0.0,y+1.0,_smooth, 5);
									break;
								}
							}
							// Else fall into four cliff scenario
						case 4:
							{
								// Four Cliffs Needs to Smooth
								_XYZV(x+0.0,y+1.0,_vertHeight, 5);
								break;
							}
						};
					}
				}
				//---------------------------------------------------------
				//   South East Corner
				//---------------------------------------------------------
				{
					if((x == (_size.x -1)) || (y == 0))
					{
						// Border Situation
						if(((_cliffRef & _CLIFF_S) == 0) || ((_cliffRef & _CLIFF_E) == 0))
						{
							if(_cliffRef & _CLIFF_S)
							{
								double _smooth = (_HEIGHT(x+1,y  ) + _tileHeight) * 0.1;
								_XYZV(x+1.0,y+0.0,_smooth, 13);
							}
							else if(_cliffRef & _CLIFF_E)
							{
								double _smooth = (_HEIGHT(x  ,y-1) + _tileHeight) * 0.1;
								_XYZV(x+1.0,y+0.0,_smooth, 13);
							}
						}
						else
						{
							// Four Cliffs Needs to Smooth
							_XYZV(x+1.0,y+0.0,_vertHeight, 13);
						}
					}
					else
					{
						switch(
							((_cliffRef & _CLIFF_S) != 0) +
							((_cliffRef & _CLIFF_E) != 0) + 
							((_CLIFF(x+1,y-1) & _CLIFF_N) != 0) +
							((_CLIFF(x+1,y-1) & _CLIFF_W) != 0)
							)
						{
						case 0:
						case 1:
							{
								double _smooth = (_HEIGHT(x+1,y-1) + _HEIGHT(x+1,y  ) + _HEIGHT(x  ,y-1) + _tileHeight) * 0.05;
								_XYZV(x+1.0,y+0.0,_smooth, 13);
								break;
							}	
						case 2:
							// Check if both cliffs belong to you
							{
								if((_cliffRef & _CLIFF_S) && (_cliffRef & _CLIFF_E))
								{
									// Nothing to smooth
									_XYZV(x+1.0,y+0.0,_vertHeight, 13);
									break;
								}
								else if(!(
									((_cliffRef & _CLIFF_S) && (_CLIFF(x+1,y-1) & _CLIFF_N)) ||
									((_cliffRef & _CLIFF_E) && (_CLIFF(x+1,y-1) & _CLIFF_W))))
								{
									double _smooth = 0.0;
									if(_cliffRef & _CLIFF_S)
										_smooth = (_HEIGHT(x+1,y-1) + _HEIGHT(x+1,y  ) + _tileHeight) * (0.2 / 3.0);
									else if(_cliffRef & _CLIFF_E)
										_smooth = (_HEIGHT(x+1,y-1) + _HEIGHT(x  ,y-1) + _tileHeight) * (0.2 / 3.0);
									else
										_smooth = (_HEIGHT(x+1,y  ) + _HEIGHT(x  ,y-1) + _tileHeight) * (0.2 / 3.0);
									_XYZV(x+1.0,y+0.0,_smooth,13);
									break;
								}
							}
						case 3:
							// Check if one of the smooth sides belongs to you
							if(((_cliffRef & _CLIFF_S) == 0) || ((_cliffRef & _CLIFF_E) == 13))
							{
								if(_cliffRef & _CLIFF_S)
								{
									double _smooth = (_HEIGHT(x+1,y  ) + _tileHeight) * 0.1;
									_XYZV(x+1.0,y+0.0,_smooth, 13);
									break;
								}
								else if(_cliffRef & _CLIFF_E)
								{
									double _smooth = (_HEIGHT(x  ,y-1) + _tileHeight) * 0.1;
									_XYZV(x+1.0,y+0.0,_smooth, 13);
									break;
								}
							}
							// Else fall into four cliff scenario
						case 4:
							{
								// Four Cliffs Needs to Smooth
								_XYZV(x+1.0,y+0.0,_vertHeight, 13);
								break;
							}
						};
					}
				}

				//---------------------------------------------------------
				//   North East Corner
				//---------------------------------------------------------
				{
					if((x == (_size.x -1)) || (y == (_size.y - 1)))
					{
						// Border Situation
						if(((_cliffRef & _CLIFF_N) == 0) || ((_cliffRef & _CLIFF_E) == 0))
						{
							if(_cliffRef & _CLIFF_N)
							{
								double _smooth = (_HEIGHT(x+1,y  ) + _tileHeight) * 0.1;
								_XYZV(x+1.0,y+1.0,_smooth, 9);
							}
							else if(_cliffRef & _CLIFF_E)
							{
								double _smooth = (_HEIGHT(x  ,y+1) + _tileHeight) * 0.1;
								_XYZV(x+1.0,y+1.0,_smooth, 9);
							}
						}
						else
						{
							// Four Cliffs Needs to Smooth
							_XYZV(x+1.0,y+1.0,_vertHeight, 5);
						}
					}
					else
					{
						switch(
							((_cliffRef & _CLIFF_N) != 0) +
							((_cliffRef & _CLIFF_E) != 0) + 
							((_CLIFF(x+1,y+1) & _CLIFF_S) != 0) +
							((_CLIFF(x+1,y+1) & _CLIFF_W) != 0)
							)
						{
						case 0:
						case 1:
							{
								double _smooth = (_HEIGHT(x+1,y+1) + _HEIGHT(x+1,y  ) + _HEIGHT(x  ,y+1) + _tileHeight) * 0.05;
								_XYZV(x+1.0,y+1.0,_smooth, 9);
								break;
							}	
						case 2:
							// Check if both cliffs belong to you
							{
								if((_cliffRef & _CLIFF_N) && (_cliffRef & _CLIFF_E))
								{
									// Nothing to smooth
									_XYZV(x+1.0,y+1.0,_vertHeight, 9);
									break;
								}
								else if(!(
									((_cliffRef & _CLIFF_N) && (_CLIFF(x+1,y+1) & _CLIFF_S)) ||
									((_cliffRef & _CLIFF_E) && (_CLIFF(x+1,y+1) & _CLIFF_W))))
								{
									double _smooth = 0.0;
									if(_cliffRef & _CLIFF_N)
										_smooth = (_HEIGHT(x+1,y+1) + _HEIGHT(x+1,y  ) + _tileHeight) * (0.2 / 3.0);
									else if(_cliffRef & _CLIFF_E)
										_smooth = (_HEIGHT(x+1,y+1) + _HEIGHT(x  ,y+1) + _tileHeight) * (0.2 / 3.0);
									else
										_smooth = (_HEIGHT(x+1,y  ) + _HEIGHT(x  ,y+1) + _tileHeight) * (0.2 / 3.0);
									_XYZV(x+1.0,y+1.0,_smooth, 9);
									break;
								}
							}
						case 3:
							// Check if one of the smooth sides belongs to you
							if(((_cliffRef & _CLIFF_N) == 0) || ((_cliffRef & _CLIFF_E) == 0))
							{
								if(_cliffRef & _CLIFF_N)
								{
									double _smooth = (_HEIGHT(x+1,y  ) + _tileHeight) * 0.1;
									_XYZV(x+1.0,y+1.0,_smooth, 9);
									break;
								}
								else if(_cliffRef & _CLIFF_E)
								{
									double _smooth = (_HEIGHT(x  ,y+1) + _tileHeight) * 0.1;
									_XYZV(x+1.0,y+1.0,_smooth, 9);
									break;
								}
							}
							// Else fall into four cliff scenario
						case 4:
							{
								// Four Cliffs Needs to Smooth
								_XYZV(x+1.0,y+1.0,_vertHeight, 9);
								break;
							}
						};
					}
				}
			}
			//--------------------------------------------------------
			// Calculate the Triangle Normals
			//--------------------------------------------------------			
			{
				UINT _tOff = (x*_size.y + y)*8;
				UINT _vOff = _tOff*2;

				//-----------------------------------
				//	Calculate Normal
				//-----------------------------------
				_tNorm[_tOff + 0] = (_mesh.getV(_vOff + 0) - _mesh.getV(_vOff + 3)) * (_mesh.getV(_vOff + 0) - _mesh.getV(_vOff + 1));
				_tNorm[_tOff + 1] = (_mesh.getV(_vOff + 2) - _mesh.getV(_vOff + 1)) * (_mesh.getV(_vOff + 2) - _mesh.getV(_vOff + 3));
				_tNorm[_tOff + 2] = (_mesh.getV(_vOff +14) - _mesh.getV(_vOff +13)) * (_mesh.getV(_vOff +14) - _mesh.getV(_vOff +15));
				_tNorm[_tOff + 3] = (_mesh.getV(_vOff +12) - _mesh.getV(_vOff +15)) * (_mesh.getV(_vOff +12) - _mesh.getV(_vOff +13));
				_tNorm[_tOff + 4] = (_mesh.getV(_vOff + 4) - _mesh.getV(_vOff + 7)) * (_mesh.getV(_vOff + 4) - _mesh.getV(_vOff + 5));
				_tNorm[_tOff + 5] = (_mesh.getV(_vOff + 6) - _mesh.getV(_vOff + 5)) * (_mesh.getV(_vOff + 6) - _mesh.getV(_vOff + 7));
				_tNorm[_tOff + 6] = (_mesh.getV(_vOff +11) - _mesh.getV(_vOff +10)) * (_mesh.getV(_vOff +11) - _mesh.getV(_vOff + 8));
				_tNorm[_tOff + 7] = (_mesh.getV(_vOff + 9) - _mesh.getV(_vOff + 8)) * (_mesh.getV(_vOff + 9) - _mesh.getV(_vOff +10));

				//-----------------------------------
				//	Normalize
				//-----------------------------------
				_tNorm[_tOff + 0].Normalize();
				_tNorm[_tOff + 1].Normalize();
				_tNorm[_tOff + 2].Normalize();
				_tNorm[_tOff + 3].Normalize();
				_tNorm[_tOff + 4].Normalize();
				_tNorm[_tOff + 5].Normalize();
				_tNorm[_tOff + 6].Normalize();
				_tNorm[_tOff + 7].Normalize();

				//-----------------------------------
				//	Invert if incorrect
				// No Check needed
				//-----------------------------------
				/*
				if(_tNorm[_tOff + 0].z < 0)				_tNorm[_tOff + 0].Invert();
				if(_tNorm[_tOff + 1].z < 0)				_tNorm[_tOff + 1].Invert();
				if(_tNorm[_tOff + 2].z < 0)				_tNorm[_tOff + 2].Invert();
				if(_tNorm[_tOff + 3].z < 0)				_tNorm[_tOff + 3].Invert();
				if(_tNorm[_tOff + 4].z < 0)				_tNorm[_tOff + 4].Invert();
				if(_tNorm[_tOff + 5].z < 0)				_tNorm[_tOff + 5].Invert();
				if(_tNorm[_tOff + 6].z < 0)				_tNorm[_tOff + 6].Invert();
				if(_tNorm[_tOff + 7].z < 0)				_tNorm[_tOff + 7].Invert();
				*/
			}
		}
		// END OF FOR LOOP
	}
	//---------------------------------------------
	//	Calculate Vertex Normals
	//---------------------------------------------
	for(UINT v = 0; v < _VCount; ++v)
	{
		//---------------------------------------
		//	Get Vertex Position
		// Calculate Six Sample Points
		//---------------------------------------
		CVector3 _vPos = _mesh.getV(v);
		CVector3 _samp[6] =
		{
			(_vPos + CVector3( 0.2, 0.2, 0.0)),
			(_vPos + CVector3( 0.2,-0.1, 0.0)),
			(_vPos + CVector3( 0.1,-0.2, 0.0)),
			(_vPos + CVector3(-0.2,-0.2, 0.0)),
			(_vPos + CVector3(-0.2, 0.1, 0.0)),
			(_vPos + CVector3(-0.1, 0.2, 0.0)),
		};

		//---------------------------------------
		//	Fetch Normals of sample points
		// Sum normals (0,3) count double
		//---------------------------------------
		CVector3 _sumNorm(0,0,0);
		for(UINT s = 0; s < 6; ++s)
		{
			CVector3& _sampR = _samp[s];
			// Get Map Position
			MAP_POS _ipos = {
				(UINT)_sampR.x, 
				(UINT)_sampR.y, 
				0};

			// Check for Range
			if((_ipos.x >= _size.x) || (_ipos.y >= _size.y) || (_sampR.x < 0) || (_sampR.y < 0))
				continue;

			// Get Base Index
			UINT _tOff = (_ipos.x*_size.y + _ipos.y)*8;

			// Gets Relative Position
			MAP_POS _rpos = {
				(UINT)((_sampR.x - _ipos.x)*2),
				(UINT)((_sampR.y - _ipos.y)*2),
				0};

			if(s % 3 == 0)
				_sumNorm = _sumNorm + (_tNorm[_tOff + (_rpos.x + (_rpos.y * 2))*2 + (s%2)] * 2);
			else
				_sumNorm = _sumNorm + _tNorm[_tOff + (_rpos.x + (_rpos.y * 2))*2 + (s%2)];
		}
		_sumNorm.Normalize();
		_mesh.setN(v,_sumNorm);
	}

	//---------------------------------------------
	//	Build Cliffs
	//---------------------------------------------
	JWL_Mesh _final(_VCount+(_CliffCount*8),GL_QUADS);
	memcpy(_final.m_array,_mesh.m_array,sizeof(Intlvd_point)*_VCount);
	UINT _offset = 0;

	for(UINT x = 0; x < _size.x; ++x)
	{
		for(UINT y = 0; y < _size.y; ++y)
		{
			UINT& _cliffRef = _CLIFF(x,y);
			//-------------------------------------------
			// Detect Cliffs
			//-------------------------------------------
			{
				//---------------------------
				// North Facing Cliff
				//---------------------------
				if(_cliffRef & _CLIFF_N)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_OFFN(0,1,0,0);
					_OFFN(0,1,0,1);
					_OFFN(0,1,0,2);
					_OFFN(0,1,0,3);
					_OFFN(0,1,0,4);
					_OFFN(0,1,0,5);
					_OFFN(0,1,0,6);
					_OFFN(0,1,0,7);
					//------------------------
					// BUILD Verts
					//------------------------
					if(y == (_size.y -1))
					{
						_OFFV(x + 1.0,y + 1.0,               0,0);
						_OFFV(x + 1.0,y + 1.0,_GET_Z(x,y  , 9),1);
						_OFFV(x + 0.5,y + 1.0,_GET_Z(x,y  , 8),2);
						_OFFV(x + 0.5,y + 1.0,               0,3);
						_OFFV(x + 0.5,y + 1.0,_GET_Z(x,y  , 6),4);
						_OFFV(x + 0.0,y + 1.0,_GET_Z(x,y  , 5),5);
						_OFFV(x + 0.0,y + 1.0,               0,6);
						_OFFV(x + 0.5,y + 1.0,               0,7);
					}
					else
					{
						_OFFV(x + 1.0,y + 1.0,_GET_Z(x,y+1,12),0);
						_OFFV(x + 1.0,y + 1.0,_GET_Z(x,y  , 9),1);
						_OFFV(x + 0.5,y + 1.0,_GET_Z(x,y  , 8),2);
						_OFFV(x + 0.5,y + 1.0,_GET_Z(x,y+1,14),3);
						_OFFV(x + 0.5,y + 1.0,_GET_Z(x,y  , 6),4);
						_OFFV(x + 0.0,y + 1.0,_GET_Z(x,y  , 5),5);
						_OFFV(x + 0.0,y + 1.0,_GET_Z(x,y+1, 1),6);
						_OFFV(x + 0.5,y + 1.0,_GET_Z(x,y+1, 2),7);
					}
					//------------------------
					// BUILD Tex
					//------------------------
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 0].vz* 0.25,0);
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 1].vz* 0.25,1);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 2].vz* 0.25,2);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 3].vz* 0.25,3);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 4].vz* 0.25,4);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 5].vz* 0.25,5);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 6].vz* 0.25,6);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 7].vz* 0.25,7);

					_offset += 8;
				}

				//---------------------------
				// South Facing Cliff
				//---------------------------
				if(y == 0)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_OFFN(0,-1,0,0);
					_OFFN(0,-1,0,1);
					_OFFN(0,-1,0,2);
					_OFFN(0,-1,0,3);
					_OFFN(0,-1,0,4);
					_OFFN(0,-1,0,5);
					_OFFN(0,-1,0,6);
					_OFFN(0,-1,0,7);
					//------------------------
					// BUILD Verts
					//------------------------
					if(y == 0)
					{
						_OFFV(x + 0.0,y + 0.0,               0,0);
						_OFFV(x + 0.0,y + 0.0,_GET_Z(x,y  , 0),1);
						_OFFV(x + 0.5,y + 0.0,_GET_Z(x,y  , 3),2);
						_OFFV(x + 0.5,y + 0.0,               0,3);
						_OFFV(x + 0.5,y + 0.0,_GET_Z(x,y  ,14),4);
						_OFFV(x + 1.0,y + 0.0,_GET_Z(x,y  ,13),5);
						_OFFV(x + 1.0,y + 0.0,               0,6);
						_OFFV(x + 0.5,y + 0.0,               0,7);
					}
					else
					{
						_OFFV(x + 0.0,y + 0.0,_GET_Z(x,y-1, 5),0);
						_OFFV(x + 0.0,y + 0.0,_GET_Z(x,y  , 0),1);
						_OFFV(x + 0.5,y + 0.0,_GET_Z(x,y  , 3),2);
						_OFFV(x + 0.5,y + 0.0,_GET_Z(x,y-1, 6),3);
						_OFFV(x + 0.5,y + 0.0,_GET_Z(x,y  ,14),4);
						_OFFV(x + 1.0,y + 0.0,_GET_Z(x,y  ,13),5);
						_OFFV(x + 1.0,y + 0.0,_GET_Z(x,y-1, 9),6);
						_OFFV(x + 0.5,y + 0.0,_GET_Z(x,y-1, 8),7);
					}
					//------------------------
					// BUILD Tex
					//------------------------
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 0].vz* 0.25,0);
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 1].vz* 0.25,1);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 2].vz* 0.25,2);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 3].vz* 0.25,3);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 4].vz* 0.25,4);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 5].vz* 0.25,5);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 6].vz* 0.25,6);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 7].vz* 0.25,7);

					_offset += 8;
				}

				//---------------------------
				// East Facing Cliff
				//---------------------------
				if(_cliffRef & _CLIFF_E)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_OFFN(1,0,0,0);
					_OFFN(1,0,0,1);
					_OFFN(1,0,0,2);
					_OFFN(1,0,0,3);
					_OFFN(1,0,0,4);
					_OFFN(1,0,0,5);
					_OFFN(1,0,0,6);
					_OFFN(1,0,0,7);
					//------------------------
					// BUILD Verts
					//------------------------
					if(x == (_size.x -1))
					{
						_OFFV(x + 1.0,y + 0.0,               0,0);
						_OFFV(x + 1.0,y + 0.0,_GET_Z(x,y  ,13),1);
						_OFFV(x + 1.0,y + 0.5,_GET_Z(x,y  ,12),2);
						_OFFV(x + 1.0,y + 0.5,               0,3);
						_OFFV(x + 1.0,y + 0.5,_GET_Z(x,y  ,10),4);
						_OFFV(x + 1.0,y + 1.0,_GET_Z(x,y  , 9),5);
						_OFFV(x + 1.0,y + 1.0,               0,6);
						_OFFV(x + 1.0,y + 0.5,               0,7);
					}
					else
					{
						_OFFV(x + 1.0,y + 0.0,_GET_Z(x+1,y, 0),0);
						_OFFV(x + 1.0,y + 0.0,_GET_Z(x,y  ,13),1);
						_OFFV(x + 1.0,y + 0.5,_GET_Z(x,y  ,12),2);
						_OFFV(x + 1.0,y + 0.5,_GET_Z(x+1,y, 1),3);
						_OFFV(x + 1.0,y + 0.5,_GET_Z(x,y  ,10),4);
						_OFFV(x + 1.0,y + 1.0,_GET_Z(x,y  , 9),5);
						_OFFV(x + 1.0,y + 1.0,_GET_Z(x+1,y, 5),6);
						_OFFV(x + 1.0,y + 0.5,_GET_Z(x+1,y, 4),7);
					}
					//------------------------
					// BUILD Tex
					//------------------------
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 0].vz* 0.25,0);
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 1].vz* 0.25,1);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 2].vz* 0.25,2);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 3].vz* 0.25,3);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 4].vz* 0.25,4);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 5].vz* 0.25,5);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 6].vz* 0.25,6);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 7].vz* 0.25,7);

					_offset += 8;
				}

				//---------------------------
				// West Facing Cliff
				//---------------------------
				if(x == 0)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_OFFN(-1,0,0,0);
					_OFFN(-1,0,0,1);
					_OFFN(-1,0,0,2);
					_OFFN(-1,0,0,3);
					_OFFN(-1,0,0,4);
					_OFFN(-1,0,0,5);
					_OFFN(-1,0,0,6);
					_OFFN(-1,0,0,7);
					//------------------------
					// BUILD Verts
					//------------------------
					if(x == 0)
					{
						_OFFV(x + 0.0,y + 1.0,               0,0);
						_OFFV(x + 0.0,y + 1.0,_GET_Z(x,y  , 5),1);
						_OFFV(x + 0.0,y + 0.5,_GET_Z(x,y  , 4),2);
						_OFFV(x + 0.0,y + 0.5,               0,3);
						_OFFV(x + 0.0,y + 0.5,_GET_Z(x,y  ,12),4);
						_OFFV(x + 0.0,y + 0.0,_GET_Z(x,y  ,13),5);
						_OFFV(x + 0.0,y + 0.0,               0,6);
						_OFFV(x + 0.0,y + 0.5,               0,7);
					}
					else
					{
						_OFFV(x + 0.0,y + 1.0,_GET_Z(x-1,y, 9),0);
						_OFFV(x + 0.0,y + 1.0,_GET_Z(x  ,y, 5),1);
						_OFFV(x + 0.0,y + 0.5,_GET_Z(x  ,y, 4),2);
						_OFFV(x + 0.0,y + 0.5,_GET_Z(x-1,y,10),3);
						_OFFV(x + 0.0,y + 0.5,_GET_Z(x  ,y,12),4);
						_OFFV(x + 0.0,y + 0.0,_GET_Z(x  ,y,13),5);
						_OFFV(x + 0.0,y + 0.0,_GET_Z(x-1,y, 0),6);
						_OFFV(x + 0.0,y + 0.5,_GET_Z(x-1,y, 1),7);
					}
					//------------------------
					// BUILD Tex
					//------------------------
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 0].vz* 0.25,0);
					_OFFT(_TEX_LEFT,	_final.m_array[_VCount + _offset + 1].vz* 0.25,1);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 2].vz* 0.25,2);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 3].vz* 0.25,3);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 4].vz* 0.25,4);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 5].vz* 0.25,5);
					_OFFT(_TEX_RIGHT,	_final.m_array[_VCount + _offset + 6].vz* 0.25,6);
					_OFFT(_TEX_MID,		_final.m_array[_VCount + _offset + 7].vz* 0.25,7);

					_offset += 8;
				}
			}
		}
	}
	//-----------------------------------------------------------
	//             Make the List
	//-----------------------------------------------------------
	UINT _mID = glGenLists(1);
	glNewList(_mID,GL_COMPILE);
	glPushMatrix();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		// Enable Clients to Recieve Data	
		glInterleavedArrays(GL_T2F_N3F_V3F,0,_final.m_array);

		glDrawArrays(_final.m_mode , 0, _final.m_size);				// Draw All Of The Quads At Once

		glCullFace(GL_BACK);
	glPopMatrix();
	glEndList();

	//--------------------------------------------
	//	Update Gui
	//--------------------------------------------
	m_Gui.AddMap(_mID,m_Map.getMapTexture());
	m_Gui.setMapNorms(_tNorm);

	//---------------------------------------
	// Clean Up
	//---------------------------------------

	#undef _XYZV
	#undef _XYZN
	#undef _XYZT	
	#undef _GET_Z
	#undef _OFFV
	#undef _OFFN
	#undef _OFFT	
	#undef _HEIGHT	
	#undef _TEXTURE

	#undef _TEX_LEFT
	#undef _TEX_MID	
	#undef _TEX_RIGHT

	#undef _CLIFF
	#undef _CLIFF_N	
	#undef _CLIFF_E	
	#undef _CLIFF_S	
	#undef _CLIFF_W	
	#undef _CLIFF_OFFSET

	if(_cliff)
		delete[] _cliff;
}

//======================================================================
//	Build LOGIC Mesh. Used for debugging and such
// Displays the map as the logical system "sees" it.
//======================================================================
void JWL_Helpers::BuildLogicMesh()
{
	#define _XYZV(X,Y,Z,V)		_final.setV(((x*_size.y + y)*4 + (V)),(X),(Y),(Z));
	#define _XYZN(X,Y,Z,V)		_final.setN(((x*_size.y + y)*4 + (V)),(X),(Y),(Z));
	#define _XYZT(X,Y,V)		_final.setT(((x*_size.y + y)*4 + (V)),(X),(Y));
	#define _GET_Z(X,Y,V)		_final.m_array[(((X)*_size.y + (Y))*4 + (V))].vz
	#define _CLIV(X,Y,Z,V)		_final.setV((_CMCount + _Coffset + (V)),(X),(Y),(Z));
	#define _CLIN(X,Y,Z,V)		_final.setN((_CMCount + _Coffset + (V)),(X),(Y),(Z));
	#define _CLIT(X,Y,V)		_final.setT((_CMCount + _Coffset + (V)),(X),(Y));
	#define _LOGV(X,Y,Z,V)		_final.setV((_CMCount + _CVCount + _Loffset + (V)),(X),(Y),(Z));
	#define _LOGN(X,Y,Z,V)		_final.setN((_CMCount + _CVCount + _Loffset + (V)),(X),(Y),(Z));
	#define _LOGT(X,Y,V)		_final.setT((_CMCount + _CVCount + _Loffset + (V)),(X),(Y));
	#define _HEIGHT(X,Y)		_grid[(X)*_size.y + (Y)].m_height
	#define _TEXTURE(X,Y)		_grid[(X)*_size.y + (Y)].m_tl

	#define _CLIFF(X,Y)			_cliff[(X)*_size.y + (Y)]

	#define _CLIFF_N			  1
	#define _CLIFF_E			  2
	#define _CLIFF_S			  4
	#define _CLIFF_W			  8
	//---------------------------------
	//	Detect How Many Verts
	// Count Logic Blocks and Cliffs
	//---------------------------------
	const MAP_CELL* _grid = m_Map.getGridPTR();
	const MAP_SIZE _size = m_Map.getMapSize();

	UINT* _cliff = new UINT[_size.x * _size.y];
	memset(_cliff,0,sizeof(UINT)*_size.x * _size.y);

	UINT _CliffCount = 0;
	UINT _BlockCount = 0;
	for(UINT x = 0; x < _size.x; ++x)
	{
		for(UINT y = 0; y < _size.y; ++y)
		{
			UINT& _cliffRef = _CLIFF(x,y);
			UINT _tileHeight = _HEIGHT(x,y);
			//-------------------------------------------
			// Detect Cliffs
			//-------------------------------------------			
			{
				if((y == (_size.y -1))	|| (_HEIGHT(x,y+1) < _tileHeight))
				{
					_cliffRef |= _CLIFF_N;					++_CliffCount;
				}

				if((y == 0)				|| (_HEIGHT(x,y-1) < _tileHeight))
				{
					_cliffRef |= _CLIFF_S;					++_CliffCount;
				}

				if((x == (_size.x -1))	|| (_HEIGHT(x+1,y) < _tileHeight))
				{
					_cliffRef |= _CLIFF_E;					++_CliffCount;
				}

				if((x == 0)				|| (_HEIGHT(x-1,y) < _tileHeight))
				{
					_cliffRef |= _CLIFF_W;					++_CliffCount;
				}
			}
			//-------------------------------------------
			// Detect Logic Blocks
			//-------------------------------------------
			MAP_CELL* _curr = _grid[(x)*_size.y + (y)].m_next;
			while(_curr)
			{
				_curr = _curr->m_next;						++_BlockCount;
			}
		}
	}

	//---------------------------------------------
	//	Build Cliffs
	//---------------------------------------------
	const UINT _CMCount = (_size.x * _size.y * 4);
	const UINT _CVCount = (_CliffCount		 * 4);
	const UINT _CBCount = (_BlockCount	*5	 * 4);
	const UINT _VCount = _CMCount + _CVCount + _CBCount;
	JWL_Mesh _final(_VCount,GL_QUADS);

	UINT _Coffset = 0;
	UINT _Loffset = 0;

	for(UINT x = 0; x < _size.x; ++x)
	{
		for(UINT y = 0; y < _size.y; ++y)
		{
			UINT& _cliffRef = _CLIFF(x,y);
			UINT _tileHeight = _HEIGHT(x,y);
			double _vertHeight = _tileHeight*0.2;
			//-------------------------------------------
			// Setup Map
			//-------------------------------------------
			{
				_XYZT(0.0,0.0,0);
				_XYZT(0.2,0.0,1);
				_XYZT(0.2,0.2,2);
				_XYZT(0.0,0.2,3);

				_XYZV(x  ,y  ,_vertHeight,0);
				_XYZV(x+1,y  ,_vertHeight,1);
				_XYZV(x+1,y+1,_vertHeight,2);
				_XYZV(x  ,y+1,_vertHeight,3);

				_XYZN(0,0,1,0);
				_XYZN(0,0,1,1);
				_XYZN(0,0,1,2);
				_XYZN(0,0,1,3);
			}
			//-------------------------------------------
			// Build Cliffs
			//-------------------------------------------
			if(_cliffRef > 0)
			{
				//---------------------------
				// North Facing Cliff
				//---------------------------
				if(_cliffRef & _CLIFF_N)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_CLIN(0,1,0,0);
					_CLIN(0,1,0,1);
					_CLIN(0,1,0,2);
					_CLIN(0,1,0,3);
					//------------------------
					// BUILD Verts
					//------------------------
					_CLIV(x + 0.0,y + 1.0,          0,0);
					_CLIV(x + 0.0,y + 1.0,_vertHeight,1);
					_CLIV(x + 1.0,y + 1.0,_vertHeight,2);
					_CLIV(x + 1.0,y + 1.0,          0,3);
					
					//------------------------
					// BUILD Tex
					//------------------------
					_CLIT(0.8,0                  ,0);
					_CLIT(0.8,(_tileHeight / 5.0),1);
					_CLIT(1.0,(_tileHeight / 5.0),2);
					_CLIT(1.0,0                  ,3);

					_Coffset += 4;
				}
				//---------------------------
				// South Facing Cliff
				//---------------------------
				if(_cliffRef & _CLIFF_S)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_CLIN(0,-1,0,0);
					_CLIN(0,-1,0,1);
					_CLIN(0,-1,0,2);
					_CLIN(0,-1,0,3);
					//------------------------
					// BUILD Verts
					//------------------------
					_CLIV(x + 1.0,y + 0.0,          0,0);
					_CLIV(x + 1.0,y + 0.0,_vertHeight,1);
					_CLIV(x + 0.0,y + 0.0,_vertHeight,2);
					_CLIV(x + 0.0,y + 0.0,          0,3);
					//------------------------
					// BUILD Tex
					//------------------------
					_CLIT(0.8,0                  ,0);
					_CLIT(0.8,(_tileHeight / 5.0),1);
					_CLIT(1.0,(_tileHeight / 5.0),2);
					_CLIT(1.0,0                  ,3);

					_Coffset += 4;
				}
				//---------------------------
				// East Facing Cliff
				//---------------------------
				if(_cliffRef & _CLIFF_E)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_CLIN(1,0,0,0);
					_CLIN(1,0,0,1);
					_CLIN(1,0,0,2);
					_CLIN(1,0,0,3);
					//------------------------
					// BUILD Verts
					//------------------------
					_CLIV(x + 1.0,y + 1.0,          0,0);
					_CLIV(x + 1.0,y + 1.0,_vertHeight,1);
					_CLIV(x + 1.0,y + 0.0,_vertHeight,2);
					_CLIV(x + 1.0,y + 0.0,          0,3);
					//------------------------
					// BUILD Tex
					//------------------------
					_CLIT(0.8,0                  ,0);
					_CLIT(0.8,(_tileHeight / 5.0),1);
					_CLIT(1.0,(_tileHeight / 5.0),2);
					_CLIT(1.0,0                  ,3);

					_Coffset += 4;
				}
				//---------------------------
				// West Facing Cliff
				//---------------------------
				if(_cliffRef & _CLIFF_W)
				{
					//------------------------
					// BUILD Norms
					//------------------------
					_CLIN(-1,0,0,0);
					_CLIN(-1,0,0,1);
					_CLIN(-1,0,0,2);
					_CLIN(-1,0,0,3);
					//------------------------
					// BUILD Verts
					//------------------------
					_CLIV(x + 0.0,y + 1.0,          0,0);
					_CLIV(x + 0.0,y + 1.0,_vertHeight,3);
					_CLIV(x + 0.0,y + 0.0,_vertHeight,2);
					_CLIV(x + 0.0,y + 0.0,          0,1);
					//------------------------
					// BUILD Tex
					//------------------------
					_CLIT(0.8,0                  ,0);
					_CLIT(0.8,(_tileHeight / 5.0),1);
					_CLIT(1.0,(_tileHeight / 5.0),2);
					_CLIT(1.0,0                  ,3);

					_Coffset += 4;
				}
			}
			//-------------------------------------------
			// Build Logic Blocks Cliffs
			//-------------------------------------------
			MAP_CELL* _curr = _grid[(x)*_size.y + (y)].m_next;
			while(_curr)
			{
				UINT& _logic = _curr->m_tl;
				double _bot = (_curr->m_height)*0.2f;
				double _top = _bot + 1.0f;
				//-----------------------
				// BLOCK
				//-----------------------
				if(_logic & LO_BLOCK)
				{
					_LOGV(x  ,y+1, _top ,0);
					_LOGV(x  ,y  , _top ,1);
					_LOGV(x+1,y  , _top ,2);
					_LOGV(x+1,y+1, _top ,3);					
				}
				else
				{
					_LOGV(x  ,y+1, _bot ,0);
					_LOGV(x  ,y  , _bot ,1);					
					_LOGV(x+1,y  , _bot ,2);
					_LOGV(x+1,y+1, _bot ,3);					
				}

				_LOGT(0,1,0);
				_LOGT(0,0,1);
				_LOGT(1,0,2);
				_LOGT(1,1,3);				

				_LOGN(0,0,1,0);
				_LOGN(0,0,1,1);
				_LOGN(0,0,1,2);
				_LOGN(0,0,1,3);

				_Loffset += 4;
				//-----------------------
				// North Wall
				//-----------------------
				if((_logic & LO_BLOCK) || (_logic & LO_WL_N))
				{
					_LOGT(0.2,0,0);
					_LOGT(0.4,0,1);
					_LOGT(0.4,1,2);
					_LOGT(0.2,1,3);

					_LOGV(x  ,y+1, _bot ,0);
					_LOGV(x  ,y+1, _top ,1);
					_LOGV(x+1,y+1, _top ,2);
					_LOGV(x+1,y+1, _bot ,3);										

					_LOGN(0,1,0,0);
					_LOGN(0,1,0,1);
					_LOGN(0,1,0,2);
					_LOGN(0,1,0,3);

					_Loffset += 4;
				}
				//-----------------------
				// South Wall
				//-----------------------
				if((_logic & LO_BLOCK) || (_logic & LO_WL_S))
				{
					_LOGT(0.2,0,0);
					_LOGT(0.4,0,1);
					_LOGT(0.4,1,2);
					_LOGT(0.2,1,3);

					_LOGV(x  ,y  , _bot ,0);
					_LOGV(x+1,y  , _bot ,1);
					_LOGV(x+1,y  , _top ,2);
					_LOGV(x  ,y  , _top ,3);

					_LOGN(0,-1,0,0);
					_LOGN(0,-1,0,1);
					_LOGN(0,-1,0,2);
					_LOGN(0,-1,0,3);

					_Loffset += 4;
				}
				//-----------------------
				// East Wall
				//-----------------------
				if((_logic & LO_BLOCK) || (_logic & LO_WL_E))
				{
					_LOGT(0.2,0,0);
					_LOGT(0.4,0,1);
					_LOGT(0.4,1,2);
					_LOGT(0.2,1,3);

					_LOGV(x+1,y  , _bot ,0);
					_LOGV(x+1,y+1, _bot ,1);
					_LOGV(x+1,y+1, _top ,2);
					_LOGV(x+1,y  , _top ,3);

					_LOGN(1,0,0,0);
					_LOGN(1,0,0,1);
					_LOGN(1,0,0,2);
					_LOGN(1,0,0,3);

					_Loffset += 4;
				}
				//-----------------------
				// West Wall
				//-----------------------
				if((_logic & LO_BLOCK) || (_logic & LO_WL_W))
				{
					_LOGT(0.2,0,0);
					_LOGT(0.4,0,1);
					_LOGT(0.4,1,2);
					_LOGT(0.2,1,3);

					_LOGV(x,y  , _bot ,0);
					_LOGV(x,y  , _top ,1);
					_LOGV(x,y+1, _top ,2);
					_LOGV(x,y+1, _bot ,3);
					

					_LOGN(-1,0,0,0);
					_LOGN(-1,0,0,1);
					_LOGN(-1,0,0,2);
					_LOGN(-1,0,0,3);

					_Loffset += 4;
				}

				_curr = _curr->m_next;
			}
		}
	}


	//-----------------------------------------------------------
	//             Make the List
	//-----------------------------------------------------------
	UINT _mID = glGenLists(1);
	glNewList(_mID,GL_COMPILE);
	glPushMatrix();
		glEnable(GL_CULL_FACE);

		// Enable Clients to Recieve Data	
		glInterleavedArrays(GL_T2F_N3F_V3F,0,_final.m_array);
		glDrawArrays(_final.m_mode , 0, _final.m_size);				// Draw All Of The Quads At Once
	glPopMatrix();
	glEndList();

	//--------------------------------------------
	//	Update Gui
	//--------------------------------------------
	m_Gui.AddMap(_mID,"Map.jpg");
	m_Gui.setMapNorms(0);

	//---------------------------------------
	// Clean Up
	//---------------------------------------
	#undef _XYZV
	#undef _XYZN
	#undef _XYZT
	#undef _GET_Z
	#undef _CLIV
	#undef _CLIN
	#undef _CLIT
	#undef _LOGV
	#undef _LOGN
	#undef _LOGT
	#undef _HEIGHT
	#undef _TEXTURE

	#undef _CLIFF

	#undef _CLIFF_N
	#undef _CLIFF_E
	#undef _CLIFF_S
	#undef _CLIFF_W
}
