#include "BaseUnit.h"

//###############################################################################
//###############################################################################
//								Initilasation
//###############################################################################
//###############################################################################
//=============================================================
//			Ghost Constructor
//=============================================================
JWL_Unit::JWL_Unit()
{
	//---------------------------
	//	Physical Stats
	//---------------------------
	m_hp		= 0;
	m_bod		= 0;
	m_armour	= 0;
	m_size		= 0;
	m_speed		= 0;
	m_agil		= 0;
	m_dodge		= 0;
	m_cost		= 0;

	//---------------------------
	//	Current Status
	//---------------------------
	m_time		= 0;
	m_morale	= 0;
	m_stance	= 0;
	m_face		= 0;
	memset(&m_pos,0,sizeof(m_pos));
	memset(m_cond,0,sizeof(m_cond));

	//---------------------------
	//	Type Information
	//	Stored in flags
	//---------------------------
	memset(m_name,0,sizeof(m_name));
	mf_type		= 0;
	mf_race		= 0;
	mf_update	= 0;
	mf_move		= 0;
	mf_climb	= 0;
	memset(&mf_melee,0,sizeof(mf_melee));
	memset(&mf_direct,0,sizeof(mf_direct));
	memset(&mf_indirect,0,sizeof(mf_indirect));
	memset(mf_abil,0,sizeof(mf_abil));
	//---------------------------
	//	Team Information
	//---------------------------
	m_control	 = 0;
	//---------------------------
	//	Renderer Information
	//---------------------------
	m_animation	 = 0;
	m_guiID		 = INVALID_PARAM;
	memset(m_mesh,0,sizeof(m_mesh));
	memset(m_skin,0,sizeof(m_skin));
}
//=============================================================
//			Load from File
//=============================================================
void JWL_Unit::Load(const char* _filename, MAP_OBJ _obj, JWL_Map* _mapRef)
{
	// Add Path to Name
	char* _finalFN = new char[strlen(_filename) + strlen(JWL_FN_UNITS)];
	strcpy(_finalFN,JWL_FN_UNITS);
	strcat(_finalFN,_filename);
	//-----------------------------
	// Check Filename
	// Load File
	//-----------------------------
	FILE* _FilePTR;
	if((_FilePTR = fopen(_finalFN, "rS")) == NULL) 
	{
		// 404 : File not Found
		JWL_Unit();		return;
	}

	//-----------------------------
	//	Create Buffer
	//-----------------------------
	char _line[255] = {0};
	//-----------------------------
	//	Read Info
	//-----------------------------
	FileOFunc::extract(_FilePTR,m_name);
	FileOFunc::extract(_FilePTR,m_mesh);
	FileOFunc::extract(_FilePTR,m_skin);

	fgets(_line, 255, _FilePTR);	// BLANK LINE

	UNIT_CON _temp = {0};
	// Health, Armour, Size		[0,0,0]
	FileOFunc::extract(_FilePTR,&_temp);
	m_bod		= _temp.m_type;		m_hp = m_bod;
	m_armour	= _temp.m_time;
	m_size		= _temp.m_amount;
	// Speed,	Agil,	Dodge	[0,0,0]
	FileOFunc::extract(_FilePTR,&_temp);
	m_speed		= _temp.m_type;
	m_agil		= _temp.m_time;
	m_dodge		= _temp.m_amount;
	// Code,	Race,	Cost	[0,0,0]
	FileOFunc::extract(_FilePTR,&_temp);
	mf_type		= _temp.m_type;
	mf_race		= _temp.m_time;
	m_cost		= _temp.m_amount;
	// Move, Climb, Passive		[0,0,0]
	FileOFunc::extract(_FilePTR,&_temp);
	mf_move		= _temp.m_type;
	mf_climb	= _temp.m_time;
	mf_update	= _temp.m_amount;

	fgets(_line, 255, _FilePTR);	// BLANK LINE

	FileOFunc::extract(_FilePTR,&mf_melee);
	FileOFunc::extract(_FilePTR,&mf_direct);
	FileOFunc::extract(_FilePTR,&mf_indirect);

	for(UINT c = 0; c < 10; ++c)
		FileOFunc::extract(_FilePTR,&(mf_abil[c]));

	//---------------------------
	//	Blank Init
	//---------------------------
	m_pos.x = _obj.x;
	m_pos.y = _obj.y;
	m_pos.z = _obj.z;
	m_face  = _obj.r;

	m_time		= 0;
	m_morale	= 0;
	m_stance	= 0;
	m_animation	= 0;
	m_control	= INVALID_PARAM;
	m_guiID		= INVALID_PARAM;
	memset(m_cond,0,sizeof(m_cond));
	fclose(_FilePTR);

	m_mapRef = _mapRef;
}

