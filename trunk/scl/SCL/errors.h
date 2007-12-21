/*****************************************************************************
  Title: Error Printout
  File: D:\CODE\SCL\SCL\ERRORS.H
  Author: Claire Blackshaw
  Created: 2007/12/08
    Copyright Flammable Penguins see http://www.flammablepenguins.com/
*****************************************************************************/

#ifndef SCL_HEADER_ERROR
#define SCL_HEADER_ERROR

/*----------------------------------
	Includes
 STDIO - Needed for FILE*
----------------------------------*/
#include <stdio.h>

#ifdef WIN32
#pragma warning (disable : 4996)
#endif

/*----------------------------------
	 Error Printer
 ----------------------------------*/
class ErrorPrinter
{
public:
	static ErrorPrinter* Instance();	// Get Instance
	void SetOutput(FILE* _file);		// Set Output File
	void CloseFlush();					// Flush and Close File
	void Print(int _num, char* _param);	// Print out Error
	void PrintDave(int _num);			// Print Out Dave Style
	~ErrorPrinter();

private:
	ErrorPrinter();
	FILE* m_outFile;					// Output File
	static ErrorPrinter* s_instance;	// Static Instance
};

#endif