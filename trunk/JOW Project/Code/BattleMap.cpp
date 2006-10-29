#include "BattleMap.h"
// ###########################################################################
// ###########################################################################
//							 System Functions
// ###########################################################################
// ###########################################################################
//========================================================================
//           Default Constructor - Testing Purposes
//========================================================================
JWL_Map::JWL_Map()
{
	strncpy(m_Name,"",100);
	strncpy(m_Tileset,"",100);
	m_Size.x = 0;
	m_Size.y = 0;
	m_totalSize = 0;
	m_OBJcount = 0;
	m_PALsize = 0;
	m_Objects = 0;
	m_ObjPallete = 0;
	m_Grid = 0;
}
//========================================================================
//           Constructor
//========================================================================
JWL_Map::JWL_Map(const char* _filename)
{
	//---------------------
	// Check File Exsists
	//---------------------
	FILE* _FilePtr;
	if((_FilePtr = fopen(_filename, "rS")) == NULL) 
	{
		// 404 : File not Found
		return;
	}

	//---------------------
	// Call Map Loader
	//---------------------
	if(!LoadMap(_FilePtr))
	{
		// SOME ERROR
		return;
	}
}

//========================================================================
//           Deconstructor
//========================================================================
JWL_Map::~JWL_Map()
{
	Delete();
}

//========================================================================
//           Constructor
//========================================================================
void JWL_Map::Delete()
{
	//---------------------
	// BLANKS
	//---------------------
	strncpy(m_Name,"",50);
	strncpy(m_Tileset,"",50);
	m_Size.x = 0;
	m_Size.y = 0;
	m_totalSize = 0;
	m_OBJcount = 0;
	m_PALsize = 0;
	//---------------------
	// Free and Delete
	//---------------------
	if(m_Objects)
	{
		free(m_Objects);
	}

	if(m_ObjPallete)
	{
		for(UINT c = 0; c < m_PALsize; ++c)
			if(m_ObjPallete[c].m_cells)
				free(m_ObjPallete[c].m_cells);
		free(m_ObjPallete);
	}

	if(m_Grid)
	{
		UINT _totalSize = m_totalSize;
		for(UINT c = 0; c < _totalSize ; ++c)
		{
			if(m_Grid[c].m_next == 0)
			{
				// No chain to Delete
			}
			else
			{
				// Travel Links until Link ends
				MAP_CELL* _iter = m_Grid[c].m_next;
				MAP_CELL* _link = 0;
				while(_iter->m_next != 0)
				{
					_link = _iter->m_next;
					delete _iter;
					_iter = _link;
				}
				// Add Link
				delete _iter;
			}
		}
		free(m_Grid);
	}
}
// ###########################################################################
// ###########################################################################
//							 Acessor Functions
// ###########################################################################
// ###########################################################################
//========================================================================
//           Is the Map Empty
//========================================================================
bool JWL_Map::isEmpty() const
{
	return 	((m_Objects == 0) && (m_ObjPallete == 0) && (m_Grid == 0));
}

//========================================================================
//           Get a Copy of Grid
//========================================================================
MAP_CELL* JWL_Map::getGridCopy() const
{
	// IF Empty Toss a Fuck you back
	if(isEmpty())
	{
		return NULL;
	}

	// Allocate Memory
	UINT _totalSize = m_totalSize;
	MAP_CELL* _Grid = (MAP_CELL*)malloc(sizeof(MAP_CELL)*_totalSize);
	memset(_Grid,0,sizeof(MAP_CELL)*_totalSize);

	//Copy Data Over (IGNORE LOGIC CUBES)
	for(UINT c=0; c < _totalSize; ++c)
	{
		_Grid[c].m_height = m_Grid[c].m_height;
		_Grid[c].m_tl = m_Grid[c].m_tl;
	}

    return _Grid;
}
//========================================================================
//           Get Map Refrence
//========================================================================
const MAP_CELL* JWL_Map::getGridPTR() const
{
	return m_Grid;
}
//========================================================================
// Get Map Dimensions
//========================================================================
MAP_CELL JWL_Map::getCellCopy(UINT x, UINT y) const
{
	if(IF_X_Y_OUT_RANGE)
	{
		MAP_CELL _temp = {0,0,0};
		return _temp;
	}
	else
	{
		MAP_CELL _temp = {GRID_XY.m_height,GRID_XY.m_tl,0};
		return _temp;
	}
}
//========================================================================
//	Get Texute ID
//========================================================================
UINT JWL_Map::getTex(UINT x, UINT y) const
{
	if(IF_X_Y_OUT_RANGE)
		return INVALID_PARAM;
	else
		return GRID_XY.m_tl;
}

