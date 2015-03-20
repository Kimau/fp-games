#ifndef CONTROLLER_LOCAL_THING_OF_PURPLE_BITS
#define CONTROLLER_LOCAL_THING_OF_PURPLE_BITS

#ifndef PRE_JWL_SCENARIO
	#define PRE_JWL_SCENARIO
	class JWL_Scenario;
#endif

#include "Structs.h"
#include "CmdQueue.h"
#include "FileHandler.h"
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

class JWL_LocalCon
{
public:
	//---------------------------------
	// System Functions
	//---------------------------------
	JWL_LocalCon();
	~JWL_LocalCon();

	void Flush();

	//---------------------------------
	// Important Parse Function
	//---------------------------------
public:
	LRESULT Parse(HWND _hWnd,UINT _uMsg,WPARAM _wParam,LPARAM _lParam);
	//---------------------------------
	// Command Functions
	//---------------------------------
	void pushCommand(JWL_CMD _cmd);
	JWL_CMD popCommand();
	void addCommand(UINT _command, UINT _var);
	void callCommand(UINT _cmdID);
	//---------------------------------
	// Selection Functions
	//---------------------------------
public:
	void selectPos(MAP_POS _pos);
	void selectUnit(UINT _index);
	//---------------------------------
	// Setup Functions
	//---------------------------------
public:
    void setKey(UINT _token, LC_MAPPED_CMD _cmd);
	void setKey(char _token, LC_MAPPED_CMD _cmd);
	//---------------------------------
	// Varibles and Accessors
	//---------------------------------
public:
	UINT getCurrTeam() const;
	UINT getCurrUnit() const;
	UINT getCurrComm() const;
	const MAP_POS& getCursorPos() const;
	UINT getCursorX() const;
	UINT getCursorY() const;
	UINT getCursorZ() const;
	const char* getString() const;

private:
	JWL_Scenario* m_SceneRef;		// Reference to Parent Scene

	LC_MAPPED_CMD m_Mapped[LC_MAX_KEYS+10];

	bool	m_editing;              // is the editor active
	char	m_edString[255];		// pointer to the string containing the typed data
	UINT	m_edOffset;				// cursor position, the index of the NEXT character in the string

	UINT m_cTeam_ID;				// Holds Currently Selected Team ID
	UINT m_cUnit_ID;				// Holds Currently Selected Unit ID
	UINT m_cComm_ID;				// Holds Currently Selected Command ID
	MAP_POS m_CurPos;				// Holds current Position of Map Cursor

	JWL_CmdQ m_cQ;					// Holds Command Que
};

#include "Scenario.h"

#endif