#include "FileHandler.h"

//=============================================================
//	Extract String From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, char* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}
//=============================================================
//	Extract Unit Con From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, UNIT_CON* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}
//=============================================================
//	Extract Map Pos From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, MAP_POS* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}
//=============================================================
//	Extract Map Size From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, MAP_SIZE* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}
//=============================================================
//	Extract Mapped Command From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, LC_MAPPED_CMD* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}
//=============================================================
//	Extract Map Object From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, MAP_OBJ* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}
//=============================================================
//	Extract UINT From File
//=============================================================
void FileOFunc::extract(FILE* _filePTR, UINT* _output)
{
	char _line[255] = {0};
	fgets(_line, 255, _filePTR);
	extract(_line,_output);
}

//=============================================================
//	Extract String From String
//=============================================================
void FileOFunc::extract(const char* _string, char* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	strncpy(_output,&(_string[_start]),_end - _start);
	_output[_end - _start] = 0;
}
//=============================================================
//	Extract Unit Con From String
//=============================================================
void FileOFunc::extract(const char* _string, UNIT_CON* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	char _temp[100] = {0};
	strncpy(_temp,&(_string[_start]),_end - _start);
	sscanf(_temp,"%u:%u:%u",&(_output->m_type),&(_output->m_time),&(_output->m_amount));
}
//=============================================================
//	Extract Map Pos From String
//=============================================================
void FileOFunc::extract(const char* _string, MAP_POS* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	char _temp[100] = {0};
	strncpy(_temp,&(_string[_start]),_end - _start);
	sscanf(_temp,"%u:%u:%u",&(_output->x),&(_output->y),&(_output->z));
}
//=============================================================
//	Extract Map Size From String
//=============================================================
void FileOFunc::extract(const char* _string, MAP_SIZE* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	char _temp[100] = {0};
	strncpy(_temp,&(_string[_start]),_end - _start);
	sscanf(_temp,"%u:%u",&(_output->x),&(_output->y));
}
//=============================================================
//	Extract Mapped Command From String
//=============================================================
void FileOFunc::extract(const char* _string, LC_MAPPED_CMD* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	char _temp[100] = {0};
	strncpy(_temp,&(_string[_start]),_end - _start);
	sscanf(_temp,"%u:%u",&(_output->m_CmdID),&(_output->m_var));
}
//=============================================================
//	Extract Map Object From String
//=============================================================
void FileOFunc::extract(const char* _string, MAP_OBJ* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	char _temp[100] = {0};
	strncpy(_temp,&(_string[_start]),_end - _start);
	sscanf(_temp,"%u:%u:%u:%u:%u",&(_output->x), &(_output->y), &(_output->z), &(_output->r), &(_output->id));
}
//=============================================================
//	Extract UINT From String
//=============================================================
void FileOFunc::extract(const char* _string, UINT* _output)
{
	UINT _start, _end;	
	_start = (UINT)(strchr(_string,'[') - _string + 1);
	_end = (UINT)(strchr(_string,']') - _string);
	char _temp[100] = {0};
	strncpy(_temp,&(_string[_start]),_end - _start);
	*(_output) = atoi(_temp);
}