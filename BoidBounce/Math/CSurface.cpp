#include "CSurface.h"
#include <math.h>		// For Float Functions
#include <stdlib.h>		// For Rand
#include <memory.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Constructors
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------
//		Generate NULL Surface
//------------------------------------------------------------
CSurface::CSurface()
{
	m_width		= 0;
	m_length	= 0;
	m_heights	= 0;
	m_normal	= 0;
}
//------------------------------------------------------------
//	Copy Contructor
//------------------------------------------------------------
CSurface::CSurface(const CSurface& _ref)
: m_width(_ref.m_width), m_length(_ref.m_length)
{
	if(this != &_ref)
	{
		int _size	= (_ref.m_width)*(_ref.m_length);

		//---------------------------------
		//	Error Checking
		//---------------------------------
		if(_size > 4)
		{
			m_heights	= new float[_size];
			m_normal	= new CVector3[_size];

			memcpy(m_heights,_ref.m_heights,(_ref.m_width*_ref.m_length));
			memcpy(m_normal,_ref.m_normal,(_ref.m_width*_ref.m_length));
		}
		else
		{
			m_heights = 0;
			m_normal = 0;
		}
	}
}
//------------------------------------------------------------
//	Copy Contructor (Equals Operator)
//------------------------------------------------------------
CSurface& CSurface::operator=(const CSurface& _ref)
{
	if(this != &_ref)
	{
		UINT _Oldsize	= m_width*m_length;

		m_width = _ref.m_width;
		m_length = _ref.m_length;

		int _size	= m_width*m_length;
		//--------------------------------------
		//	New Block Copy
		//--------------------------------------
		if(_Oldsize < 5000)
		{
			if(m_heights)	delete[] m_heights;
			if(m_normal) 	delete[] m_normal;
		}

		m_heights	= new float[_size];
		m_normal	= new CVector3[_size];

		memcpy(m_heights,_ref.m_heights,(_ref.m_width*_ref.m_length));
		memcpy(m_normal,_ref.m_normal,(_ref.m_width*_ref.m_length));
	}
	return *this;
}
//------------------------------------------------------------
//		Randomly Generate Terrain
//------------------------------------------------------------
CSurface::CSurface(UINT _width, UINT _length, UINT _maxHeight)
{
	m_width		= _width;
	m_length	= _length;

	int _size	= _width*_length;

	//------------------------------------
	//	Error Checking
	//------------------------------------
	if(_size > 4)
	{
		m_heights	= new float[_size];
		m_normal	= new CVector3[_size];

		memset(m_heights,0,sizeof(float)*_size);
		memset(m_normal,0,sizeof(CVector3)*_size);

		GenerateHeights(_maxHeight);
		GenerateNormals();
	}
	else
	{
		m_heights = 0;
		m_normal = 0;
	}
}
//------------------------------------------------------------
//		Destructors
//------------------------------------------------------------
CSurface::~CSurface()
{
	if(m_heights)	delete[] m_heights;
	if(m_normal)	delete[] m_normal;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------
//		Generate Heights from Randomly
//------------------------------------------------------------
void CSurface::GenerateHeights(UINT _maxHeight)
{
	UINT _size = m_width*m_length;
	srand(_size + _maxHeight + rand());
	//-------------------------------------------------------------------
	//	Generate a Control Map
	//	5x5 Groupings
	//-------------------------------------------------------------------
	const UINT _group = 5;
	UINT _step = m_width / _group;
	float _control[_group*_group] = {0};
	for(UINT _index = 0; _index < (_group*_group); ++_index)
	{
		_control[_index] = float((rand()+1) % (_maxHeight*100)) / 100.0f;
	}
	//-----------------------------------------------------------------
	//	We Now smooth out the Function
	//-----------------------------------------------------------------
	for(UINT x = 0; x < m_width; ++x)
	{
		for(UINT y = 0; y < m_length; ++y)
		{
			//-------------------------------------------
			//	Declare empty array
			//-------------------------------------------
			UINT _mark[4] = {0,0,0,0};
			//-------------------------------------------
			//	Get X Points
			//-------------------------------------------
			if(x < (_step*2))
			{
				_mark[0] = 0;	
				_mark[1] = 1;
			}
			else if(x > (m_width- (_step*2)))
			{
				_mark[0] = _group-2;	
				_mark[1] = _group-1;
			}
			else
			{
				_mark[0] = x / _step;
				_mark[1] = _mark[0] + 1;
			}
			//-------------------------------------------
			//	Get Y points
			//-------------------------------------------
			if(y < (_step*2))
			{
				_mark[2] = 0;	
				_mark[3] = 1;
			}
			else if(y > (m_width- (_step*2)))
			{
				_mark[2] = _group-2;	
				_mark[3] = _group-1;
			}
			else
			{
				_mark[2] = y / _step;
				_mark[3] = _mark[2] + 1;
			}
			//-------------------------------------------
			//	Get the index of the points
			//-------------------------------------------
			int _index[4] =
			{
				_mark[0] + _mark[2]*_group,
				_mark[0] + _mark[3]*_group,
				_mark[1] + _mark[2]*_group,
				_mark[1] + _mark[3]*_group
			};
			//----------------------------------------------------------
			//	Measure Distances
			//	
			//	Note the distances SHOULD always be less
			//	than one. However a glitch occurs when it
			//	sometimes isn't due to oddly sized surface.
			//	For this reason we do a percentage
			//----------------------------------------------------------
			float _distance[4] = {0,0,0,0};
			float _totalDis = 0.0f;
			for(UINT c = 0; c < 4; ++c)
			{
				UINT _cx = (_index[c] % _group) * _step;
				UINT _cy = (_index[c] / _group) * _step;
				_distance[c] = 1.0f / (float)(pow(int(x-_cx),2) + pow(int(y-_cy),2));
				_totalDis += _distance[c];
			}
			//-------------------------------------------------------
			//	Add Heights
			//	
			//	This is propotional to distance. Not however distance
			//	squared.
			//-------------------------------------------------------
			float _height = 0.0f;
			for(UINT c = 0; c < 4; ++c)
			{
				if(_distance[c] < 500000.0f)
				{
					float _weight = (_distance[c] / _totalDis);
					_height += _control[_index[c]] * _weight;
				}
				else
				{
					_height = _control[_index[c]];
					break;
				}
			}

			m_heights[y*m_width + x] = _height;
		}
	}
}
//------------------------------------------------------------
//		Generate FACE Normals
//	You will notice if you look at the logic we leave a row
//	and a cloumn blank or zerod. This is for the logic.
//	You see we always check the lower vertex for the face.
//	The other reason we are using what I called halved faces
//	Named such as they only represent half the face is because
//	Later a fragment shader will do normals.
//------------------------------------------------------------
void CSurface::GenerateNormals()
{
	//------------------------
	//	Iterate through the
	//	heights and generate
	//	normals
	//------------------------
	for(UINT x = 0; x < (m_width - 1); ++x)
	{
		for(UINT y = 0; y < (m_length - 1); ++y)
		{
			CVector3 _Apoint( x   , y   ,getHeight( x   , y   ));
			CVector3 _Bpoint( x   ,(y+1),getHeight( x   ,(y+1)));
			CVector3 _Cpoint((x+1), y   ,getHeight((x+1), y   ));

			m_normal[(y*m_length + x)] = (_Apoint - _Cpoint) * (_Cpoint - _Bpoint);
		}
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Accessors
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------------------------------------------------
//		Get Height for Location
//------------------------------------------------------------
float CSurface::getHeight(UINT _x, UINT _y) const
{
	return m_heights[_y*m_width + _x];
}
//------------------------------------------------------------
//		Get Normal for Location
//------------------------------------------------------------
const CVector3& CSurface::getNormal(UINT _x, UINT _y, UINT _n) const
{
	return m_normal[(_y/2)*m_width + (_x/2) + _n];
}
//------------------------------------------------------------
//		Get Normal for Location
//------------------------------------------------------------
const CVector3& CSurface::getNormal(float _x, float _y) const
{
	if(((_x + _y) / floorf(_x + _y)) > 0.5f)
        return m_normal[UINT(_y/2)*m_width + UINT(_x/2) + 1];
	else
		return m_normal[UINT(_y/2)*m_width + UINT(_x/2)];
}
//------------------------------------------------------------
//		Get the Width of the Surface
//------------------------------------------------------------
UINT  CSurface::getWidth() const
{
	return m_width;
}
//------------------------------------------------------------
//		Get the Length of the Surface
//------------------------------------------------------------
UINT  CSurface::getLength() const
{
	return m_length;
}