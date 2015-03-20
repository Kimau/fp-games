#include "Loaders.h"

#include "jpeglib.h"
#include "3ds.h"
#include "WaveObj.h"
#include "tga.h"

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       TEXTURE FUNCTIONS                                                         ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				DECODE JPEG
//////////////////////////////////////////////////////////////////////////////
void DecodeJPG(jpeg_decompress_struct* _info, tImageJPG* _ImgData)
{
	// Read in the header of the jpeg file
	jpeg_read_header(_info, TRUE);
	
	// Start to decompress the jpeg file with our compression info
	jpeg_start_decompress(_info);

	// Get the image dimensions and row span to read in the pixel data
	_ImgData->rowSpan = _info->image_width * _info->num_components;
	_ImgData->sizeX   = _info->image_width;
	_ImgData->sizeY   = _info->image_height;
	
	// Allocate memory for the pixel buffer
	_ImgData->data = new unsigned char[_ImgData->rowSpan * _ImgData->sizeY];
		
	// Here we use the library's state variable _info.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	
	// Create an array of row pointers
	unsigned char** _rowPtr = new unsigned char*[_ImgData->sizeY];
	for (int i = 0; i < _ImgData->sizeY; i++)
		_rowPtr[i] = &(_ImgData->data[i*_ImgData->rowSpan]);

	// Now comes the juice of our work, here we extract all the pixel data
	int _rowsRead = 0;
	while (_info->output_scanline < _info->output_height) 
	{
		// Read in the current row of pixels and increase the rowsRead count
		_rowsRead += jpeg_read_scanlines(_info, &_rowPtr[_rowsRead], _info->output_height - _rowsRead);
	}
	
	// Delete the temporary row pointers
	delete [] _rowPtr;

	// Finish decompressing the data
	jpeg_finish_decompress(_info);
}

//////////////////////////////////////////////////////////////////////////////
//				Load JPEG
//////////////////////////////////////////////////////////////////////////////
INT_IMAGE* MyLoaders::LoadJPG(const char *_filename)
{
	struct jpeg_decompress_struct _info;
	memset(&_info,0,sizeof(_info));
	tImageJPG* _ImageData = NULL;
	FILE* _File = fopen(_filename, "rb");
	
	// This is the only function you should care about.  You don't need to
	// really know what all of this does (since you can't cause it's a library!) :)
	// Just know that you need to pass in the jpeg file name, and get a pointer
	// to a tImageJPG structure which contains the width, height and pixel data.
	// Be sure to free the data after you are done with it, just like a bitmap.
	
	// Open a file pointer to the jpeg file and check if it was found and opened 
	if(_File == NULL) 
	{
		char strMessage[100] = {0};
		sprintf(strMessage, "Unable to find the file: %s!", _filename);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return NULL;
	}
	
	// Create an error handler
	jpeg_error_mgr _JPEGerr;

	// Have our compression info object point to the error handler address
	_info.err = jpeg_std_error(&_JPEGerr);
	
	// Initialize the decompression object
	jpeg_create_decompress(&_info);
	
	// Specify the data source (Our file pointer)	
	jpeg_stdio_src(&_info, _File);
	
	// Allocate the structure that will hold our eventual jpeg data (must free it!)
	_ImageData = (tImageJPG*)malloc(sizeof(tImageJPG));

	// Decode the jpeg file and fill in the image data structure to pass back
	DecodeJPG(&_info, _ImageData);
	
	// This releases all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&_info);
	
	// Close the file pointer that opened the file
	fclose(_File);

	// Convert tImageJPG to INT_IMAGE
    INT_IMAGE* _retImg = new INT_IMAGE;
	_retImg->m_data = _ImageData->data;
	_retImg->x = _ImageData->sizeX;
	_retImg->y = _ImageData->sizeY;
	_retImg->m_format = MAGIC_NUM_JPEG;
	// Return the jpeg data (remember, you must free this data after you are done)
	return _retImg;
}

//////////////////////////////////////////////////////////////////////////////
//				Load JPEG
//////////////////////////////////////////////////////////////////////////////
INT_IMAGE* MyLoaders::LoadTGA(const char *_filename)
{
	return 0;
}

