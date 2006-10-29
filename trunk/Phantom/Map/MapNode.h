#ifndef MAP_NODE_OF_BASIC_2346
#define MAP_NODE_OF_BASIC_2346

//------------------------------------------
//	Includes
//------------------------------------------
#include "..\GLHeader.h"

//------------------------------------
//	Basic Node Class
//------------------------------------
class MapNode
{
public:
	//--------------------------------
	//	Contructors
	//--------------------------------
	MapNode() : m_Type(0), m_ID(0)
	{}
	~MapNode()
	{}

	//--------------------------------
	//	Functions
	//--------------------------------
	virtual bool Render()
	{
		return false;
	}
	//----------------
	//	Varibels
	//----------------
	CVector3 m_Pos;			// Position (Graphics)
	int m_Type;				// Type
	int m_ID;				// Logical ID
};

#endif