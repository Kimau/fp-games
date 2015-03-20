#ifndef HEADER_GAME_RENDER_32454543
#define HEADER_GAME_RENDER_32454543

//--------------------------------
//	Includes
//--------------------------------
#include "ptk.h"
#include "ptkImgCollection.h"
#include "ptkSpriteManager.h"
#include "MyGame.h"

//--------------------------------
//	Define
//--------------------------------
#define		PTK_SCREENW		800
#define		PTK_SCREENH		600

//--------------------------------
//	Render the Game using PTK api
//--------------------------------
class ptkGameRender
{
public:
	ptkGameRender();
	~ptkGameRender();

	void Update(unsigned long _delta);
	void Render();
	KWindow* Setup(MyGame* _game);
	void Cleanup();

private:
	KWindow*	m_gameWin;			// PTK Window
	MyGame*		m_GamePtr;
	KGraphic*	m_gameGFX;
	KTrueText*	m_gameTxt;
	pImageCollection m_Images;
	pSpriteManager	 m_Sprites;
};

#endif