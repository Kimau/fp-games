#pragma once

#include <stdio.h>
#include "gwVector.h"
#include "gwMapObject.h"

// WARNING :: NO ERROR CHECKING DONE IN THIS FILE!!!

#define GW_TYPE_OFFSET	10000

namespace GW_File
{
	void save(const float& _inData, FILE* _file);
	void load(float& _outData, FILE* _file);

	void save(const gwVector& _inData, FILE* _file);
	void load(gwVector& _outData, FILE* _file);

	void save(const gwSegment& _inData, FILE* _file);
	void load(gwSegment& _outData, FILE* _file);

	void save(gwMapObject* _inData, FILE* _file);
	gwMapObject* loadObject(FILE* _file);
};