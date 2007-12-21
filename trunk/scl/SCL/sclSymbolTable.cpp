#include "sclSymbolTable.h"

//============================================================================
//  Constructor
//============================================================================
sclSymTable::sclSymTable()
{
	m_print = ErrorPrinter::Instance();
}

//============================================================================
//  Deconstructor
//============================================================================
sclSymTable::~sclSymTable()
{
	/*----------------------------------
		 Free Memory from Map
	 ----------------------------------*/
	for(SymMap::iterator _iter = m_SymbolMap.begin(); _iter != m_SymbolMap.end(); ++_iter)
	{
		delete((_iter->second)->m_data);
		delete((_iter->second)->m_ident);
		delete(_iter->second);
	}
}

//============================================================================
//  Generate Symbol Table
//============================================================================
sclLexTok* sclSymTable::Generate(sclLexTok* _head)
									// *IN* The First Token to read
{
	while(_head)
	{
		/*----------------------------------
			 Read In Symbol from Tokens
		 ----------------------------------*/
		sclSymbol* _symbol = ReadSymbol(_head);
		/*----------------------------------
			 Check for EOF Table
		 ----------------------------------*/
		if(_symbol == 0)
		{
			return _head;
		}
		/*----------------------------------
			 Check if exists
		 ----------------------------------*/
		if(m_SymbolMap[_symbol->m_ident])
		{
			m_print->Print(23,"Already Exists");
		}
		/*----------------------------------
			 Add Symbol
		 ----------------------------------*/
		m_SymbolMap[_symbol->m_ident] = _symbol;
	}

	return 0;	// Reached end of Token
}

//============================================================================
//  Lookup Symbol on Table
//============================================================================
sclSymbol* sclSymTable::operator[](const char* _ident) const 
									// *IN* Ident to check
{
	return 0;
}

//============================================================================
//  Loop through the Table
//============================================================================
sclSymbol* sclSymTable::getNext(bool _start /* = false */) const
								// *IN* Used to restart the loop
{
	static SymMap::const_reverse_iterator s_iter = m_SymbolMap.rend();

	/*----------------------------------
		 Check if new Loop
	 ----------------------------------*/
	if(_start)
	{
		s_iter = m_SymbolMap.rbegin();
	}

	/*----------------------------------
		 End of Map
		Return Null
	 ----------------------------------*/
	if(s_iter == m_SymbolMap.rend())
	{
		return 0;
	}
	/*----------------------------------
		 Return Current
		 Move to Next
	 ----------------------------------*/
	else
	{
		sclSymbol* _ret = s_iter->second;
		++s_iter;
		return _ret;
	}
}

//============================================================================
//  Read Symbol from Tokens
//============================================================================
sclSymbol* sclSymTable::ReadSymbol( sclLexTok*& _head )
{
	/*----------------------------------
		 Create New Symbol
	 ----------------------------------*/
	sclSymbol* _symbol = new sclSymbol;
	_symbol->m_type = sclSymbol::NULL_TYPE;
	_symbol->m_data = 0;
	_symbol->m_ident = 0;

	/*----------------------------------
		 LET
	 ----------------------------------*/
	if(_head->m_tag != sclLexTok::LET)
	{
		delete _symbol;	// No Symbol Needed
		return 0;		// Return
	}
	_head = _head->m_next;	// Get Next Token
	/*----------------------------------
		 TYPE
	 ----------------------------------*/
	switch(_head->m_tag)
	{
	case sclLexTok::BOOL_TYPE :	
		_symbol->m_type = sclSymbol::BOOL_TYPE;	
		break;
	case sclLexTok::INT_TYPE :
		_symbol->m_type = sclSymbol::INT_TYPE;	
		break;
	case sclLexTok::FLOAT_TYPE :
		_symbol->m_type = sclSymbol::FLOAT_TYPE;	
	    break;
	case sclLexTok::STRING_TYPE :
		_symbol->m_type = sclSymbol::STRING_TYPE;	
	    break;
	default:
		m_print->Print(2,"Unknown Type");
	    break;
	}
	_head = _head->m_next;	// Get Next Token
	/*----------------------------------
		 IDENT
	 ----------------------------------*/
	if(_head->m_tag != sclLexTok::IDENT)
	{
		m_print->Print(5,"Expected IDENT");
	}
	else
	{
		int _length = (int)(strlen((char*)_head->m_data));
		char* _ident = new char[_length+1];
		strncpy(_ident,(char*)(_head->m_data),_length);
		_ident[_length] = 0;
		_symbol->m_ident = _ident;
	}
	_head = _head->m_next;	// Get Next Token
	/*----------------------------------
		 EQUALS
	 ----------------------------------*/
	if(_head->m_tag != sclLexTok::ASSIGN)
	{
		m_print->Print(5,"Expected ASSIGN");
	}
	_head = _head->m_next;	// Get Next Token
	/*----------------------------------
		 VALUE
	 ----------------------------------*/
	switch(_head->m_tag)
	{
		/*----------------------------------
			 Get Bool Literal
		 ----------------------------------*/
	case sclLexTok::BOOL_LIT :	
		if(_symbol->m_type == sclSymbol::BOOL_TYPE)
		{
			bool* _value = new bool;
			*_value = (_head->m_sub == sclLexTok::LIT_TRUE);
			_symbol->m_data = _value;
		}
		else
		{
			m_print->Print(6,"BOOL LIT Expected");
		}
		break;
		/*----------------------------------
			 Get Int Literal
		 ----------------------------------*/
	case sclLexTok::INT_LIT :
		if(_symbol->m_type == sclSymbol::INT_TYPE)
		{
			_symbol->m_data = new int;
			memcpy(_symbol->m_data,_head->m_data,sizeof(int));
		}
		else
		{
			m_print->Print(6,"INT LIT Expected");
		}
		break;
		/*----------------------------------
			 Get Float Literal
		 ----------------------------------*/
	case sclLexTok::FLOAT_LIT :
		if(_symbol->m_type == sclSymbol::FLOAT_TYPE)
		{
			_symbol->m_data = new float;
			memcpy(_symbol->m_data,_head->m_data,sizeof(float));
		}
		else
		{
			m_print->Print(6,"FLOAT LIT Expected");
		}
		break;
		/*----------------------------------
			 Get String Literal
		 ----------------------------------*/
	case sclLexTok::STRING_LIT :
		if(_symbol->m_type == sclSymbol::STRING_TYPE)
		{
			int _length = (int)(strlen((char*)_head->m_data));
			char* _strLit = new char[_length+1];			
			strncpy(_strLit,(char*)(_head->m_data),_length);
			_strLit[_length] = 0;
			_symbol->m_data = _strLit;
		}
		else
		{
			m_print->Print(6,"STRING LIT Expected");
		}
		break;
	default:
		m_print->Print(2,"Expected Literal");
		break;
	}
	_head = _head->m_next;	// Get Next Token
	/*----------------------------------
		 IN
	 ----------------------------------*/
	if(_head->m_tag != sclLexTok::IN_OP)
	{
		m_print->Print(5,"Expected LET!");
	}
	_head = _head->m_next;	// Get Next Token

	/*----------------------------------
		 Return New Symbol
	 ----------------------------------*/
	return _symbol;
}