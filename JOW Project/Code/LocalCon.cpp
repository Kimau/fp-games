#include "LocalCon.h"
//####################################################################################################
//####################################################################################################
//									System Functions
//####################################################################################################
//####################################################################################################
//===================================================================
//		Constructor
//===================================================================
JWL_LocalCon::JWL_LocalCon()
{
	m_SceneRef	= 0;
	m_edOffset	= 0;

	m_editing	= false;

	m_cTeam_ID	= 0;
	m_cUnit_ID	= 0;
	m_cComm_ID	= 0;
	m_CurPos.x	= 0;
	m_CurPos.y	= 0;
	m_CurPos.z	= 0;

	strncpy(m_edString,"",255);	
	memset(m_Mapped,0,sizeof(m_Mapped));

	//---------------------------------------
	//	Load Keys From File
	//---------------------------------------
	FILE* _filePtr = fopen(JWL_FN_KEYBOARD,"rS");

	//--------------------------
	// Check for File
	//--------------------------
	if(_filePtr == 0)
	{
		// Generate Error
		return;
	}

	//--------------------------
	// Setup Buffer
	//--------------------------
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_BACK]));			// BACKSPACE key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_TAB]));			// TAB key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_RETURN]));		// ENTER key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_SHIFT]));			// SHIFT key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_CONTROL]));		// CTRL key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_MENU]));			// ALT key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_PAUSE]));			// PAUSE key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_CAPITAL]));		// CAPS LOCK key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_ESCAPE]));		// ESC key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_SPACE]));			// SPACEBAR 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_PRIOR]));			// PAGE UP key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NEXT]));			// PAGE DOWN key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_END]));			// END key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_HOME]));			// HOME key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_LEFT]));			// LEFT ARROW key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_UP]));			// UP ARROW key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_RIGHT]));			// RIGHT ARROW key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_DOWN]));			// DOWN ARROW key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_SELECT]));		// SELECT key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_PRINT]));			// PRINT key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_EXECUTE]));		// EXECUTE key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_SNAPSHOT]));		// PRINT SCREEN key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_INSERT]));		// INS key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_DELETE]));		// DEL key
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'0']));		// 0 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'1']));		// 1 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'2']));		// 2 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'3']));		// 3 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'4']));		// 4 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'5']));		// 5 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'6']));		// 6 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'7']));		// 7 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'8']));		// 8 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'9']));		// 9 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'A']));		// A key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'B']));		// B key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'C']));		// C key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'D']));		// D key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'E']));		// E key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'F']));		// F key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'G']));		// G key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'H']));		// H key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'I']));		// I key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'J']));		// J key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'K']));		// K key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'L']));		// L key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'M']));		// M key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'N']));		// N key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'O']));		// O key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'P']));		// P key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'Q']));		// Q key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'R']));		// R key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'S']));		// S key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'T']));		// T key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'U']));		// U key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'V']));		// V key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'W']));		// W key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'X']));		// X key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'Y']));		// Y key 
	FileOFunc::extract(_filePtr,&(m_Mapped[(UINT)'Z']));		// Z key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD0]));		// Numeric keypad 0 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD1]));		// Numeric keypad 1 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD2]));		// Numeric keypad 2 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD3]));		// Numeric keypad 3 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD4]));		// Numeric keypad 4 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD5]));		// Numeric keypad 5 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD6]));		// Numeric keypad 6 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD7]));		// Numeric keypad 7 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD8]));		// Numeric keypad 8 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_NUMPAD9]));		// Numeric keypad 9 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_MULTIPLY]));		// Multiply key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_ADD]));			// Add key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_SEPARATOR ]));	// Separator key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_SUBTRACT ]));		// Subtract key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_DECIMAL ]));		// Decimal key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_DIVIDE ]));		// Divide key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F1 ]));			// F1 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F2 ]));			// F2 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F3 ]));			// F3 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F4 ]));			// F4 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F5 ]));			// F5 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F6 ]));			// F6 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F7 ]));			// F7 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F8 ]));			// F8 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F9 ]));			// F9 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F10 ]));			// F10 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F11 ]));			// F11 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_F12 ]));			// F12 key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_LSHIFT ]));		// Left SHIFT key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_RSHIFT ]));		// Right SHIFT key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_LCONTROL ]));		// Left CONTROL key 
	FileOFunc::extract(_filePtr,&(m_Mapped[VK_RCONTROL ]));		// Right CONTROL key

	fclose(_filePtr);
}
//===================================================================
//		Deconstructor
//===================================================================
JWL_LocalCon::~JWL_LocalCon()
{
	Flush();
}
//===================================================================
//		Flush all the Data Down the Tube
//===================================================================
void JWL_LocalCon::Flush()
{
}

