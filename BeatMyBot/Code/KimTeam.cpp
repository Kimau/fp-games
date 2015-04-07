/********************************************************************
	created:	11:11:2009   19:07
	file path:	c:\Dev\BeatMyBot\Code\KimTeam.cpp
	author:		Claire Blackshaw
	
	purpose:	
*********************************************************************/

//////////////////////////////////////////////////////////////////////////
///	Game Includes
#include "KimTeam.h"
#include <stdio.h>
#include <math.h>

KimTeam	KimTeam::s_inst;

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::KimTeam
KimTeam::KimTeam()
{
	m_bInit = FALSE;
	m_iTeamNum = 0;
	m_pTeamData = NULL;
	m_aNodes = NULL;
	m_nNodes = NULL;
	m_pMapData = NULL;
	m_GameState = NULL;
	m_teamlLeader = NULL;

	memset(m_aRobots, 0 , sizeof(m_aRobots));	
}

//////////////////////////////////////////////////////////////////////////
///	[virtual ] KimTeam::~KimTeam
KimTeam::~KimTeam()
{
	for(int iNode = 0; iNode < m_nNodes; ++iNode)
	{
		delete[] m_aNodes[iNode].m_EdgeArray;
	}
	delete[] m_aNodes;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::InitTeam
void KimTeam::InitTeam( MapData* pTheMapData, int teamNumber )
{
	if(m_bInit == TRUE)
	{
		return;
	}
	
	m_pMapData = pTheMapData;
	GenerateNodes(pTheMapData);

	m_pTeamData = NULL;
	m_iTeamNum = teamNumber;
	m_currTeamState = ts_RushFlag;
	m_bInit = TRUE;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::AddRobot
void KimTeam::AddRobot( KimBot* pBot, int iBotNum )
{
	m_aRobots[iBotNum] = pBot;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::Update
void KimTeam::Update( GameState* pTheGameState )
{
	// First Pass
	if(m_pTeamData == NULL)
	{
		m_pTeamData = &(pTheGameState->mrgTeams[m_iTeamNum]);
		m_GameState = pTheGameState;

		ShortestPath(pTheGameState->mrgFlags[1].mPosition, 0);
		EvalTeamTactic();
	}	

	switch (m_currTeamState)
	{
		case ts_RushFlag:
			UpdateRushFlag();
			break;
		case ts_WolfPack:
			UpdateWolfPack();
			break;
		case ts_Tortise:
			UpdateTortoise();
			break;
		case ts_SearchDestroy:
			UpdateHunt();
			break;
	}

	EvalTargets();
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::GenerateNodes
void KimTeam::GenerateNodes( MapData* pTheMapData )
{
	// Generate Nodes
	m_nNodes = pTheMapData->miNumBlocks * 4;
	m_aNodes = new MapNode[m_nNodes];
	memset(m_aNodes, 0, sizeof(MapNode) * m_nNodes);

	for(int iBlock = 0; iBlock < pTheMapData->miNumBlocks; ++iBlock)
	{
		// Get Position
		m_aNodes[iBlock*4 + 0].m_Position.XValue = pTheMapData->mrgRectangles[iBlock].mArea.left  - 15;
		m_aNodes[iBlock*4 + 1].m_Position.XValue = pTheMapData->mrgRectangles[iBlock].mArea.left  - 15;
		m_aNodes[iBlock*4 + 2].m_Position.XValue = pTheMapData->mrgRectangles[iBlock].mArea.right + 15;
		m_aNodes[iBlock*4 + 3].m_Position.XValue = pTheMapData->mrgRectangles[iBlock].mArea.right + 15;

		m_aNodes[iBlock*4 + 0].m_Position.YValue = pTheMapData->mrgRectangles[iBlock].mArea.top    - 15;
		m_aNodes[iBlock*4 + 1].m_Position.YValue = pTheMapData->mrgRectangles[iBlock].mArea.bottom + 15;
		m_aNodes[iBlock*4 + 2].m_Position.YValue = pTheMapData->mrgRectangles[iBlock].mArea.top    - 15;
		m_aNodes[iBlock*4 + 3].m_Position.YValue = pTheMapData->mrgRectangles[iBlock].mArea.bottom + 15;

		// Calculate Resource Value
		for(int iPoint = 0; iPoint < 4; ++iPoint)
		{
			double resVal = 0;
			Vector2D nodePos = m_aNodes[iBlock*4 + iPoint].m_Position;

			// Find Health packs which you can walk directly to divided by distance
			for(int iHealth = 0; iHealth < pTheMapData->miNumHealthPacks; ++iHealth)
			{
				if(pTheMapData->LineOfWalk(nodePos, pTheMapData->mrgHealthPacks[iHealth]) == TRUE)
				{
					resVal += 100 / pTheMapData->EffectiveWalkDistance(nodePos, pTheMapData->mrgHealthPacks[iHealth]);
				}
			}

			for(int iAmmo = 0; iAmmo < pTheMapData->miNumAmmoPacks; ++iAmmo)
			{
				if(pTheMapData->LineOfWalk(nodePos, pTheMapData->mrgAmmoPacks[iAmmo]) == TRUE)
				{
					resVal += 100 / pTheMapData->EffectiveWalkDistance(nodePos, pTheMapData->mrgAmmoPacks[iAmmo]);
				}
			}

			m_aNodes[iBlock*4 + iPoint].m_ResourceValue = resVal;
		}
	}

	// Check Edges
	int nEdge = 0;
	MapEdge scrathList[50] = { NULL };

	for(int aNode = 0; aNode < m_nNodes; ++aNode)
	{
		m_aNodes[aNode].m_nEdges = NULL;

		for(int bNode = 0; bNode < m_nNodes; ++bNode)
		{
			if(aNode == bNode)
			{
				continue;
			}
			
			// Check if Edge is already calculated 
			// By logic if we have tested AB then we have tested BA
			if(aNode > bNode) 
			{
				for(int iCompEdge = 0; iCompEdge < m_aNodes[bNode].m_nEdges; ++iCompEdge)
				{
					if(m_aNodes[bNode].m_EdgeArray[iCompEdge].m_Node == &(m_aNodes[aNode]))
					{
						scrathList[m_aNodes[aNode].m_nEdges].m_dist = m_aNodes[bNode].m_EdgeArray[iCompEdge].m_dist;
						scrathList[m_aNodes[aNode].m_nEdges].m_Node = &(m_aNodes[bNode]);
						m_aNodes[aNode].m_nEdges += 1;
					}
				}
			}
			else
			{
				// Check if edge is valid
				if(pTheMapData->LineOfWalk(m_aNodes[aNode].m_Position, m_aNodes[bNode].m_Position) == TRUE)
				{
					scrathList[m_aNodes[aNode].m_nEdges].m_dist = pTheMapData->EffectiveWalkDistance(m_aNodes[aNode].m_Position, m_aNodes[bNode].m_Position);
					scrathList[m_aNodes[aNode].m_nEdges].m_Node = &(m_aNodes[bNode]);
					m_aNodes[aNode].m_nEdges += 1;

					nEdge += 1;
				}
			}
		}

		// Create Copy of List
		m_aNodes[aNode].m_EdgeArray = new MapEdge[m_aNodes[aNode].m_nEdges];
		memcpy(m_aNodes[aNode].m_EdgeArray, scrathList, sizeof(MapEdge) * m_aNodes[aNode].m_nEdges);
	}

	// printf("Built % Nodes and % Edges", m_nNodes, nEdge);
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::NearestNode
KimTeam::MapNode* KimTeam::NearestNode( const Vector2D& testPos )
{
	double closest = -1;
	MapNode* pNode = NULL;

	for(int iNode = 0; iNode < m_nNodes; ++iNode)
	{
		double nodeDist = (m_aNodes[iNode].m_Position - testPos).magnitude();

		if((pNode == NULL) || (closest > nodeDist))
		{
			pNode = &(m_aNodes[iNode]);
			closest = nodeDist;
		}
	}

	return pNode;
}


//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ValidPath
bool KimTeam::ValidPath( const Vector2D& posA, const Vector2D& posB )
{
	return m_pMapData->LineOfWalk(posA, posB);
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ShortestPath
void KimTeam::ShortestPath( const Vector2D& destPos, int iBotNum )
{
	// Destination is always last node
	m_aRobots[iBotNum]->NewPath(destPos);

	// Special Case :: Check for straight Path no need for A*
	if(ValidPath(destPos, m_aRobots[iBotNum]->m_pCharData->mPosition) == TRUE)
	{
		return;
	}

	// Get Root and Goal
	MapNode* pFirstNode = NearestNode(m_aRobots[iBotNum]->m_pCharData->mPosition);
	MapNode* pGoalNode = NearestNode(destPos);

	// Special Case :: Start and End node are the same
	if(pFirstNode == pGoalNode)
	{
		m_aRobots[iBotNum]->AddPoint(pFirstNode->m_Position);
		return;
	}

	// Reset Data
	for(int iNode = 0; iNode < m_nNodes; ++iNode)
	{
		m_aNodes[iNode].m_bClosed = FALSE;
		m_aNodes[iNode].m_knownDist = 0;
		m_aNodes[iNode].m_estDist = 0;
	}
	m_pFrontier.clear();

	// Setup Route
	ATree rootNode(pFirstNode, NULL);	
	rootNode.AddOpenNodes(m_pFrontier, destPos);
	FrontierPoint* pShortest = FindShortestRoute();

	while(pShortest != NULL)
	{
		// Move to Closed List & Remove from Frontier
		ATree* pLeaf = new ATree(pShortest->m_Node, pShortest->m_Route);
		pShortest->m_Route->AddChild(pLeaf);
		m_pFrontier.remove(*pShortest);

		// Check if Goal Reached
		if(pLeaf->m_pNode == pGoalNode)
		{
			// Climb the Tree leaving breadcrumb trail for bot
			const ATree* treeClimber = pLeaf;
			while(treeClimber != NULL)
			{
				m_aRobots[iBotNum]->AddPoint(treeClimber->m_pNode->m_Position);
				treeClimber = treeClimber->m_pParent;
			}
			
			// JOB DONE!
			return; // Tree deconstructor cleans up the mess
		}

		// Add any new nodes to frontier
		pLeaf->AddOpenNodes(m_pFrontier, destPos);

		// Get Next Shortest Point
		pShortest = FindShortestRoute();
	}

	// FAILED
}


//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::FindShortestRoute
KimTeam::FrontierPoint* KimTeam::FindShortestRoute()
{
	FrontierPoint* pShortPoint = NULL;

	FrontList::iterator listIter;
	for(listIter = m_pFrontier.begin(); listIter != m_pFrontier.end(); ++listIter)
	{
		if( (pShortPoint == NULL) || 
			 (listIter->m_Node->m_estDist + listIter->m_Node->m_knownDist) < 
			 (pShortPoint->m_Node->m_estDist + pShortPoint->m_Node->m_knownDist))
		{
			pShortPoint = &(*listIter);
		}
	}

	return pShortPoint;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::SetTeamTactic
void KimTeam::SetTeamTactic( TeamState newState )
{
// 	printf("------------------------------------------------- New Team Tactic \n");

	FindTeamLocale();

	// Switch to State
	switch(newState)
	{
	case ts_RushFlag:
		{
//			printf("========= Rush Flag \n");

			Vector2D flagDirA = m_GameState->mrgFlags[0].mPosition - m_teamLocal;
			Vector2D flagDirB = m_GameState->mrgFlags[1].mPosition - m_teamLocal;

			if((m_GameState->mrgFlags[0].miTeamNo != m_iTeamNum) && (m_GameState->mrgFlags[1].miTeamNo != m_iTeamNum))
			{
				if(flagDirA.magnitude() < flagDirB.magnitude())
				{
					MoveToPoint(m_GameState->mrgFlags[0].mPosition);
				}
				else
				{
					MoveToPoint(m_GameState->mrgFlags[1].mPosition);
				}
			}
			// Flag A
			else if(m_GameState->mrgFlags[0].miTeamNo != m_iTeamNum)
			{			
				MoveToPoint(m_GameState->mrgFlags[0].mPosition);
			}
			// Flag B
			else if(m_GameState->mrgFlags[1].miTeamNo != m_iTeamNum)
			{			
				MoveToPoint(m_GameState->mrgFlags[1].mPosition);
			}
			else
			{
				EvalTeamTactic();
			}
		}
		break;

	case ts_WolfPack:
//		printf("========= Wolf Pack \n");
		break;

	case ts_Tortise:
//		printf("========= Roman Tortoise \n");
		break;

	case ts_SearchDestroy:
//		printf("========= Search and Destroy \n");
		break;
	}

	m_currTeamState = newState;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::FindTeamLocale
void KimTeam::FindTeamLocale()
{
	// Find Team Locale
	int LocalLeader[5] = { NULL };
	int LocalWeight[5] = { NULL };
	Vector2D LocalPos[5];
	int nLocals = 0;

	for(int iBot = 0; iBot < NOOF_ROBOTS; ++iBot)
	{
		if( (m_aRobots[iBot] == NULL) ||
			(m_aRobots[iBot]->IsDead() == TRUE))
		{
			// Skip Him
		}
		else
		{
			if(nLocals == 0)
			{
				LocalWeight[0] = 1;
				LocalPos[0] = m_aRobots[iBot]->m_pCharData->mPosition;
				LocalLeader[0] = iBot;
				nLocals = 1;
				//printf("Local Position Established [%i:%i] \n", LocalPos[0].XValue, LocalPos[0].YValue);
			}
			else
			{
				bool bFound = FALSE;
				for(int iLocal = 0; iLocal < nLocals; ++iLocal)
				{
					if(ManhattanDist(LocalPos[iLocal] - m_aRobots[iBot]->m_pCharData->mPosition) < 100)
					{
						// Assign to this Local
						bFound = TRUE;
						LocalWeight[iLocal] += 1;
						break;
					}
				}

				// Create New Local
				if((nLocals < 5) && (bFound == FALSE))
				{
					LocalWeight[nLocals] = 1;
					LocalPos[nLocals] = m_aRobots[iBot]->m_pCharData->mPosition;
					LocalLeader[nLocals] = iBot;
					//printf("Local Position Established [%i:%i] \n", LocalPos[nLocals].XValue, LocalPos[nLocals].YValue);
					nLocals += 1;
				}
			}
		}
	}

	// Find Team Local Point
	int nStrongPoint = 0;
	for(int iLocal = 0; iLocal < nLocals; ++iLocal)
	{
		if(LocalWeight[iLocal] > nStrongPoint)
		{
			m_teamlLeader = LocalLeader[nLocals];
			m_teamLocal = LocalPos[iLocal];
			nStrongPoint = LocalWeight[iLocal];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::EvalTeamTactic
void KimTeam::EvalTeamTactic()
{
	// Evaluate new State
	if( (m_GameState->mrgFlags[0].miTeamNo != m_iTeamNum) ||
		(m_GameState->mrgFlags[1].miTeamNo != m_iTeamNum))
	{
		// Flag Rush
		SetTeamTactic(ts_RushFlag);
	}
	else
	{
		SetTeamTactic(ts_WolfPack);
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::MoveToPoint
void KimTeam::MoveToPoint( const Vector2D& tarPoint )
{
	ShortestPath(tarPoint, m_teamlLeader);

	for(int iBot = 0; iBot < NOOF_ROBOTS; ++iBot)
	{
		if( (m_aRobots[iBot] != NULL) && 
			(m_aRobots[iBot]->IsDead() == FALSE) &&
			(iBot != m_teamlLeader))
		{
			// If you can see the leader then follow him
			if(ValidPath(m_aRobots[iBot]->m_pCharData->mPosition, m_aRobots[m_teamlLeader]->m_pCharData->mPosition) == TRUE)
			{
				m_aRobots[iBot]->m_nPathLength = m_aRobots[m_teamlLeader]->m_nPathLength;
				memcpy(m_aRobots[iBot]->m_currPath, m_aRobots[m_teamlLeader]->m_currPath, sizeof(Vector2D) * m_aRobots[m_teamlLeader]->m_nPathLength);
				m_aRobots[iBot]->AddPoint(m_aRobots[m_teamlLeader]->m_pCharData->mPosition);
			}
			// Can't see him plot your own path
			else
			{
				ShortestPath(tarPoint, iBot);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::UpdateHunt
void KimTeam::UpdateHunt()
{
	// Update Search and Destroy Tactic
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::UpdateTortoise
void KimTeam::UpdateTortoise()
{
	// Update Tortoise Tactic
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::UpdateWolfPack
void KimTeam::UpdateWolfPack()
{
	// Update Wolf Pack Tactic
	FindTeamLocale();

	int iPack = 0;
	while((iPack < m_GameState->miNumHealthPacks) &&
		  (m_GameState->mrgHealthPacks[iPack].mdSpawnTime > 0))
	{
		++iPack;
	}

	if(iPack < m_GameState->miNumHealthPacks)
	{
		MoveToPoint(m_GameState->mrgHealthPacks[iPack].mPosition);	
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::UpdateRushFlag
void KimTeam::UpdateRushFlag()
{
	// Update Rush Flag Tactic
	if( (m_GameState->mrgFlags[0].miTeamNo == m_iTeamNum) &&
		(m_GameState->mrgFlags[1].miTeamNo == m_iTeamNum))
	{
		EvalTeamTactic();
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ManhattanDist
double KimTeam::ManhattanDist( const Vector2D& line )
{
	return (fabs(line.XValue) + fabs(line.YValue));
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::EvalTargets
void KimTeam::EvalTargets()
{
	// Check for Hostiles and Valid Targets
	double iTargetScore[10] = { NULL };
	for(int iFriend = 0; iFriend < m_GameState->mrgTeams[m_iTeamNum].miNumCharacters; ++iFriend)
	{
		Character& rFriend = m_GameState->mrgTeams[m_iTeamNum].mrgMembers[iFriend];

		for(int iFoe = 0; iFoe < m_GameState->mrgTeams[!m_iTeamNum].miNumCharacters; ++iFoe)
		{
			Character& rFoe = m_GameState->mrgTeams[!m_iTeamNum].mrgMembers[iFoe];

			if(m_pMapData->LineOfSight(rFriend.mPosition, rFoe.mPosition) == TRUE)
			{
				iTargetScore[iFoe] += (10000.0 / m_pMapData->EffectiveShootDistance(rFriend.mPosition, rFoe.mPosition));
			}
		}
	}

	double iStrongScore = 1;
	int iBestTarget = 0;
	for(int iTarget = 0; iTarget < 10; ++iTarget)
	{
		if(iTargetScore[iTarget] > iStrongScore)
		{
			iStrongScore = iTargetScore[iTarget];
			iBestTarget = iTarget;
		}
	}

	if(iBestTarget > 0)
	{
		Character& rTarget = m_GameState->mrgTeams[!m_iTeamNum].mrgMembers[iBestTarget];

		for(int iFriend = 0; iFriend < m_GameState->mrgTeams[m_iTeamNum].miNumCharacters; ++iFriend)
		{
			m_aRobots[iFriend]->SetTarget(&rTarget, iBestTarget);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ATree::ATree
KimTeam::ATree::ATree( MapNode* pNode, const ATree* pParent )
{
	m_pNode = pNode; 
	pNode->m_bClosed = TRUE; 
	
	m_pParent = pParent; 
	m_pSibling = NULL; 
	m_pFirstChild = NULL;
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ATree::~ATree
KimTeam::ATree::~ATree()
{
	// These will cause a chain reaction
	// Removing the need for iteration
	delete m_pSibling;		
	delete m_pFirstChild; 
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ATree::AddOpenNodes
void KimTeam::ATree::AddOpenNodes( FrontList& rList, const Vector2D& destPos )
{
	for(int iEdge = 0; iEdge < m_pNode->m_nEdges; ++iEdge)
	{
		if(m_pNode->m_EdgeArray[iEdge].m_Node->m_bClosed == FALSE)
		{
			MapNode* pTarNode = m_pNode->m_EdgeArray[iEdge].m_Node;

			// Calculate Distances
			Vector2D crowFlies = destPos - pTarNode->m_Position;
			double knownDist = m_pNode->m_knownDist + m_pNode->m_EdgeArray[iEdge].m_dist;
			double estDist = ManhattanDist(crowFlies);

			// Is this node on the Frontier
			bool bFound = FALSE;
			FrontList::iterator listIter;
			for(listIter = rList.begin(); listIter != rList.end(); ++listIter)
			{
				// Check if a Match
				if(listIter->m_Node == pTarNode)
				{
					bFound = TRUE;

					// Check if a shorter path
					if((knownDist + estDist) < (pTarNode->m_knownDist + pTarNode->m_estDist))
					{
						listIter->m_Route = this;
						pTarNode->m_knownDist = knownDist;
						pTarNode->m_estDist = estDist;
					}
					break;
				}
			}

			// Node not on frontier add to list
			if(bFound == FALSE)
			{
				pTarNode->m_knownDist = knownDist;
				pTarNode->m_estDist = estDist;

				FrontierPoint fPoint;
				fPoint.m_Node = pTarNode;
				fPoint.m_Route = this;

				rList.push_back(fPoint);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
///	[] KimTeam::ATree::AddChild
void KimTeam::ATree::AddChild( ATree* pNewChild )
{
	// Safety Check
	pNewChild->m_pParent = this;
	
	if(m_pFirstChild == NULL)
	{
		m_pFirstChild = pNewChild;
	}
	else
	{
		ATree* pChildIter = m_pFirstChild;
		while(pChildIter->m_pSibling != NULL)
		{
			pChildIter = pChildIter->m_pSibling;
		}

		pChildIter->m_pSibling = pNewChild;
	}
}