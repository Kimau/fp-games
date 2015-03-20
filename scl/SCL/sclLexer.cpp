/*****************************************************************************
  Title: SCL LEXER CPP
  File: D:\CODE\SCL\SCL\SCLLEXER.CPP
  Author: Claire Blackshaw
  Created: 2007/12/09
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#include "sclLexer.h"
#include <ctype.h>		// Used for Is* Calls
#include <string.h>		// Used for memset

//============================================================================
//  SCL LEXER CONSTRUCTOR
//============================================================================
sclLexer::sclLexer() : m_head(0), m_curr(0), m_clean(0), m_marker(0)
{
	m_print = ErrorPrinter::Instance();
}
//============================================================================
//  SCL LEXER DECONSTRUCTOR
//============================================================================
sclLexer::~sclLexer()
{
	FlushTokens(m_head);
}

//============================================================================
//  Flush out the Tokens and Free Memory
//============================================================================
void sclLexer::FlushTokens(sclLexTok* _token) // *IN-OUT* Deletable Token
{
	/*----------------------------------
		 Delete Tail
	 ----------------------------------*/
	if(_token->m_next)
	{
		// Recursive Clean out
		FlushTokens(_token->m_next);
	}
	/*----------------------------------
		 Delete Token
	 ----------------------------------*/
	if((_token->m_tag == sclLexTok::IDENT) ||
		(_token->m_tag == sclLexTok::STRING_LIT))
	{
		delete[] _token->m_data;
	}
	else
	{
		delete _token->m_data;
	}
	delete _token;
}

//============================================================================
//  Generate SCL Lexer Token List
//============================================================================
void sclLexer::Generate(FILE* _inFile) // *IN* File to be processed
{
	/*----------------------------------
		 Clean Stream
	 ----------------------------------*/
	CleanFile(_inFile);

	/*----------------------------------
		 READ FILE
	 ----------------------------------*/
	while(m_clean[m_marker] != NULL)
	{
		/*----------------------------------
		Generate a Fresh Token
		----------------------------------*/
		GenToken();
		/*----------------------------------
			 Skip Whitespace
		 ----------------------------------*/
		while(m_clean[m_marker] == ' ')
		{
			++m_marker;
		}
		/*----------------------------------
			 Get Word
		 ----------------------------------*/
		if(isalpha(m_clean[m_marker]))
		{
			ReadWord();
		}
		/*----------------------------------
			 Get Number
		 ----------------------------------*/
		else if(isdigit(m_clean[m_marker]))
		{
			ReadNumber();
		}
		/*----------------------------------
			 Get Operator
		 ----------------------------------*/
		else if(m_clean[m_marker] != 0)
		{
			ReadOper();
		}
		/*----------------------------------
			Skip Whitespace
		----------------------------------*/
		while(m_clean[m_marker] == ' ')
		{
			++m_marker;
		}
	}

	/*----------------------------------
		 Exit
	 ----------------------------------*/
	m_marker = 0;
}

//============================================================================
//  Generate SCL Lexical Token
//============================================================================
void sclLexer::GenToken()
{
	/*----------------------------------
		 Check for Empty Head
	 ----------------------------------*/
	if(m_head == 0)
	{
		m_head = new sclLexTok();
		m_curr = m_head;
	}
	/*----------------------------------
		 Append New Token
	 ----------------------------------*/
	else
	{
		m_curr->m_next = new sclLexTok();
		m_curr = m_curr->m_next;
	}

	/*----------------------------------
		 Set to Null Token
	 ----------------------------------*/
	m_curr->m_tag = sclLexTok::NULL_TYPE;
	m_curr->m_sub = sclLexTok::NULL_SUB;
	m_curr->m_data = 0;
	m_curr->m_next = 0;
}

