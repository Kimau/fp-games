/*****************************************************************************
  Title: Generate 68000 code
  File: D:\CODE\SCL\SCL\SCLGEN68.H
  Author: Claire Blackshaw
  Created: 2007/12/11
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#ifndef SCL_HEADER_68000_CODE_GENERATOR
#define SCL_HEADER_68000_CODE_GENERATOR

/*----------------------------------
	Includes
	STDIO - Needed for FILE*
	AST - Needed for Polish
	Lexer for Tokens
	Errors for print outs
----------------------------------*/
#include <stdio.h>
#include "sclLexer.h"
#include "sclSymbolTable.h"
#include "sclAbSynTree.h"
#include "errors.h"

/*----------------------------------
	 68000 Code Generator
 ----------------------------------*/
class scl68Gen
{
public:
	/*----------------------------------
		 Con/ Decon
	 ----------------------------------*/
	scl68Gen();
	~scl68Gen();

	/*----------------------------------
		 Generate Code File
	 ----------------------------------*/
	void Generate(FILE* _file, sclSymTable* m_SymTab, sclAbSynTree* m_AST);
	void GenCode();
	void GenFuncLib();
	void GenData();

private:
	FILE* m_cFile;
	sclSymTable* m_SymTab;
	sclAbSynTree* m_AST;
};

#endif