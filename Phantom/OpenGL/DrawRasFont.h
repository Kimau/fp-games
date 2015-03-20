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

#pragma warning(disable : 4996) // Disables silly warning about win "security"

class BGLRasterFont
{
	//----------------------------------------
	//	Initialisation
	//----------------------------------------
public:
	BGLRasterFont();
	BGLRasterFont(const BGLRasterFont& _ref);
	BGLRasterFont& operator=(const BGLRasterFont& _ref);
	~BGLRasterFont();
	//----------------------------------------
	//	Render Functions
	//----------------------------------------
	void Load(WinGLengine* _engine, int _height = 10, const char* _font = "Arial");
	void Render(float x, float y, const char* _format, ...);
	void RenderBlock(float x, float y, char* _block);
	//----------------------------------------
	//	Varibles
	//----------------------------------------
	WinGLengine* m_engine;
	int m_height;
	int m_id;
};

#endif