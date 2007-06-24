#ifndef HEADER_GAME_RENDER_32454543
#define HEADER_GAME_RENDER_32454543

//--------------------------------
//	Includes
//--------------------------------
#include "ptk.h"
#include "ptkImgCollection.h"
#include "gEvents.h"
#include "KISSproto.h"

//--------------------------------
//	Define
//--------------------------------
#define		PTK_SCREENW		800
#define		PTK_SCREENH		600

struct pointI	{int	x; int	 y;};
struct pointF	{float	x; float y;};

struct MenuItem // HACK :: Not A proper System
{
	char m_name[20];
	gEvent m_trigger;
};

//--------------------------------
//	Render the Game using PTK api
//--------------------------------
class ptkGameRender
{
public:
	ptkGameRender();
	~ptkGameRender();

	//------------------------------------
	//	Interface
	//------------------------------------
	void Update(unsigned long _delta);
	void Render();
	KWindow* Setup(KISSproto* _game);
	void Cleanup();

	void MouseClick(int x, int y, bool _button = false);

private:
	void RenderStageMenu();
	void RenderPortrait();
	void RenderStage();
	void RenderTimeline();
	void RenderMouseMap();

	void setMouseClip(unsigned int _id, gEvent _event, int _tx, int _ty, int _bx, int _by, float _depth = 0.0f, bool _button = false);
	void moveMouseClip(unsigned int _id, int _tx, int _ty, int _bx, int _by, float _depth = 0.0f);

	pointF Unproject(int x, int y);
	pointI Project(float x, float y);

	gEventManager m_EventQ;
	KWindow*	m_gameWin;			// PTK Window
	KGraphic*	m_gameGFX;
	KTrueText*	m_gameTxt;
	KISSproto*	m_gamePTR;
	pImageCollection m_Images;
	pointF		m_marker;

	KGraphic*	m_gfxActor;
	KGraphic*	m_gfxBack;
	KGraphic*	m_gfxFront;

	MenuItem	m_menu[8];	// HACK :: Not Proper Menu System
};

#endif