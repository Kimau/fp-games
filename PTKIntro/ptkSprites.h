#ifndef HEADER_PTK_SPRITES_45435
#define HEADER_PTK_SPRITES_45435

//----------------------------------
//	Includes
//----------------------------------
#include "ptk.h"
#include "ptkImgCollection.h"

//----------------------------------
//	Structures
//----------------------------------
struct pAnimCell
{
	int m_slice[4];
	unsigned long m_time;
	pAnimCell* m_next; // Null Terminated Array
};

struct pSpriteFrame
{
	KGraphic* m_ref;
	char m_file[50];
	pAnimCell* m_first;			// First Anim	
};

struct pSprite
{
	int m_pos[2];				// Current Position
	pAnimCell* m_curr;			// Current Marker
	unsigned long m_remain;		// Remaining time on Current Cell
};

//------------------------------
//	PTK Sprite Manager
//------------------------------
class ptkSpriteManager
{
public:
	//------------------------------
	//	Constructor
	//------------------------------
	ptkSpriteManager();
	~ptkSpriteManager();

	//------------------------------
	//	Sprite Creation
	//------------------------------
	void start(const char* _filename);
	void addCell(int _x1, int _y1, int _x2, int _y2, unsigned long _time);
	void end();

	//------------------------------
	//	
	//------------------------------
	//------------------------------
	//	Update
	//------------------------------
	void update(unsigned long _delta);
	void delSprite(pSpriteFrame* _sprite);

private:
	pSpriteFrame m_current;
	pAnimCell* m_last;
	pImageCollection m_Images;
};

#endif