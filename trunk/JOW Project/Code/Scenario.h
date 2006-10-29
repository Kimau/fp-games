#ifndef UBER_SCENARIO_MAP_GAME_THINGY_OF_FLOWERLY_BITS_546
#define UBER_SCENARIO_MAP_GAME_THINGY_OF_FLOWERLY_BITS_546

#include "BattleMap.h"
#include "BaseUnit.h"
#include "Team.h"
#include "LocalCon.h"
#include "Structs.h"

class JWL_Scenario
{
	//=====================================
	//		SCENARIO
	//=====================================
public:
	JWL_Scenario();
	JWL_Scenario(const char* _filename);
	~JWL_Scenario();

	// Accessors
	UINT getGameMode() const;
	UINT getGameRules() const;
	//=====================================
    //		CONTROLLERS
	//=====================================
public:
	const JWL_LocalCon& LCon() const;

	// Mutation
	LRESULT Parse(HWND _hWnd,UINT _uMsg,WPARAM _wParam,LPARAM _lParam);
	void selectPos(UINT x, UINT y);
	void setPath(MAP_PATH* _path);
	JWL_CMD processStack();
	//=====================================
	//		TEAMS
	//=====================================
	// TODO :: IMPLEMENT TEAMS
	//=====================================
	//		UNITS
	//=====================================
	
	// Mutation
	void setUnitGui(UINT _index, UINT _gui);
	// Accessors
	const JWL_Unit& Unit(UINT _index) const;
	UINT getSelUnitID() const;
	UINT getUnitCount() const;	
private:
	bool selectUnit(UINT x, UINT y);
	JWL_Unit* getUnit(MAP_POS _pos);
	//=====================================
	//		MAP
	//=====================================
public:
	const JWL_Map& Map() const;
	bool LoadMap(const char* _filename);
	//=====================================
	//		VARIBLES
	//=====================================
private:
	UINT m_TeamCount;
	UINT m_UnitCount;
	UINT m_GameMode;									// Holds Current Game Mode
	UINT m_GameRules;									// Holds Current Game Rules (Mode Specific)
	char m_name[50];

	MAP_PATH* m_Path;

	JWL_Team m_Teams[_MAX_TEAM_COUNT];					// Holds Team information
	JWL_Unit m_Units[_MAX_UNIT_COUNT];					// Holds Unit Information

	JWL_LocalCon m_LocalCon;							// Local Conntroller handles mouse and keyboard input
	JWL_Map m_Map;										// Holds Map Information

};

#endif