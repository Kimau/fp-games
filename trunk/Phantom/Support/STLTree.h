#ifndef SIMPLE_STL_TREE_343
#define SIMPLE_STL_TREE_343

//--------------------------
//	Includes
//--------------------------
#include <vector>

//--------------------------
//	Simple STL Tree
//--------------------------
template<class SimpleTreeItem> class SimpleTree
{
public:
	SimpleTree() : m_level(0), m_parent(0)
	{
	}
	SimpleTree(SimpleTree* _parent) : m_level(_parent->m_level + 1), m_parent(_parent)
	{
	}

	~SimpleTree()
	{
		for(int c = int(m_child.size() -1); c >= 0; --c)
			delete m_child[c];
	}
	//------------------------
	//	Inex Operator
	//------------------------
	SimpleTree* operator[](int _index) const
	{
		return m_child[_index];
	}
	//------------------------
	//	Branch Count
	//------------------------
	int branches() const
	{
		return int(m_child.size() -1);
	}
	//------------------------
	//	Depth
	//------------------------
	int depth() const
	{
		int _size = int(m_child.size());
		int _maxDepth = 0;
		if(m_child.size() > 0)
		{
			for(int c = 0; c < _size; ++c)
				_maxDepth = max(_maxDepth,m_child[c]->depth() + 1);
		}
		
		return _maxDepth;
	}

	//------------------------
	//	Varibles
	//------------------------
	int m_level;						// Tree Level (0 = Root)
	SimpleTree* m_parent;				// Parent Node (0 = Root / Blind)
	SimpleTreeItem m_item;				// Node Data
	std::vector<SimpleTree*> m_child;	// Children
};

#endif