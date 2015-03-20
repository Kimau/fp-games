#include "WaveObj.h"

//=======================================================================================
//=======================================================================================
//		Load WaveObject
//=======================================================================================
//=======================================================================================
WaveObject::WaveObject(FILE* _filePT)
{
	char _line[255] = {0};
	char _char = 0;

	m_filePT = _filePT;
	m_fCount = 0;
	m_vCount = 0;

	while(!feof(m_filePT))
	{
		// Get the beginning character of the current line in the .obj file
		_char = fgetc(m_filePT);

		switch(_char)
		{
		case 'v':						// Check if we just read in a 'v' (Could be a vertice/normal/textureCoord)
			// Decipher this line to see if it's a vertex ("v"), normal ("vn"), or UV coordinate ("vt")
			ReadVertex();
			break;
		case 'f':						// Check if we just read in a face header ('f')
			// If we get here we then we need to read in the face information.
			// The face line holds the vertex indices into our vertex array, or if
			// the object has texture coordinates then it holds those too.
			ReadFace();
			break;
		case '\n':
			// If we read in a newline character, we've encountered a blank line in
			// the .obj file.  We don't want to do the default case and skip another
			// line, so we just break and do nothing.
			break;
		default:

			// If we get here then we don't care about the line being read, so read past it.
			fgets(_line, 100, m_filePT);
			break;
		}
	}
	fclose(m_filePT);

	ComputeNormals();
}

//=======================================================================================
//=======================================================================================
//		Deconstruct
//=======================================================================================
//=======================================================================================
WaveObject::~WaveObject()
{
}

//=======================================================================================
//=======================================================================================
//		Load Vertex Info
//=======================================================================================
//=======================================================================================
void WaveObject::ReadVertex()
{
	char _line[255] = {0};
	char _char = 0;

	// Read the next character in the file to see if it's a vertice/normal/UVCoord
	_char = fgetc(m_filePT);

	switch(_char)
	{
	case ' ':
		{
			//-------------------------------
			//     Read Vertex
			//-------------------------------
			// Here we read in a vertice.  The format is "v x y z"
			float _in[3] = {0};
			fscanf(m_filePT, "%f %f %f", &(_in[0]), &(_in[1]), &(_in[2]));

			// Read the rest of the line so the file pointer returns to the next line.
			fgets(_line, 100, m_filePT);

			// Add a new vertice to our list
			++(m_vCount);
			m_Verts.push_back(CVector3(_in[0],_in[1],_in[2]));
		}
		break;
	case 't':
		{
			//-------------------------------
			//     Read Texture
			//-------------------------------
			// Here we read in a texture coordinate.  The format is "vt u v"
			float _in[2] = {0};
			fscanf(m_filePT, "%f %f", &(_in[0]), &(_in[1]));


			// Read the rest of the line so the file pointer returns to the next line.
			fgets(_line, 100, m_filePT);

			// Add a new texture coordinate to our list
			m_Coords.push_back(CTexCoord(_in[0],_in[1]));
		}
		break;
	case 'n':
		//-------------------------------
		//     Read Normal
		//-------------------------------
		break;
	default:
		// Not Vertex Information Read Past it
		fgets(_line, 100, m_filePT);
	}
}

//=======================================================================================
//=======================================================================================
//		Load Face Infomation
//=======================================================================================
//=======================================================================================
void WaveObject::ReadFace()
{
	CFace _face;
	char _line[255] = {0};

	// This function reads in the face information of the object.
	// A face is a polygon (a triangle in this case) that has information about it.
	// It has the 3D points that make up the polygon and may also have texture coordinates.
	// When reading in an .obj, objects don't have to have UV texture coordinates so we
	// need to read in the face information differently in that case.  If the object does have
	// UV coordinates, then the format will look like this:
	// "f vertexIndex1/coordIndex1 vertexIndex2/coordIndex2 vertexIndex3/coordIndex3"
	// otherwise the format will look like this:"f vertexIndex1 vertexIndex2 vertexIndex3"
	// The index values index into our vertice and texture coordinate arrays.  More explained in RenderScene().
	// *Note* Make sure if you cut this code out for your own use you minus 1 from the indices.
	// This is because arrays are zero based and the .obj indices start at 1.  Look at FillInObjectInfo().

	// Check if this object has texture coordinates before reading in the values
	// Here we read in the object's vertex and texture coordinate indices.
	// Here is the format: "f vertexIndex1/coordIndex1 vertexIndex2/coordIndex2 vertexIndex3/coordIndex3"
	fscanf(m_filePT, "%d/%d %d/%d %d/%d",	&_face.v[0], &_face.c[0],
											&_face.v[1], &_face.c[1],
											&_face.v[2], &_face.c[2]);	

	// Fix the iteration Problem
	--_face.v[0]; --_face.v[1]; --_face.v[2];
	--_face.c[0]; --_face.c[1]; --_face.c[2];

	// Read the rest of the line so the file pointer returns to the next line.
	fgets(_line, 100, m_filePT);
				
	// Add the new face to our face list
	++(m_fCount);
	m_Faces.push_back(_face);
}

///////////////////////////////// COMPUTE NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This computes the normals for an object
/////
///////////////////////////////// COMPUTE NORMALS  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void WaveObject::ComputeNormals()
{
	// What are vertex normals?  And how are they different from other normals?
	// Well, if you find the normal to a triangle, you are finding a "Face Normal".
	// If you give OpenGL a face normal for lighting, it will make your object look
	// really flat and not very round.  If we find the normal for each vertex, it makes
	// the smooth lighting look.  This also covers up blocky looking objects and they appear
	// to have more polygons than they do.    Basically, what you do is first
	// calculate the face normals, then you take the average of all the normals around each
	// vertex.  It's just averaging.  That way you get a better approximation for that vertex.

	CVector3* _fNorms = new CVector3[m_fCount];

	//----------------------------------------
	//	Calculate Face Normals
	//----------------------------------------
	for(UINT i=0; i < m_fCount; ++i)
	{
		// BUG :: No matter what I do I can't seem to get a standerd set of normals
		CFace& _face = m_Faces[i];

		CVector3 _vec1 = m_Verts[_face.v[1]] - m_Verts[_face.v[0]];
		CVector3 _vec2 = m_Verts[_face.v[0]] - m_Verts[_face.v[2]];

		_fNorms[i] = _vec1 * _vec2;
	}

	//----------------------------------------
	//	Calculate Vertex Normals
	//----------------------------------------
	for (UINT i = 0; i < m_vCount; ++i)
	{
		CVector3 _sum(0,0,0);
		CVector3 _ref(1,1,1);
		UINT _shared   = 0;

		for (UINT j = 0; j < m_fCount; ++j)				// Go through all of the triangles
		{												// Check if the vertex is shared by another face
			CFace& _face = m_Faces[j];
			if (_face.v[0] == i || _face.v[1] == i || _face.v[2] == i)
			{
				_sum = _sum + _fNorms[j];

				++_shared;
			}
		}      
		
		// Normalize and Export
		_sum.Normalize();
		m_Norms.push_back(_sum);
	}

	delete[] _fNorms;
}