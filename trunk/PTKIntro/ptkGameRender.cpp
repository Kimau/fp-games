#include "ptkGameRender.h"

//--------------------------------
//	Define
//--------------------------------
#define		PTK_TITLE		"Flammable Penguins: Intro to PTK"

//========================================================================
//	Constructor
//========================================================================
ptkGameRender::ptkGameRender()
{
	m_gameWin = 0;
	m_GamePtr = 0;
	m_gameGFX = 0;
	m_gameTxt = 0;
}
//========================================================================
//	Deconstructor
//========================================================================
ptkGameRender::~ptkGameRender()
{
	Cleanup();
}
//========================================================================
//	Setup Game API
//========================================================================
KWindow* ptkGameRender::Setup(MyGame* _game)
{
	m_GamePtr = _game;
	//-----------------------------------
	//	Create Window
	//-----------------------------------
	m_gameWin = KPTK::createKWindow(K_OPENGL);										// Init Window
	m_gameWin->createGameWindow(PTK_SCREENW,PTK_SCREENH,32,true,PTK_TITLE);			// Create Window

	m_Sprites.loadSpriteFile("sprites.spr");
	m_gameGFX = m_Images["ship.png"];

	//-----------------------------------
	//	Create Text
	//-----------------------------------
	m_gameTxt = new KTrueText("5by5.ttf");
	m_gameTxt->loadFontFile("5by5.ttf");
	m_gameTxt->setHeightPix(60);
	m_gameTxt->setColor(0.0f,0.0f,1.0f,1.0);

	m_Sprites.addObject("Spaceship");
	m_Sprites.addObject("Bar");

	return m_gameWin;
}
//========================================================================
//	Clean Up
//========================================================================
void ptkGameRender::Cleanup()
{
	m_Images.Purge();
	m_Sprites.Purge();
}

//========================================================================
//	Update Render
//========================================================================
void ptkGameRender::Update(unsigned long _delta)
{
	m_Sprites.update(_delta);

	gfxEvent _event = m_GamePtr->popGFX();
	switch(_event.m_type)
	{
	case GFX_NULL:
		break;
	case GFX_EXPLO:
		m_Sprites.addParticle("Explosion", 500, _event.m_d1, float(PTK_SCREENH) - _event.m_d2);
		break;
	case GFX_DEBRIS:
		m_Sprites.addParticle("Debri", 500, _event.m_d1, float(PTK_SCREENH) - _event.m_d2, _event.m_d3 - 1.0f, (0 - _event.m_d4)/2, 0.0f, 0.1f);
		m_Sprites.addParticle("Debri", 500, _event.m_d1, float(PTK_SCREENH) - _event.m_d2, _event.m_d3 - 0.4f, 0 - _event.m_d4, 0.0f, 0.1f);
		m_Sprites.addParticle("Debri", 500, _event.m_d1, float(PTK_SCREENH) - _event.m_d2, _event.m_d3 + 0.4f, 0 - _event.m_d4, 0.0f, 0.1f);
		m_Sprites.addParticle("Debri", 500, _event.m_d1, float(PTK_SCREENH) - _event.m_d2, _event.m_d3 + 1.0f, (0 - _event.m_d4)/2, 0.0f, 0.1f);
		break;
	};
}

//========================================================================
//	Render Game
//========================================================================
void ptkGameRender::Render()
{
	//-------------------------
	//	Clean
	//-------------------------
	m_gameWin->setDefaultWorldView();
	m_gameWin->setClearColor(0,0,0,1);

	//-------------------------
	//	Draw Blank
	//-------------------------
	m_gameGFX->drawRect(0,0,PTK_SCREENW,PTK_SCREENH, 0.0f, 0.0f, 0.0f, 1.0f);

	//-------------------------
	//	Draw Blocks
	//-------------------------
	GObjects::const_iterator _last = m_GamePtr->getBlocksEnd();
	for(GObjects::const_iterator _iter = m_GamePtr->getBlocksBegin(); _iter != _last; ++_iter)
	{
		m_Sprites.drawObject(0,int((*_iter)->m_pos[0]),int(PTK_SCREENH - (*_iter)->m_pos[1]));
		/*
		m_gameGFX->blitAlphaRect(
			0,0, 32,32, 
			short((*_iter)->m_pos[0] - 16), 
			short(PTK_SCREENH - (*_iter)->m_pos[1] - 16));*/
	}

	m_Sprites.drawObject(1,400,PTK_SCREENH - 20);

	//-------------------------
	//	Draw Particles
	//-------------------------
	m_Sprites.drawParticles();

	//-------------------------
	// Draw Score
	//-------------------------
	int _score = m_GamePtr->getScore();
	char _buffer[20];
	sprintf(_buffer,"%i", _score);
	m_gameTxt->drawStringCentered(_buffer,0,800,500);

	//-------------------------
	// Buffer Flip
	//-------------------------
	m_gameWin->flipBackBuffer();
}