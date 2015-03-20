#ifndef BOID_WINDOWS_OPENGL_HEAD_45
#define BOID_WINDOWS_OPENGL_HEAD_45

//=======================================================
//	Defines
//=======================================================
#define WIN_NAME	"WinName"
#define WIN_TITLE	"WinTitle"
#define WIN_HEIGHT	600
#define WIN_WIDTH	800

//=======================================================
//	Header Includes
//=======================================================
#include "Math\CVector2.h"
#include "Math\CVector3.h"
#include "OpenGL\BaseGL.h"
#include "OpenGL\DrawRasFont.h"
#include "OpenGL\Tex_Bitmap.h"
#include "OpenGL\DrawBoid.h"
#include "OpenGL\DrawCube.h"
#include "OpenGL\DrawCyl.h"
#include "Flock\CFlock.h"

//=======================================================
//	CallBack Resolution
//	Unfortnatly due to the fact WinProc is a callback
//	function it is extremly impractical to make it an
//	encaspulated for this reason its practical to resort
//	to sister functinos
//=======================================================
class BoidBounce
{
	//=======================================================
	//	Boid Creation and Destruction
	//=======================================================
private:
	BoidBounce();
public:
	BoidBounce(HINSTANCE _Instance, HWND _Wnd, UINT _height, UINT _width);
	~BoidBounce();
	//=======================================================
	//	Boid Functions
	//=======================================================
public:
	void GameStep();
	void KeyPressed(WPARAM _key);
	void MouseMove(WPARAM _wPar, LPARAM _lPar);

	//=======================================================
	//	Varibles
	//=======================================================
private:
	float m_xAxis;
	float m_yAxis;
	float m_zAxis;

	UINT m_xSize;
	UINT m_ySize;
	UINT m_zSize;

	CVector3 m_mouseStart;
	CVector3 m_mouseDir;

	UINT m_WinHeight;
	UINT m_WinWidth;

	HINSTANCE m_Instance;
	HWND m_Window;

	WinGLengine*	m_enginePTR;
	BGLRasterFont*	m_rasFontPTR;
	BGL_Bitmap		m_textures[5];
	BGLDrawBoid*	m_drawFlockPTR;
	BGLDrawCube*	m_drawCubePTR;
	BGLDrawTube*	m_drawTubePTR;
	bFlock3*		m_flockPTR;
};

#endif