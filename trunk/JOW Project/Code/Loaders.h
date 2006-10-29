#ifndef MY_FILE_LOADING_LIBARY_89756
#define MY_FILE_LOADING_LIBARY_89756

//----------------------------------
// Headers
//----------------------------------
#include "Structs.h"

namespace MyLoaders
{
	//----------------------------------
	// Texture Functions
	//----------------------------------
	INT_IMAGE* LoadJPG(const char* _filename);									// Loads JPEG
	INT_IMAGE* LoadTGA(const char* _filename);									// Loads TGA

	//----------------------------------
	// Model Functions
	//----------------------------------
	JWL_Mesh* LoadWaveObj(const char* _filename);								// Loads Wavefront Object
	JWL_Mesh* Load3DS(const char* _filename);									// Loads 3DS Object
}

#endif