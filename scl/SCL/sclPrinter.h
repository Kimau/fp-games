/*****************************************************************************
  Title: SCL Printers
  File: D:\CODE\SCL\SCL\SCLPRINTER.H
  Author: Claire Blackshaw
  Created: 2007/12/11
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#ifndef SCL_HEADER_PRINTERS
#define SCL_HEADER_PRINTERS

/*----------------------------------
Includes
STDIO - Needed for FILE*
IOSTREAM - Cout
LEXER - SCL Lexer
SYMTAB - Symbol Table
AST - Abstract Syntax Tree
----------------------------------*/
#include <stdio.h>
#include <iostream>
#include "sclLexer.h"
#include "sclSymbolTable.h"
#include "sclAbSynTree.h"

/*----------------------------------
	Predeclare
----------------------------------*/
void PrintTokenTag(sclLexTok* _tok, FILE* _outFile); // Print Lex Token Tag Only
void PrintToken(sclLexTok* _tok, FILE* _outFile); // Print Lex Token
void PrintSymbol( sclSymbol* _symbol, FILE* _outFile ); // Print Symbol

#endif