#ifndef FILE_HANDLING_FUNCTIONS
#define FILE_HANDLING_FUNCTIONS

#include <fstream>
#include <iostream>
#include "Structs.h"

namespace FileOFunc
{
	// From File
	void extract(FILE* _filePTR, char* _output);
	void extract(FILE* _filePTR, UNIT_CON* _output);
	void extract(FILE* _filePTR, MAP_POS* _output);
	void extract(FILE* _filePTR, MAP_SIZE* _output);
	void extract(FILE* _filePTR, LC_MAPPED_CMD* _output);
	void extract(FILE* _filePTR, MAP_OBJ* _output);
	void extract(FILE* _filePTR, UINT* _output);

	// From String
	void extract(const char* _string, char* _output);
	void extract(const char* _string, UNIT_CON* _output);
	void extract(const char* _string, MAP_POS* _output);
	void extract(const char* _string, MAP_SIZE* _output);
	void extract(const char* _string, LC_MAPPED_CMD* _output);
	void extract(const char* _string, MAP_OBJ* _output);
	void extract(const char* _string, UINT* _output);	
}

#endif