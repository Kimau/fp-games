//================================================================================================
//	This Basic Utility class is used for any subclass that needs to call a render function
//	The reason for embedded Renders is that I want to use a lot of customised math for rendering
//	Different ICE and other effects. The choices were a helper libary, ID renderer or embed
//	functions.
//================================================================================================
#ifndef OPENGL_UTILITY_HEADER_3834
#define OPENGL_UTILITY_HEADER_3834

//----------------------------------
//	Defines
//----------------------------------
#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	767 // 768
#define WINDOW_NAME		"Phantom"
#define WINDOW_TITLE	"Phantom Test Engine"
#define WINDOW_STYLE	WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP

#define PH_LMOUSE	1
#define PH_RMOUSE	2


//------------------------------------------------------
//	Includes
//------------------------------------------------------
#include "OpenGL\BaseGL.h"
#include "OpenGL\BaseCG.h"
#include "OpenGL\Helpers.h"
#include "OpenGL\Tex_Loader.h"
#include "OpenGL\DrawRasFont.h"
#include "OpenGL\Log.h"
#include "OpenGL\Light.h"
#include "OpenGL\QCamera.h"
#include "OpenGL\Model3DS.h"

#include "Math\CVector2.h"
#include "Math\CVector3.h"
#include "Math\CPlane.h"

#include "Support\Singleton.h"
//------------------------------------------------------
//	Singleton
//------------------------------------------------------
#define PH_ENGINE	CSingletonT<WinGLengine>::Get()
#define PH_SHADER	CSingletonT<BaseCG>::Get()
#define PH_LOG		CSingletonT<BGLLog>::Get()
#define PH_LIGHT	CSingletonT<BGLLights>::Get()
#define PH_TEX		CSingletonT<BGLTexture>::Get()

#endif

extern unsigned long x_delta;