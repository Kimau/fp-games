#ifndef JEWL_HELPER_CLASS
#define JEWL_HELPER_CLASS

#include "WinGL.h"
#include "Scenario.h"

class JWL_Helpers
{
public:
	//--------------------------------
	//	System Functions
	//--------------------------------
	JWL_Helpers(JWL_Scenario& _scence, WinGLengine& _gui);
	~JWL_Helpers();
	
	//--------------------------------
	//	Path Functions
	//--------------------------------
	MAP_PATH* PlotPath(MAP_POS _pos, MAP_POS _dest, UINT _depth);		// Plot Path between points (RECURSSIVE FUNCTION)
	void KillPath(MAP_PATH* _path);										// Kills Path
	UINT Distance(const MAP_PATH* _path) const;							// Measure Distance of Path
	double Distance(const MAP_POS _pos, const MAP_POS _dest) const;		// Calculate Distance Between Points

	//--------------------------------
	//	Move From Scence to Gui
	//--------------------------------
	void ObjectTransfer();										// Transfers Objects from Game into Graphics Engine
	void AddUnitMeshes();										// Loads Units being used Into Mem

	//--------------------------------
	//	Update and Process Functions
	//--------------------------------
	LRESULT Parse(HWND _hWnd, UINT _iMsg, WPARAM wParam, LPARAM lParam);	// Parse Windows Messages when in Game Mode
	void ProcessCommands();													// Process Command Stack
	void UpdateGui(JWL_CMD _cmd);											// Update the Graphics Engine
	void ConvertMousePos(UINT_POINT _curPos);								// Convert Mouse Position

	//--------------------------------
	//	Build Functions
	//--------------------------------
	void BuildStripMesh();									// Build Strip Map Mesh
	void BuildQuadMesh();									// Build Map Mesh of Quads
	void BuildMapMesh();									// Build Cliff Map Mesh
	void BuildLogicMesh();									// Build Logical Map Mesh
private:
	float SamplePoint(float x, float y) const;				// Sample Point's Height
	float* mapTexTile(float x, float y) const;				// get Tex Coords
	void BuildVNGrid(JWL_Mesh& _mesh, CNorm* _tNorm);		// Build Grid of Vertex and Normal Calcs

	//--------------------------------
	//	Varibles
	//--------------------------------
public:
	WinGLengine&	m_Gui;			// Refrence to Graphics System
	JWL_Scenario&	m_Scence;		// Refrence to Game Scence
	const JWL_Map&	m_Map;			// Refrence to Map
	int				m_Msmooth;		// Mouse Smoothing Varible
};

#endif