#include "DrawSurface.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Initialsation Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//----------------------------------------
//	Blank Constructor
//	Only used when comming into scope
//----------------------------------------
BGLDrawSurface::BGLDrawSurface()
: m_vertArray(0), m_engine(WinGLengine(0,0,0,0)), m_surface(CSurface(0,0,0))
{
}
//----------------------------------------
//	Constructor
//----------------------------------------
BGLDrawSurface::BGLDrawSurface(const WinGLengine& _engine, const CSurface& _surface)
: m_engine(_engine), m_surface(_surface)
{
	//-----------------------------------
	// Create Array
	// Using this format GL_T2F_V3F
	//-----------------------------------
	int _size = m_surface.getWidth() * (m_surface.getLength() - 1) * 10;
	m_vertArray = new float[_size];
	memset(m_vertArray,0, sizeof(float) * _size);
	CreateArray();
}
//----------------------------------------
//	Copy constructor
//----------------------------------------
BGLDrawSurface::BGLDrawSurface(const BGLDrawSurface& _ref)
: m_engine(_ref.m_engine), m_surface(_ref.m_surface)
{
	if(this != &_ref)
	{
		int _size = m_surface.getWidth() * (m_surface.getLength() - 1) * 10;
		memcpy(m_vertArray,_ref.m_vertArray,_size);
	}
}
//----------------------------------------
//	Copy constructor
//----------------------------------------
BGLDrawSurface& BGLDrawSurface::operator=(const BGLDrawSurface& _ref)
{
	if(this != &_ref)
	{
		//---------------------------------------
		//	Due to the fact this uses a const
		//	cast the garbarge collector will not
		//	link the variable. So be carefull of
		//	The data being garbarged
		//---------------------------------------
		const_cast<WinGLengine&>(m_engine) = _ref.m_engine;
		const_cast<CSurface&>(m_surface) = _ref.m_surface;
		//--------------------------------------
		//	Copy Over block
		//--------------------------------------
		int _size = m_surface.getWidth() * (m_surface.getLength() - 1) * 10;
		if(m_vertArray)		delete[] m_vertArray;
		m_vertArray = new float[_size];
		memcpy(m_vertArray,_ref.m_vertArray,_size);
	}
	return *this;
}
//----------------------------------------
//	Deconstructor
//----------------------------------------
BGLDrawSurface::~BGLDrawSurface()
{
	if(m_vertArray)		delete[] m_vertArray;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Array Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//---------------------------------------------------------------------
//		Creates the array
//		This is done because 4/5 of the data is constant
//---------------------------------------------------------------------
void BGLDrawSurface::CreateArray()
{
	UINT _xSize = m_surface.getWidth();
	UINT _ySize = m_surface.getLength() - 1;	// Minus due to double usage
	UINT _offset = 0;							// Used for speed

	//----------------------------------------
	//	Iterate through the Surface
	//----------------------------------------
	for(UINT y = 0; y < _ySize; ++y)
	{
		//----------------------------
		//	Precalculate
		//----------------------------
		float _yTexBot = ((float)y / (float)_ySize);
		float _yTexTop = ((float)(y+1) / (float)_ySize);

		//---------------------------------------------
		//	Generate the Strip
		//---------------------------------------------
		for(UINT x = 0; x < _xSize; ++x)		
		{
			float _xTex = ((float)x) / ((float)(_xSize - 1));
			//-----------------------------
			//	Bottom Vert
			//-----------------------------
			m_vertArray[_offset++] = _xTex;
			m_vertArray[_offset++] = _yTexBot;
			m_vertArray[_offset++] = float(x);
			m_vertArray[_offset++] = float(y);
			m_vertArray[_offset++] = m_surface.getHeight(x,y);

			//-----------------------------
			//	Top Vert
			//-----------------------------
			m_vertArray[_offset++] = _xTex;
			m_vertArray[_offset++] = _yTexTop;
			m_vertArray[_offset++] = float(x);
			m_vertArray[_offset++] = float(y+1);
			m_vertArray[_offset++] = m_surface.getHeight(x,y+1);
		}
		// End of Strip
	}
}
//--------------------------------------------------------------------------
//	Updates the array with new Information
//	Only updates the Heights
//--------------------------------------------------------------------------
void BGLDrawSurface::UpdateArray()
{
	UINT _xSize = m_surface.getWidth();
	UINT _ySize = m_surface.getLength() - 1;	// Minus due to double usage
	UINT _offset = 4;							// Used for speed, 4 is start
	//----------------------------------------
	//	Iterate through the Surface
	//----------------------------------------
	for(UINT y = 0; y < _ySize; ++y)
	{
		//---------------------------------------------
		//	Calculate Heights
		//---------------------------------------------
		for(UINT x = 0; x < _xSize; ++x)		
		{
			m_vertArray[_offset] = m_surface.getHeight(x,y);
			_offset += 5;
			m_vertArray[_offset] = m_surface.getHeight(x,y+1);
			_offset += 5;
		}
		// End of Strip
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Render Functions
//		Note non of these functions update the array
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//----------------------------------------
//	Default Render Function
//----------------------------------------
void BGLDrawSurface::Render()
{
	glPushMatrix();
		//glEnable(GL_CULL_FACE);

		//---------------------------------------------
		//	Precalculate
		//---------------------------------------------
		UINT _StripSize = m_surface.getWidth() * 2;		// Double Line
		UINT _ySize = m_surface.getLength() - 1;		// Minus due to double usage
		
		//---------------------------------------------
		//	Enable Interleaved Arrays
		//	then Draw one strip at a time
		//---------------------------------------------
		glInterleavedArrays(GL_T2F_V3F,0,m_vertArray);
		for(UINT y = 0; y < _ySize; ++y)
		{			
			glDrawArrays(GL_TRIANGLE_STRIP, _StripSize*y, _StripSize);
		}
	glPopMatrix();
}