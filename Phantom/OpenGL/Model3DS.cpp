#include "Model3DS.h"

int gBuffer[50000] = {0};			// This is used to read past unwanted data

//####################################################
//####################################################
//			Construct and Allocate
//####################################################
//####################################################
//=============================================================================
//	Setup Basic Class and Allocate Memory
//=============================================================================
BGLModel3ds::BGLModel3ds()
{
	m_Model.m_MatCount = 0;
	m_Model.m_Objcount = 0;
	m_FilePointer = NULL;
}
//=============================================================================
//	Deallocate Memory
//=============================================================================
BGLModel3ds::~BGLModel3ds()
{
	for(int i = 0; i < m_Model.m_Objcount; ++i)
	{
		BGLObject& _object = m_Model.m_Objs[i];
		if(_object.m_Verts)		delete[] _object.m_Verts;
		if(_object.m_Faces)		delete[] _object.m_Faces;
		if(_object.m_fNormals)	delete[] _object.m_fNormals;
		if(_object.m_vNormals)	delete[] _object.m_vNormals;		
		if(_object.m_TexVerts)	delete[] _object.m_TexVerts;
	}
}

//####################################################
//####################################################
//				LOAD FROM FILE
//####################################################
//####################################################
//=============================================================================
// Base Call which calls several other functions
//=============================================================================
bool BGLModel3ds::Import3ds(const char* _filename)
{
	//---------------------------------------------
	//	Allocate and Error Check Params
	//---------------------------------------------
	char _message[255] = {0};
	maxChunk _curr = {0};

	m_FilePointer = fopen(_filename, "rb");

	if(!m_FilePointer) 
	{
		sprintf(_message, "Unable to find the file: %s!", _filename);
		MessageBox(NULL, _message, "Error", MB_OK);
		return false;
	}

	//------------------------------------------------
	//	Confirm Magic Number for 3DS Files
	//------------------------------------------------
	ReadChunk(&_curr);

	if (_curr.m_ID != CH_PRIMARY)
	{
		sprintf(_message, "Unable to load PRIMARY chuck from file: %s!", _filename);
		MessageBox(NULL, _message, "Error", MB_OK);
		return false;
	}


	//------------------------------------------------
	//	Call Recurrsive Function
	//------------------------------------------------
	ProcessNextChunk(&_curr);

	//------------------------------------------------
	//	Compute Normals
	//------------------------------------------------
	ComputeFaceNormals();

	//------------------------------------------------
	//	Clean Up
	//------------------------------------------------
	CleanUp();

	return true;
}

//=============================================================================
// This reads in a string and saves it in the char array passed in
//=============================================================================
int BGLModel3ds::GetString(char* _string)
{
	int _index = 0;

	// Read 1 byte of data which is the first letter of the string
	fread(_string, 1, 1, m_FilePointer);

	//---------------------------------------
	//	Read until the end of the
	//	 NULL terminated string
	//---------------------------------------
	while (*(_string + _index++) != 0)
	{
		// Read in a character at a time until we hit NULL.
		fread(_string + _index, 1, 1, m_FilePointer);
	}

	// Return the string length, which is how many bytes we read in (including the NULL)
	return (UINT)(strlen(_string) + 1);
}