//=============================================================
//			Deconstructor
//=============================================================
JWL_Unit::~JWL_Unit()
{
}

//###############################################################################
//###############################################################################
//			Accessors
//###############################################################################
//###############################################################################
//=============================================================
//		Get Unit Stance
//=============================================================
UINT JWL_Unit::getStance() const
{
	return m_stance;
}
//=============================================================
//		Get Unit Facing
//=============================================================
UINT JWL_Unit::getFacing() const
{
	return m_face;
}
//=============================================================
//		Get Unit Position
//=============================================================
MAP_POS JWL_Unit::getPosition() const
{
	return m_pos;
}
//=============================================================
//		Get Unit Animation Frame
//=============================================================
UINT JWL_Unit::getAnimation() const
{
	return m_animation;
}
//=============================================================
//		Get Unit Condition
//=============================================================
UNIT_CON JWL_Unit::getCondition(UINT _id) const
{
	return m_cond[_id];
}
//=============================================================
//		Get Unit Cost
//=============================================================
UINT JWL_Unit::getCost() const
{
	return m_cost;
}
//=============================================================
//		Get Unit Controller
//=============================================================
UINT JWL_Unit::getController()const
{
	return m_control;
}
//=============================================================
//		Get Unit Race
//=============================================================
UINT JWL_Unit::getRace() const
{
	return mf_race;
}
//=============================================================
//		Get GUI ID
//=============================================================
UINT JWL_Unit::getGui() const
{
	return m_guiID;
}
//=============================================================
//		Get Unit Name
//=============================================================
const char* JWL_Unit::getName() const
{
	return m_name;
}
//=============================================================
//		Get Unit Race
//=============================================================
const char* JWL_Unit::getMesh() const
{
	return m_mesh;
}
//=============================================================
//		Get Unit Race
//=============================================================
const char* JWL_Unit::getTex() const
{
	return m_skin;
}
//###############################################################################
//###############################################################################
//			Update Functions (ie Passive)
//###############################################################################
//###############################################################################
//=============================================================
//		Update Unit and Passive Mods Functions
//=============================================================
void JWL_Unit::Update(UINT _phase)
{
}
//###############################################################################
//###############################################################################
//			Movement Functions
//###############################################################################
//###############################################################################
//=============================================================
//		Stance Change Functions
//=============================================================
void JWL_Unit::Stance(UINT _stance)
{
	m_stance = _stance;
	// Update Unit
}

