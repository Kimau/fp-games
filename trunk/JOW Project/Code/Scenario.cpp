#include "Scenario.h"

//#######################################################################
//#######################################################################
//							SCENARIO
//#######################################################################
//#######################################################################
//===============================================================
//		Create Blank Scenario - Testing Purposes
//===============================================================
JWL_Scenario::JWL_Scenario()
{
	m_TeamCount		= 0;
	m_UnitCount		= 0;
	m_GameMode		= 0;
	m_GameRules		= 0;
	m_Path			= 0;

	memset(m_name,0,sizeof(m_name));
}
//===============================================================
//		Load Scenario From File
//===============================================================
JWL_Scenario::JWL_Scenario(const char* _filename)
{
	m_TeamCount		= 0;
	m_UnitCount		= 0;
	m_GameMode		= 0;
	m_GameRules		= 0;
	m_Path			= 0;

	memset(m_name,0,sizeof(m_name));

	// Add Path to Name
	char* _finalFN = new char[strlen(_filename) + strlen(JWL_FN_SCENARIOS)];
	strcpy(_finalFN,JWL_FN_SCENARIOS);
	strcat(_finalFN,_filename);	

	FILE* _FilePTR;
	if((_FilePTR = fopen(_finalFN, "rS")) == NULL) 
	{
		// 404 : File not Found
		return;
	}

	//-----------------------------
	//	Create Buffer
	//-----------------------------
	char _line[255] = {0};
	//-----------------------------
	//	Read Info
	//-----------------------------
	FileOFunc::extract(_FilePTR,m_name);

	// Load Map
	FileOFunc::extract(_FilePTR,_line);
	if (!LoadMap(_line))
	{
		// THROW ERROR
		return;
	}

	fgets(_line, 255, _FilePTR);	// BLANK LINE

	FileOFunc::extract(_FilePTR,&(m_GameMode));
	FileOFunc::extract(_FilePTR,&(m_GameRules));
	
	// Load Script 
	FileOFunc::extract(_FilePTR,_line);
	// TODO :: Implement Script Loading

	fgets(_line, 255, _FilePTR);	// BLANK LINE

	FileOFunc::extract(_FilePTR,&(m_TeamCount));

	// Loop Through Teams
	m_UnitCount = 0;
	for(UINT t = 0; t < m_TeamCount; ++t)
	{
		UINT _unitCount = 0;
		FileOFunc::extract(_FilePTR,&(_unitCount));

		// Loop Through Units
		for(UINT u = 0; u < _unitCount; ++u)
		{
			MAP_OBJ _temp = {0};
			FileOFunc::extract(_FilePTR,&(_temp));
			
			//----------------------------
			//	Get Filename for Unit
			//----------------------------
			char _filename[255] = {0};
			
			//-------------------------------
			// Load Unit From File
			//-------------------------------
			switch(_temp.id)
			{
			case 0:
				break;
			case UID_ME_STEAMHEAD:
				strcpy(_filename,UFN_ME_STEAMHEAD);
				break;
			case UID_ME_LUMBERJACK:
				strcpy(_filename,UFN_ME_LUMBERJACK);
				break;
			case UID_ME_SLICENDICE:
				strcpy(_filename,UFN_ME_SLICENDICE);
				break;
			case UID_ME_RATATAT:
				strcpy(_filename,UFN_ME_RATATAT);
				break;
			case UID_ME_STEAMFIEND:
				strcpy(_filename,UFN_ME_STEAMFIEND);
				break;
			case UID_ME_BIGBOOM:
				strcpy(_filename,UFN_ME_BIGBOOM);
				break;
			case UID_ME_COPTER:
				strcpy(_filename,UFN_ME_COPTER);
				break;
			}
			//-------------------------------
			// Set Unit Position and Facing
			//-------------------------------
			m_Units[m_UnitCount].Load(_filename,_temp,&m_Map);
			
			//--------------------------------------
			// Add Unit and change Unit Controller
			//--------------------------------------
			m_Teams[t].addUnit(&(m_Units[m_UnitCount]));
			m_Units[m_UnitCount].m_control = t;

			//-------------------------------
			// Increment Unit Count
			//-------------------------------
			++m_UnitCount;
		}
	}
	fclose(_FilePTR);
}

//===============================================================
//		Deconstructor FREE AND RELEASE
//===============================================================
JWL_Scenario::~JWL_Scenario()
{
}

//===============================================================
//		Get Game Mode
//===============================================================
UINT JWL_Scenario::getGameMode() const
{
	return m_GameMode;
}
//===============================================================
//		Get Game Rules
//===============================================================
UINT JWL_Scenario::getGameRules() const
{
	return m_GameRules;
}

