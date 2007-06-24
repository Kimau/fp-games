#ifndef HEADER_SPRITE_MANAGER_324324
#define HEADER_SPRITE_MANAGER_324324

//--------------------------------
//	Includes
//--------------------------------
#include "ptk.h"
#include "ptkImgCollection.h"

//--------------------------------
//	Structs
//--------------------------------
struct ptkSprite
{
	char m_name[30];			// Sprite Name
	char m_sheet[30];			// Sheet Filename
	KGraphic* m_refSheet;		// Temp Ref
	bool m_loop;				// Does animation Loop
	unsigned long m_millpframe; // Milliseconds per Frame
	int m_frames;				// Amount of Frames
	int* m_slice;				// Slice of Frame (Frames * 4)
};

struct ptkObject
{
	ptkSprite* m_sprite;		// Sprite Refrence
	unsigned long m_time;		// Time, used to control frames
};

struct ptkParticle
{
	ptkSprite* m_sprite;		// Sprite Refrence
	unsigned long m_time;		// Time, used to control frames
	long m_life;				// Time left before object dies
	float m_pos[2];
	float m_vel[2];
	float m_acl[2];
};

//--------------------------------
//	Sprite Manager
//--------------------------------
class pSpriteManager
{
public:
	//---------------------------
	//	Setup
	//---------------------------
	pSpriteManager();
	~pSpriteManager();
	void Purge();
	void loadSpriteFile(const char* _filename);

	//---------------------------
	//	Update
	//---------------------------
	void drawParticles();
	void drawObject(unsigned int _id, int x, int y);
	void drawSprite(ptkSprite* _sprite, unsigned long _time, int x, int y);
	void update(unsigned long _delta);
	
	//---------------------------
	//	Management
	//---------------------------
	void addParticle(const char* _sName, long _life, 
					 float _px = 0.0f, float _py = 0.0f,
					 float _vx = 0.0f, float _vy = 0.0f,
					 float _ax = 0.0f, float _ay = 0.0f);
	unsigned int addObject(const char* _sName);
	void removeObject(unsigned int _id);

private:
	pImageCollection m_Images;
};

#endif