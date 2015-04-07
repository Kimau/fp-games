/********************************************************************
	created:	11:11:2009   19:07
	file path:	c:\Dev\BeatMyBot\Includes\KimTeam.h
	author:		Claire Blackshaw
	
	purpose:	Team Management Software
*********************************************************************/

#ifndef HEADER_INCLUDE_KIMTEAM
#define HEADER_INCLUDE_KIMTEAM

// Base Define
#include "KimBot.h"
#include <list>

///-------------------------------------
/// KimTeam
///
/// Brief: Team Manager
///
///-------------------------------------
class KimTeam
{
public:
	//----------------------------------------------------- Public Statics
	class MapNode;
	class ATree;

	class MapEdge
	{
	public:
		double		m_dist;
		MapNode*	m_Node;
	};

	class MapNode 
	{
	public:
		// Node Data
		Vector2D	m_Position;
		double		m_ResourceValue;

		// Edge Data
		int			m_nEdges;
		MapEdge*	m_EdgeArray;		

		// A* Data
		bool		m_bClosed;
		double		m_knownDist;
		double		m_estDist;
	};

	struct FrontierPoint
	{
		ATree*		m_Route;
		MapNode*	m_Node;

		bool operator == ( const FrontierPoint& other ) const
		{
			return (m_Node == other.m_Node);
		}
	};

	typedef std::list<FrontierPoint> FrontList;

	class ATree
	{
	public:
		ATree(MapNode* pNode, const ATree* pParent);
		~ATree();

		void AddOpenNodes(FrontList& rList, const Vector2D& destPos);
		void AddChild(ATree* pNewChild);

		MapNode* m_pNode;
		const ATree* m_pParent;	// Parent
		ATree* m_pSibling;		// Next Sibling (Null terminated Linked List)
		ATree* m_pFirstChild;	// First Child (Null terminated Linked List)
	};

	//----------------------------------------------------- Public Functions
					KimTeam();
	virtual 		~KimTeam();

	void			InitTeam( MapData* pTheMapData, int teamNumber );
	void			AddRobot( KimBot* pBot, int iBotNum );

	void			Update( GameState* pTheGameState );

	void			ShortestPath( const Vector2D& destPos, int iBotNum );	
	bool			ValidPath( const Vector2D& posA, const Vector2D& posB);
	void			EvalTeamTactic();

	static double	ManhattanDist( const Vector2D& line );

	const MapData*	GetMap()  { return m_pMapData; }

	static KimTeam	s_inst;

protected:
	//----------------------------------------------------- Protected Functions
private:
	//----------------------------------------------------- Private Statics
	static const int NOOF_ROBOTS = 6;

	enum TeamState
	{
		ts_RushFlag,
		ts_WolfPack,
		ts_Tortise,
		ts_SearchDestroy,
		NOOF_TEAM_STATES
	};

	//----------------------------------------------------- Private Functions
	void			GenerateNodes( MapData* pTheMapData );
	MapNode*		NearestNode( const Vector2D& testPos );
	FrontierPoint*	FindShortestRoute();
	void			ClearFrontier();
	void			SetTeamTactic( TeamState newState );
	void			MoveToPoint( const Vector2D& tarPoint );
	void			FindTeamLocale();

	void			UpdateHunt();
	void			UpdateTortoise();
	void			UpdateWolfPack();
	void			UpdateRushFlag();

	void			EvalTargets();
	//----------------------------------------------------- Private Variables

	KimBot*			m_aRobots[NOOF_ROBOTS];
	int				m_iTeamNum;
	Team*			m_pTeamData;
	int				m_nNodes;
	bool			m_bInit;

	int				m_teamlLeader;
	Vector2D		m_teamLocal;

	TeamState		m_currTeamState;
	MapNode*		m_aNodes;
	const MapData*	m_pMapData;
	GameState*		m_GameState;

	FrontList		m_pFrontier;
};

#endif // HEADER_INCLUDE_KIMTEAM