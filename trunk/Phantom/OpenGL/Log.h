#ifndef COOL_LOG_THAT_SAVES_ME_TROUBLE_342
#define COOL_LOG_THAT_SAVES_ME_TROUBLE_342

//--------------------------------------
//	Header Includes
//--------------------------------------
#include "BaseGL.h"
#include "DrawRasFont.h"
#include "..\Math\CVector3.h"
#include "..\Math\CPlane.h"
#include <string.h>
#include <vector>

#pragma warning(disable : 4996) // Disables silly warning about win "security"

using namespace std;

//--------------------------------------
//	Message Struct
//--------------------------------------
struct Lmsg
{
	char m_msg[50];
	int	 m_flag;
};
//--------------------------------------
//	Defines
//--------------------------------------
#define LG_SYSTEM_REPORT	1
#define LG_STATE_REPORT		2
#define LG_ACTION_REPORT	3

//--------------------------------------
//	Basic OpenGL Logger
//--------------------------------------
class BGLLog
{
private:
	BGLLog(const BGLLog& _ref);
	BGLLog& operator=(const BGLLog& _ref);
public:
	//----------------------------------------
	//	Construction
	//----------------------------------------
	BGLLog();
	~BGLLog();
	void Load(WinGLengine* _engine, char* _font = "Arial", int _cSize = 15, int _lines = 5, float x = 0, float y = 0);
	//----------------------------------------
	//	Functions
	//----------------------------------------
	void RenderFlat();
	bool LoadFile(char* _filename);
	bool SaveFile(char* _filename);
	bool Print(int _flag, char* _message);
	bool PrintS(int _flag, const char* _format, ...);
	//----------------------------------------
	//	Set Functions
	//----------------------------------------
	void setFont(char* _font = "Arial", int _size = 10);
	void setLine(int _lines);
	void setPos(float x, float y);
	//----------------------------------------
	//	Accesors
	//----------------------------------------
	const Lmsg* getLine(int _id) const;
	int getLineCount() const;
	float getX() const;
	float getY() const;

	//----------------------------------------
	//	Varibles
	//----------------------------------------
private:
	int m_lines;
	float m_pos[2];
	BGLRasterFont m_font;
	vector<Lmsg> m_data;
	WinGLengine* m_engine;
};

#endif