//#######################################################################
//#######################################################################
//						PLAYERS || CONTROLLERS
//#######################################################################
//#######################################################################
//===============================================================
//		Get Const copy of Local Controller
//===============================================================
const JWL_LocalCon& JWL_Scenario::LCon() const
{
	return m_LocalCon;
}
//===============================================================
//		Bridge Function for Parsing
//===============================================================
LRESULT JWL_Scenario::Parse(HWND _hWnd,UINT _uMsg,WPARAM _wParam,LPARAM _lParam)
{
	return m_LocalCon.Parse(_hWnd,_uMsg,_wParam,_lParam);
}
//===============================================================
//		Bridge Function for Selecting Map Position
//===============================================================
void JWL_Scenario::selectPos(UINT x, UINT y)
{
	MAP_POS _temp = {x,y,m_Map.getHeight(x,y)};
	m_LocalCon.selectPos(_temp);
}
//===============================================================
//		Set Path for Unit to Follow
//===============================================================
void JWL_Scenario::setPath(MAP_PATH* _path)
{
	while(m_Path)
	{
		MAP_PATH* _old = m_Path;
		m_Path = m_Path->m_next;
		delete _old;		
	}
	m_Path = _path;
}
//===============================================================
//		Checks the stack for commands and routes them to the
//		Correct location
//===============================================================
JWL_CMD JWL_Scenario::processStack()
{
	JWL_CMD _null = {0};
	while(1)
	{
		//-------------------------------------------
		//	Fetch oldest command
		//	Check if valid
		//-------------------------------------------
		JWL_CMD _cmd = m_LocalCon.popCommand();
		//-----------------------------------------------
		//	NULL Commands
		//-----------------------------------------------
		if(_cmd.m_CmdID == 0)
		{
			// Blank Command
			return _null;
		}
		//-----------------------------------------------
		//	System Commands
		//-----------------------------------------------
		if(_cmd.m_CmdID < 200)
		{
			return _cmd;
		}
		//-----------------------------------------------
		//	Unit Commands
		//-----------------------------------------------
		if((_cmd.m_CmdID / 100) == 2)
		{
			// Refrence Unit
			JWL_Unit& _unit = m_Units[_cmd.m_UnitID];

			//------------------------------------------
			// Check Correct Team is Acting
			//------------------------------------------
			if(_unit.m_control != _cmd.m_TeamID)				continue;

			//------------------------------------------
			//	Get Vars
			//------------------------------------------
			MAP_POS _dest = {_cmd.x,_cmd.y,_cmd.z};
			MAP_POS _pos  = _unit.m_pos;
			UINT _dir = m_Map.getDir(_unit.m_pos, _dest);
			JWL_Unit* _target = getUnit(_dest);

			//------------------------------------------
			//	Switch Command
			//------------------------------------------
			switch(_cmd.m_CmdID)
			{
			case  JC_UNIT_MOVE_N:	
				{
					++(_pos.y);				
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);
					_unit.Move(_pos);	
					break;
				}
				
			case  JC_UNIT_MOVE_NE:		
				{
					++(_pos.y);  
					++(_pos.x);
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);	
					_unit.Move(_pos);
					break;
				}	
			case  JC_UNIT_MOVE_E:			
				{
					++(_pos.x);	
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);		
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_MOVE_SE:		
				{
					--(_pos.y); 
					++(_pos.x);
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);	
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_MOVE_S:	
				{
					--(_pos.y);					
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);
					_unit.Move(_pos);	
					break;
				}
			case  JC_UNIT_MOVE_SW:	
				{
					--(_pos.y); 
					--(_pos.x);
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);		
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_MOVE_W:	
				{
					--(_pos.x);	
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);	
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_MOVE_NW:		
				{
					++(_pos.y);  
					--(_pos.x);	
					_pos.z = m_Map.getHeight(_pos.x,_pos.y);	
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_MOVE_UP:	
				{
					++(_pos.z);				
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_MOVE_DOWN:	
				{
					--(_pos.z);				
					_unit.Move(_pos);
					break;
				}
			case  JC_UNIT_SELECT:
				{// BEGIN MARK

					//------------------------------------
					//	Select Unit
					//------------------------------------
					if(selectUnit(_cmd.x,_cmd.y))
						break;

					//------------------------------------
					//	Plot Path to Point for Movement
					//------------------------------------
					if(m_Path == 0)
					{
						_cmd.m_CmdID = JC_SYS_PLOT_PATH;
						m_LocalCon.pushCommand(_cmd);
						m_LocalCon.addCommand(JC_SYS_PLOT_PATH,0);
						break;
					}
					else
					{
						//------------------------------------
						//	Is Path already Plotted
						//------------------------------------
						MAP_PATH* _last = m_Path;
						while(_last->m_next)
						{
							_last = _last->m_next;
						}

						if((_last->m_pos.x != _cmd.x) || (_last->m_pos.y != _cmd.y))						
						{
							//------------------------------------
							//	Different Path Replot
							//------------------------------------
							_cmd.m_CmdID = JC_SYS_PLOT_PATH;
							m_LocalCon.pushCommand(_cmd);
							break;
						}
						else
						{
							//------------------------------------
							//	Move along Path
							//------------------------------------
							MAP_PATH* _curr = m_Path->m_next;
							while(_curr)
							{
								JWL_CMD _newcmd = {
									_cmd.m_TeamID,_cmd.m_UnitID,JC_UNIT_MOVEXY,
									_curr->m_pos.x,_curr->m_pos.y,_curr->m_pos.z,
									0};

								m_LocalCon.pushCommand(_newcmd);

								_curr = _curr->m_next;
							}
						}
					}
				}// END MARK
				break;

			case  JC_UNIT_MOVEXY:			_unit.Move(_dest);								break;
			case  JC_UNIT_DUCK:				_unit.Stance(1);								break;
			case  JC_UNIT_STAND:			_unit.Stance(0);								break;
			case  JC_UNIT_CLIMB:			_unit.Climb(_dir,true);							break;
			case  JC_UNIT_ATTACK:			_unit.Attack(*_target);							break;
			case  JC_UNIT_SPECIAL_0:		_unit.Special(_unit.m_pos,*_target,0);			break;
			case  JC_UNIT_SPECIAL_1:		_unit.Special(_unit.m_pos,*_target,1);			break;
			case  JC_UNIT_SPECIAL_2:		_unit.Special(_unit.m_pos,*_target,2);			break;
			case  JC_UNIT_SPECIAL_3:		_unit.Special(_unit.m_pos,*_target,3);			break;
			case  JC_UNIT_SPECIAL_4:		_unit.Special(_unit.m_pos,*_target,4);			break;
			case  JC_UNIT_SPECIAL_5:		_unit.Special(_unit.m_pos,*_target,5);			break;
			case  JC_UNIT_SPECIAL_6:		_unit.Special(_unit.m_pos,*_target,6);			break;
			case  JC_UNIT_SPECIAL_7:		_unit.Special(_unit.m_pos,*_target,7);			break;
			case  JC_UNIT_SPECIAL_8:		_unit.Special(_unit.m_pos,*_target,8);			break;
			case  JC_UNIT_SPECIAL_9:		_unit.Special(_unit.m_pos,*_target,9);			break;
			};
			//----------------------------------------
			// Update GUI
			//----------------------------------------
			_cmd.m_CmdID = JC_SYS_UPDATEUNIT;
			return _cmd;

			continue;
		}
	}
	return _null;
}

