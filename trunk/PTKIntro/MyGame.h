#ifndef HEADER_BORING_MY_GAME_2345547653
#define HEADER_BORING_MY_GAME_2345547653

//------------------------
//	Includes
//------------------------
#include "gObject.h"
#include "gEvents.h"
#include <list>

enum GameState {GS_NULL = 0, GS_SETUP, GS_PLAY, GS_GAMEOVER};
enum GfxEvent {GFX_NULL = 0, GFX_EXPLO, GFX_DEBRIS};

//------------------------
//	Structs
//------------------------
struct gfxEvent
{
	GfxEvent m_type;
	float m_d1;
	float m_d2;
	float m_d3;
	float m_d4;
	float m_d5;
	float m_d6;
	float m_d7;
	float m_d8;
};

//------------------------
//	Defines
//------------------------
#define MG_BLOCK_COUNT 30

typedef std::list<GObject*> GObjects;
typedef std::list<gfxEvent> GFX_Events;

//------------------------
//	A Boring MyGame Class for Podcast Intro
//------------------------
class MyGame
{
public:
	//-----------------------
	//	Starters and Stopers
	//-----------------------
	MyGame();
	~MyGame();
	GameState Setup();
	GameState Update(unsigned long _delta);


	gfxEvent popGFX();
	//-----------------------
	//	Accesors
	//-----------------------
	GameState getState() const;
	int getScore() const;

	GObjects::const_iterator getBlocksBegin() const;
	GObjects::const_iterator getBlocksEnd() const;

private:
	void GenerateBlock(GObject* _block);
	void ProcessEvent(gEvent& _event);

	void Shoot(int x, int y);
	void PostGFX(GfxEvent _type, 
				 float _d1 = 0.0f, float _d2 = 0.0f, float _d3 = 0.0f, float _d4 = 0.0f,
				 float _d5 = 0.0f, float _d6 = 0.0f, float _d7 = 0.0f, float _d8 = 0.0f);
	//-----------------------
	//	Varibles
	//-----------------------
	GameState m_State;
	int m_Score;
	gEventManager m_EventQ;
};

#endif