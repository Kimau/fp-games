/*****************************************************************************
  Title: Lex Table Generator
  File: D:\CODE\SCL\SCL\SCLLEXER.H
  Author: Claire Blackshaw
  Created: 2007/12/09
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#ifndef SCL_HEADER_LEXER
#define SCL_HEADER_LEXER

/*----------------------------------
	Includes
 STDIO - Needed for FILE*
 ERROR - Printing out Errors
----------------------------------*/
#include <stdio.h>
#include "errors.h"

/*----------------------------------
	 SCL Lexer Token
 ----------------------------------*/
struct sclLexTok
{
	enum Type{
		NULL_TYPE = 0,
		CONST = 10, LET, IN_OP, ASSIGN, END,			// Reserved Words
		IDENT = 20,										// Identifier
		FLOAT_LIT = 30, STRING_LIT, INT_LIT, BOOL_LIT,	// Literals
		FLOAT_TYPE = 40, STRING_TYPE, INT_TYPE, BOOL_TYPE,	// Type Declr
		RELOP = 50, MULOP, ADDOP, NOTOP,				// Operators
		LEFTBKT = 60, RIGHTBKT};						// Brackets

	enum SubType{
		NULL_SUB = 0,
		// -- Relational --
		EQUAL = 10, NOT_EQ,			// Equality
		LESS = 20, GREAT,			// < >
		LESS_EQ = 30, GREAT_EQ,		// <= >=
		// -- Add Ops --
		ADD = 40, SUB, OR,			// + - ||
		// -- Mul Ops --
		TIMES = 50, DIV, MOD, AND,	// * / % &&
		// -- Bool Lits --
		LIT_FALSE = 60, LIT_TRUE
		};

	Type m_tag;
	SubType m_sub;
	void* m_data;
	sclLexTok* m_next;
};

/*----------------------------------
	 SCL Lexer Table
 ----------------------------------*/
class sclLexer
{
public:
	/*----------------------------------
		 Setup and Shutdown
	 ----------------------------------*/
	sclLexer();
	~sclLexer();

	void FlushTokens(sclLexTok* _token);	// Flush and Free Memory

	/*----------------------------------
		 Generate Table
	 ----------------------------------*/
	void Generate(FILE* _inFile);

	/*----------------------------------
		 Accessors
	 ----------------------------------*/
	sclLexTok* getHead() {return m_head;}

private:	
	/*----------------------------------
		 Read in
		   Operator
		   Number
		   Word
	 ----------------------------------*/
	void ReadOper();
	void ReadNumber();
	void ReadWord();

	/*----------------------------------
		 Generate Token
	 ----------------------------------*/
	void GenToken();

	/*----------------------------------
		 Strip out Whitespace
	 ----------------------------------*/
	void CleanFile(FILE* _inFile);

	sclLexTok* m_head;	// Head of SCL Token
	sclLexTok* m_curr;	// Last SCL Token
	char* m_clean;		// Current Pointer to Clean Stream
	int m_marker;		// Marker for clean stream
	ErrorPrinter* m_print;	// Error Printer
};

#endif