//#######################################################################
//#######################################################################
//								TEAMS
//#######################################################################
//#######################################################################

//#######################################################################
//#######################################################################
//								UNITS
//#######################################################################
//#######################################################################
//===============================================================
//		Get Unit at Position
//===============================================================
JWL_Unit* JWL_Scenario::getUnit(MAP_POS _pos)
{
	//------------------------------------------
	//	Get Target if one exsists
	//------------------------------------------
	JWL_Unit* _target = 0;
	for(UINT c = 0; c < m_UnitCount; ++c)
	{
		MAP_POS _uPos = m_Units[c].getPosition();
		if((_pos.x == _uPos.x) && (_pos.y == _uPos.y))
		{
			return &(m_Units[c]);
		}
	}
	return 0;
}
//===============================================================
//		Select Unit at Position
//===============================================================
bool JWL_Scenario::selectUnit(UINT x, UINT y)
{
	for(UINT c = 0; c < m_UnitCount; ++c)
	{
		JWL_Unit& _unit = m_Units[c];
		if((_unit.m_pos.x == x) && (_unit.m_pos.y == y))
		{
			if(m_LocalCon.getCurrUnit() != c)
			{
				m_LocalCon.selectUnit(c);
				// HACK :: QUICK TIME UNIT FIX
				_unit.m_time = 50000;
				return true;
			}
		}
	}
	return false;
}
//===============================================================
//		Get Unit Mesh Data
//===============================================================
void JWL_Scenario::setUnitGui(UINT _index, UINT _gui)
{
	m_Units[_index].m_guiID = _gui;
}
//===============================================================
//		Get Const of Unit
//===============================================================
const JWL_Unit& JWL_Scenario::Unit(UINT _index) const
{
	return m_Units[_index];
}
//===============================================================
//		Get Selected Unit ID
//===============================================================
UINT JWL_Scenario::getSelUnitID() const
{
	return m_LocalCon.getCurrUnit();
}
//===============================================================
//		Get Unit Count
//===============================================================
UINT JWL_Scenario::getUnitCount() const
{
	return m_UnitCount;
}
//#######################################################################
//#######################################################################
//								MAP
//#######################################################################
//#######################################################################
//===============================================================
//		Get CONST pointer to map data
//===============================================================
const JWL_Map& JWL_Scenario::Map() const
{
	return m_Map;
}

//===============================================================
//		Load Map From File
//===============================================================
bool JWL_Scenario::LoadMap(const char* _filename)
{
	if(!m_Map.isEmpty())
	{
		m_Map.Delete();
	}

	// Add Path to Name
	char* _finalFN = new char[strlen(_filename) + strlen(JWL_FN_MAPS)];
	strcpy(_finalFN,JWL_FN_MAPS);
	strcat(_finalFN,_filename);

	FILE* _FilePTR;
	if((_FilePTR = fopen(_finalFN, "rS")) == NULL) 
	{
		// FAILED TO OPEN FILE
		return false;
	}

	return m_Map.LoadMap(_FilePTR);
	fclose(_FilePTR);
}
