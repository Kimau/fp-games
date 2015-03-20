#include "ptkSpriteManager.h"
#include <map>
#include <list>

//-------------------------------
//	Comparision Class
//-------------------------------
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return stricmp(s1, s2) < 0;
	}
};
struct ltint
{
	bool operator()(int i1, int i2) const
	{
		return (i1 < i2);
	}
};

typedef std::map<const char*, ptkSprite*, ltstr> SpriteMap;
typedef std::map<unsigned int, ptkObject, ltint> ObjectMap;
typedef std::list<ptkParticle> ParticleList;
typedef std::list<int> IntList;

//---------------------------
//		Collections
//---------------------------
SpriteMap	 s_Sprites;		// Sprite Collection
ObjectMap	 s_Objects;		// Object Collection
ParticleList s_Particles;	// Particle List

//#################################################
//#################################################
//					SETUP
//#################################################
//#################################################
//===============================================================================
//	Constructor
//===============================================================================
pSpriteManager::pSpriteManager()
{}
//===============================================================================
//	Deconstructor
//===============================================================================
pSpriteManager::~pSpriteManager()
{}
//===============================================================================
//	Purge Sprites
//===============================================================================
void pSpriteManager::Purge()
{
	//------------------------------------
	//	Clean out Memory
	//------------------------------------
	for(SpriteMap::iterator _iter = s_Sprites.begin(); _iter != s_Sprites.end(); ++_iter)
	{
		ptkSprite* _sprite = _iter->second;

		if(_sprite->m_slice != 0)		delete[] _sprite->m_slice;
		delete _sprite;
	}

	s_Sprites.clear();
	s_Objects.clear();
	s_Particles.clear();
}
//===============================================================================
//	Load Sprite File
//===============================================================================
void pSpriteManager::loadSpriteFile(const char* _filename)
{
	//---------------------------------
	//	Open File
	//---------------------------------
	FILE* _filePtr = fopen(_filename, "r");
	if(_filePtr == 0)
	{
		perror("Error opening file");
		return;
	}

	//---------------------------------
	//	Read File
	//---------------------------------
	char _buffer[1024];
	char* _line = fgets(_buffer,1024,_filePtr);

	while(_line != 0)
	{
		//---------------------------------
		//	Strip Comments
		//---------------------------------
		{
			char* _commentPtr = strchr(_buffer,'#');
			if(_commentPtr != 0)
			{
				_commentPtr[0] = 0;
			}
		}
		//---------------------------------
		//	Strip Space
		//---------------------------------
		{
			char* _spacePtr = strchr(_buffer,' ');
			while(_spacePtr != 0)
			{
				strcpy(_spacePtr, _spacePtr+1);
				_spacePtr = strchr(_buffer,' ');
			}
		}
		//---------------------------------
		//	Read Sprite
		//---------------------------------
		if(strlen(_buffer) > 6)
		{
			ptkSprite* _sprite = new ptkSprite;		// Assign Memory for sprite
			memset(_sprite,0,sizeof(ptkSprite));

			IntList _slices;						// Temp Slice
			
			char* _tok = strtok(_buffer,",");
			int _count = 0;
			while(_tok != 0)
			{
				//-------------------------------
				//	Add Slice
				//-------------------------------
				if(_count > 3)
				{
					int _pos[4];
					sscanf(_tok,"%d / %d / %d / %d",&(_pos[0]),&(_pos[1]),&(_pos[2]),&(_pos[3]));
					_slices.push_back(_pos[0]);
					_slices.push_back(_pos[1]);
					_slices.push_back(_pos[2]);
					_slices.push_back(_pos[3]);
				}
				//-------------------------------
				//	Header information
				//-------------------------------
				else if(_count == 0)	{ strncpy(_sprite->m_name,_tok,29); }	// Sprite Name
				else if(_count == 1)	{ strncpy(_sprite->m_sheet,_tok,29); }	// Sheet filename
				else if(_count == 3)	{ _sprite->m_millpframe = atoi(_tok); }	// Milliseconds per Frame
				else if(_count == 2)
				{
					if((_tok[0] == 'y') || (_tok[0] == 'Y'))
					{
						_sprite->m_loop = true;
					}
					else
					{
						_sprite->m_loop = false;
					}
				}
				
				//-------------------------------
				//	Next Part of String
				//-------------------------------
				++_count;
				_tok = strtok(0,",");
			}
			//---------------------------------
			//	Add Sprite
			//---------------------------------
			_sprite->m_refSheet = m_Images.Assign(_sprite->m_sheet);
			_sprite->m_frames = int(_slices.size()) / 4;

			_sprite->m_slice = new int[_slices.size()];
			int _sCount = 0;
			for(IntList::iterator _iter = _slices.begin(); _iter != _slices.end(); ++_iter)
			{
				_sprite->m_slice[_sCount] = *(_iter);
				++_sCount;
			}

			s_Sprites[_sprite->m_name] = _sprite;// Spaceship

			_slices.clear();
		}
		//---------------------------------
		//	Read Next String
		//---------------------------------
		_line = fgets(_buffer,1024,_filePtr);
	}

	fclose(_filePtr);
}