//####################################################################################################
//####################################################################################################
//					The All Important Parse Imput Function
//####################################################################################################
//####################################################################################################
//===================================================================
//		Parse Input
//===================================================================
LRESULT JWL_LocalCon::Parse(HWND _hWnd,UINT _uMsg,WPARAM _wParam,LPARAM _lParam)
{
	{
		/*================================================
		* If editor is active
		* .:. Check for editor Active
		*================================================*/
		if(m_editing)
		{
			if(_uMsg == WM_CHAR)
			{
				// ENTER = editor is complete, send string through.
				if(_wParam == VK_RETURN)
				{
					m_editing = false;
					addCommand(JC_SYS_STRING, m_edOffset);
					return NULL;
				}
				// BACKSPACE = remove last character
				if((_wParam == VK_BACK) && (m_edOffset != 0) )
				{
					/*
					*  Notice use of PRE increment means
					*  code is equivilient to
					*  m_edOffset -= 1;
					*  m_edString[m_edOffset] = _wParam;
					*/
					m_edString[--m_edOffset] = NULL;
					return NULL;
				}
				// IF offset is less than length then return 0
				if(m_edOffset < 255)
				{
					/*
					*  Notice use of POST increment means
					*  code is equivilient to
					*  m_edString[m_edOffset] = _wParam;
					*  m_edOffset += 1;
					*/
					m_edString[m_edOffset++] = (char)_wParam;
				}
				return NULL;
			}
		}
		/*================================================
		* If editor is NOT active
		* .:. Check for key input and Process
		*================================================*/
		else
		{
			switch (_uMsg)   
			{
			case WM_CHAR:
					switch(_wParam)
					{
					case VK_RETURN:
						{
							m_editing = true;
							m_edOffset = 0;
							memset(m_edString,0,sizeof(m_edString)); 
						}
						return NULL;
					};
			case WM_KEYUP:
				{
					switch(_wParam)
					{
					case VK_RETURN:
						return NULL;
					default:
						{
							callCommand((UINT)_wParam);
							addCommand(m_cComm_ID,LC_KEY_CALL);
							return NULL;
						}
					};
				}
			};
		}
		/*================================================
		* Check for mouse input
		*================================================*/
		switch (_uMsg)									// Check For Windows Messages
		{
		case WM_LBUTTONUP:
			{
				addCommand(JC_UNIT_SELECT, 0);
			}
			break;
		case WM_RBUTTONUP:
			{
				if(m_cComm_ID)
				{
					addCommand(m_cComm_ID,0);
					m_cComm_ID = 0;
				}
				else
				{
					addCommand(JC_UNIT_MOVEXY,0);
				}
			}
			break;
		
			return NULL;
		};
  }
  return DefWindowProc(_hWnd,_uMsg,_wParam,_lParam);
}
//####################################################################################################
//####################################################################################################
//					Command Stack
//####################################################################################################
//####################################################################################################
//===================================================================
//		Pop Command Off Stack
//===================================================================
void JWL_LocalCon::pushCommand(JWL_CMD _cmd)
{
	m_cQ.addCMD(_cmd);
}
//===================================================================
//		Pop Command Off Stack
//===================================================================
JWL_CMD JWL_LocalCon::popCommand()
{
	return m_cQ.popCMD();
}
//===================================================================
//		Call Command and place onto Stack
//===================================================================
void JWL_LocalCon::callCommand(UINT _cmdID)
{
	//---------------------------------------
	//		Does Command Exists
	//---------------------------------------
	if(m_Mapped[_cmdID].m_CmdID == 0)
		return;

	m_cComm_ID = m_Mapped[_cmdID].m_CmdID;
}

//===================================================================
//		Add Command Onto Stack
//===================================================================
void JWL_LocalCon::addCommand(UINT _command, UINT _var)
{
	if(_var == LC_KEY_CALL)
	{
		// CHECK IF COMMAND SHOULD BE IMMEDIATLY EXECUTED OR QUED
	}
	m_cQ.addCMD(m_cTeam_ID, m_cUnit_ID, _command, m_CurPos.x, m_CurPos.y, m_CurPos.z, _var);
}
//####################################################################################################
//####################################################################################################
//					Selection Functions
//####################################################################################################
//####################################################################################################
//===================================================================
//		Select Map Position
//===================================================================
void JWL_LocalCon::selectPos(MAP_POS _pos)
{
	m_CurPos = _pos;
}
//===================================================================
//		Select Map Position
//===================================================================
void JWL_LocalCon::selectUnit(UINT _index)
{
	m_cUnit_ID = _index;
}
//####################################################################################################
//####################################################################################################
//					Setup Key Functions
//####################################################################################################
//####################################################################################################
//===================================================================
//		Set Key
//===================================================================
void JWL_LocalCon::setKey(UINT _token, LC_MAPPED_CMD _cmd)
{
	m_Mapped[_token] =  _cmd;
}
//===================================================================
//		Set Key
//===================================================================
void JWL_LocalCon::setKey(char _token, LC_MAPPED_CMD _cmd)
{
	// Please note raw conversion, BEWARE
	m_Mapped[_token] =  _cmd;
}
//####################################################################################################
//####################################################################################################
//					Accessor Functions
//####################################################################################################
//####################################################################################################
//===============================================================
//		Get Current Team ID
//===============================================================
UINT JWL_LocalCon::getCurrTeam() const
{
	return m_cTeam_ID;
}
//===============================================================
//		Get Currently Selected Unit's ID
//===============================================================
UINT JWL_LocalCon::getCurrUnit() const
{
	return m_cUnit_ID;
}
//===============================================================
//		Get Currently Selected Command ID
//===============================================================
UINT JWL_LocalCon::getCurrComm() const
{
	return m_cComm_ID;
}

//===============================================================
//		Get Current Cursor Position
//===============================================================
const MAP_POS& JWL_LocalCon::getCursorPos() const
{
	return m_CurPos;
}

//===============================================================
//		Get Current Cursor Position X
//===============================================================
UINT JWL_LocalCon::getCursorX() const
{
	return m_CurPos.x;
}
//===============================================================
//		Get Current Cursor Position Y
//===============================================================
UINT JWL_LocalCon::getCursorY() const
{
	return m_CurPos.y;
}
//===============================================================
//		Get Current Cursor Position Z
//===============================================================
UINT JWL_LocalCon::getCursorZ() const
{
	return m_CurPos.z;
}
//===============================================================
//		Get String
//===============================================================
const char* JWL_LocalCon::getString() const
{
	if(m_editing)
		return 0;
	else
		return m_edString;
}
