#ifndef MATRIX_MAP_COME_DUNGEON_CRAWLER_343
#define MATRIX_MAP_COME_DUNGEON_CRAWLER_343

//---------------------------------
//	Includes
//---------------------------------
#include "..\gameBlock.h"
#include "..\Support\STLTree.h"
#include "MapNode.h"

//---------------------------------
//	Defines
//---------------------------------
#define PH_NODE_SIZE 0.2f
#define PH_NODE_SEPERATION 1.2f
#define PH_NODE_DROP -1.5f
#define PH_CPU		1
#define PH_SUB		2
#define PH_PROC		4
#define PH_BRANCH	8
#define PH_DATA		16
#define PH_PORTAL	32

typedef SimpleTree<MapNode> NodeTree;

//---------------------------------
//	Structs
//---------------------------------


//---------------------------------------
//	Matrix Map Class
//---------------------------------------
class matrixBlock : public gameBlock
{
public:
	//-----------------------------------
	//	Contructors
	//-----------------------------------
	matrixBlock();
	~matrixBlock();
	//-----------------------------------
	//	Interface Functions
	//-----------------------------------
	int Open();
	int Step();
	int Input(UINT _iMsg, WPARAM wParam, LPARAM lParam);
	int Exit();

private:
	//---------------------------------
	//	Generation
	//---------------------------------
	void GenTree();
	void GenBranch(NodeTree* _branch, CVector3 _basePos);
	void BalanceLoad();
	// Need Seeded Generation

	//---------------------------------
	//	File Management
	//---------------------------------
	bool LoadTree(char* _file);
	bool SaveTree(char* _file) const;

	//---------------------------------
	//	Navigation
	//---------------------------------
	bool NavGoto(NodeTree* _branch);

	//---------------------------------
	//	Display
	//---------------------------------
	void Render();
	void RenderBranch(NodeTree* _branch);

	//---------------------------------
	//	Helper
	//---------------------------------
	CVector3 getRealPos(NodeTree* _branch);
	void updateCam(bool _anim = false);
	NodeTree* getMouseCollision();
	NodeTree* getMouseCollision(NodeTree* _branch, float& _minDist, CVector3 _dir, CVector3 _basePos);

	//------------------------------
	//	Logical Varibles
	//------------------------------
	int m_pSelection;			// Player Selection
	float m_log;				// Tracks Activity
	float m_alert;				// Alert
	NodeTree m_tree;			// Tree
	NodeTree* m_pLocation;		// Player Location	
	NodeTree* m_Selection;		// Mouse Selection

	//------------------------------
	//	Support and Graphics
	//------------------------------
	int m_Count;						// Node Count
	CVector2 m_Mouse;					// Mouse XY
	QCamera m_Camera;					// Camera
	std::vector<MapNode*> m_NodePTR;	// Nodes PTR
	std::vector<CVector3> m_PosPTR;		// Nodes Real Position
	std::vector<GLuint>	  m_ModelList;	// Model Vector
};

#endif