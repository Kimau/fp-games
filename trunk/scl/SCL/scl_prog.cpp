/*****************************************************************************
  Title: SCL Test Program
  File: D:\CODE\SCL\SCL\SCL_PROG.CPP
  Author: Claire Blackshaw
  Created: 2007/12/08
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/

/*----------------------------------
	 Includes
  STDIO - Needed for FILE*
  IOSTREAM - Cout
  ERROR - Printing out Errors
  LEXER - SCL Lexer
  SYMTAB - Symbol Table
  AST - Abstract Syntax Tree
 ----------------------------------*/
#include <stdio.h>
#include <iostream>
#include "errors.h"
#include "sclLexer.h"
#include "sclSymbolTable.h"
#include "sclAbSynTree.h"
#include "sclPrinter.h"

//============================================================================
//  Main
//============================================================================
int main(int _argc, char* _argv[]) 
{
	/*----------------------------------
		 Open Input File
		 Open Output File
	 ----------------------------------*/
	if(_argc < 3)
	{
		std::cout << "Parameters not less \n";
		return -1;
	}

	FILE* _inFile = fopen(_argv[1],"r");	// Open In File
	if (_inFile == 0)
	{
		std::cout << "Unable to open In File" << _argv[0] << ". \n";
		return -1;
	}

	FILE* _outFile = fopen(_argv[2],"w");	// Open Out File
	if (_outFile == 0)
	{
		std::cout << "Unable to open Out File" << _argv[1] << ". \n";
		return -1;
	}
	/*----------------------------------
		 Setup Error Printer
	 ----------------------------------*/
	ErrorPrinter* _instance = ErrorPrinter::Instance();
	_instance->SetOutput(_outFile);

	/*----------------------------------
		 Generate Lex Tokens
	 ----------------------------------*/
	sclLexer _lexList;
	_lexList.Generate(_inFile);

	/*----------------------------------
		 Close Input File
	 ----------------------------------*/
	fclose(_inFile);

	/*----------------------------------
		 Generate Syntax Tree
	 ----------------------------------*/
	sclSymTable _symTable;
	sclLexTok* _last = _symTable.Generate(_lexList.getHead());

	/*----------------------------------
	Print Out Expression Block
	Dave Style
	----------------------------------*/
	sclLexTok* _temp = _last;
	while(_temp)
	{
		PrintTokenTag(_temp, _outFile);
		_temp = _temp->m_next;
	}

	/*----------------------------------
	Print Out Symbol Table
	Dave Style
	----------------------------------*/
	fprintf(_outFile,"\nName\t Type\t Value \n");
	sclSymbol* _symbol = _symTable.getNext(true);
	while(_symbol != 0)
	{
		PrintSymbol(_symbol, _outFile);


		_symbol = _symTable.getNext();	// Next Symbol
	} // END OF while(_symbol != 0)

	/*----------------------------------
		 Generate Code
	 ----------------------------------*/
	sclAbSynTree _abSynTree;
	_abSynTree.Generate(_last,&_symTable);

	/*----------------------------------
		 Print Out Polish
	 ----------------------------------*/
	fprintf(_outFile,"=============================================\n");
	fprintf(_outFile,"           PRINT OUT POLISH QUEUE            \n");
	fprintf(_outFile,"=============================================\n");
	sclLexTok* _token = _abSynTree.PopPolish();
	while(_token == 0)
	{
		PrintTokenTag(_token,_outFile);
		_token = _abSynTree.PopPolish();
	}

	/*----------------------------------
		 Close Output File
	 ----------------------------------*/
	fclose(_outFile);
	return 0;
}