//========================================================================
// Get Map Height at pos xy
//========================================================================
UINT JWL_Map::getHeight(UINT x, UINT y) const
{
	if(IF_X_Y_OUT_RANGE)
		return 0;
	else
		return GRID_XY.m_height;
}
//========================================================================
// Get Map Height at pos xy
//========================================================================
float JWL_Map::getHeightf(float x, float y) const
{
	if(((UINT)x >= m_Size.x) || ((UINT)y >= m_Size.y))
		return 0;
	else
		return ((m_Grid[((UINT)x)*m_Size.y + ((UINT)y)].m_height) * 0.2f);
}
//========================================================================
// Get Map Texture File
//========================================================================
const char* JWL_Map::getMapTexture() const
{
	return m_Tileset;
}
//========================================================================
// Get Map Dimensions
//========================================================================
MAP_SIZE JWL_Map::getMapSize() const
{
	return m_Size;
}
//========================================================================
//	Get Map Pallete Size
//========================================================================
UINT JWL_Map::getPalleteSize() const
{
	return m_PALsize;
}
//========================================================================
//	Get Object Count
//========================================================================
UINT JWL_Map::getObjectCount() const
{
	return m_OBJcount;
}
//========================================================================
// Get Pallete Object
//========================================================================
MAP_O_PAL JWL_Map::getMapPallete(UINT _index) const
{
	if(_index < m_PALsize)
		return m_ObjPallete[_index];
	{
		MAP_O_PAL _null = {0,0,0,0,0,0,0};
		return _null;
	}
}
//========================================================================
//	Get Map Object
//========================================================================
MAP_OBJ JWL_Map::getMapObject(UINT _index) const
{
	if( _index < m_OBJcount)
		return m_Objects[_index];
	else
	{
		MAP_OBJ _null = {0,0,0,0,0};
		return _null;
	}
}
// ###########################################################################
// ###########################################################################
//							 Map Loading Functions
// ###########################################################################
// ###########################################################################
//========================================================================
// Load Map
//========================================================================
bool JWL_Map::LoadMap(FILE* _FilePTR)
{
	char _line[255] = {0};
	UINT _start = 0;
	UINT _end = 0;
	//---------------------
	// Load Header
	//---------------------
	// Load Map Name []
	FileOFunc::extract(_FilePTR, m_Name);	
	
	// Map Dimensions [50:50]
	FileOFunc::extract(_FilePTR, &(m_Size));
	m_totalSize = m_Size.x * m_Size.y;

	// Tileset []
	FileOFunc::extract(_FilePTR, m_Tileset);

	//---------------------
	// Load Base
	//---------------------
	if(!LoadBase(_FilePTR))
		return false;
	//---------------------
	// Load Pallete
	//---------------------
	if(!LoadPallete(_FilePTR))
		return false;
	//---------------------
	// Load Objects
	//---------------------
	if(!LoadObjects(_FilePTR))
		return false;
	//---------------------
	// Load Game Info
	// TODO:: Implement
	//---------------------

	// EOF
	fclose(_FilePTR);
	return true;
}
//========================================================================
// LOad Map Base
//========================================================================
bool JWL_Map::LoadBase(FILE* _FilePTR)
{
	char _line[255] = {0};	
	//---------------------
	// Allocate Memory
	//---------------------
	m_Grid = (MAP_CELL*)malloc(sizeof(MAP_CELL)*m_totalSize);
	memset(m_Grid,0,sizeof(MAP_CELL)*m_totalSize);

	//---------------------
	// Loop and Load
	// Height Map
	//---------------------
	fgets(_line, 255, _FilePTR); // LINE OF ##############################
	if(fgetc(_FilePTR) == ':')
		fgets(_line, 255, _FilePTR); // Comment
	else
		return false;

	for(UINT y = 0; y < m_Size.y; ++y)
	{
		// Check Start Marker [
		if(fgetc(_FilePTR) != '[')	
			return false;

		char _num[2] = {0};
		for(UINT x = 0; x < m_Size.x; ++x)
		{
            fread(_num,sizeof(char),2,_FilePTR);
			GRID_XY.m_height = atoi(_num);
		}

		// Read rest of line
		fgets(_line, 255, _FilePTR);
	}
	//---------------------
	// Loop and Load
	// Texture Map
	//---------------------
	fgets(_line, 100, _FilePTR); // LINE OF ##############################
	if(fgetc(_FilePTR) == ':')
		fgets(_line, 255, _FilePTR); // Comment
	else
		return false;

	for(UINT y = 0; y < m_Size.y; ++y)
	{
		// Check Start Marker [
		if(fgetc(_FilePTR) != '[')	
			return false;

		char _num[2] = {0};
		for(UINT x = 0; x < m_Size.x; ++x)
		{
            fread(_num,sizeof(char),2,_FilePTR);
			GRID_XY.m_tl = atoi(_num);
		}

		// Read rest of line
		fgets(_line, 255, _FilePTR);
	}	
	return true;
}
//========================================================================
// Load Pallete of Objects
//========================================================================
bool JWL_Map::LoadPallete(FILE* _FilePTR)
{
	char _line[255] = {0};

	fgets(_line, 100, _FilePTR); // LINE OF ##############################

	if(fgetc(_FilePTR) == ':')
		fgets(_line, 255, _FilePTR); // Comment

	// Pallete Size [int]
	FileOFunc::extract(_FilePTR,&(m_PALsize));

	//---------------------
	// Allocate Memory
	//---------------------
	m_ObjPallete = (MAP_O_PAL*)malloc(sizeof(MAP_O_PAL)*m_PALsize);
	memset(m_ObjPallete,0,sizeof(MAP_O_PAL)*m_PALsize);

	//---------------------
	// Loop and Load
	//---------------------
	UINT _start = 0;
	UINT _end = 0;
	for(UINT c = 0; c < m_PALsize; ++c)
	{
		MAP_O_PAL& _palObj = m_ObjPallete[c];
		//------------------
		// Header Comment
		//------------------
		if(fgetc(_FilePTR) == ':')
			fgets(_line, 255, _FilePTR); 
		//--------------
		//   Mesh
		//--------------
		FileOFunc::extract(_FilePTR, _palObj.m_mesh);
		//--------------
		//   Texture
		//--------------
		FileOFunc::extract(_FilePTR, _palObj.m_tex);
		//--------------
		//   Logic
		//--------------
		{
			char _logFilename[100] = {0};
			FileOFunc::extract(_FilePTR, _logFilename);
			//-----------------------------------
			// Load Logic Information From File
			//-----------------------------------
			LoadLogic(_palObj,_logFilename);
		}
	}
	return true;
}
//========================================================================
// Load Logic component of Map Pallete Object FROM FILE
//========================================================================
void JWL_Map::LoadLogic(MAP_O_PAL& _palObj, const char* _filename)
{
	// Add Path to Name
	char* _finalFN = new char[strlen(_filename) + strlen(JWL_FN_LOGIC)];
	strcpy(_finalFN,JWL_FN_LOGIC);
	strcat(_finalFN,_filename);

	FILE* _logFile = fopen(_finalFN,"rS");
	if(_logFile)
	{
		bool _flag = LoadLogic(_palObj,_logFile);
		fclose(_logFile);
	}
	else
	{
		// No Game Info
		_palObj.m_cellCount = 0;
		_palObj.m_cells = 0;
		_palObj.xSize = 0;
		_palObj.ySize = 0;
	}
}
//========================================================================
// Load Logic component of Map Pallete Object
//========================================================================
bool JWL_Map::LoadLogic(MAP_O_PAL& _palObj, FILE* _FilePTR)
{
	char _line[255] = {0};
	fgets(_line, 200, _FilePTR); // ---- LOGIC FILE
	fgets(_line, 200, _FilePTR); // ---- CELL INFORMATION

	//-----------------------
	//	Get Size 
	//-----------------------
	MAP_POS _size = {0};
	FileOFunc::extract(_FilePTR,&_size);

	//-----------------------
	// Allocate Space
	//-----------------------
	UINT _totSize = _size.x*_size.y*_size.z;
	_palObj.m_cells = new MAP_XYZ_LOGIC[_totSize];
	memset(_palObj.m_cells,0,sizeof(MAP_XYZ_LOGIC)*_totSize);
	
	_palObj.m_cellCount = _totSize;
	_palObj.xSize = _size.x;
	_palObj.ySize = _size.y;

	//-------------------------------------
	//		Get Cell Information
	//-------------------------------------
	for(UINT z = 0; z < _size.z; ++z)
	{
		//-------------------------------------
		//		Start Level
		//-------------------------------------
		fgets(_line, 200, _FilePTR);
		char _level[3][200] = {0};

		for(UINT y = 0; y < _size.y; ++y)
		{
			//-------------------------------------
			//	Get last line if there is one
			//-------------------------------------
			if(y > 0)
				strcpy(_level[0],_level[2]);
			else
				fgets(_level[0], 200, _FilePTR);

			fgets(_level[1], 200, _FilePTR);
			fgets(_level[2], 200, _FilePTR);
			for(UINT x = 0; x < _size.x; ++x)
			{
				MAP_XYZ_LOGIC _cell = {x,y,z*5,0};
				if(_level[1][x*2 + 1] == ' ')
				{
					_cell.l = NULL;
				}
				else if((_level[1][x*2 + 1] < 48) || (_level[1][x*2 + 1] > 53))
				{
					_cell.l = LO_BLOCK;
				}
				else
				{
					UINT _logic = LO_FLOOR;
					if(_level[0][x*2 + 1] != ' ')	_logic += LO_WL_N;
					if(_level[1][x*2 + 0] != ' ')	_logic += LO_WL_W;
					if(_level[1][x*2 + 2] != ' ')	_logic += LO_WL_E;
					if(_level[2][x*2 + 1] != ' ')	_logic += LO_WL_S;

					_cell.l = _logic;
					UINT _num = atoi(&(_level[1][x*2 + 1]));
					if(_num < 5)
						_cell.z += 5*z + _num;
					else
						_cell.z += 5*z;
				}
				_palObj.m_cells[x + (y * _size.x) + (z * _size.x * _size.y)] = _cell;
			}
		}
	}

	return true;
}
//========================================================================
// Load Objects
//========================================================================
bool JWL_Map::LoadObjects(FILE* _FilePTR)
{
	char _line[255] = {0};
	fgets(_line, 100, _FilePTR); // LINE OF ##############################
	//------------------
	// Header Comment
	//------------------
	if(fgetc(_FilePTR) == ':')
		fgets(_line, 255, _FilePTR); 
	//------------------
	// List Size [int]
	//------------------
	FileOFunc::extract(_FilePTR,&(m_OBJcount));
	//---------------------
	// Allocate Memory
	//---------------------
	m_Objects = (MAP_OBJ*)malloc(sizeof(MAP_OBJ)*m_OBJcount);
	memset(m_Objects,0,sizeof(MAP_OBJ)*m_OBJcount);
	//---------------------
	// Loop, Load and Build
	//---------------------
	for(UINT c = 0; c < m_OBJcount; ++c)
	{
		//-----------------------
		// Load Data
		// NOTE ON ID
		// It starts at 1 not 0
		// [X:Y:Z:R:ID]
		//------------------------
		FileOFunc::extract(_FilePTR,&(m_Objects[c]));

		//---------------------
		// Update Logic Grid
		//---------------------
		MAP_O_PAL& _palObj = m_ObjPallete[m_Objects[c].id - 1];
		MAP_OBJ& _obj = m_Objects[c];
		UINT _cellCount = _palObj.m_cellCount;
		
		//---------------------------
		// Loop Through Cells
		//---------------------------
		for(UINT d = 0; d < _palObj.m_cellCount; ++d)
		{			
			MAP_XYZ_LOGIC& _cube = _palObj.m_cells[d];
			MAP_POS _altpos = {_cube.x, _cube.y, _cube.z};

			switch(_obj.r)
			{
			case 0: // North Facing
				// Do Nothing
				break;
			case 1: // East  Facing (X= Y) (Y=-X)
				_altpos.x = _cube.y;
				_altpos.y = _palObj.xSize - _cube.x - 1;
				break;
			case 2: // South Facing (X=-X) (Y=-Y)			
				_altpos.y = _palObj.xSize - _cube.x - 1;
				_altpos.x = _palObj.ySize - _cube.y - 1;
				break;
			case 3: // West  Facing (X=-Y) (Y= X)
				_altpos.y = _cube.x;
				_altpos.x = _palObj.ySize - _cube.y - 1;
				break;
			};

			MAP_POS _final = {
				_altpos.x + _obj.x,
				_altpos.y + _obj.y,
				_altpos.z + _obj.z,
			};
			//---------------------------
			//	Add Cell
			//---------------------------
			AddCell(_final,_cube.l);
		}
	}
	return true;
}
// ###########################################################################
// ###########################################################################
//						 Map Movement and Path Functions
//
//		Note all functions are const I just avoid the stupid fucking
//			 const operator like the plauge
// ###########################################################################
// ###########################################################################
//========================================================================
// Add Logic Cube to Map
//========================================================================
void JWL_Map::AddCell(MAP_POS _pos, UINT _logic)
{
	//---------------------
	// Check Validity
	//---------------------
	if(_logic == 0)
		return;

	MAP_CELL& _base = GRID_PXY;

	if((_pos.z+4) < _base.m_height)	
		return;

	//---------------------
	// Setup Cell
	//---------------------
	MAP_CELL* _dest = new MAP_CELL;
	_dest->m_height = _pos.z;
	_dest->m_tl = _logic;
	_dest->m_next = 0;

	//---------------------
	// Find Position
	//---------------------
	if IF_POS_OUT_RANGE
		return;

	MAP_CELL* _curr = _base.m_next;
	MAP_CELL* _prev = 0;
	//---------------------------------
	//   Loop through Pile
	//---------------------------------
	while(_curr)
	{
		//---------------------------------
		//	If Matched then compile logic
		//---------------------------------
		if(_curr->m_height == _pos.z)
		{
			_curr->m_tl |= _logic;
			return;
		}
		//---------------------------------
		// If lower then slide under
		//---------------------------------
		if(_curr->m_height > _pos.z)
		{
			if(_prev)
				_prev->m_next = _dest;
			_dest->m_next = _curr;
			return;
		}
		// Iterate
		_prev = _curr;
		_curr = _curr->m_next;
	}
	//---------------------------------
	// New High Add to tail
	//---------------------------------
	if(_prev)
		_prev->m_next = _dest;
	else
		_base.m_next = _dest;
}
// ###########################################################################
// ###########################################################################
//						 Map Movement and Path Functions
//
//		Note all functions are const I just avoid the stupid fucking
//			 const operator like the plauge, okay so i had to implement it
// ###########################################################################
// ###########################################################################
//========================================================================
// Retrieve Logic Cube From Location
//========================================================================
bool JWL_Map::getLogic(MAP_POS _pos) const
{
	if(IF_POS_OUT_RANGE)
	{
		//-----------------------
		// Return Solid Block
		// IF OUT_OF_RANGE
		// OR 'Underground'
		//-----------------------
		return false;
	}
	return (GRID_PXY.m_next != 0);
}
//========================================================================
// Retrieve Logic Cube From Location
//========================================================================
UINT JWL_Map::getLogic(MAP_POS _pos, UINT _dir) const
{
	if(IF_POS_OUT_RANGE)
	{
		//-----------------------
		// Return Solid Block
		// IF OUT_OF_RANGE
		// OR 'Underground'
		//-----------------------
		return LO_BLOCK;
	}
	//--------------------------
	// Counters Step Effect
	//--------------------------
	UINT _height = GRID_PXY.m_height;
	if(_dir & (MAP_MOVE_UP | MAP_MOVE_DOWN))
	{
		if((_height > (_pos.z)))
			return LO_BLOCK;
	}
	else
	{
		if(_height > (_pos.z + 4))
			return LO_BLOCK;
	}

	if(GRID_PXY.m_next == 0)
		return NULL;
	
	MAP_CELL* _curr = GRID_PXY.m_next;

	// Loop
	while(_curr)
	{
		UINT _height = _curr->m_height;

		if((_pos.z + 5) < _height)
			return NULL;
		else if(_pos.z > (_height + 5))
			_curr = _curr->m_next;
		else 
			return _curr->m_tl;
	}
	return NULL;
}
//========================================================================
// Check a LOGIC number for collisions
// Return true when NO COLLISION occurs
//========================================================================
bool JWL_Map::checkLogic(UINT _logic, UINT _dir) const
{
	return((_logic & _dir) == 0);
}
//========================================================================
// Check a REVERSE LOGIC number for collisions
// The Reverse means we take the DIRECTION and Apply oppisites
// Return true when NO COLLISION occurs
//========================================================================
bool JWL_Map::checkRLogic(UINT _logic, UINT _dir) const
{
	UINT _nDir = 0;
	if(_dir & LO_WL_N)		_nDir |= LO_WL_S;
	if(_dir & LO_WL_E)		_nDir |= LO_WL_W;
	if(_dir & LO_WL_S)		_nDir |= LO_WL_N;
	if(_dir & LO_WL_W)		_nDir |= LO_WL_E;

	return((_logic & _nDir) == 0);
}
//========================================================================
// Can you move from POSITION in a certain DIRECTION
//========================================================================
bool JWL_Map::canMove(MAP_POS _pos, UINT _dir) const
{
	//-------------------------
	// Check Curr Cell
	//-------------------------
	UINT _cLogic = getLogic(_pos, _dir);
	if(_cLogic == 1)
		return false;

	//-------------------------
	// If you want to stay PUT
	//-------------------------
	if(_dir == 0)
		return true;

	//-------------------------
	// Get Dest Cell
	//-------------------------
	MAP_POS _dest = _pos;
	if(_dir & MAP_MOVE_NORTH)	
		++_dest.y;
	if(_dir & MAP_MOVE_SOUTH)	
		--_dest.y;
	if(_dir & MAP_MOVE_WEST)	
		++_dest.x;
	if(_dir & MAP_MOVE_EAST)	
		--_dest.x;	
	if(_dir & MAP_MOVE_UP)		
		_dest.z += 5;
	if(_dir & MAP_MOVE_DOWN)	
		_dest.z -= 5;

	//-------------------------
	// Check Dest Cell
	//-------------------------
	UINT _dLogic = getLogic(_dest, _dir);
	if(_dLogic == 1)
		return false;

	//-------------------------
	// Check Walls Inbetween
	//-------------------------
 	return(checkLogic(_cLogic,_dir) | checkRLogic(_dLogic,_dir));
}
//========================================================================
// Get Direction from position to Position
//========================================================================
UINT JWL_Map::getDir(MAP_POS _pos, MAP_POS _dest) const
{
	MAP_POS _delta = {(_pos.x - _dest.x),(_pos.y - _dest.y),(_pos.z - _dest.z)};

	UINT _dir = 0;

	if(abs((int)_delta.x) == 1)
	{
		if(_delta.x > 0)
			_dir |= MAP_MOVE_WEST;
		else
			_dir |= MAP_MOVE_EAST;
	}

	if(abs((int)_delta.y) == 1)
	{
		if(_delta.y < 0)
			_dir |= MAP_MOVE_SOUTH;
		else
			_dir |= MAP_MOVE_NORTH;
	}

	if(abs((int)_delta.z) > 0)
	{
		if(_delta.z > 0)
			_dir |= MAP_MOVE_DOWN;
		else
			_dir |= MAP_MOVE_UP;
	}
	return _dir;
}
//========================================================================
// Get Direction from position to Position
//========================================================================
bool JWL_Map::canMove(CVector3 _pos, CVector3 _dest) const
{
	MAP_POS _posMP =	{(UINT)_pos.x,	(UINT)_pos.y,	(UINT)(_pos.z * 5)};
	MAP_POS _destMP =	{(UINT)_dest.x, (UINT)_dest.y,	(UINT)(_dest.z * 5)};
	
	return canMove(_posMP,_destMP);
}
//========================================================================
// Can you move from POSITION in a certain DIRECTION
//========================================================================
bool JWL_Map::canMove(MAP_POS _pos, MAP_POS _dest) const
{
	return canMove(_pos,getDir(_pos,_dest));
}

//========================================================================
// Can you move along this PATH
// Returns 0 if true
// Return incomplete path (IE At Crash Point)
//========================================================================
MAP_PATH* JWL_Map::canMove(MAP_PATH* _path) const
{
	//-------------------------
	// Check Path Exsists
	//-------------------------
	if(_path == 0)
		return 0;

	if(_path->m_next == 0)
		return _path;

	//-------------------------
	// Recursively Scan Path
	//-------------------------
	if(canMove(_path->m_pos,_path->m_next->m_pos))
	{
		return canMove(_path->m_next);
	}
	else
	{
		//-------------------------
		// Kill Dead branch
		//-------------------------
		MAP_PATH* _curr = _path->m_next;
		while(_curr)
		{
			MAP_PATH* _old = _curr;
			_curr = _curr->m_next;
			delete _old;			
		}
		_path->m_next = 0;
		return _path;
	}
}