//============================================================================
//  Read Operator
//============================================================================
void sclLexer::ReadOper()
{
	switch(m_clean[m_marker])
	{
		/*----------------------------------
			Single Character
		----------------------------------*/
	case '-':	m_curr->m_tag = sclLexTok::ADDOP;	
				m_curr->m_sub = sclLexTok::SUB;		
				break;

	case '+':	m_curr->m_tag = sclLexTok::ADDOP;	
				m_curr->m_sub = sclLexTok::ADD;
				break;

	case '*':	m_curr->m_tag = sclLexTok::MULOP;	
				m_curr->m_sub = sclLexTok::TIMES;
				break;

	case '%':	m_curr->m_tag = sclLexTok::MULOP;	
				m_curr->m_sub = sclLexTok::MOD;
				break;
	case '/':	m_curr->m_tag = sclLexTok::MULOP;	
				m_curr->m_sub = sclLexTok::DIV;
				break;
	case '(':	m_curr->m_tag = sclLexTok::LEFTBKT;		
				break;
	case ')':	m_curr->m_tag = sclLexTok::RIGHTBKT;	
				break;
		/*----------------------------------
			String Literal 
		 ----------------------------------*/
	case '"':	
		{
			++m_marker;								// Move Forward
			m_curr->m_tag = sclLexTok::STRING_LIT;
			/*----------------------------------
				 Get String Length
			 ----------------------------------*/
			int _tempMarker = m_marker;
			while(m_clean[_tempMarker] != '"')
			{
				++_tempMarker;
			}
			int _length = _tempMarker - m_marker;
			/*----------------------------------
				 Copy String
			 ----------------------------------*/
			char* _strlit = new char[_length + 1];
			strncpy(_strlit,&(m_clean[m_marker]),_length);
			_strlit[_length] = 0;
			m_curr->m_data = _strlit;
			m_marker = _tempMarker + 1;
		} break;
		/*----------------------------------
			Double Character
		----------------------------------*/
	case '!':	
		{
			if (m_clean[m_marker+1] == '=')
			{
				m_curr->m_tag = sclLexTok::RELOP;	// Not Equal
				m_curr->m_sub = sclLexTok::NOT_EQ;
				++m_marker;
			} 
			else
			{
				m_curr->m_tag = sclLexTok::NOTOP;	// Not Op
			}
		} break;

	case '=':	
		{
			if (m_clean[m_marker+1] == '=')
			{
				m_curr->m_tag = sclLexTok::RELOP;	// Not Equal
				m_curr->m_sub = sclLexTok::EQUAL;
				++m_marker;
			} 
			else
			{
				m_curr->m_tag = sclLexTok::ASSIGN;	// Not Op
			}
		} break;

	case '<':	
		{
			m_curr->m_tag = sclLexTok::RELOP;
			if (m_clean[m_marker+1] == '=')
			{
				m_curr->m_sub = sclLexTok::LESS_EQ;	// Lesser or Equal
				++m_marker;
			} 
			else
			{
				m_curr->m_sub = sclLexTok::LESS;	// Lesser Than
			}
		} break;

	case '>':	
		{
			m_curr->m_tag = sclLexTok::RELOP;
			if (m_clean[m_marker+1] == '=')
			{	
				m_curr->m_sub = sclLexTok::GREAT_EQ;// Greater or Equal
				++m_marker;
			} 
			else
			{
				m_curr->m_sub = sclLexTok::GREAT;	// Greater Than
			}
		} break;

	case '|':
		{
			if (m_clean[m_marker+1] == '|')
			{
				m_curr->m_tag = sclLexTok::ADDOP;	// OR
				m_curr->m_sub = sclLexTok::OR;
				++m_marker;
			}
		} break;

	case '&':
		{
			if (m_clean[m_marker+1] == '&')
			{
				m_curr->m_tag = sclLexTok::MULOP;
				m_curr->m_sub = sclLexTok::AND;
				++m_marker;
			}
		} break;

	default:
		m_print->Print(m_clean[m_marker],"Unknown Operator");
		break;
	} // End of switch(m_clean[m_marker])

	/*----------------------------------
		 Advance Marker
	 ----------------------------------*/
	++m_marker;
}

//============================================================================
//  Read a Number
//============================================================================
void sclLexer::ReadNumber()
{
	/*----------------------------------
		 Get Length
		 Float or Int
	 ----------------------------------*/
	int _tempMark = m_marker;
	bool _isFloat = false;

	while(isdigit(m_clean[_tempMark]) ||
		  (m_clean[_tempMark] == '.') ||
		  (m_clean[_tempMark] == '^'))
	{
		if(m_clean[_tempMark] == '.')
		{
			_isFloat = true;
		}
		else if(m_clean[_tempMark] == '^')
		{
			m_clean[_tempMark] = 'e';
			_isFloat = true;
		}
		++_tempMark;
	}
	/*----------------------------------
		 Read Number
	 ----------------------------------*/
	if (_isFloat)
	{
		float* _heapFloat = new float;
		sscanf(&(m_clean[m_marker]),"%f",_heapFloat);
		m_curr->m_tag = sclLexTok::FLOAT_LIT;
		m_curr->m_data = _heapFloat;
	} 
	else
	{
		int* _heapInt = new int;
		sscanf(&(m_clean[m_marker]),"%i",_heapInt);
		m_curr->m_tag = sclLexTok::INT_LIT;
		m_curr->m_data = _heapInt;
	}

	/*----------------------------------
		 Advance Marker
	 ----------------------------------*/
	m_marker = _tempMark;
}

