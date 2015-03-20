/*****************************************************************************
  Title: Symbol Table
  File: D:\CODE\SCL\SCL\SCLSYMBOLTABLE.H
  Author: Claire Blackshaw
  Created: 2007/12/10
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/
#ifndef SCL_HEADER_SYMBOL_TABLE
#define SCL_HEADER_SYMBOL_TABLE

/*----------------------------------
	 Header
  STL MAP used for  Sym Tab
  Lexer for Tokens
  Errors for print outs
 ----------------------------------*/
#include <map>
#include "sclLexer.h"
#include "errors.h"

/*----------------------------------
	 Comparison Class
 ----------------------------------*/
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

/*----------------------------------
	 SCL Symbol
 ----------------------------------*/
struct sclSymbol 
{
	enum Type{NULL_TYPE, BOOL_TYPE, INT_TYPE, FLOAT_TYPE, STRING_TYPE};
	Type m_type;
	char* m_ident;
	void* m_data;
};

typedef std::map<char*,sclSymbol*,ltstr> SymMap;
/*----------------------------------
	 Symbol Table
 ----------------------------------*/
class sclSymTable
{
public:
	/*----------------------------------
		 Constructor
	 ----------------------------------*/
	sclSymTable();
	~sclSymTable();

	/*----------------------------------
		 Generate Table
	 ----------------------------------*/
	sclLexTok* Generate(sclLexTok* _head);

	/*----------------------------------
		 Look Up
	 ----------------------------------*/
	sclSymbol* operator[](const char* _ident) const;
	sclSymbol* getNext(bool _start = false) const;

private:
	/*----------------------------------
		 Generate Symbol Table
	 ----------------------------------*/
	sclSymbol* ReadSymbol(sclLexTok*& _head);

	/*----------------------------------
		 Variables
	 ----------------------------------*/
	SymMap m_SymbolMap;	// Symbol Map
	ErrorPrinter* m_print;	// Error Printer
};

#endif