/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				BASIC OPENGL STRUCTS
	Its important to remember memory management when passing these around
	The reason the dont have their own functions is it ends up being a lot
	faster to do client side memory management.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef BASIC_GL_MODEL_STRUCTS_345
#define BASIC_GL_MODEL_STRUCTS_345

#include <vector>
#include "BaseGl.h"
#include "..\Math\CVector2.h"
#include "..\Math\CVector3.h"

using namespace std;

//-----------------------------------------------
// This holds the information for a material.
// It may be a texture map of a color.
//-----------------------------------------------
struct BGLMaterialInfo
{
	char  m_strName[255];		// The texture name
	char  m_strFile[255];		// The texture file name (If this is set it's a texture map)
	BYTE  m_color[3];			// The color of the object (R, G, B)
	int   m_texureId;			// the texture ID
	float m_uTile;				// u tiling of texture
	float m_vTile;				// v tiling of texture
	float m_uOffset;			// u offset of texture
	float m_vOffset;			// v offset of texture
} ;

//-----------------------------------------------
//	Face Info
//	Holds Basic Face Infomation
//-----------------------------------------------
struct BGLFace
{
	int m_vert[3];
	int m_tex[3];
};

//-----------------------------------------------
//	This Holds All the Information for a
//	3D Object
//-----------------------------------------------
struct BGLObject 
{
	int  m_vertCount;			// The number of verts in the model
	int  m_faceCount;			// The number of faces in the model
	int  m_texCount;			// The number of texture coordinates
	int  m_matID;				// The texture ID to use, which is the index into our texture array
	bool m_textured;			// This is TRUE if there is a texture map for this object
	char m_strName[255];		// The name of the object
	CVector3	*m_Verts;		// The object's vertices
	CVector3	*m_fNormals;	// Face Normals
	CVector3	*m_vNormals;	// Vertex Normal
	CVector2	*m_TexVerts;	// The texture's UV coordinates
	BGLFace 	*m_Faces;		// The faces information of the object
};

//-----------------------------------------------
//	This stores multiple models and materials
//-----------------------------------------------
struct BGLModel 
{
	int m_Objcount;					// The number of objects in the model
	int m_MatCount;					// The number of materials for the model

	vector<BGLMaterialInfo> m_Mats;	// The list of material information (Textures and colors)
	vector<BGLObject> m_Objs;		// The object list for our model
};

#endif