//=============================================================
//		Face in a Certain Direction
//=============================================================
void JWL_Unit::Face(UINT _dir)
{
	if(m_time > 1)
	{
		//---------------------------------
		// Remove Time Unit
		//---------------------------------
		m_time -= 1;
		//---------------------------------
		// Remove Invalid Direction Info
		//---------------------------------
		_dir &= MAP_MOVE_XY;

		//---------------------------------
		// Calulate Direction (HARD WAY)
		//---------------------------------
		switch(_dir)
		{
		case (MAP_MOVE_NORTH				 ):		m_face = 0;		break;
		case (MAP_MOVE_NORTH + MAP_MOVE_EAST ):		m_face = 1;		break;
		case				  (MAP_MOVE_EAST ):		m_face = 2;		break;
		case (MAP_MOVE_SOUTH + MAP_MOVE_EAST ):		m_face = 3;		break;
		case (MAP_MOVE_SOUTH				 ):		m_face = 4;		break;
		case (MAP_MOVE_SOUTH + MAP_MOVE_WEST ):		m_face = 5;		break;
		case				  (MAP_MOVE_WEST ):		m_face = 6;		break;
		case (MAP_MOVE_NORTH + MAP_MOVE_WEST ):		m_face = 7;		break;
		};
		Update(U_PHASE_MID);
	}
}
//=============================================================
//		Move in a specific Direction
//=============================================================
void JWL_Unit::Move(MAP_POS _dest)
{
	//------------------------------------------
	// Move Unit
	//------------------------------------------
	switch(mf_move)
	{
	case 0: // BASIC WALK
		Move_BasicWalk(_dest);
		break;
	case 1:	// BASIC FLY
		return;
	case 2: // BASIC HOVER
		return;
	};
}
//=============================================================
//		Move :: Basic Walk Function
//=============================================================
void JWL_Unit::Move_BasicWalk(MAP_POS _dest)
{
	//------------------------------------------
	// Assume checks NO Checks have been made
	//------------------------------------------
	if(
		(abs((int)m_pos.y - (int)_dest.y) > 1) || 
		(abs((int)m_pos.x - (int)_dest.x) > 1))
		return;
	//------------------------------------------
	// Get Movement Direction
	//------------------------------------------
	UINT _dir = 0;

	if(m_pos.y > _dest.y) 
		_dir |= MAP_MOVE_NORTH;
	else if(m_pos.y != _dest.y) 	
		_dir |= MAP_MOVE_SOUTH;

	if(m_pos.x < _dest.x) 
		_dir |= MAP_MOVE_EAST;
	else if(m_pos.x != _dest.x) 		
		_dir |= MAP_MOVE_WEST;

	//------------------------------------------
	// Check if Unit has to "Climb"
	//------------------------------------------
	if(_dest.z > (m_pos.z + 4))
	{
		Climb(_dir,true);
		return;
	}
	//------------------------------------------
	// Check Movement
	//------------------------------------------
	if(m_mapRef->canMove(m_pos,_dir) == 0)
		return;
	//------------------------------------------
	// Check Time Units
	//------------------------------------------
	if(m_time > m_speed)
		m_time -= m_speed;		
	else
		return;
	//------------------------------------------
	// Check Falling Damage
	//------------------------------------------
	if(_dest.z < (m_pos.z - 8))
	{
		m_hp -= (m_pos.z - _dest.z - 8)*5;
	}
	//------------------------------------------
	// Update Pos and Facing
	//------------------------------------------
	Stance(0);
	m_pos = _dest;
	Face(_dir);	

	//------------------------------------------
	//	Update Animation
	//------------------------------------------
	m_animation = 1;
	//------------------------------------------
	//	Update
	//------------------------------------------
	Update(U_PHASE_MID);
}
//=============================================================
//		Scale Wall
//=============================================================
void JWL_Unit::Climb(UINT _dir, bool _up)
{
	if(m_agil == 0)
		return;
	//------------------------------------------
	// Assume checks NO Checks have been made
	// Get Dest Cell First
	//------------------------------------------
	MAP_POS _dest = m_pos;

	switch(_dir)
	{
	case MAP_MOVE_NORTH:	++_dest.y;		break;
	case MAP_MOVE_EAST :	++_dest.x;		break;
	case MAP_MOVE_SOUTH:	--_dest.y;		break;
	case MAP_MOVE_WEST :	--_dest.x;		break;
	default:								return;
	};
	_dest.z = m_mapRef->getHeight(_dest.x,_dest.y);

	//------------------------------------------
	// Check there is a cliff to climb
	//	UP or DOWN
	// Climbing EQ.
	// time = move_speed * (height / climb)
	//------------------------------------------
	int _height = (int)_dest.z - (int)m_pos.z;
	if((_up) && (_height > 4))
	{
		// Enough Time to scale
		UINT _rTime = m_speed * (_height / m_agil);
		if(_rTime < m_time)
		{
			m_time -= _rTime;
			//------------------------------------------
			// Update Pos and Facing
			//------------------------------------------
			m_pos = _dest;
			Stance(0);
			Face(_dir);	

			//------------------------------------------
			//	Update Animation
			//------------------------------------------
			m_animation = 1;
			//------------------------------------------
			//	Update
			//------------------------------------------
			Update(U_PHASE_MID);
		}
	}
	else if((_up) && (_height < 4))
	{
		_height = 0 -_height;
		// Enough Time to scale
		UINT _rTime = m_speed * (_height / (m_agil+1));
		if(_rTime < m_time)
		{
			m_time -= _rTime;
			//------------------------------------------
			// Update Pos and Facing
			//------------------------------------------
			m_pos = _dest;
			Stance(0);
			Face(_dir);	

			//------------------------------------------
			//	Update Animation
			//------------------------------------------
			m_animation = 1;
			//------------------------------------------
			//	Update
			//------------------------------------------
			Update(U_PHASE_MID);
		}
	}
}
//###############################################################################
//###############################################################################
//			Offensive Functions
//###############################################################################
//###############################################################################
//=============================================================
//		Attack Specific Unit
//=============================================================
void JWL_Unit::Attack(JWL_Unit& _target)
{}
//=============================================================
//		Fire Directly or Indirectly at Position
//=============================================================
void JWL_Unit::Fire(MAP_POS _pos)
{}
//=============================================================
//		Charge in a straight line then bash or melee attack
//=============================================================
void JWL_Unit::Charge(UINT _dir)
{}
//###############################################################################
//###############################################################################
//			Special Functions
//###############################################################################
//###############################################################################
//=============================================================
//		Special Functions
//=============================================================
void JWL_Unit::Special(MAP_POS _pos, JWL_Unit& _target, UINT _id)
{}