//============================================================================
//  Read a Word
//============================================================================
void sclLexer::ReadWord()
{
	int _tempMark = m_marker;

	/*----------------------------------
		 Find Length of Word
	 ----------------------------------*/
	while(((isalnum(m_clean[_tempMark])) || m_clean[_tempMark] == '_') && 
		   (m_clean[_tempMark] != ' '))
	{
		++_tempMark;
	}
	/*----------------------------------
		Get Word
	 ----------------------------------*/
	int _length = _tempMark - m_marker;					// Get Length
	char* _cleanWord = new char[_length + 1];			// Create Buffer
	strncpy(_cleanWord,&(m_clean[m_marker]),_length);	// Copy Word
	_cleanWord[_length] = 0;							// Set Null Term Char

	/*----------------------------------
		 Check for Reserved Word
	 ----------------------------------*/
	if(strncmp(_cleanWord,"let",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::LET; } 
	else if(strncmp(_cleanWord,"in",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::IN_OP; }
	else if(strncmp(_cleanWord,"=",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::ASSIGN; }
	else if(strncmp(_cleanWord,"const",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::CONST; }
	else if(strncmp(_cleanWord,"end",_length) == 0)		
		{ m_curr->m_tag = sclLexTok::END; }
	else if(strncmp(_cleanWord,"string",_length) == 0)	
	{ m_curr->m_tag = sclLexTok::STRING_TYPE; }
	else if(strncmp(_cleanWord,"bool",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::BOOL_TYPE; }
	else if(strncmp(_cleanWord,"float",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::FLOAT_TYPE; }
	else if(strncmp(_cleanWord,"int",_length) == 0)	
		{ m_curr->m_tag = sclLexTok::INT_TYPE; }
	/*----------------------------------
		 Bool Lit
	 ----------------------------------*/
	else if(strncmp(_cleanWord,"false",_length) == 0)	
	{
		m_curr->m_tag = sclLexTok::BOOL_LIT; 
		m_curr->m_sub = sclLexTok::LIT_FALSE;
	}
	else if(strncmp(_cleanWord,"true",_length) == 0)	
	{ 
		m_curr->m_tag = sclLexTok::BOOL_LIT; 
		m_curr->m_sub = sclLexTok::LIT_TRUE;
	}
	/*----------------------------------
		 Identifier
	 ----------------------------------*/
	else
	{
		m_curr->m_tag  = sclLexTok::IDENT;
		m_curr->m_data = _cleanWord;		// Name
	}	

	m_marker = _tempMark;
}

//============================================================================
//  Get File and Generate a clean character stream
//============================================================================
void sclLexer::CleanFile(FILE* _inFile)
{
	/*----------------------------------
		 Remove Old Stream
	 ---------------------------------*/
	delete m_clean;

	/*----------------------------------
		 Generate Clean Stream 
	 ----------------------------------*/
	m_marker = 0;
	int _charSize = ftell(_inFile);
	fseek(_inFile,0,SEEK_END);
	_charSize = ftell(_inFile) - _charSize;
	m_clean = new char[_charSize];
	memset(m_clean,0,sizeof(char)*(_charSize));
	rewind(_inFile);

	/*----------------------------------
		 Copy Data from inFile
	 ----------------------------------*/
	char _next = fgetc(_inFile);
	while(_next != EOF)
	{
		/*----------------------------------
			 Check for Comments
		 ----------------------------------*/
		if(_next == '/')
		{
			_next = fgetc(_inFile);
			if(_next == '/')
			{
				/*----------------------------------
					 Strip Comment
				 ----------------------------------*/
				while(_next != '\n')
				{
					_next = fgetc(_inFile);
				}
			}
			else
			{
				/*----------------------------------
					Add Character to Clean Stream
				----------------------------------*/
				m_clean[m_marker++] = '/';
			}
		}
		/*----------------------------------
			 Copy Non Control Character
		 ----------------------------------*/
		else if(iscntrl(_next) == false)
		{
			/*----------------------------------
				 Avoid double space
			 ----------------------------------*/
			if(_next == ' ')
			{
				if(m_clean[m_marker-1] != ' ')
				{
					m_clean[m_marker++] = _next;
				}
			}
			/*----------------------------------
				 Add Character to Clean Stream
			 ----------------------------------*/
			else
			{
				m_clean[m_marker++] = _next;
			}
		}
		/*----------------------------------
			 Add Space for processing
		 ----------------------------------*/
		else if(m_clean[m_marker-1] != ' ')
		{
			m_clean[m_marker++] = ' ';
		}

		/*----------------------------------
			 Get Next Character
		 ----------------------------------*/
		_next = fgetc(_inFile);
	} // End of While

	/*----------------------------------
		 Reset File Marker
	 ----------------------------------*/
	m_marker = 0;
}