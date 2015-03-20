#ifndef THIS_IS_THE_DYNAMIC_unit_LIST_HEADER_FILE_FOR_JOW
#define THIS_IS_THE_DYNAMIC_unit_LIST_HEADER_FILE_FOR_JOW

#include "Structs.h"
#include "BaseUnit.h"

class JWL_DUnit
{
public:
	JWL_DUnit(JWL_Unit* _unit) : m_unit(_unit), m_next(0), m_prev(0)
	{}
	~JWL_DUnit()
	{}

	JWL_Unit* m_unit;
	JWL_DUnit* m_next;
	JWL_DUnit* m_prev;
private:
	JWL_DUnit();
};

class JWL_DUnitList
{
public:
	JWL_DUnitList() : m_head(0), m_tail(0), m_size(0), m_curr(0)
	{}
	~JWL_DUnitList()
	{
		while(m_head)
		{
			if(m_head->m_prev)		delete (m_head->m_prev);
			if(m_head->m_next)		m_head = m_head->m_next;
			else	{ delete m_head;	m_head = 0;	}
		}
	}

	JWL_Unit& getHead()		{return *(m_head->m_unit);}
	JWL_Unit& getTail()		{return *(m_tail->m_unit);}
	JWL_Unit& getCurr()		{return *(m_curr->m_unit);}
	UINT getSize()			{return m_size;}

	//---------------------
	// Remove Unit
	//---------------------
	bool remUnit(JWL_Unit* _unit)
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
			if(m_curr->m_unit == _unit)
			{
				if(m_curr->m_prev)			m_curr->m_prev->m_next = m_curr->m_next;
				if(m_curr->m_next)			m_curr->m_next->m_prev = m_curr->m_prev;
				delete m_curr;
				return true;
			}

			//---------------------
			// Iter Through List
			//---------------------
			JWL_DUnit* _curr = m_head;
			JWL_DUnit* _next = 0;
			while(_curr)
			{
				_next = _curr->m_next;
				if(_curr->m_unit == _unit)
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
	// Select Unit
	//---------------------
	bool selUnit(JWL_Unit* _unit)
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
			if(m_curr->m_unit == _unit)		return true;

			//---------------------
			// Iter Through List
			//---------------------
			JWL_DUnit* _curr = m_head;
			JWL_DUnit* _next = 0;
			while(_curr)
			{
				_next = _curr->m_next;
				if(_curr->m_unit == _unit)
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
	void addUnit(JWL_Unit* _unit)
	{
		JWL_DUnit* _new = new JWL_DUnit(_unit);
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
	JWL_DUnit* m_head;
	JWL_DUnit* m_tail;
	JWL_DUnit* m_curr;
	UINT m_size;
};

#endif