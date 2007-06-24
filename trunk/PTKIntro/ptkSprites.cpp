#include "ptkSprites.h"
#include <list>

typedef std::list<pSpriteFrame> FrameList;
FrameList s_Frames;

typedef std::list<pSprite> SpriteList;
SpriteList s_Sprites;

//###########################################
//###########################################
//		Constructor / Decon
//###########################################
//###########################################
//========================================================================
//		Constructor
//========================================================================
ptkSpriteManager::ptkSpriteManager()
{
	memset(&m_current,0,sizeof(pSpriteFrame));
}

//========================================================================
//		Deconstructor
//========================================================================
ptkSpriteManager::~ptkSpriteManager()
{
	delSprite(&(s_Frames.front()));
	s_Frames.pop_front();

	s_Sprites.clear();
}

//###########################################
//###########################################
//		Sprite Creation
//###########################################
//###########################################
//========================================================================
//		Start Creating a Sprite
//========================================================================
void ptkSpriteManager::start(const char* _filename)
{
	//---------------------------------
	//	If Previous exsist
	//	And is different then Remove
	//---------------------------------
	if((m_current.m_ref != 0) && (strcmp(m_current.m_file,_filename) != 0))
	{
		m_Images.Release(m_current.m_file);
		strcpy(m_current.m_file,"");
		m_current.m_ref = 0;
	}

	//---------------------------------
	//	Assign
	//---------------------------------
	m_current.m_ref = m_Images.Assign(_filename);
}

//========================================================================
//		Add Cell
//========================================================================
void ptkSpriteManager::addCell(int _x1, int _y1, int _x2, int _y2, unsigned long _time)
{
	//------------------------------
	//	Create Cell
	//------------------------------
	pAnimCell* _cell = new pAnimCell();
	_cell->m_slice[0] = _x1;
	_cell->m_slice[1] = _x1;
	_cell->m_slice[2] = _y2;
	_cell->m_slice[3] = _y2;
	_cell->m_time   = _time;
	
	//-------------------------------
	//	Add onto the end of the List
	//-------------------------------
	if(m_last == 0)
	{
		m_current.m_first = _cell;
	}
	else
	{
		m_last->m_next = _cell;
	}
	//------------------------------
	//	Set Last Cell
	//------------------------------
	m_last = _cell;
	_cell = 0;
}
//========================================================================
//		End Cell and Add to List
//========================================================================
void ptkSpriteManager::end()
{
	s_Frames.push_back(m_current);
	memset(&m_current,0,sizeof(pSpriteFrame));
}

//###########################################
//###########################################
//		Update Sprites
//###########################################
//###########################################
//========================================================================
//		Update Sprites
//========================================================================
void ptkSpriteManager::update(unsigned long _delta)
{
	for(FrameList::iterator _iter = s_Frames.begin(); _iter != s_Frames.end(); )
	{
		pAnimCell* _cell = _iter->m_curr;

		//-------------------------
		//	Draw Cell
		//-------------------------
		_iter->m_ref->blitAlphaRect((float)_cell->m_slice[0],
									(float)_cell->m_slice[1],
									(float)_cell->m_slice[2],
									(float)_cell->m_slice[3],
									_iter->m_pos[0],
									_iter->m_pos[1]);

		//-------------------------
		//	Move to next Cell
		//-------------------------
		while(_delta > 0)
		{
			//-------------------------
			//	Remove remaining Time
			//-------------------------
			if(_delta < _iter->m_remain)
			{
				_iter->m_remain -= _delta;
				_delta = 0;
			}
			//-------------------------
			//	Advance Next Cell
			//-------------------------
			else
			{
				_delta -= _iter->m_remain;
				if(_cell->m_next == 0)
				{
					// _iter->m_curr = _iter->m_first;
					delSprite(&(*_iter));
					_delta = 0;
				}
				else
				{
					_iter->m_curr = _iter->m_curr->m_next;
					_iter->m_remain = _iter->m_curr->m_time;
				}				
			}
		}
	}
}
//========================================================================
//		Update Sprites
//========================================================================
void ptkSpriteManager::delSprite(pSpriteFrame* _sprite)
{
	pAnimCell* _cell = _sprite->m_first;
	while(_cell != 0)
	{
		pAnimCell* _next = _cell->m_next;
		delete _cell;
		_cell = _next;
	}
	delete _sprite;
}