//=============================================================================
// This reads the next chunk
//=============================================================================
void BGLModel3ds::ReadChunk(maxChunk* _chunk)
{
	//----------------------------------------------------------
	//	Read in chunk ID
	//----------------------------------------------------------
	_chunk->m_bytesRead = (UINT)fread(&_chunk->m_ID, 1, 2, m_FilePointer);

	//----------------------------------------------------------
	//	Read Chunk Length
	//----------------------------------------------------------
	_chunk->m_bytesRead += (UINT)fread(&_chunk->m_length, 1, 4, m_FilePointer);
}
//=============================================================================
// This reads the next large chunk
//=============================================================================
void BGLModel3ds::ProcessNextChunk(maxChunk* _prev)
{
	//---------------------------------------
	//	Allocate Storage
	//---------------------------------------
	BGLObject _Object = {0};
	BGLMaterialInfo _Texture = {0};

	maxChunk _curr = {0};
	maxChunk _temp = {0};

	//--------------------------------------------------------------------------
	// Below we check our chunk ID each time we read a new chunk.  Then, if
	// we want to extract the information from that chunk, we do so.
	// If we don't want a chunk, we just read past it.  
	//
	// Continue to read the sub chunks until we have reached the length.
	// After we read ANYTHING we add the bytes read to the chunk and then check
	// check against the length.
	//--------------------------------------------------------------------------
	while (_prev->m_bytesRead < _prev->m_length)
	{
		// Read next Chunk
		ReadChunk(&_curr);

		// Check the chunk ID
		switch (_curr.m_ID)
		{
			//----------------------------------------------
			//	The Version Chunk
			//	Supported Version is 3
			//----------------------------------------------
		case CH_VERSION:
			{
				_curr.m_bytesRead += (UINT)fread(gBuffer, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);

				if ((_curr.m_length - _curr.m_bytesRead == 4) && (gBuffer[0] > 0x03)) 
				{
					MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
				}
			}
			break;

			//----------------------------------------------
			//	The Object Information Chunk
			//----------------------------------------------
		case CH_OBJECTINFO:
			{

				// Read the next chunk
				ReadChunk(&_temp);

				// Get the version of the mesh
				_temp.m_bytesRead += (UINT)fread(gBuffer, 1, _temp.m_length - _temp.m_bytesRead, m_FilePointer);

				// Increase the bytesRead by the bytes read from the last chunk
				_curr.m_bytesRead += _temp.m_bytesRead;

				// Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.
				ProcessNextChunk(&_curr);
			}
			break;			

			//----------------------------------------------
			//	The Object Data Chunk
			//	Increment Count
			//	Add to list and create refrence
			//	Memset, get name, process chunks
			//----------------------------------------------
		case CH_OBJECT:
			{
				++(m_Model.m_Objcount);

				m_Model.m_Objs.push_back(_Object);

				BGLObject& _refObj = m_Model.m_Objs[m_Model.m_Objcount - 1];
                				
				memset(&_refObj, 0, sizeof(BGLObject));
				_curr.m_bytesRead += GetString(_refObj.m_strName);
				ProcessNextObjectChunk(&_refObj, &_curr);
			}
			break;

			//---------------------------------------------
			//	Data we do not care about or support
			//	 Read Past
			//---------------------------------------------
		default: 	
			_curr.m_bytesRead += (UINT)fread(gBuffer, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);
			break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		_prev->m_bytesRead += _curr.m_bytesRead;
	}
}
//=============================================================================
// This reads the object chunks
//=============================================================================
void BGLModel3ds::ProcessNextObjectChunk(BGLObject *pObject, maxChunk* _prev)
{
	//-----------------------------
	//	Local Workboard
	//-----------------------------
	maxChunk _curr = {0};

	//-----------------------------
	//	Read until End of Chunk
	//-----------------------------
	while (_prev->m_bytesRead < _prev->m_length)
	{
		//-----------------------------
		//	Read In Data
		//	Switch and Handle
		//-----------------------------
		ReadChunk(&_curr);

		// Check which chunk we just read
		switch (_curr.m_ID)
		{
			//---------------------------------------
			//	Reading new Object
			//---------------------------------------
		case CH_OBJECT_MESH:
			ProcessNextObjectChunk(pObject, &_curr);
			break;

			//---------------------------------------
			//	Reading Vertices
			//---------------------------------------
		case CH_OBJECT_VERTICES:
			ReadVertices(pObject, &_curr);
			break;

			//---------------------------------------
			//	Reading Face Info
			//---------------------------------------
		case CH_OBJECT_FACES:
			ReadVertexIndices(pObject, &_curr);
			break;

			//---------------------------------------
			//	Reading Material Name
			//	Note this has other information in it
			//	Regarding Multi-Texturing ect...
			//---------------------------------------
		case CH_OBJECT_MATERIAL:
			ReadObjectMaterial(pObject, &_curr);			
			break;

			//---------------------------------------
			//	Reading Texture Co-ordinates
			//---------------------------------------
		case CH_OBJECT_UV:
			ReadUVCoordinates(pObject, &_curr);
			break;

			//---------------------------------------
			//	Reading the Local Axis Information
			//---------------------------------------
		case CH_OBJECT_LOCAL:
			ReadAxis(pObject, &_curr);
			break;

			//---------------------------------------
			//	Unkown or Ignored Chunks
			//---------------------------------------
		default: 
			_curr.m_bytesRead += (UINT)fread(gBuffer, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);
			break;
		}

		//---------------------------------------
		//	Add Reading Length
		//---------------------------------------
		_prev->m_bytesRead += _curr.m_bytesRead;
	}
}
//=============================================================================
// This reads the material chunks
//=============================================================================
void BGLModel3ds::ProcessNextMaterialChunk(maxChunk* _prev)
{
	//-----------------------------
	//	Local Workboard
	//-----------------------------
	maxChunk _curr = {0};

	//----------------------------
	//	Read till End of Chunk
	//----------------------------
	while (_prev->m_bytesRead < _prev->m_length)
	{
		//-----------------------------
		//	Read In Data
		//	Switch and Handle
		//-----------------------------
		ReadChunk(&_curr);

		// Check which chunk we just read
		switch (_curr.m_ID)
		{
			//----------------------------
			// This chunk holds the name
			// of the material
			//----------------------------
		case CH_MATNAME:
			_curr.m_bytesRead += (UINT)fread(m_Model.m_Mats[m_Model.m_MatCount -1].m_strName, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);
			break;

			//----------------------------
			// This holds the RGB
			// color of our object
			//----------------------------
		case CH_MATDIFFUSE:
			ReadColorChunk(&(m_Model.m_Mats[m_Model.m_MatCount -1]), &_curr);
			break;
		
			//----------------------------
			// This is the header for
			// the texture info
			//----------------------------
		case CH_MATMAP:							
			ProcessNextMaterialChunk(&_curr);
			break;

			//----------------------------
			// This stores the file
			// name of the material
			//----------------------------
		case CH_MATMAPFILE:						
			_curr.m_bytesRead += (UINT)fread(m_Model.m_Mats[m_Model.m_MatCount -1].m_strFile, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);
			break;

			//------------------------------------------
			//	We either don't know it our we
			//	Dont care about it
			//------------------------------------------
		default:
			_curr.m_bytesRead += (UINT)fread(gBuffer, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);
			break;
		}

		//-------------------------------------------
		//	Move Parent Marker and Jump Back
		//-------------------------------------------
		_prev->m_bytesRead += _curr.m_bytesRead;
	}
}
//=============================================================================
// This reads the RGB value for the object's color
//=============================================================================
void BGLModel3ds::ReadColorChunk(BGLMaterialInfo *pMaterial, maxChunk* _prev)
{
	//-----------------------------
	//	Local Workboard
	//-----------------------------
	maxChunk _curr = {0};

	//-----------------------------
	//	Read in Information
	//-----------------------------
	ReadChunk(&_curr);
	_curr.m_bytesRead += (UINT)fread(pMaterial->m_color, 1, _curr.m_length - _curr.m_bytesRead, m_FilePointer);

	//-----------------------------
	//	Move Marker and Return
	//-----------------------------
	_prev->m_bytesRead += _curr.m_bytesRead;
}
//=============================================================================
// This reads the objects vertices
//=============================================================================
void BGLModel3ds::ReadVertices(BGLObject *pObject, maxChunk* _prev)	
{
	//------------------------------------------
	//	Read Header
	//------------------------------------------
	_prev->m_bytesRead += (UINT)fread(&(pObject->m_vertCount), 1, 2, m_FilePointer);
	UINT _vCount = pObject->m_vertCount;

	//------------------------------------------
	//	Allocate Memory
	//	First Final Storage then temp
	//	Array into which you can read
	//------------------------------------------
	pObject->m_Verts = new CVector3[_vCount];
	memset(pObject->m_Verts, 0, sizeof(CVector3) * _vCount);

	float* _temp = new float[_vCount*3];
	memset(_temp, 0, sizeof(float) * _vCount* 3);

	//------------------------------------------
	//	Read in Vertices
	//------------------------------------------
	_prev->m_bytesRead += (UINT)fread(_temp, 1, _prev->m_length - _prev->m_bytesRead, m_FilePointer);

	//------------------------------------------
	//	Convert into Vectors
	//------------------------------------------
	for(UINT i = 0; i < _vCount; i++)
	{		
		pObject->m_Verts[i].x =  _temp[i*3 + 0];
		pObject->m_Verts[i].y =  _temp[i*3 + 1];
		pObject->m_Verts[i].z =  _temp[i*3 + 2];
	}

	//------------------------------------------
	//	Clean Up
	//------------------------------------------
	delete[] _temp;
}
//=============================================================================
// This reads the objects face information
//=============================================================================
void BGLModel3ds::ReadVertexIndices(BGLObject *pObject, maxChunk* _prev)	
{
	unsigned short _temp = 0;

	//-----------------------------------
	//	Get Size and Allocate
	//-----------------------------------
	_prev->m_bytesRead += (UINT)fread(&pObject->m_faceCount, 1, 2, m_FilePointer);

	int _fCount = pObject->m_faceCount;
	pObject->m_Faces = new BGLFace[_fCount];
	memset(pObject->m_Faces, 0, sizeof(BGLFace) * _fCount);

	//-----------------------------------
	//	Read in All The Faces
	//-----------------------------------
	for(int i = 0; i < _fCount; i++)
	{
		//--------------------------------------------------------
		//	Read in the 3 Verts Data The last is a visbility flag
		//	Which we ignore
		//-------------------------------------------------------
		for(int j = 0; j < 4; j++)
		{
			// Read the first vertice index for the current face 
			_prev->m_bytesRead += (UINT)fread(&_temp, 1, sizeof(_temp), m_FilePointer);

			if(j < 3)
			{
				pObject->m_Faces[i].m_vert[j] = _temp;
			}
		}
		// END OF FACE
	}
}
//=============================================================================
//	 This reads the texture coodinates of the object
//=============================================================================
void BGLModel3ds::ReadUVCoordinates(BGLObject *pObject, maxChunk* _prev)	
{
	//----------------------------------------
	//	Read In Size
	//----------------------------------------
	_prev->m_bytesRead += (UINT)fread(&pObject->m_texCount, 1, 2, m_FilePointer);
	UINT _tCount = pObject->m_texCount;

	//-----------------------------------------
	//	Allocate Memory
	//	Need Temporary Array to read into
	//-----------------------------------------
	pObject->m_TexVerts = new CVector2[_tCount];
	memset(pObject->m_TexVerts, 0, sizeof(CVector2) * _tCount);

	float* _temp = new float[_tCount*2];
	memset(_temp, 0, sizeof(float) * _tCount*2);

	//-----------------------------------------
	//	Read In Data
	//-----------------------------------------
	_prev->m_bytesRead += (UINT)fread(_temp, 1, _prev->m_length - _prev->m_bytesRead, m_FilePointer);

	//-----------------------------------------
	//	Convert to Vector
	//-----------------------------------------
	for(UINT i = 0; i < _tCount; i++)
	{		
		pObject->m_TexVerts[i].x =  _temp[i*2 + 0];	
		pObject->m_TexVerts[i].y =  _temp[i*2 + 1];	
	}
	//-----------------------------------------
	//	Clean Up
	//-----------------------------------------
	delete[] _temp;
}
//=============================================================================
// This reads the local axis information of the Object
//=============================================================================
void BGLModel3ds::ReadAxis(BGLObject *pObject, maxChunk* _prev)
{
	//------------------------------------------------------------
	//	Local axis info.
	// The three first blocks of three floats are the definition
	// (in the absolute axis) of the local axis X Y Z of the object.
	// And the last block of three floats is the local center of the object.
	//------------------------------------------------------------

	float _temp[12] = {0};
	_prev->m_bytesRead += (UINT)fread(_temp, 1, (_prev->m_length - _prev->m_bytesRead), m_FilePointer);

	//---------------------------------------------
	//	Quick note I use XY as the ground plane
	//	Same as Max
	//---------------------------------------------
}
//=============================================================================
// This reads in the material name assigned to
// the object and sets the materialID
//=============================================================================
void BGLModel3ds::ReadObjectMaterial(BGLObject *pObject, maxChunk* _prev)
{
	//-----------------------------------------------
	//	Get Name String 
	//-----------------------------------------------
	char _strName[255] = {0};
	_prev->m_bytesRead += GetString(_strName);

	//-----------------------------------------------
	//	Has Material Previously been Used
	//-----------------------------------------------
	for(int i = 0; i < m_Model.m_MatCount; ++i)
	{
		//---------------------------------
		//	Found Match
		//---------------------------------
		if(strcmp(_strName, m_Model.m_Mats[i].m_strName) == 0)
		{
			pObject->m_matID = i;

			//----------------------------------
			//	Is Texture File?
			//----------------------------------
			if(strlen(m_Model.m_Mats[i].m_strFile) > 0) 
			{
				pObject->m_textured = true;
			}	
			break;
		}
		else
		{
			//---------------------------
			//	No Material Found
			//---------------------------
			pObject->m_matID = -1;
		}
	}

	//------------------------------------------------------
	//	Ignore Rest of Infomation
	//------------------------------------------------------
	_prev->m_bytesRead += (UINT)fread(gBuffer, 1, _prev->m_length - _prev->m_bytesRead, m_FilePointer);
}

//=============================================================================
// This computes the face normals for the object
//=============================================================================
void BGLModel3ds::ComputeFaceNormals()
{
	//----------------------------------------------------
	//	Check if there is anything to calculate
	//----------------------------------------------------
	if(m_Model.m_Objcount <= 0)
		return;

	//----------------------------------------------------
	//	Temporary Storage
	//----------------------------------------------------
	CVector3 _Vector1, _Vector2, _Normal, _Poly[3];


	//------------------------------------------------------
	//	Loop through Objects
	//------------------------------------------------------
	for(int index = 0; index < m_Model.m_Objcount; ++index)
	{
		//---------------------------
		// Refrence Object
		//---------------------------
		BGLObject& _Object = m_Model.m_Objs[index];

		//---------------------------------
		// Here we allocate all the memory 
		// we need to calculate the normals
		//---------------------------------
		_Object.m_fNormals = new CVector3[_Object.m_faceCount];

		//---------------------------------------------
		//	Iterate Through The Faces
		//---------------------------------------------
		for(int f = 0; f < _Object.m_faceCount; ++f)
		{												
			//---------------------------------------------
			// To cut down LARGE code
			// we extract the 3 points of this face
			//---------------------------------------------
			_Poly[0] = _Object.m_Verts[_Object.m_Faces[f].m_vert[0]];
			_Poly[1] = _Object.m_Verts[_Object.m_Faces[f].m_vert[1]];
			_Poly[2] = _Object.m_Verts[_Object.m_Faces[f].m_vert[2]];

			//-------------------------------------------
			//	Calculate FACE normals
			//-------------------------------------------
			_Vector1 = _Poly[0] - _Poly[1];
			_Vector2 = _Poly[1] - _Poly[2];

			//-------------------------------------------
			//	Save UnNormalised Normal so we know the
			//	Size(importance) of face
			//-------------------------------------------
			_Normal  = _Vector1* _Vector2;
			_Object.m_fNormals[f] = _Normal;
		}

		//---------------------------------------------
		//	Calculate Vertex Normals
		//---------------------------------------------
		ComputeVertexNormals();
		for(int f = 0; f < _Object.m_faceCount; ++f)
		{			
			_Object.m_fNormals[f].Normalize();
		}
	}
}

//=============================================================================
// This computes the vertex normals for the object
//=============================================================================
void BGLModel3ds::ComputeVertexNormals()
{
	//----------------------------------------------------
	//	Check if there is anything to calculate
	//----------------------------------------------------
	if(m_Model.m_Objcount <= 0)
		return;

	//----------------------------------------------------
	//	Temporary Storage
	//----------------------------------------------------
	CVector3 _Vector1, _Vector2, _Normal, _Poly[3];


	//------------------------------------------------------
	//	Loop through Objects
	//------------------------------------------------------
	for(int index = 0; index < m_Model.m_Objcount; ++index)
	{
		//---------------------------
		// Refrence Object
		//---------------------------
		BGLObject& _Object = m_Model.m_Objs[index];

		//---------------------------------
		// Here we allocate all the memory 
		// we need to calculate the normals
		//---------------------------------
		_Object.m_vNormals = new CVector3[_Object.m_vertCount];

		//---------------------------------------------
		//	Iterate Through Vertexs
		//---------------------------------------------
		for(int i = 0; i < _Object.m_vertCount; ++i)
		{
			CVector3 _normTotal;
			//---------------------------------------------
			//	Iterate Through The Faces
			//---------------------------------------------
			for(int f = 0; f < _Object.m_faceCount; ++f)
			{
				if( (i == _Object.m_Faces[f].m_vert[0]) ||
					(i == _Object.m_Faces[f].m_vert[1]) ||
					(i == _Object.m_Faces[f].m_vert[2]))
				{
					_normTotal += _Object.m_fNormals[f];
				}
			} // END FACE
			_Object.m_vNormals[i] = _normTotal.getNormalized();
		} // END VERTEX
	} // END OBJECTS
}
//=============================================================================
// This frees memory and closes the file
//=============================================================================
void BGLModel3ds::CleanUp()
{
	if (m_FilePointer)
	{
		fclose(m_FilePointer);					// Close the current file pointer
		m_FilePointer = NULL;
	}
}
//####################################################
//####################################################
//			RENDER FUNCTIONS
//####################################################
//####################################################
//=============================================================================
//	Render the Objects
//=============================================================================
void BGLModel3ds::RenderObjects() const
{
	const int& _oCount = m_Model.m_Objcount;
	for(int i = 0; i < _oCount; ++i)
	{
		const BGLObject& _object = m_Model.m_Objs[i];
		const int& _fCount = _object.m_faceCount;

		glBegin(GL_TRIANGLES);
		for(int m = 0; m < _fCount; ++m)
		{
			BGLFace& _face = _object.m_Faces[m];
			glNormal3f(
					_object.m_fNormals[m].x,
					_object.m_fNormals[m].y,
					_object.m_fNormals[m].z);

			for(int v = 0; v < 3; ++v)
			{
				int& _vertID = _face.m_vert[v];
				glTexCoord2f(
					_object.m_TexVerts[_vertID].x,
					_object.m_TexVerts[_vertID].y);
				glNormal3f(
					_object.m_vNormals[_vertID].x,
					_object.m_vNormals[_vertID].y,
					_object.m_vNormals[_vertID].z);
				glVertex3f(
					_object.m_Verts[_vertID].x,
					_object.m_Verts[_vertID].y,
					_object.m_Verts[_vertID].z);
			}			
		}
		glEnd();
	}	
}
//=============================================================================
//	Build Gl List
//=============================================================================
GLuint BGLModel3ds::getList() const
{
	GLuint _listID = glGenLists(1);

	glNewList(_listID,GL_COMPILE);
		RenderObjects();
	glEndList();

	return _listID;
}