//#################################################
//#################################################
//					UPDATE
//#################################################
//#################################################
//===============================================================================
//	Draw Particles
//===============================================================================
void pSpriteManager::drawParticles()
{
	for(ParticleList::iterator _iter = s_Particles.begin(); _iter != s_Particles.end(); ++_iter)
	{
		int x = int(_iter->m_pos[0]);
		int y = int(_iter->m_pos[1]);

		ptkSprite* _sprite = _iter->m_sprite;
		drawSprite(_sprite,_iter->m_time, x, y);
	}
}

//===============================================================================
//	Draw Object
//===============================================================================
void pSpriteManager::drawObject(unsigned int _id, int x, int y)
{
	ptkSprite* _sprite = s_Objects[_id].m_sprite;
	drawSprite(_sprite, s_Objects[_id].m_time, x, y);
}

//===============================================================================
//	Draw Sprite
//===============================================================================
void pSpriteManager::drawSprite(ptkSprite* _sprite, unsigned long _time, int x, int y)
{
	if(_sprite == 0)	return;
	//--------------------------------
	//	Get Frame
	//--------------------------------
	int _frame = (_time) / (_sprite->m_millpframe);
	if(_sprite->m_loop)
	{
		_frame = _frame % _sprite->m_frames;
	}
	else
	{
		_frame = max(0,min(_sprite->m_frames - 1, _frame));
	}
	_frame = _frame * 4;

	//--------------------------------
	//	Draw Centered Image
	//--------------------------------
	if(_sprite->m_refSheet == 0)	return;

	int _dx = x - ((_sprite->m_slice[_frame+2] - _sprite->m_slice[_frame+0]) / 2);
	int _dy = y - ((_sprite->m_slice[_frame+3] - _sprite->m_slice[_frame+1]) / 2);
	_sprite->m_refSheet->blitAlphaRect(	(float)(_sprite->m_slice[_frame+0]),
										(float)(_sprite->m_slice[_frame+1]),
										(float)(_sprite->m_slice[_frame+2]),
										(float)(_sprite->m_slice[_frame+3]),
										_dx, _dy);
}

//===============================================================================
//	Update Sprites
//===============================================================================
void pSpriteManager::update(unsigned long _delta)
{
	//-------------------------------
	//	Update Particles
	//-------------------------------
	for(ParticleList::iterator _iter = s_Particles.begin(); _iter != s_Particles.end();)
	{
		//-------------------------------
		//	Age and Die
		//-------------------------------
		_iter->m_life -= _delta;
		_iter->m_time += _delta;
		if(_iter->m_life < 0)
		{
			s_Particles.erase(_iter++);
		}
		else
		{
			++_iter;
		}

		//-------------------------------
		//	Move
		//-------------------------------
		for(int i = 0; i < 2; ++i)
		{
			_iter->m_pos[i] = _iter->m_pos[i] + (_iter->m_vel[i]) * _delta * 0.1f;
			_iter->m_vel[i] = _iter->m_vel[i] + (_iter->m_acl[i]) * _delta * 0.1f;		
		}
	}

	//-------------------------------
	//	Update Objects
	//-------------------------------
	for(ObjectMap::iterator _iter = s_Objects.begin(); _iter != s_Objects.end(); ++_iter)
	{
		_iter->second.m_time += _delta;
	}
}

//#################################################
//#################################################
//					MANAGEMENT
//#################################################
//#################################################
//===============================================================================
//	Add Particle
//===============================================================================
void pSpriteManager::addParticle(const char* _sName, long _life, float _px, float _py,
								 float _vx, float _vy, float _ax, float _ay)
{
	ptkParticle _part;
	_part.m_sprite = s_Sprites[_sName];
	_part.m_time = 0;
	_part.m_life = _life;
	_part.m_pos[0] = _px;		_part.m_pos[1] = _py;
	_part.m_vel[0] = _vx;		_part.m_vel[1] = _vy;
	_part.m_acl[0] = _ax;		_part.m_acl[1] = _ay;

	s_Particles.push_back(_part);
}
//===============================================================================
//	Add Object
//===============================================================================
unsigned int pSpriteManager::addObject(const char* _sName)
{
	ptkObject _obj;
	_obj.m_sprite =  s_Sprites[_sName];
	_obj.m_time = 0;

	unsigned int _size = (unsigned int)(s_Objects.size());
	s_Objects[_size] = _obj;
	return _size;
}
//===============================================================================
//	Remove Object
//===============================================================================
void pSpriteManager::removeObject(unsigned int _id)
{
	s_Objects.erase(s_Objects.find(_id));
}