#ifndef SCARY_VOID_LIST_OF_DOOM_42
#define SCARY_VOID_LIST_OF_DOOM_42

//----------------------------------------------------------------------------------
//	The Scary VOID list of Doom deserves some introduction
//	It is jsut a linked list of void pointers quite simple really.
//	VERY flexible and easy to write and use.
//	Not as well constructed as OO options and Data structures like Abstract Factories
//	The truth of the matter is DoomVoid gets the job done very well.
//----------------------------------------------------------------------------------
typedef void* VOID_PTR;

//----------------------------------------------
//	DoomVoid Item
//	Notice NO MEMORY management at all
//----------------------------------------------
class VoidItem
{
public:
	VoidItem(VOID_PTR _data) : m_data(_data), m_next(0), m_prev(0)	{}
	~VoidItem()	{}

	VOID_PTR m_data;
	VoidItem* m_next;
	VoidItem* m_prev;
private:
	VoidItem();
};

//----------------------------------------------
//	Void List of Doom
//	Notice no OBJ the reason for this is void
//	list of doom operates a LOT faster when
//	not compiled as an object. The reason
//	behind this is linking slows it down.
//	long story short.... it saves you MUCH grief
//----------------------------------------------
class VoidList
{
public:
	//------------------------------------------------
	//	Constructors and Deletion
	//------------------------------------------------
	VoidList() : m_head(0), m_tail(0), m_size(0), m_curr(0)	{}
	~VoidList()
	{
		while(m_head)
		{
			if(m_head->m_prev)		delete (m_head->m_prev);
			if(m_head->m_next)		m_head = m_head->m_next;
			else	{ delete m_head;	m_head = 0;	}
		}
	}

	//------------------------------------------------
	//	Accessors
	//------------------------------------------------
	VOID_PTR getHead() const		{return (m_head->m_data);}
	VOID_PTR getTail() const		{return (m_tail->m_data);}
	VOID_PTR getCurr() const		{return (m_curr->m_data);}
	int getSize() const		{return m_size;}

	//-----------------------------------------------
	// Remove Item
	//-----------------------------------------------
	bool remove(VOID_PTR _data)
	{
		//---------------------
		// Empty List Check
		//---------------------
		if(m_size)
		{
			//---------------------
			// Check Curr first
			// More Likely
			//---------------------
			if(m_curr->m_data == _data)
			{
				if(m_curr->m_prev)			m_curr->m_prev->m_next = m_curr->m_next;
				if(m_curr->m_next)			m_curr->m_next->m_prev = m_curr->m_prev;
				delete m_curr;
				return true;
			}

			//---------------------
			// Iter Through List
			//---------------------
			VoidItem* _curr = m_head;
			VoidItem* _next = 0;
			while(_curr)
			{
				_next = _curr->m_next;
				if(_curr->m_data == _data)
				{
					if(_curr->m_prev)	_curr->m_prev->m_next = _next;
					if(_next)			_next->m_prev = _curr->m_prev;
					delete _curr;
					return true;
				}
				_curr = _next;
			}
		}
		return false;
	}

	//---------------------
	// Select Item
	//---------------------
	bool select(VOID_PTR _data)
	{
		//---------------------
		// Empty List Check
		//---------------------
		if(m_size)
		{
			//---------------------
			// Check Curr first
			// More Likely
			//---------------------
			if(m_curr->m_data== _data)		return true;

			//---------------------
			// Iter Through List
			//---------------------
			VoidItem* _curr = m_head;
			VoidItem* _next = 0;
			while(_curr)
			{
				_next = _curr->m_next;
				if(_curr->m_data == _data)
				{
					m_curr = _curr;	
					return true;
				}
				_curr = _next;
			}
		}
		return false;
	}

	//---------------------
	// Get Unit
	//---------------------
	void add(VOID_PTR _data)
	{
		VoidItem* _new = new VoidItem(_data);
		if(m_size)
		{			
			_new->m_prev = m_tail;
			m_tail->m_next = _new;
			m_tail = _new;
		}
		else
		{
			m_tail = _new;
			m_head = _new;
			m_curr = _new;
		}
		++m_size;
	}

private:
	VoidItem* m_head;
	VoidItem* m_tail;
	VoidItem* m_curr;		// Currently Selected Item. Very usefull to speed things up
	int m_size;			// Size 
};

#endif