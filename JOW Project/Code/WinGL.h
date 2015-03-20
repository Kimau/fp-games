#ifndef WINDOWS_OPENGL_BASE_3245
#define WINDOWS_OPENGL_BASE_3245

//----------------------------------
// HEader Includes
//----------------------------------
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\glu.h>
#include <gl\gl.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "Structs.h"
#include "Loaders.h"

class WinGLengine
{
	//----------------------------------
	// Initialisation Functions
	//----------------------------------
public:
	WinGLengine(HINSTANCE _hInstance, HWND _hWnd);
private:
	bool CreateOpenGL();									// Create OpenGL
	bool InitializeOpenGL();								// Init OpenGL
	bool CreatePixelFormat(HDC _hdc);						// Creates Pixel Format
	bool CreateLights();									// Create Lighting
	bool CreateCamera();									// Create Camera
	bool CreateRasterFont();								// Creates Raster Font
	bool toFullScreen();									// Change to Fullscreen
	bool SizeOpenGL();										// Resize Screen

	//----------------------------------
	// Addition Functions
	//----------------------------------
public:
	void AddMap(UINT _MapListID, const char* _tex);			// Adds Map Mesh to List
	UINT AddTexture(const char* _filename);					// Load file into texture database
	UINT AddMesh(const char* _filename);					// Load mesh into mesh database from File
	UINT AddMesh(const JWL_Mesh& _Mesh);					// Build List from Mesh
	UINT AddLight(INT_LIGHT& _Light);						// Add Light to Scence
	UINT AddObject(UINT _meshID, UINT _texID,CVector3 _pos, CVector3 _up, float _face);
	UINT AddUnit(UINT _meshID, UINT _texID, UINT _team, CVector3 _pos, CVector3 _up, float _face);

	//----------------------------------
	// Manipulation Functions
	//----------------------------------
public:
	void MoveObject(const UINT _objID, const CVector3 _pos, const CVector3 _up, const float _face);				// Move Object
	void MoveUnit(const UINT _objID, const CVector3 _pos, const CVector3 _up, const float _face);				// Move Unit
	void MoveCursor(const CVector3 _newPos);
	void selUnit(const UINT _index);
	void setMouse(const UINT_POINT _pos);
	void setHalo(CVector3 _newPos);
	void setMapNorms(CNorm* _norm);
	void setPath(MAP_PATH* _path);

	//----------------------------------
	// Destruction Functions
	//----------------------------------
public:
	~WinGLengine();
private:
	bool DestroyOpenGL();									// Destroy's OpenGL
	bool ResetObjects();
	bool ResetTextures();
	bool ResetMeshes();
	bool ResetUnits();

	//----------------------------------
	// Camera Functions
	//----------------------------------
public:
	void CamRotateView(float _angle, double X, double Y, double Z);							// Rotate Camera
	void CamRotateAroundPoint(CVector3 _point, double _height, double _distance, float _rot);// Rotate Camera around point
	void CamLook();																			// Updates Camera
	void CamLook(CVector3 _pos);															// Moves and updates camera
	void CamLook(CVector3 _pos, CVector3 _view, CVector3 _up);								// Updates Camera
	void CamLookAt(CVector3 _view);
	void CamLookCur();
	

	//----------------------------------
	// Render Functions
	//----------------------------------
public:
	bool Render();													// All important Render Call
private:
	void DrawObjectList();											// Draw all exsisting Objects
	void DrawMap();
	void DrawUnitList();											// Draw all exsisting Units
	void DrawMapCursor();											// Draw Map Cursor
	void DrawUnitCursor();											// Draw Circle Around Selected Unit
	void DrawPath();												// Draw
	void DrawRasterFont(UINT x, UINT y, const char* _format, ...);	// Draw Raster Font
	// ORTHO MODE
	void DrawMouse();

	//-------------------------------
	// Variables and Accessors
	//-------------------------------
public:
	bool isFullscreen() const;

	      CVector3   getObjPos(UINT _ObjID) const;					// Get Objects Position
    const CVector3&  getMapPos() const;
	const CVector3&  getCamPos() const;								// Get Camera Position Vector
	const CVector3&  getCamView() const;							// Get Camera View Vector
	const CVector3&  getCamUp() const;								// Get Camera's Up Vector
private:
	// Windows Varibles
	HWND m_hWnd;							// Pointer to Window Handle Windows
	HDC m_hDevCon;							// Pointer to a Struct of Device Controller Handle
	HGLRC m_hGLRemCon;						// Pointer to a Struct of OpenGL Remote Controller Handle
	HINSTANCE m_hInstance;					// Pointer to a Struct of Instance Handle
	
	DEVMODE m_dmSettings;					// FIGURE OUT ? -> ? to Device Mode Struct
	DWORD m_DrawStyle;						// Holds Drawing Style of Window
    UINT m_Height;							// Height of Window
	UINT m_Width;							// Width of Window
	UINT m_Depth;							// Depth of Window

	// OpenGL Varibles
	bool  m_Fullscreen;						// Fullscreen or not
	UINT  m_TexCount;						// How many textures currently used
    UINT  m_MeshCount;						// How many Display Lists currently used
	UINT  m_UnitCount;						// How many Units are there currently
	UINT  m_ObjCount;						// How many Objects are currently Used
	UINT  m_LightCount;						// How many lights are currently used	
	UINT  m_FontID;							// Holds Font List ID

	// OpenGL Data
	OGL_FILE 	m_Textures[_MAX_TEX_COUNT];	// Holds OpenGL Textures
	OGL_FILE 	m_Meshes[_MAX_MESH_COUNT];	// Holds OpenGL DrawList
	OGL_OBJ		m_Objects[_MAX_OBJ_COUNT];	// Holds Abstract Objects
	OGL_UNIT	m_Units[_MAX_UNIT_COUNT];	// Holds Units

	// Camera Varibles
	CVector3 m_cPosition;					// The camera's position
	CVector3 m_cView;						// The camera's view
	CVector3 m_cUpVector;					// The camera's up vector

	// Cursors
	UINT_POINT m_mouse;						// Mouse Cursor
	CVector3 m_mapCur;						// Map Cursor
	OGL_OBJ  m_halo;						// Halo Cursor
	UINT     m_unitCur;						// Unit Cursor

	// Map Data
	CNorm*	m_mapNorms;						// Holds Face Normals of the Map
	const MAP_PATH* m_path;					// Holds Path to be displayed

	// Display Stuff
	public:
	char m_string[255];
};

#endif WINDOWS_OPENGL_BASE_3245