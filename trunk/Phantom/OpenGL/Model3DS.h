//=============================================
//	This class should (In order of Priority)
//	+ load 3ds models
//	+ Render Object (Should Always be Package into Display List)
//	+ Export to Runtime Formats
//=============================================
#ifndef BASIC_OPENGL_MODEL_LOADER_3ds
#define BASIC_OPENGL_MODEL_LOADER_3ds

//===================================
//	Headers
//===================================
#include <fstream>
#include <assert.h>
#include <vector>
#include "BaseGl.h"
#include "Model.h"
#include "..\Math\CVector3.h"

#pragma warning(disable : 4996) // Disables silly warning about win "security"

using namespace std;

//===================================
//	DEFINES
//===================================
#define CH_PRIMARY       0x4D4D				// Primary Chunk, at the beginning of each file

//------------------------------
//		Main Chunks
//------------------------------
#define CH_OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
#define CH_VERSION       0x0002				// This gives the version of the .3ds file
#define CH_EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

//------------------------------
//		OBJECTINFO
//------------------------------
#define CH_MATERIAL	  0xAFFF				// This stored the texture info
#define CH_OBJECT		  0x4000			// This stores the faces, vertices, etc...

//------------------------------
//		MATERIAL
//------------------------------
#define CH_MATNAME       0xA000				// This holds the material name
#define CH_MATDIFFUSE    0xA020				// This holds the color of the object/material
#define CH_MATMAP        0xA200				// This is a header for a new material
#define CH_MATMAPFILE    0xA300				// This holds the file name of the texture

//------------------------------
//		OBJECT
//------------------------------
#define CH_OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

//------------------------------
//		OBJECT_MESH
//------------------------------
#define CH_OBJECT_VERTICES		0x4110		// The objects vertices
#define CH_OBJECT_FACES			0x4120		// The objects faces
#define CH_OBJECT_MATERIAL		0x4130		// This is found if the object has a material, either texture map or color
#define CH_OBJECT_UV			0x4140		// The UV texture coordinates
#define CH_OBJECT_LOCAL			0x4160		// The Local Axis

//==========================================================================
//		Structures
//==========================================================================
//-----------------------------------------------
// Here is our structure for our 3ds indicies
// since .3ds stores 4 unsigned shorts
// This will hold point 1, 2, and 3 index's
// into the vertex array plus a visible flag
//-----------------------------------------------
struct Indices3ds
{							
	unsigned short a, b, c, bVisible;
};

//-----------------------------------------------
// This holds the chunk info
//-----------------------------------------------
struct maxChunk
{
	unsigned short int m_ID;					// The chunk's ID		
	unsigned int m_length;						// The length of the chunk
	unsigned int m_bytesRead;					// The amount of bytes read within that chunk
};

class BGLModel3ds
{
	//------------------------------------------
	//	System Construct and Decon
	//------------------------------------------
public:
	BGLModel3ds();								// This inits the data members
	~BGLModel3ds();

	//------------------------------------------
	//	LOAD FROM FILE
	//------------------------------------------
public:
	bool Import3ds(const char* _filename);		// Base Call

private:
	int GetString(char* _string);				// This reads in a string and saves it in the char array passed in

	void ReadChunk(maxChunk* _chunk);			// This reads the next chunk
	void ProcessNextChunk(maxChunk* _prev);		// This reads the next large chunk
	void ProcessNextObjectChunk(BGLObject *pObject, maxChunk* _prev); // This reads the object chunks
	void ProcessNextMaterialChunk(maxChunk* _prev); // This reads the material chunks

	void ReadColorChunk(BGLMaterialInfo *pMaterial, maxChunk* _prev); // This reads the RGB value for the object's color
	void ReadVertices(BGLObject *pObject, maxChunk* _prev);			// This reads the objects vertices
	void ReadVertexIndices(BGLObject *pObject, maxChunk* _prev);	// This reads the objects face information
	void ReadUVCoordinates(BGLObject *pObject, maxChunk* _prev);	// This reads the texture coodinates of the object
	void ReadAxis(BGLObject *pObject, maxChunk* _prev);				// This reads the local axis information of the Object
	void ReadObjectMaterial(BGLObject *pObject, maxChunk* _prev);	// This reads in the material name assigned to the object and sets the materialID
	
	void ComputeFaceNormals();			// This computes the face normals for the object
	void ComputeVertexNormals();		// This computes the vertex normals for the object
	void CleanUp();						// This frees memory and closes the file
	
	//------------------------------------------
	//	RENDER OBJECT
	//------------------------------------------
public:
	void RenderObjects() const;
	GLuint getList() const;
	//------------------------------------------
	//	Varibles
	//------------------------------------------
private:
	FILE *m_FilePointer;		// The file pointer
	BGLModel m_Model;			// The Model Data
};

#endif