/*****************************************************************************
  Title: SCL Abstract Syntax Tree
  File: D:\CODE\SCL\SCL\SCLABSYNTREE.H
  Author: Claire Blackshaw
  Created: 2007/12/10
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#ifndef SCL_HEADER_ABSTACT_SYNTAX_TREE
#define SCL_HEADER_ABSTACT_SYNTAX_TREE

/*----------------------------------
	Header
	Lexer for Tokens
	Errors for print outs
	Use Stack and Queue for rPolish
----------------------------------*/
#include "sclLexer.h"
#include "sclSymbolTable.h"
#include "errors.h"
#include <stack>
#include <queue>

/*----------------------------------
	 SCL Exp
 ----------------------------------*/
struct sclExp
{
	enum Type{	NULL_TYPE, 
				OPERATION,
				VALUE};
	Type m_type;
	enum Sub{			//	DATA = 0
		NULL_SUB = 0,
		// -- TREE --		DATA = SUB[3]
		TREE = 5,
		// -- OPERATION --	DATA = 0
		EQUAL = 10, NOT_EQ,			// Equality
		LESS = 20, GREAT,			// < >
		LESS_EQ = 30, GREAT_EQ,		// <= >=
		ADD = 40, SUB, OR,			// + - ||
		TIMES = 50, DIV, MOD, AND,	// * / % &&
		NEGATE = 60,				// Negate
		// -- VALUE --		DATA = Value / Ident
		SYMBOL = 70, 
		BOOL_LIT = 80, INT_LIT, FLOAT_LIT, STRING_LIT};
	Sub m_oper;
	void* m_data;	// Ident or Value	
};

/*----------------------------------
	 Type  Defines
 ----------------------------------*/
typedef std::queue<sclLexTok*> rPolishQ;
typedef std::stack<sclLexTok*> rPolStack;

/*----------------------------------
	 Abstract Syntax Tree
 ----------------------------------*/
class sclAbSynTree
{
public:
	sclAbSynTree();
	~sclAbSynTree();

	/*----------------------------------
		 Generate
	 ----------------------------------*/
	void Generate(sclLexTok* _token, sclSymTable* _symTab);
	sclExp* GenExpression(sclLexTok*& _token, sclExp* _prevExp);
	sclExp* GenOper(sclLexTok* _token);

	sclExp* CreateExp();					// Create Blank Exp

	void RevPolish(sclLexTok* _token);		// Generate Reverse Polish

	sclLexTok* PopPolish();

private:
	sclExp* m_expBlock;			// Expression Block
	ErrorPrinter* m_print;		// Error Printer
	sclSymTable* m_symTab;		// Symbol Table
	rPolishQ m_polTokQ;
};

#endif
