#ifndef BGL_DRAW_RASTER_FONT_43534
#define BGL_DRAW_RASTER_FONT_43534

//----------------------------------
//	Includes
//----------------------------------
#include "BaseGl.h"

//----------------------------------
//	Defines
//----------------------------------
#define BGL_MAX_CHAR	255

class BGLRasterFont
{
	//----------------------------------------
	//	Initialisation
	//----------------------------------------
public:
	BGLRasterFont();
	BGLRasterFont(const WinGLengine& _engine, int _height = 10, const char* _font = "Arial");
	BGLRasterFont(const BGLRasterFont& _ref);
	BGLRasterFont& operator=(const BGLRasterFont& _ref);
	~BGLRasterFont();
	//----------------------------------------
	//	Render Functions
	//----------------------------------------
	void Render(UINT x, UINT y, const char* _format, ...);
	//----------------------------------------
	//	Varibles
	//----------------------------------------
	const WinGLengine& m_engine;
	int m_height;
	int m_id;
};

#endif