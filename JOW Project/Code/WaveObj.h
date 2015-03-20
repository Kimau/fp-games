#ifndef WAVEFRONT_OBJECT_LOADER_CLASS_3454
#define WAVEFRONT_OBJECT_LOADER_CLASS_3454

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include "CVector3.h"

using namespace std;

class WaveObject
{
public:
	WaveObject(FILE* _filePT);
	~WaveObject();

private:
	WaveObject() {}

	void ReadVertex();
	void ReadFace();
	void ComputeNormals();

public:
	FILE*	m_filePT;
	UINT	m_fCount;
	UINT	m_vCount;

	// Vectors hold current information
	vector<CVert>		m_Verts;
	vector<CFace>		m_Faces;
	vector<CNorm>		m_Norms;
	vector<CTexCoord>	m_Coords;
};

#endif