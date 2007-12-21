/*****************************************************************************
  Title: Error Code
  File: D:\CODE\SCL\SCL\ERRORS.CPP
  Author: Claire Blackshaw
  Created: 2007/12/09
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/

/*----------------------------------
	 Includes
 STDLIB - used for exit
 ----------------------------------*/
#include "errors.h"
#include <stdlib.h>

/*----------------------------------
	 Set Instance
 ----------------------------------*/
ErrorPrinter* ErrorPrinter::s_instance = 0;

//============================================================================
//  Constructor
//============================================================================
ErrorPrinter::ErrorPrinter() : m_outFile(0)
{
}

//============================================================================
//  Deconstructor
//============================================================================
ErrorPrinter::~ErrorPrinter()
{

}

//============================================================================
//  Flush the Instance and Close File
//============================================================================
void ErrorPrinter::CloseFlush()
{
	/*----------------------------------
		 Close Output File
	 ----------------------------------*/
	if(m_outFile)
	{
		fclose(m_outFile);
	}
}

//============================================================================
//  Get Instance
//============================================================================
ErrorPrinter* ErrorPrinter::Instance()
{
	/*----------------------------------
		 Create Instance if None exists
	 ----------------------------------*/
	if(s_instance == 0)
	{
		s_instance = new ErrorPrinter();
	}

	return s_instance;
}

//============================================================================
//  Set Output File
//============================================================================
void ErrorPrinter::SetOutput(FILE* _file) // *IN* Set Output File
{
	m_outFile = _file;
}

//============================================================================
//  Print Out Error
//============================================================================
void ErrorPrinter::Print(int _num, char* _param)
{
	PrintDave(_num);
	// fprintf(m_outFile,"%i: %s \n", _num, _param);
	CloseFlush();	// Close up after
	exit(_num);		// Clean Up
}

//============================================================================
//  Print Out Dave Error
//============================================================================
void ErrorPrinter::PrintDave(int _num)
{
	/*----------------------------------
		 Fetch Prewritten Error Message
	 ----------------------------------*/
}