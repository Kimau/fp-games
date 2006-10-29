#ifndef BATTLE_MAP_PART_OF_CODE_1942
#define BATTLE_MAP_PART_OF_CODE_1942

//---------------------------------------------
//	INCLUDES
//---------------------------------------------
#include "Structs.h"
#include "FileHandler.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

class JWL_Map
{
public:
	//-------------------------------------
	//    System Functions
	//-------------------------------------
	JWL_Map();
	JWL_Map(const char* _filename);
	~JWL_Map();
	void Delete();

	//-------------------------------------
	//    Accessors
	//-------------------------------------
	bool			isEmpty() const;
	MAP_CELL		getCellCopy(UINT x, UINT y) const;
	MAP_CELL*		getGridCopy() const;
	const MAP_CELL*	getGridPTR() const;	
	UINT			getHeight(UINT x, UINT y) const;
	UINT			getTex(UINT x, UINT y) const;
	float			getHeightf(float x, float y) const;
	const char*		getMapTexture() const;
	MAP_SIZE		getMapSize() const;

    UINT getPalleteSize() const;
	UINT getObjectCount() const;
	MAP_O_PAL getMapPallete(UINT _index) const;
	MAP_OBJ getMapObject(UINT _index) const;
	//-------------------------------------
	//    Map Loading Functions
	//-------------------------------------
public:
	bool LoadMap(FILE* _FilePTR);
private:
	bool LoadBase(FILE* _FilePTR);
	bool LoadPallete(FILE* _FilePTR);
	void LoadLogic(MAP_O_PAL& _palObj, const char* _filename);
	bool LoadLogic(MAP_O_PAL& _palObj, FILE* _FilePTR);
	bool LoadObjects(FILE* _FilePTR);

	//-------------------------------------
	//		Mutation Functions
	//-------------------------------------
	void AddCell(MAP_POS _pos, UINT _logic);
	//-------------------------------------
	//    Map Movement and Path Functions
	//
	//	Note all functions are const
	// I just avoid the stupid fucking
	// const operator like the plauge
	//-------------------------------------
public:
	bool getLogic(MAP_POS _pos) const;
	UINT getLogic(MAP_POS _pos, UINT _dir) const;
	bool checkLogic(UINT _logic, UINT _dir) const;
	bool checkRLogic(UINT _logic, UINT _dir) const;
	UINT getDir(MAP_POS _pos, MAP_POS _dest) const;
	bool canMove(CVector3 _pos, CVector3 _dest) const;
	bool canMove(MAP_POS _pos, MAP_POS _dest) const;
	bool canMove(MAP_POS _pos, UINT _dir) const;	
	MAP_PATH* canMove(MAP_PATH* _path) const;


private:
	// Header Info
	char m_Name[100];
	char m_Tileset[100];
	UINT m_OBJcount;
	UINT m_PALsize;
	UINT m_totalSize;

	// Grid Info
	MAP_SIZE m_Size;
	MAP_OBJ* m_Objects;
	MAP_O_PAL* m_ObjPallete;
	MAP_CELL* m_Grid;
};

#endif