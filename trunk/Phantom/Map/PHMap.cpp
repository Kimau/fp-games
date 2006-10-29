#include "PHMap.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			CONTRUCTORS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//		Blank Matrix
//==========================================================================
matrixBlock::matrixBlock()
{
	m_State = -1;
}
//==========================================================================
//		Clean up After Matrix
//==========================================================================
matrixBlock::~matrixBlock()
{
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		INTERFACE FUNCTIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//		Generate a Dorment Matrix
//==========================================================================
int matrixBlock::Open()
{
	if(m_State < 0)
	{
		//----------------------------------------
		//	Setup Camera
		//----------------------------------------
		m_Camera.setCamera(15,0,0, 0,0,0, 0,0,1);
		m_Camera.m_spPercent = 0.001f;
		m_Camera.m_spUnit = 1.0f;
		m_Camera.m_progress = 1.1f;

		PH_LIGHT->AddLight();
		PH_LIGHT->m_lights[0].m_linear_attenuation = 2.5f;

		//----------------------------------------
		//	Load Models
		//----------------------------------------
		{ BGLModel3ds _temp;		_temp.Import3ds("Data/nCPU.3ds");		m_ModelList.push_back(_temp.getList());}
		{ BGLModel3ds _temp;		_temp.Import3ds("Data/nSPU.3ds");		m_ModelList.push_back(_temp.getList());}
		{ BGLModel3ds _temp;		_temp.Import3ds("Data/nPROC.3ds");		m_ModelList.push_back(_temp.getList());}
		{ BGLModel3ds _temp;		_temp.Import3ds("Data/nBRANCH.3ds");	m_ModelList.push_back(_temp.getList());}
		{ BGLModel3ds _temp;		_temp.Import3ds("Data/nDATA.3ds");		m_ModelList.push_back(_temp.getList());}
		{ BGLModel3ds _temp;		_temp.Import3ds("Data/nPORT.3ds");		m_ModelList.push_back(_temp.getList());}
		//----------------------------------------
		//	Basic Varibels
		//----------------------------------------
		m_log = 0.0f;
		m_alert = 0.0f;

		//----------------------------------------
		//	Gen Tree
		//----------------------------------------
		m_Count = 0;
		GenTree();

		updateCam();

		PH_LOG->Print(LG_STATE_REPORT,"Matrix Opened");
		m_State = 1;
	}
	return m_State;
}
//==========================================================================
//		Take A Step In Game
//==========================================================================
int matrixBlock::Step()
{
	//-----------------------------
	//	Check State
	//-----------------------------
	if(m_State > 0)
	{
		// Logic
		m_Camera.UpdateUnit(x_delta);
		m_Selection = getMouseCollision();

		// Render
		Render();
	}
	return m_State;
}
//==========================================================================
//		Process Input
//==========================================================================
int matrixBlock::Input(UINT _iMsg, WPARAM wParam, LPARAM lParam)
{
	//-----------------------------
	//	Check State
	//-----------------------------
	if(m_State > 0)
	{
		switch(_iMsg)
		{
			//-----------------------------
			// Mouse Movement
			//-----------------------------
		case WM_MOUSEMOVE:
			{
				m_Mouse = CVector2(LOWORD(lParam),HIWORD(lParam));
				return 1;
			}
		case WM_LBUTTONDOWN:
			{
				return 1;
			}
		case WM_LBUTTONUP:
			if(m_Selection)
			{
				m_pLocation = m_Selection;
				if(m_pLocation->m_child.empty())
					m_pSelection = -1;
				else
					m_pSelection = 0;

				PH_LOG->PrintS(LG_ACTION_REPORT,"Mouse Pick: ID: %i", (m_pLocation->m_item.m_ID));
				updateCam(true);
			}
			return 1;
		case WM_RBUTTONDOWN:
			{
				return 1;
			}
		case WM_RBUTTONUP:
			{
				return 1;
			}

			//-----------------------------
			//	Key Press
			//-----------------------------
			case WM_KEYUP:
			{
				switch(wParam)
				{
				case 'w':
				case 'W':
				case 's':
				case 'S':
					return 1;
				case 'a':
				case 'A':
				case 'd':
				case 'D':
					return 1;
				//---------------------------
				//	Move to Selection
				//---------------------------
				case VK_UP:
					if(m_pSelection >= 0)
					{
						m_pLocation = m_pLocation->m_child[m_pSelection];
						if(m_pLocation->m_child.empty())
							m_pSelection = -1;
						else
							m_pSelection = 0;

						PH_LOG->PrintS(LG_ACTION_REPORT,"ID: %i Selection: %i", (m_pLocation->m_item.m_ID), (m_pSelection));
						updateCam(true);
					}
					return 1;
				//---------------------------
				//	Move to Parent
				//---------------------------
				case VK_DOWN:
					if(m_pLocation->m_parent)
					{
						m_pLocation = m_pLocation->m_parent;
						m_pSelection = 0;

						PH_LOG->PrintS(LG_ACTION_REPORT,"Level: %i Selection: %i", (m_pLocation->m_level), (m_pSelection));
						updateCam(true);
					}
					return 1;
				//---------------------------
				//	Select Next Child
				//---------------------------
				case VK_LEFT:
					if(m_pSelection >= 0)
					{
						++m_pSelection;
						if(m_pSelection == m_pLocation->m_child.size())
							m_pSelection = 0;

						PH_LOG->PrintS(LG_ACTION_REPORT,"Level: %i Selection: %i", (m_pLocation->m_level), (m_pSelection));
						updateCam(true);
					}
					return 1;

				case VK_ESCAPE:
					return -1;
				};
				break;
			}
			case WM_KEYDOWN:
			{
				switch(wParam)
				{
					//-----------------------------------
					//	 Orbit Position
					//-----------------------------------
				case 'w':
				case 'W':					
					m_Camera.RotatePos(1.0f, m_Camera.getLatitude());
					return 1;
				case 's':
				case 'S':
					m_Camera.RotatePos(-1.0f, m_Camera.getLatitude());
					return 1;
				case 'a':
				case 'A':
					m_Camera.RotatePos(1.0f,CVector3(0,0,1));
					return 1;
				case 'd':
				case 'D':
					m_Camera.RotatePos(-1.0f,CVector3(0,0,1));
					return 1;
				};
				break;
			}
		};
	}
	return 0;
}
//==========================================================================
//		Exit Game Block
//==========================================================================
int matrixBlock::Exit()
{
	return m_State;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			GENERATION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//	Generate Tree (Useless without Seed)
//==========================================================================
void matrixBlock::GenTree()
{
	//-------------------------
	//	Seed Random
	//-------------------------
	srand(GetTickCount());

	//-------------------------
	//	Clear Nodes
	//-------------------------
	m_NodePTR.clear();
	m_PosPTR.clear();

	//-------------------------
	//	Generate Tree
	//-------------------------
	m_NodePTR.push_back(&(m_tree.m_item));
	m_PosPTR.push_back(CVector3(0,0,0));
	GenBranch(&m_tree, CVector3(0,0,0));

	//-------------------------
	//	Set Selection
	//-------------------------
	m_pLocation = &m_tree;
	m_pSelection = -1;
	if(m_pLocation->m_child.empty())
		m_pSelection = -1;
	else
		m_pSelection = 0;

	//-------------------------
	//	Balance Load
	//-------------------------
	BalanceLoad();

	//-------------------------
	//	Place Portals
	//-------------------------
	bool _flag = true;
	NodeTree* _curr = &m_tree;
	while(_flag)
	{
		int _maxDepth = 0;
		int _size = int(_curr->m_child.size());
		NodeTree* _next = 0;
		if(_size > 0)
		{
			for(int c = 0; c < _size; ++c)
			{
				int _depth = _curr->m_child[c]->depth() + 1;
				if(_maxDepth < _depth)
				{
					_next = _curr->m_child[c];
					_maxDepth = _depth;
				}
			}
			_curr = _next;
		}
		//-----------------------------------
		//	Tag Portal at Furthest Corner
		//-----------------------------------
		else
		{
			NodeTree* _tree = new NodeTree(_curr);
			_tree->m_item.m_ID = m_Count++;
			_tree->m_item.m_Type = PH_PORTAL;
			_tree->m_item.m_Pos.z = PH_NODE_DROP;

			_flag = false;
			_curr->m_child.push_back(_tree);
		}
	}

	//-------------------------
	//	Clear Nodes
	//-------------------------
	m_NodePTR.clear();
	m_PosPTR.clear();
}
//==========================================================================
//	Generate Branch
//==========================================================================
void matrixBlock::GenBranch(NodeTree* _parent, CVector3 _basePos)
{
	//-------------------------------------
	//	Node ID
	//-------------------------------------
	MapNode& _item = _parent->m_item;
	_item.m_ID = m_Count++;

	//-------------------------------------
	//	Node Type
	//-------------------------------------
	if(_parent->m_parent)
	{
		switch(_parent->m_parent->m_item.m_Type)
		{
		case PH_CPU:
			switch(rand() % 4)
			{
			case 0:	
			case 1:	
					_item.m_Type = PH_SUB;		break;
			case 2:	_item.m_Type = PH_PROC;		break;
			case 3:	_item.m_Type = PH_BRANCH;	break;
			};
			break;
		case PH_SUB:		
			switch(rand() % 3)
			{
			case 0:	_item.m_Type = PH_DATA;		break;
			case 1:	_item.m_Type = PH_BRANCH;	break;
			case 2:	_item.m_Type = PH_PROC;		break;
			};
			break;
		case PH_PROC:		
			switch(rand() % 3)
			{
			case 0:	
			case 1:
					_item.m_Type = PH_DATA;		break;
			case 2:	_item.m_Type = PH_PROC;		break;
			};
			break;
		case PH_BRANCH:	
			switch(rand() % 5)
			{
			case 0:	
			case 1:
					_item.m_Type = PH_SUB;		break;
			case 2:
			case 3:
					_item.m_Type = PH_DATA;		break;			
			case 4:	_item.m_Type = PH_BRANCH;	break;
			};
			break;
		default:
			_item.m_Type = -1;	
			break;
		};
	}
	else
		_item.m_Type = PH_CPU;

	//---------------------------------
	//	Calculate Child Count
	//---------------------------------
	int _childCount = 0;
	if((_item.m_Type < 9) && (_parent->m_level < 4))
		_childCount = 6;	
	//---------------------------------
	//	Store Pointers For LoadBalance
	//---------------------------------
	m_NodePTR.push_back(&(_item));
	m_PosPTR.push_back(_basePos);
	//-------------------------------------
	//	Generate children
	//-------------------------------------
	if(_childCount)
	{
		float _deg  = float(rand() % 360);
		_childCount = min(4 - _parent->m_level,_childCount);
		//-------------------------------------
		//	Build Branches
		//-------------------------------------
		for(int c = 0; c < _childCount; ++c)
		{
			_parent->m_child.push_back(new NodeTree(_parent));
			
			//--------------------------------
			//	Set Position for Child
			//--------------------------------
			CVector3& _pos = _parent->m_child[c]->m_item.m_Pos;
			
			float _rise = -10.0f * _parent->m_level;
			_pos = CVector3(1.0f,0.0f,0.0f);
			_pos.RotateY(_rise);
			_pos.RotateZ(_deg);
			_deg += float(rand() % 180 + 180) / float(_childCount);
			_pos.z = PH_NODE_DROP;
			CVector3 _newPos = _basePos + _pos;

			//--------------------------------
			//	Call Recursive Function
			//--------------------------------
			GenBranch(_parent->m_child[c],_newPos);
		}
	}
}
//==========================================================================
//	Generate Branch
//==========================================================================
void matrixBlock::BalanceLoad()
{
	int _count = int(m_NodePTR.size());

	bool _flag = true;
	//------------------------------------
	//	Loop Until No Movement takes Place
	//------------------------------------
	while(_flag)
	{
		_flag = false;
		//------------------------------------
		//	Iterate Through Points
		//------------------------------------
		for(int c = 1; c < _count; ++c)
		{
			CVector3 _move(0,0,0);
			CVector3 _start = m_PosPTR[c];

			//------------------------------------
			//	Calculate Movement
			//------------------------------------
			for(int i = 0; i < _count; ++i)
			{
				if(c != i)
				{
					CVector3 _temp = _start - m_PosPTR[i];
					float _dist = _temp.Magnitude();
					if(_dist < PH_NODE_SEPERATION)
					{
						_move += _temp.getNormalized() * _dist;
					}
				}
			}

			//------------------------------------
			//	Move Piece if movement 
			//------------------------------------
			float _size = _move.Magnitude();
			if(_size > 0.1f)
			{
				_move.z = 0.0f;
				if(_size > 1.0f)
					_move.Normalize();
				m_PosPTR[c] += _move;
				m_NodePTR[c]->m_Pos += _move;
				_flag = true;
			}
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			FILE MANAGEMENT
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//	Load Tree from File
//==========================================================================
bool matrixBlock::LoadTree(char* _file)
{
	return false;
}
//==========================================================================
//	Save Tree to File
//==========================================================================
bool matrixBlock::SaveTree(char* _file) const
{
	return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			NAVIGATION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//	Navigate to a different Point
//==========================================================================
bool matrixBlock::NavGoto(NodeTree* _branch)
{
	if(_branch)
	{
		m_pLocation = _branch;
		return true;
	}
	return false;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			DISPLAY
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//	Render Game
//==========================================================================
void matrixBlock::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	m_Camera.LookAt();

	//---------------------------
	//	Render Tree
	//---------------------------
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(0.1f);
	RenderBranch(&m_tree);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	ShowCursor(true);
	//---------------------------
	//	Render Log
	//---------------------------
	PH_LOG->setPos(700,0);
	PH_LOG->RenderFlat();

	//-----------------------------------------------------------------------------------------------------
	//	SWAP BUFFERS
	//-----------------------------------------------------------------------------------------------------
	PH_ENGINE->SwapGL_Buffers();
}
//==========================================================================
//	Render Branch
//==========================================================================
void matrixBlock::RenderBranch(NodeTree* _branch)
{
	const int& _level = _branch->m_level;
	CVector3 _pos = _branch->m_item.m_Pos;
	
	if(_level > 0)
	{
		glVertex3f(_pos.x,_pos.y,_pos.z);
		glEnd();
	}

	//---------------------------------------
	//	Render Node
	//---------------------------------------
	glPushMatrix();
		glTranslatef(_pos.x,_pos.y,_pos.z);

		switch(_level)
		{
		case 0:
			glColor3f(1.0f,1.0f,1.0f);
			break;
		case 1:
			glColor3f(0.0f,0.0f,1.0f);
			break;
		case 2:
			glColor3f(0.0f,1.0f,0.0f);
			break;
		case 3:
			glColor3f(1.0f,0.0f,0.0f);
			break;
		case 4:
			glColor3f(1.0f,0.0f,1.0f);
			break;
		};			

		//---------------------------------------
		//	Place Light at Selection Node
		//---------------------------------------
		if(_branch == m_Selection)
		{
			PH_LIGHT->m_lights[0].m_position[0] = _pos.x;
			PH_LIGHT->m_lights[0].m_position[1] = _pos.y;
			PH_LIGHT->m_lights[0].m_position[2] = _pos.z;
			PH_LIGHT->m_lights[0].m_position[3] = 1.0f;
			PH_LIGHT->update(0);
			glDisable(GL_LIGHTING);
		}
		else
			glEnable(GL_LIGHTING);

		glPushMatrix();
			glScalef(PH_NODE_SIZE,PH_NODE_SIZE,PH_NODE_SIZE);
			switch(_branch->m_item.m_Type)
			{
			case PH_CPU:	glCallList(m_ModelList[0]);		break;
			case PH_SUB:	glCallList(m_ModelList[1]);		break;
			case PH_PROC:	glCallList(m_ModelList[2]);		break;
			case PH_BRANCH:	glCallList(m_ModelList[3]);		break;
			case PH_DATA:	glCallList(m_ModelList[4]);		break;
			case PH_PORTAL:	glCallList(m_ModelList[5]);		break;
			};
			glDisable(GL_LIGHTING);
		glPopMatrix();
		//---------------------------------------
		//	Render Sub-Branches
		//---------------------------------------
		for(int c = int(_branch->m_child.size() - 1); c >= 0; --c)
		{
			if((m_pSelection >= 0) && (_branch->m_child[c] == m_pLocation->m_child[m_pSelection]))
			{
				glLineWidth(5.0f);
				glColor3f(1.0f,1.0f,1.0f);
			}
			else
			{
				glLineWidth(0.5f);
				glColor3f(0.2f,0.2f,0.2f);
			}

			glBegin(GL_LINES);
				glVertex3f(0,0,0);

			RenderBranch(_branch->m_child[c]);
		}
	glPopMatrix();
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			HELPER
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//==========================================================================
//	Get Real Location of Node
//==========================================================================
CVector3 matrixBlock::getRealPos(NodeTree* _branch)
{
	NodeTree* _curr = _branch;
	CVector3 _total(0,0,0);
	while(_curr)
	{
		_total += _curr->m_item.m_Pos;
		_curr = _curr->m_parent;
	}

	return _total;
}
//==========================================================================
//	Update the Camera
//==========================================================================
void matrixBlock::updateCam(bool _anim)
{
	CVector3 _loc = getRealPos(m_pLocation);
	m_Camera.setEyeVector(_loc.x,_loc.y,_loc.z,_anim);

	if(m_pLocation->m_child.empty())
	{
		m_Camera.setEyeVector(_loc.x,_loc.y,_loc.z,_anim);
		m_Camera.setPosition(_loc.x - 3.0f, _loc.y - 3.0f, _loc.z + 3.0f,_anim);
	}
	else
	{
		CVector3 _eye = getRealPos(m_pLocation->m_child[m_pSelection]);
		CVector3 _temp = _loc - _eye;
		_temp.Normalize();
		_temp *= 3.0f;
		_temp = _loc + _temp;
		m_Camera.setPosition(_temp.x,_temp.y,_temp.z,_anim);
	}
}
//==========================================================================
//	Calculate Mouse collision
//==========================================================================
NodeTree* matrixBlock::getMouseCollision()
{
	CVector3 _dir(BGLHelper::UnProject(m_Mouse));

	//-----------------------------
	//	Iterate through Nodes
	//-----------------------------
	float _minDist = 999999.0f;
	CVector3 _basePos(-m_Camera.m_current[0],-m_Camera.m_current[1],-m_Camera.m_current[2]);

	NodeTree* _closest = getMouseCollision(&m_tree,_minDist,_dir,_basePos);

	return _closest;
}

//==========================================================================
//	Calculate Mouse collision for Branch
//==========================================================================
NodeTree* matrixBlock::getMouseCollision(NodeTree* _branch, float& _minDist, CVector3 _dir, CVector3 _basePos)
{
	//--------------------------
	//	Check Node
	//--------------------------
	_basePos += _branch->m_item.m_Pos;
	NodeTree* _curr = 0;

	float _dist = _dir.DistanceFast(_basePos);
	if((_dist < _minDist) && (m_pLocation != _branch))
	{
		_minDist = _dist;
		_curr = _branch;
	}

	//--------------------------
	//	Check Children
	//--------------------------
	int _size = int(_branch->m_child.size());
	if(_size > 0)
	{
		for(int c = (_size - 1); c >= 0; --c)
		{
			NodeTree* _new = getMouseCollision(_branch->m_child[c],_minDist,_dir,_basePos);
			if(_new)
				_curr = _new;
		}
	}

	return _curr;
}