//#####################################################################################
//#####################################################################################
//##                                                                                 ##
//##       MODEL FUNCTIONS                                                           ##
//##                                                                                 ##
//#####################################################################################
//#####################################################################################
//////////////////////////////////////////////////////////////////////////////
//				Load Wavefront Object
//////////////////////////////////////////////////////////////////////////////
JWL_Mesh* MyLoaders::LoadWaveObj(const char* _filename)
{
	//-----------------------------------------
	// Open a file pointer to the Wavefront
	// file and check if it was found and opened 
	//-----------------------------------------
	FILE* _File;
	if((_File = fopen(_filename, "r")) == NULL) 
	{
		char strMessage[100] = {0};
		sprintf(strMessage, "Unable to find the file: %s!", _filename);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return NULL;
	}
	
	//-----------------------------------------
	// Call the file loading function from
	// the Wavefront Libary to load data into
	// intermediate data struct
	//-----------------------------------------
	WaveObject* _info = new WaveObject(_File);

	//-----------------------------------------
	// Move information into final struct
	//-----------------------------------------
	UINT _vCount = _info->m_fCount *3;

	JWL_Mesh* _final = new JWL_Mesh(_vCount,4);	// MODE 4 is GL_TRIANGLES
	
	for(UINT c = 0; c < _info->m_fCount; ++c)
	{
		CFace& _face = _info->m_Faces[c];
		
		_final->setT(c*3 + 0,_info->m_Coords[_face.c[0]]);
		_final->setT(c*3 + 1,_info->m_Coords[_face.c[1]]);
		_final->setT(c*3 + 2,_info->m_Coords[_face.c[2]]);

		_final->setV(c*3 +0, _info->m_Verts[_face.v[0]]);
		_final->setV(c*3 +1, _info->m_Verts[_face.v[1]]);
		_final->setV(c*3 +2, _info->m_Verts[_face.v[2]]);
	}
	for(UINT v = 0; v < _info->m_vCount; ++v)
	{
		_final->setN(v, _info->m_Norms[v]);
	}

	//-----------------------------------------
	// Free unused Data
	//-----------------------------------------
	if (_info)
	{
		delete(_info);
	}

	// Return Data
	return _final;
}

//////////////////////////////////////////////////////////////////////////////
//				Load 3D Studio Format
//////////////////////////////////////////////////////////////////////////////
JWL_Mesh* MyLoaders::Load3DS(const char* _filename)
{
	CLoad3DS _loader;
	
	//-----------------------------------------
	// Allocate space for models
	//-----------------------------------------
	t3DModel _model;
	memset(&_model,0,sizeof(t3DModel));

	//-----------------------------------------
	// Load Model
	//-----------------------------------------
	if(!_loader.Import3DS(&_model,_filename))
		return 0;

	//-----------------------------------------
	// Move information into final struct
	//-----------------------------------------
	JWL_Mesh* _final = 0;
	UINT _vcount = 0;
	for(int i = 0; i < _model.numOfObjects; ++i)
	{
		_vcount += _model.pObject[i].numOfFaces*3;
	}

	UINT _offset = 0;
	for(int i = 0; i < _model.numOfObjects; ++i)
	{
		//------------------------------
		// Refrence Object
		// Avoid uneccary [] calls
		//------------------------------
		t3DObject& _object = _model.pObject[i];
		//------------------------------
		//	Allocate space for
		//	final model
		//------------------------------
		_final = new JWL_Mesh(_vcount,4);	// MODE 4 is GL_TRIANGLES

		//------------------------------
		//	Loop through faces
		//	copying verts across
		//------------------------------
		for(int f = 0; f < _object.numOfFaces; ++f)
		{
			for(int v = 0; v < 3; ++v)
			{
				int _index = _object.pFaces[f].v[v];
				_final->setP(_offset,_object.pTexVerts[_index],_object.pNormals[_index],_object.pVerts[_index]);
				++_offset;
			}			
		}
	}

	// Return Data
	return _final;
}